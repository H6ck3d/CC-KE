#include <iostream>
#include <sstream>
#include <ctime>
#include "../lib/tinyxml2.h"

std::string getCurrentDate()
{
    std::time_t t = std::time(nullptr); // Aktuelle Zeit als Zeitstempel
    std::tm *now = std::localtime(&t);  // Umwandlung in lokale Zeit

    // StringStream zum Formatieren des Datums
    std::ostringstream oss;
    oss << (now->tm_mday) << '-'    // Tag
        << (now->tm_mon + 1) << '-' // Monat (0-basiert, daher +1)
        << (now->tm_year + 1900);   // Jahr (tm_year ist Jahre seit 1900)

    return oss.str(); // Rückgabe des formatierten Datums als String
}

std::string get_templateName()
{
    std::string templateName = "example.xml"; // Standard Template Name
    std::string tmp;                          // Temporäre Variable für ggf. User eingabe

    std::cout << "Standard Name ist \"example\"" << std::endl;
    std::cout << "Bitte Name der Vorlage eingeben:" << std::endl;

    if (std::getline(std::cin, tmp)) // User eingabe getline damit auch return erkannt wird
    {
        if (tmp != "") // Wenn eingabe ist nicht leer
        {
            templateName = tmp + ".xml"; // user eingabe Dateiendung anfügen
        }
    }

    return templateName; // Rückgabe des Wertes
}

std::string get_configName()
{
    std::string date = getCurrentDate();                   // Abfrage von Datum in DD-MM-YYYY Format
    std::string configName = "CC-Config " + date + ".xml"; // Standard String Erstellung aus Text und Datum
    std::string tmp;

    std::cout << "Standard Name ist \"CC-Config DD-MM-YYYY\"" << std::endl;
    std::cout << "Bitte Name fuer die neue Konfiguration eingeben:" << std::endl;
    if (std::getline(std::cin, tmp)) // Abfrage von User eingabe, auch blank
    {
        if (tmp != "") // wenn eingabe nicht leer
        {
            configName = tmp + ".xml"; // Dateiendung anfügen
        }
    }

    return configName; // Rückgabe des Config Namen
}

int *get_channelNumbers() // Funktion zur Abfrage der 15 Kanal Nummern
{
    static int channel[16];

    std::cout << "Bitte Channel Number von 1 - 128 eingeben." << std::endl;

    for (int x = 1; x <= 15; x++) // Schleife welche 15x Wiederholt wird
    {
        int tmp;

        std::cout << "Channel " << x << ": ";
        while (!(std::cin >> tmp) || tmp <= 0 || tmp > 128) // Wiederhole wenn keine Eingabe oder eingabe kleiner gleich 0 oder größer als 128
        {
            std::cin.clear();            // Fehlerflags zurücksetzen
            std::cin.ignore(1000, '\n'); // Eingabepuffer leeren
            std::cout << "Ungültige Eingabe. Bitte eine Zahl zwischen 1 und 128 eingeben: ";
        }

        channel[x] = tmp; // Gültige Eingabe wird gespeichert
    }

    return channel; // Rückgabe des Array mit eingegebenen Channel eingaben
}

void check_channelNumbers(int channelNumbers[]) // Funktion um welche bei Aufruf ausgibt an welcher position welche Nummer im array steht
{
    std::cout << "" << std::endl;
    for (int x = 1; x <= 15; x++) // Wiederhole 15x mal
    {
        std::cout << "Number " << x << ": " << channelNumbers[x] << std::endl; // Gebe wert der position x also Nummer des aktuellen Durchlaufs aus
    }
}

std::string update_xml(int channel[], std::string templateName, std::string configName)
{
    // XML-Dokument laden
    tinyxml2::XMLDocument doc;                                      // Erstellung des Objektes
    tinyxml2::XMLError result = doc.LoadFile(templateName.c_str()); // Laden des Templates
    if (result != tinyxml2::XML_SUCCESS)                            // Wenn Laden des Template nicht erfolgreich
    {
        return "Fehler beim Laden der Datei!"; // Beende und Gebe spezifische Beschreibung zurück
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement("CustomControllerConfig"); // Definierung des Root Elements des Templates
    if (!root)
    {
        return "Kein Root-Element gefunden!"; // Beendigung wenn Root Element nicht vorhanden
    }

    for (int userVal = 1; userVal <= 8; ++userVal) // Standard Template besitzt 8 unterschiedliche User
    {
        std::string userStr = std::to_string(userVal); // Umwandlung von Int to String

        tinyxml2::XMLElement *layout = root->FirstChildElement("Layouts")->FirstChildElement("Layout"); // Eine ebene tiefer gehen zu Einem Layout
        while (layout)                                                                                  // Wieder hole so oft wie es Layouts gibt
        {
            const char *users = layout->Attribute("users");   // Zwischenspeicherung der Aktuellen User Nummer
            if (users && strcmp(users, userStr.c_str()) == 0) // wenn Aktuelle durchlaufsnummer mit der des Layouts übereinstimmt
            {
                // std::cout << "Layout mit users=\"" << userStr << "\" gefunden." << std::endl;

                tinyxml2::XMLElement *widget = layout->FirstChildElement("Widget"); // Wähle Nächstes Unterelement Widget
                while (widget)                                                      // wiederhole so oft wie Widget gefunden wird
                {
                    tinyxml2::XMLElement *tab = widget->FirstChildElement("Tab"); // Finde Tab Elemente
                    if (tab)                                                      // Wenn Tab Element vorhanden
                    {
                        while (tab) // Wiederhole so oft wie Tab vorhanden
                        {
                            tinyxml2::XMLElement *widgetInTab = tab->FirstChildElement("Widget"); // Suche nach Widget Elementen in einem Tab
                            while (widgetInTab)                                                   // Wiederhole für jedes Widget in Tab
                            {
                                const char *channelNumberStr = widgetInTab->Attribute("channelNumber"); // Zwischenspeicherung von aktueller Channel Nummer im Template
                                const char *channelType = widgetInTab->Attribute("channelType");        // Zwischenspeicherung von Aktuellen Channel Typen im Template
                                // std::cout << channelType << std::endl;
                                if (channelNumberStr && strcmp(channelType, "aux")) // Wenn ChannelNummer vorhanden und Channel Typ ist nicht aux
                                {
                                    int channelNumber = std::stoi(channelNumberStr); // Umwandlung der Channel Nummer von String zu Integer

                                    widgetInTab->SetAttribute("channelNumber", channel[channelNumber]); // Aktuallisierung der Channel Nummer der Aktuellen Widget
                                }
                                widgetInTab = widgetInTab->NextSiblingElement("Widget"); // Gehe zu nächstem gefundenen Widget
                            }
                            tab = tab->NextSiblingElement("Tab"); // Gehe zu nächstem vorhandenem Tab
                        }
                    }
                    widget = widget->NextSiblingElement("Widget"); // Gehe zu nächstem gefundenen Widget
                }
            }
            layout = layout->NextSiblingElement("Layout"); // Gehe zu nächstem gefundenen Layout
        }
    }

    doc.SaveFile(configName.c_str()); // Speicherung des im Ram geänderten Templates mit von user eingegebenen Namen

    return "Die Neue Konfiguration wurde als " + configName + " gespeichert"; // Rückgabe von Bestätigung der Bearbeitung und des Namens der neuen Configuration
}

int main()
{
    std::string templateName = get_templateName(); // Aufruf der Funktion get_templateName und speicherung des Rückgabewertes
    std::string configName = get_configName();     // Aufruf der Funktion get_configName und speicherung des Rückgabewertes
    update_xml(get_channelNumbers(), templateName, configName); // Aufruf der Update Funktion mit übergabe der Rückgabewerten der funktionen get_templateName und get_configName
}
