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
const char* ssid = "WasserSystem";
const char* password = "digitalVHS";
//unter welcher addresse soll die website erreichbar sein?
//WICHTIG - am besten keine Website wählen, die "echt" ist und anwender bei fehlerhafter konfiguration auf SCHADCODE, MALWARE etc. führen könnte!
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

//beschreibt den grenzwert, unter dem die Pumpe gestartet wird.
int threshold = 3400;

//beschreibt die Pause zwischen Pumpstößen
int Sequence_Delay(3000);

//beschreibt distanzwert wenn tank voll (CM).
const float distanceWhenFull = 0.5;

//beschreibt distanzwert wenn tank leer (CM) .
const float distanceWhenEmpty = 13;

//Feuchtigkeit wird hier gespeichert damit der wert global verfügbar ist.
//so kann er in der Netzwerk-UI angezeigt werden
int moisture = 0;

//Distanz zum wasser wird hier gespeichert
float currentDistance = 0.5;


//intervall zwischen feuchtigkeitsmessungen in ms
int interv = 500;
//hält nächste messzeit
long nextMeasure = 1000;

//implementiert Funktionalität für Multi-Core
//Ein Core macht Internet, ein Core alles andere.
//so kann man stets die Internetseite aufrufen und Manipulieren, sogar wenn das System "beschäftigt" ist
TaskHandle_t WaterTask;
TaskHandle_t CommunicationTask;

//boolean wert der das manuelle Pumpen steuert
//wird auf "true" gesetzt, dann wird gepumpt, dann auf "false" gesetzt
boolean manualPumpTriggered = false;

//boolean wert der das manuelle Pumpen steuert
//wird auf "true" gesetzt, dann wird gepumpt, dann auf "false" gesetzt
boolean autoCalibrationTriggered = false;

//boolean wert zum verzögerten pumpen
//wird auf "true" gesetzt, dann wird gepumpt, dann auf "false" gesetzt
//vielleicht brauchen wir hier ein MUTEX-System
boolean delayedPumpTriggered = false;
long nextPumpWaitTime = 0;
int delayedPumpDuration = 3;


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

  //erstelle Prozess für Internet-Funktionen
  xTaskCreatePinnedToCore(
    CommunicationCode,
    "Com-Task",
    10000,
    NULL,
    1,
    &CommunicationTask,
    1);

  delay(500);

  //erstelle Prozess für sämtliche andere Funktionen
  xTaskCreatePinnedToCore(
    WaterCode,
    "Water-Task",
    10000,
    NULL,
    1,
    &WaterTask,
    0);
}

void CommunicationCode(void * pvParameters ) {
  //verknüpft URLs mit Funktionen und startet Server
  serverStart();
  //endlosschleife: verarbeite Anfragen an Server
  while (true) {
    dnsServer.processNextRequest();
    delay(1);
  }
}


void WaterCode(void * pvParameters ) {
  while (true) {
    //wenn nächster Messzeitpunkt erreicht, lese feuchtigkeitssensor aus
    if (millis() > nextMeasure) {
      //lese Wassersensor
      moisture = analogRead(WaterSensor);
      delay(1);
      //printe messwert von wassersensor
      Serial.print("Wassersensor misst: ");
      Serial.println(moisture);
      nextMeasure += interv;

      //trockener = moisture ist kleiner
      //feuchter =  moisture ist höher
      //wenn moisture kleiner grenzwert, dann pumpen
      while (moisture > threshold) {
        Serial.println("Pumpvorgang");
        Serial.print("Sensorwert:");
        Serial.println(analogRead(WaterSensor));
        LED_Red();
        digitalWrite(PumpRelay, HIGH);
        delay(Sequence_Delay);
        LED_Green();
        digitalWrite(PumpRelay, LOW);
        moisture = analogRead(WaterSensor);
        delay(Sequence_Delay);
      }
      LED_Green();
      digitalWrite(PumpRelay, LOW);
    }




    //wenn ein manuelles Pumpen ausgelöst wurde, führe den Pumpvorgang durch
    //parameter sind hier fest gesetzt und müssen evtl angepasst werden - eine anpassung durch Nutzende in der UI ist nicht vorgesehen
    if (manualPumpTriggered) {
      //mit den werten in den klammern kann man pumpdauer - in ms - und wiederholungen sowie die Anzahl der wiederholungen einstellen
      PumpSequence(3000, 3);
      manualPumpTriggered = false;

    }


    //wenn die verzögerte Pumpierung aktiviert wurde, prüfe ob der gewünschte Zeitpunkt schon erreicht ist, pumpe, schalte verzögerte Pumpierung aus
    if (delayedPumpTriggered) {
      if (millis() > nextPumpWaitTime) {
        digitalWrite(PumpRelay, HIGH);
        delay(delayedPumpDuration * 1000);
        digitalWrite(PumpRelay, LOW);
        delayedPumpTriggered = false;
        Serial.println("Delayed Pump");
      }


    }

    currentDistance = measureDistance();
    Serial.print("[TEST]Messe Distanz mit UltraSchall: ");
    Serial.println(currentDistance);
    Serial.print("[TEST]Percentage:");
    Serial.println(percentageFromDistance(currentDistance));

    if (autoCalibrationTriggered) {
      PerformAutoCalibration(10);
      autoCalibrationTriggered = false;
    }
  }
}

void loop() {
  //leerer loop weil die aktivität in den tasks geschieht
  delay(1);

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

//calculates the remaining liquid percentage from the distance obtained by US
int percentageFromDistance(float distance) {
  //<mindist = more than 100% (reservoir is too full)
  //distance = mindist = 100%
  //distance = maxdist = 0%
  //>maxdist = less than 0% empty - i.e. impossible
  float scale = distanceWhenEmpty - distanceWhenFull;
  float actualValue = distance - distanceWhenFull;
  //map actual value to percentage and "reverse" the value
  int percentage = 100 - (actualValue / scale * 100);
  return percentage;
}

//Teste LED  und Relais um den Start anzuzeigen
void bootupTest() {
  LED_Red();
  digitalWrite(PumpRelay, HIGH);
  delay(100);
  LED_Yellow();
  digitalWrite(PumpRelay, LOW);
  delay(100);
  LED_Black();
  digitalWrite(PumpRelay, HIGH);
  delay(100);
  LED_Green();
  digitalWrite(PumpRelay, LOW);
  delay(100);
  LED_Black();

}

void LED_Red() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
}
void LED_Green() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, LOW);
}
void LED_Yellow() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);
  digitalWrite(blueLED, HIGH);
}

void LED_Black() {
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, LOW);
}

void LED_Purple() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);
  digitalWrite(blueLED, HIGH);
}

void PumpSequence(int delay_burst, int repeat) {
  for (int i = 0; i < repeat; i++) {
    LED_Red();
    digitalWrite(PumpRelay, HIGH);
    delay(delay_burst);
    LED_Green();
    digitalWrite(PumpRelay, LOW);
    delay(delay_burst);
  }
}

//misst alle 50 sekunden den zustand des wasserstandssensors
//wird cycles-mal wiederholt
//messwerte werden addiert und durch cycles geteilt
// threshold wird cycles + 100 - wenn die erde ein bisschen trockener wird, wird gepumpt
void PerformAutoCalibration(int cycles) {
  int value = 0;
  for (int i = 0; i < cycles; i++) {
    LED_Purple();
    value += analogRead(WaterSensor);
    delay(50);
  }
  threshold = (value / cycles) + 100;
}

void DelayedPumpSequence(int delayToPump, int dur) {
  delayedPumpTriggered = true;
  nextPumpWaitTime = millis() + (delayToPump * 1000);;
  delayedPumpDuration = dur;
  Serial.println("Delayed Pump SET");

}

//startet server und verknüpft urls mit funktionen
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

  //löst den Manuellen Pumpvorgang aus
  server.on("/pumpTest", HTTP_GET, [](AsyncWebServerRequest * request) {
    manualPumpTriggered = true;
    request->redirect("/");
  });

  //löst den Manuellen Pumpvorgang aus
  server.on("/autoCalibration", HTTP_GET, [](AsyncWebServerRequest * request) {
    autoCalibrationTriggered = true;
    request->redirect("/");
  });

  //api-route - gibt nur messwert des sensors zurück
  server.on("/moisture", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", showMoisture());
  });

  //api route für infos ob manuelles pumpen gerade aktiv ist
  server.on("/manualPumpActive", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", showManualPumpState());
  });

  //api route für infos ob manuelles pumpen gerade aktiv ist
  server.on("/delayedPumpActive", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", showDelayedPumpState());
  });

  //api route für infos ob autokalibrierung gerade aktiv ist
  server.on("/autoCalActive", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", showAutoCalState());
  });

    //api route für grenzwert 
  server.on("/threshold", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", showThreshold());
  });

  //erlaubt die änderung von Systemvariablen
  server.on("/valueChange", HTTP_GET, changeValues);



  //erlaubt die änderung von Systemvariablen
  server.on("/delayedPump", HTTP_GET, delayedPump);
  server.begin();
}
