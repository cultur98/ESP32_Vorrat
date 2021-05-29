# ESP32_Vorrat
Mit dieser Software kann man sich die Liste seiner Lebensmittelvorräte auf einem ePaper Display anzeigen lassen.
Die Verwaltung der Lebensmittel erfolgt über die [Android-App Vorrat](https://play.google.com/store/apps/details?id=de.sebag.Vorrat) oder in einer Google Tabelle.

<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/vorrat_app.png" alt="Vorrat App" width="200"/>

Angezeigt werden alle Vorräte, die ein MHD enthalten, die bereits abgelaufen sind oder bald (mit konfigurierbarer Tagesanzahl) ablaufen werden.

Wenn eine Batterie angeschlossen ist, kann der Controller zu bestimmten Zeiten geweckt werden,
um die Anzeige aktualisieren.


## Unterstützte Displays
| LilyGo T5 2.7" | LilyGo T5 2.9" | LilyGo T5 4.7" |
|:--:|:--:|:--:|
|  <img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/lilygo_t5_2.7.png" alt="LilyGo T5 2.7" width="150"/> |  <img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/lilygo_t5_2.9.png" alt="LilyGo T5 2.9" width="150"/> | <img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/lilygo_t5_4.7.png" alt="LilyGo T5 4.7" width="300"/> |

Die unterstützen ESP32 Boadrs mit ePaper Display werden im folgenden kurz ESP genannt.

**Funktion der Tasten**

<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/LilyGo_gehaeuse.png" alt="ESP32_Vorrat" width="700"/>

| Taste | während des Starts | im Betrieb |
| ------: | ------- | ------- |
| `vor` | Online Update durchführen (nur Vorrat Version) | eine Seite weiter blättern |
| `zurück` | Konfiguration bei verbundenem WLAN anzeigen | eine Seite zurück blättern |
| `vor` & `zurück` | in den AP Modus wechseln und Verbindungsdaten anzeigen| (keine) |
| `Wecken` | weckt den ESP | (keine) |

_Anmerkung_: <br>
Im Betrieb funktioniert das Blättern nur, wenn die Liste bereits vollständig
aufgebaut wurde. <br>
Das dauert bei den 2.x" Displays ein paar Sekunden länger, als man Änderungen im Display
wahrnehmen kann.<br>

**Gehäuse für den 3D-Druck**<br>
Im Unterordner [stl](https://github.com/cultur98/ESP32_Vorrat/blob/main/stl/) befinden sich Daten für den 3D-Druck der Gehäuse.<br>
Die Displays der 2.7" Variante sind unsauber verklebt.<br>
Bei jedem ESP sitzt die Anzeige mehr oder weniger schief auf der Platine (s.o.).<br>
 Daher muss bei der 2.7" Variante das Fenster an den jeweiligen ESP angeapsst werden.

## Konfiguration der Software

Wird die Software das erste mal gestartet, muss zunächst eine Verbindung 
zu einem WLAN hergestellt werden.
Durch die Nutzung des [ESP32_WiFiManagers](https://github.com/khoih-prog/ESP_WiFiManager/) wird auf dem ESP ein WiFi Access Point 
gestartet, auf dem man sich z.B. mit einem Smartphone verbinden kann.

Der Name des Access Points beginnt mit "VORRAT" gefolgt durch die MACID des ESPs.

Ist der ESP mit dem WLAN verbunden kann man per Browser die Konfiguration
vornehmen.

## Einstellungen per Webschnittstelle

**Bildschirmausrichtung**<br>
`hochkant` oder `quer`.<br>

_Anmerkung_: <br>
Das LilyGo 4.7" Display funktioniert nur im Querformat.

**Vorrat ID**<br>
Die Gruppen-Zugriffscode aus der [Vorrat-App](https://play.google.com/store/apps/details?id=de.sebag.Vorrat).<br>
Diesen findet man findet in der App unter<br>
`Grundmenü` -> `Gruppe` -> `Menü` -> `Datensicherung` -> `Gruppen-Zugriffscode`.<br>
Der Code wird dort angezeigt und gleichzeitig in den Zwischenspeicher geschrieben.<br>
So kann man ihn einfach mit `Paste` in die Webschnittstelle übertragen.

<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/vorrat_gruppe.png" alt="Gruppenzugriffscode" width="380"/>


**Google API Key**<br>
Kann man in seinem Google Account konfigurieren.

**Google Sheet ID**<br>
Die ID der Tabelle.<br>
Die Tabelle muss diesen Aufbau haben:


<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/google_tabelle.png" alt="Google Tabelle" width="500"/>

Die erste Spalte enthält den Produktnamen, die zweite den Lagerort und die dritte
das Mindesthaltbarkeitsdatum.<br>
Der Inhalt der Kopfzeile wird nicht eingelesen.a

_Anmerkung_: <br>
Der ESP zeigt nur die Produkte aus einer Quelle an.
<br>
Hat man sowohl die Vorrat ID angegeben als auch gültige Daten für die Google Tabelle hinterlegt, dann
werden die Produkte aus der Vorrat-App angezeigt.

**Lagerorte**<br>
Hier kann die Anzeige nach angegebenen Lagerorten gefiltert werden.
Ist das Feld leer, dann werden alle Produkte eingelesen.<br>
Mehrere Lagerorte können durch ein Semikolon (ohne anschließendes Leerzeichen!) getrennt werden.<br>
Damit kann man erreichen, dass ein ESP z.B. nur den Inhalt des Kühlschranks
oder des Gefrierfachs anzeigt.<br> 

**MHD Warnzeit (Tage)**<br>
Vorwarnzeit in Tagen, ab wann die Produkte in hervorgehobener Schrift
angezeigt werden.

**Sprache**<br>
Derzeit `Deutsch` oder `Englsich`.

**Zeitzone**<br>
Derzeit `Mitteleuropäische Zeit` oder `Mitteleuropäische Sommerzeit`.

**Weckzeit # [hh:mm]**<br>
Bis zu drei Zeiten, zu denen der ESP startet und den Inhalt
der Anzeige aktualisiert.

Die weiteren Optionen dienen der individuellen Anpassung des 
Layouts. Die jeweiligen Maße sind in diesem Bild dagestellt.

Alle geometrischen Größen werden in Pixeln eingegeben.

<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/Screen_Layout_ger.png" alt="Screen Parameter" width="700"/>


**Batteriezustand**<br>
Der ESP mit dem 4.7" Display hat eine integrierte Messung des Batteriezustands.<br>
Die 2.7" und 2.9" Variante haben diese nicht.<br>
 Hier kann man die Anzeige des Batteriezustands verhindern, wenn man bei der Option `Abstand Batteriezustand`"0" einträgt.

**Nachrüsten der Messung des Batteriezustands**<br>

Mit einem einfachen Spannungsteiler bestehend aus zwei 100k Widerständen und einem Kondensator lässt sich die Messung des Batteriezustands einfach nachrüsten.
Dies führt zu einer Erhöhung des Ruhestroms im _Deep Sleep Modus_ um ca. 10%.<br>
Im Fall der ESP 2.7" Variante steigt der Ruhestom mit Spannungsteiler von 364µA auf 404µA.

<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/spannungsteiler.png" alt="LilyGo T5 2.7" height="400"/> 
<img src="https://github.com/cultur98/ESP32_Vorrat/blob/main/doc/spannungsteiler_mit.png" alt="LilyGo T5 2.9" height="300"> 

**Update**<br>
Für jedes Display gibt es zwei Varianten der Software.

* Eine die nur mit der [Vorrat-App](https://play.google.com/store/apps/details?id=de.sebag.Vorrat) funktioniert.
Diese Version ermöglicht Online Updates.

* Eine Entwicklerversion, die sowohl die Vorrat-App oder eine Google-Tabelle als Datenquelle nutzt.

## Timeouts<br>
Bei Inaktivität wechselt der ESP in den stromsparenden _Deep Sleep Modus_, was einen 
Betrieb mit einem Akku ermöglicht. <br>
Alle unterstützten ESPs bieten die Möglichkeit einen Lithium-Polymer-Akku anzuschließen.

**Aufwecken per Timer**<br>
Wenn der ESP per Timer geweckt wurde, geht er 30 Sekunden nach der Anzeige der Vorräte in 
den _Deep Sleep Modus_. <br>
In diesem Zeitraum kann man beispielsweise umblättern.

**Umblättern**<br>
Nach dem Umblättern geht der ESP nach 60 Sekunden in den _Deep Sleep Modus_. <br>
Blättert man noch einmal um, wird der Timeout wieder auf 60 Sekunden gesetzt.

**Konfiguration oder Wechsel in den AP Modus**<br>
Wählt man beim Start "Anzeige der Konfiguration" oder "Wechsel in den AP Modus" aus,<br>
dann geht der ESP 5 Minuten nach Inaktivität in den Deep Sleep Modus. <br>

Dies gilt auch für einen frisch programmiern und noch nicht konfigurierten ESP.


## Konfiguration für Platform IO

In der Datei `platformio.ini` kann die Konfiguration für den verwendeten ESP gewählt werdeb.<br>
Mit den Optionen `lilygo21`, `lilygo27`, `lilygo29` und `lilygo47` wird die Software für
die Verwaltung der Lebensmittel mit der  [Vorrat-App](https://play.google.com/store/apps/details?id=de.sebag.Vorrat) erstellt.<br>
Diese Versionen können per Online-Update aktualisiert werden.

Mit den Optionen `lilygo21dev`, `lilygo27dev`, `lilygo29dev` und `lilygo47dev` wird die Software für
die Verwaltung der Lebensmittel mit der [Vorrat-App](https://play.google.com/store/apps/details?id=de.sebag.Vorrat) oder einer Google-Tabelle erstellt.<br>
Bei diesen Versionen ist kein Online-Update möglich.


## Danke!

Ohne zahlreiche Anregungen und Beiträge wäre dieses Projekt nie zustande gekommen.

Besonderer Dank gilt Rainard Buchmann, dem Entwickler der großartigen [Vorrat-App](https://play.google.com/store/apps/details?id=de.sebag.Vorrat). Er hat das Projekt von Anfang an unterstützt und sofort eine Schnittstelle zu den Vorräten entwickelt.<br>
Mit viel Leidenschaft und Sachverstand hat er viele nützliche Funktionen angeregt und zahlreiche Fehler gefunden.<br>
Die 3D-Gehäuse wurden auch von ihm entwickelt.

In der Datei [wifiman.cpp](https://github.com/cultur98/ESP32_Vorrat/blob/main/src/wifiman.cpp) wird eine angepasste Version des Programms   [ESP32_FSWebServer_DRD.ino](https://github.com/khoih-prog/ESP_WiFiManager/blob/master/examples/ESP32_FSWebServer_DRD/ESP32_FSWebServer_DRD.ino) des [ESP_WiFimanagers](https://github.com/khoih-prog/ESP_WiFiManager/) von [Khoi Hoang](https://github.com/khoih-prog/) genutzt.

Die Webschnittstelle zum Konfigurieren der Software basiert auf [Gerald Lechners](https://github.com/GerLech) [WebConfig](https://github.com/GerLech/WebConfig). Eine sehr gute Grundlage, die ich mit wenigen Änderungen an meine Bedürfnisse anpassen konnte.

Die verfügbaren Treiber für die unterstützten Displays waren einfach zu integrieren. <br>
Daher Dank an [olikraus](https://github.com/olikraus/u8g2), [ZinggJM](https://github.com/ZinggJM/GxEPD2) und [LilyGo](https://github.com/Xinyuan-LilyGO/LilyGo-EPD47).
