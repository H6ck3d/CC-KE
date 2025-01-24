# CC-KE - Custom Control Konfiguration Editor

Dieses Projekt ist ein C++-Programm, das eine XML-Konfigurationsdatei für das Allen & Heath Custom Control erstellt. Es ermöglicht die Anpassung von In-Ear-Monitoring-Konfigurationen für bis zu 8 Benutzer, wobei die ersten 1–8 Mono-Aux-Mixes des Mischpults verwendet werden.

## Features

- **Datumserstellung**: Automatische Generierung des aktuellen Datums im Format `DD-MM-YYYY` für die Benennung der Konfigurationsdatei.
- **Vorlagenname**: Benutzer können den Namen der zu bearbeitenden Vorlage angeben.
- **Konfigurationsname**: Benutzerdefinierter Name für die erstellte Konfigurationsdatei mit einem Standardvorschlag.
- **Kanalnummern**: Eingabe von 15 Kanalnummern mit Validierung (Zahlen zwischen 1 und 128).
- **XML-Bearbeitung**: Aktualisierung der Kanalnummern im Template basierend auf den Benutzereingaben.
- **Dateispeicherung**: Speicherung der bearbeiteten XML-Datei unter dem benutzerdefinierten Namen.

## Abhängigkeiten

- C++ Compiler (z. B. c++)
- [tinyxml2](https://github.com/leethomason/tinyxml2): Eine kleine und einfache XML-Bibliothek.

## Aufbau des Programms

### Funktionen

1. **`getCurrentDate()`**  
   Gibt das aktuelle Datum im Format `DD-MM-YYYY` zurück.

2. **`get_templateName()`**  
   Fragt den Benutzer nach dem Namen der Vorlage und fügt `.xml` an. Falls keine Eingabe erfolgt, wird `example.xml` verwendet.

3. **`get_configName()`**  
   Fragt den Benutzer nach dem Namen der neuen Konfigurationsdatei. Falls keine Eingabe erfolgt, wird ein Standardname im Format `CC-Config DD-MM-YYYY.xml` verwendet.

4. **`get_channelNumbers()`**  
   Fragt den Benutzer nach 15 Kanalnummern und stellt sicher, dass diese zwischen 1 und 128 liegen.

5. **`update_xml()`**  
   - Lädt das Template.
   - Aktualisiert die Kanalnummern basierend auf den Benutzereingaben.
   - Speichert die bearbeitete XML-Datei unter dem angegebenen Namen.

### `main()`-Funktion

- Fragt den Namen der Vorlage und der Konfigurationsdatei ab.
- Ruft die Funktionen zur Eingabe der Kanalnummern und zur Bearbeitung des Templates auf.

## Kompilierung und Ausführung

#### Windows

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
```

#### Linux

```bash
mkdir build
cd build
cmake ..
make
```

#### macOS

```bash
mkdir build
cd build
cmake ..
make
```

### Ausführung

```bash
./CC-KE
```

Das Programm führt Sie durch die Eingaben und erstellt eine neue Konfigurationsdatei basierend auf dem angegebenen XML-Template.

## Hinweise zur In-Ear-Monitoring-Konfiguration

Für die Einrichtung eines In-Ear-Monitoring-Systems mit dem Allen & Heath Custom Control sind folgende Schritte zu beachten:

1. **Benutzer anlegen**: Legen Sie im Mischpult bis zu 8 Benutzer an, die das In-Ear-Monitoring nutzen sollen.

2. **Aux-Mixes konfigurieren**: Weisen Sie den Benutzern die ersten 1–8 Mono-Aux-Mixes zu. Diese Aux-Mixes dienen als separate Monitorwege für jeden Benutzer.

3. **Custom Control App**: Verwenden Sie die [Custom Control App](https://www.allen-heath.com/hardware/controllers/custom-control/), um benutzerdefinierte Steuerungsoberflächen für die Benutzer zu erstellen. Die App ermöglicht es, individuelle Mixe direkt vom Smartphone oder Tablet aus zu steuern. ([allen-heath.com](https://www.allen-heath.com/hardware/controllers/custom-control/?utm_source=chatgpt.com))

4. **ME-1 Personal Mixer**: Alternativ können Sie den [ME-1 Personal Mixer](https://www.allen-heath.com/ahproducts/me-1/) einsetzen, der speziell für In-Ear-Monitoring entwickelt wurde. Er bietet jedem Musiker die Möglichkeit, seinen eigenen Mix zu erstellen und anzupassen. ([allen-heath.com](https://www.allen-heath.com/content/uploads/2023/06/ME-1-User-Guide-AP8997_3.pdf?utm_source=chatgpt.com))

## Ressourcen

- [Custom Control – Allen & Heath](https://www.allen-heath.com/hardware/controllers/custom-control/)
- [ME-1 Personal Mixer – Allen & Heath](https://www.allen-heath.com/ahproducts/me-1/)

## Lizenz

Dieses Projekt steht unter der [GPLv3-Lizenz](https://www.gnu.org/licenses/gpl-3.0.html).

## Autor

- Erstellt von [H6ck3d](https://github.com/H6ck3d), 2025
