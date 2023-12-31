Software for automatic plant watering system using esp32 webserver as a UI. Following documentation is written in German.

# Watersystem

Das "Wassersysten" erlaubt die automatische Bewässerung von Pflanzen. 

Eine exakte Anwendungsdokumentation ist dem Dokument "WasserSystem UserDoc in diesem Repository zu entnehmen. 

## Komponenten

Bauteile können verlötet oder mit Kabeln verbunden werden. 

| Komponente                                        | Benutzung                                                                                         |
|---------------------------------------------------|---------------------------------------------------------------------------------------------------|
| ESP32 Devkit                                      | Microcontroller, hostet Website, liest Sensorik                                                   |
| LED RGB (optional)                                | Zeigt Systemstatus                                                                                |
| Kapazitativer Bodenfeuchtigkeitssensor            | Liest Bodenfeuchte                                                                                |
| Pumpe 5V                                          | Fördert Wasser                                                                                    |
| Ultraschallsensor HC SR 04 (optional)             | Liest Wasserstand                                                                                 |
| Relais 5V                                         | Steuert Pumpenstrom                                                                               |
| Lochrasterplatte (optional)                       | Hält andere Komponenten                                                                           |
| 3*2er Schraubverbinder (optional)                 | Erlauben das Verbinden externer Komponenten (1* 4er für Ultraschallsensor, 1*2er für Motor Ground) |
| 1*3er Schraubverbinder (optional)                 | Erlaubt das Verbinden externer Komponenten (Feuchtigkeitssensor)                                  |
| Luftfahrtstecker 16mm  (optional)                              | Erlauben das Verbinden externer Komponenten                                                       |
| Kabel, Draht, Schrauben etc. (optional)                        | 4* M3 Schrauben für Plexiglas, 2* M5 Schrauben für Elektronikbox-Deckel,  2* M3 Schrauben für Wasserdeckel                       |
| Schlauch 6mm (optional)                                         | Kanal für Wasser                                                                                  |
| Plastik-Dosen mit Deckel (ca. 120mm*120mm) (1.3l) (optional)   | Kann durch "PlasticJar" + "PlasticJarCover"(2* )" + "PlasticJarCoverAlternative" ersetzt werden. - Vorsicht: Gedruckte Teile evtl. nicht wasserdicht, am besten in PETG herstellen  |


## Komponenten zur Fertigung auf 3D Drucker und Lasercutter

Bauteile sind für Druck auf 20* 20* 20cm Druckern ausgelegt.
Plexiglas ist ein optionales Bauteil. 

| Bauteile      | Kommentar                                                                                                         |
|---------------|-------------------------------------------------------------------------------------------------------------------|
| Basis         | Aus PETG, 4 Teile + Bais-Steckverbinder. Bauteile sollten verklebt werden, ausrichten mit Steckverbindern, dann kleben                                        |
| Basis-Bodenplatten | PLA, PETG (4 Teile) - von unten an die Basis kleben                                                                                                      |
| Muschelgriffe | Bambus-PLA - Einstecken und verkleben. Eventuell die Steckverbinder Nachschleifen (2 Teile)                               |
| Elektronikbox | PLA, PETG -  Box, Deckel, Einschraubbare Führungen für Schlauch (4 Teile insgesamt)               |
| Wasserdeckel  | PLA, PETG (2 Teile)                                                                                                        |
| Plexiglas     | Lasercutter - Kann durch Bauteil "BlindCover" aus .step Datei ersetzt werden, 4mm Plexiglas, Vorsicht vor giftigen Kunststoffen |
| Bais-Steckverbinder    |PLA,PETG, Teilweise sehr klein, nur der Kreuzverbinder in der Mitte ist zwingend erforderlich. |
| Schlauch-Steckverbinder    |SLA-Druck|

## Platinenübersicht

<img src="./Platine Wassersystem.svg">

## Programmlauf

<img src="./WasserSystemAblauf.svg">
