//VHS Water System
//Unter verwendung zweier Feuchtigkeitssensoren werden Pumpmotoren an- und ausgeschaltet.
// Ultraschallsensoren überwachen den Flüssigkeitsstand
// Ein Potentiometer erlaubt das einfache EInstellen eines Grenzwertes
// eine RGB-LED zeigt Informationen zum Systemstatus
//zwei Knöpfe erlauben das Manuelle Starten des Pumpprozesses.
// bei verwendung eines ESP-32 kann ein per Wlan zugängliches UI erstellt werden.

//libraries für den webserver
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>


//LOGIN DATEN FÜR SYSTEM
const char* ssid = "WasserSystemVHS";
const char* password = "digitalVHS";
//unter welcher addresse soll die website erreichbar sein?
//WICHTIG - am besten keine Website wählen, die "echt" ist und anwender bei fehlerhafter konfiguration auf schadcode etc. führt
//wasser . de ist zum beispiel ein pumpen - onlinehandel
const char* domainName = "wasser-system-vhs.xyz";

//erstelle server objekte - dns server ist ein "wegweiser" (nur für die costum URL)
DNSServer dnsServer;
AsyncWebServer server(80);
IPAddress ip;


//definiere Pin-belegung für pumpenrelais
const int PumpRelay = 26;


//definiere Pin für sensor
const int WaterSensor = 32;


//definiere Pins für LED
const int redLED = 5;
const int greenLED = 18;
const int blueLED = 19;

//definiere pins für HC04 ultraschallsensor
//US = Ultrasound
const int echoPinUS = 22;
const int triggerPinUS = 23 ;

//beschreibt den grenzwert, ab denen die jeweiligen Pumpen gestartet werden.
int threshold = 500;

//beschreibt distanzwert wenn tank voll (CM).
const float distanceWhenFull = 0.5;

//beschreibt distanzwert wenn tank leer (CM) .
const float distanceWhenEmpty = 13;

//Feuchtigkeit wird hier gespeichert damit der wert global verfügbar ist
int moisture = 0;

//Distanz zum wasser wird hier gespeichert
float currentDistance = 0.5;


//intervall zwischen feuchtigkeitsmessungen 
int interv = 500; 
//hält nächste messzeit
long nextMeasure = 1000;



//setup wird bei start des arduino einmalig aufgerufen
void setup() {
  Serial.begin(115200);
  Serial.println("Bewasserungssystem by MM 2023");


  //definiere Pins als ein und Ausgänge
  pinMode(PumpRelay, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  pinMode(triggerPinUS, OUTPUT);
  pinMode(echoPinUS, INPUT);
  pinMode(WaterSensor, INPUT);

  //führe test bei boot durch
  bootupTest();

  //Starte Webserver
  serverStart();



}

void loop() {
  //regelt alles was mit dem server/der website zu tun hat asynchron
  dnsServer.processNextRequest();
  delay(1);

//wenn nächster Messzeitpunkt erreicht, lese feuchtigkeitssensor aus 
  if (millis() > nextMeasure) {
    //lese Wassersensor
    moisture = analogRead(WaterSensor);
    delay(1);
    //printe messwert von wassersensor
    Serial.print("Wassersensor misst: ");
    Serial.println(moisture);
    nextMeasure += interv;
  }



  //wenn wassersensor-messwert größer als grenzwert schalte pumpe und blaue LED ein
  if (moisture > threshold) {
    digitalWrite(PumpRelay, HIGH);
    digitalWrite(blueLED, HIGH);
  }

  //ansonsten schalte BlueLed aus
  else {
    digitalWrite(blueLED, LOW);
    digitalWrite(PumpRelay, LOW);
  }


  //currentDistance = measureDistance();
  // Serial.print("[TEST]Messe Distanz mit UltraSchall: ");
  //Serial.println(currentDistance);


}

//gibt distanz in cm zurück
float measureDistance() {
  long duration = 0;
  float distance = 0;
  digitalWrite(triggerPinUS, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPinUS, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPinUS, LOW);
  duration = pulseIn(echoPinUS, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void bootupTest() { //Teste LED um den Start anzuzeigen
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(PumpRelay, HIGH);
  delay(300);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);
  digitalWrite(PumpRelay, LOW);
  delay(300);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, HIGH);
  digitalWrite(PumpRelay, HIGH);
  delay(300);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
  digitalWrite(PumpRelay, LOW);
  delay(300);

  //Schalte LED wieder aus
  digitalWrite(greenLED, HIGH);
}

void serverStart() {
  //starte im Access Point Modus (esp = router)
  WiFi.softAP(ssid, password);

  //hole die eigene IP Adresse
  ip = WiFi.softAPIP();
  Serial.print("IP-Addresse ist: ");
  Serial.println(ip);

  //starte DNS Server - sorgt dafür, dass wir nicht mit 192.168.y.x auf die website zugreifen, sondern unter der als domainName konfigurierten adresse
  dnsServer.start(53, domainName, ip);
  dnsServer.setTTL(1);

  //wenn die index page (domainname/) besucht wird, gib folgendes zurück (siehe Webpages)
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", MainPage());
  });
  //"geheime" adminseite mit zusätzlichen funktionen und Serverfunktionen
  server.on("/adminPage", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", adminPage());
  });

  server.on("/valueChange", HTTP_GET, changeValues);

  server.begin();
}
