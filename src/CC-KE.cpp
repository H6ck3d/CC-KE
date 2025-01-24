#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <ctime>
#include <curl/curl.h>
#include "../lib/tinyxml2.h"

// Function to write the response data to a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch (std::bad_alloc& e) {
        // Handle memory problem
        return 0;
    }
    return newLength;
}

std::map<std::string, std::string> loadConfig(const std::string &fileName)
{
    std::ifstream file(fileName);
    std::map<std::string, std::string> config;
    std::string line;

    while (std::getline(file, line))
    {
        size_t pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            config[key] = value;
        }
    }
    return config;
}

std::string getCurrentDate()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);
    std::ostringstream oss;
    oss << (now->tm_mday) << '-' << (now->tm_mon + 1) << '-' << (now->tm_year + 1900);
    return oss.str();
}

std::string get_templateName()
{
    std::string templateName = "example.xml";
    std::string tmp;
    std::cout << "Standard Name ist \"example\"" << std::endl;
    std::cout << "Bitte Name der Vorlage eingeben:" << std::endl;

    if (std::getline(std::cin, tmp) && !tmp.empty())
    {
        templateName = tmp + ".xml";
    }
    return templateName;
}

std::string get_configName()
{
    std::string date = getCurrentDate();
    std::string configName = "CC-Config " + date + ".xml";
    std::string tmp;

    std::cout << "Standard Name ist \"CC-Config DD-MM-YYYY\"" << std::endl;
    std::cout << "Bitte Name fuer die neue Konfiguration eingeben:" << std::endl;
    if (std::getline(std::cin, tmp) && !tmp.empty())
    {
        configName = tmp + ".xml";
    }
    return configName;
}

int *get_channelNumbers()
{
    static int channel[16];
    std::cout << "Bitte Channel Number von 1 - 128 eingeben." << std::endl;

    for (int x = 1; x <= 15; x++)
    {
        int tmp;
        std::cout << "Channel " << x << ": ";
        while (!(std::cin >> tmp) || tmp <= 0 || tmp > 128)
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Ungültige Eingabe. Bitte eine Zahl zwischen 1 und 128 eingeben: ";
        }
        channel[x] = tmp;
    }
    return channel;
}

std::string update_xml(int channel[], std::string templateName, std::string configName)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(templateName.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        return "Fehler beim Laden der Datei!";
    }

    tinyxml2::XMLElement *root = doc.FirstChildElement("CustomControllerConfig");
    if (!root)
    {
        return "Kein Root-Element gefunden!";
    }

    for (int userVal = 1; userVal <= 8; ++userVal)
    {
        std::string userStr = std::to_string(userVal);
        tinyxml2::XMLElement *layout = root->FirstChildElement("Layouts")->FirstChildElement("Layout");
        while (layout)
        {
            const char *users = layout->Attribute("users");
            if (users && strcmp(users, userStr.c_str()) == 0)
            {
                tinyxml2::XMLElement *widget = layout->FirstChildElement("Widget");
                while (widget)
                {
                    tinyxml2::XMLElement *tab = widget->FirstChildElement("Tab");
                    if (tab)
                    {
                        while (tab)
                        {
                            tinyxml2::XMLElement *widgetInTab = tab->FirstChildElement("Widget");
                            while (widgetInTab)
                            {
                                const char *channelNumberStr = widgetInTab->Attribute("channelNumber");
                                const char *channelType = widgetInTab->Attribute("channelType");
                                if (channelNumberStr && strcmp(channelType, "aux"))
                                {
                                    int channelNumber = std::stoi(channelNumberStr);
                                    widgetInTab->SetAttribute("channelNumber", channel[channelNumber]);
                                }
                                widgetInTab = widgetInTab->NextSiblingElement("Widget");
                            }
                            tab = tab->NextSiblingElement("Tab");
                        }
                    }
                    widget = widget->NextSiblingElement("Widget");
                }
            }
            layout = layout->NextSiblingElement("Layout");
        }
    }

    doc.SaveFile(configName.c_str());
    return "Die Neue Konfiguration wurde als " + configName + " gespeichert";
}

void performCurlRequest(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        } else {
            std::cout << "Response data: " << readBuffer << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

void sendMail(const std::string& configName) {
    CURL* curl;
    CURLcode res;
    struct curl_slist* recipients = NULL;
    auto config = loadConfig("config.txt");
    std::string from = config.at("SMTP_USER");
    std::string to = "recipient@example.com"; // Replace with the actual recipient
    std::string subject = "Subject: New Configuration File";
    std::string body = "The new configuration file " + configName + " has been created.";

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, config.at("SMTP_SERVER").c_str());
        curl_easy_setopt(curl, CURLOPT_USERNAME, config.at("SMTP_USER").c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, config.at("SMTP_PASS").c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

        recipients = curl_slist_append(recipients, to.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        std::string data = "To: " + to + "\r\n" + subject + "\r\n\r\n" + body;
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_READDATA, &data);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            std::cout << "Mail wurde erfolgreich gesendet!" << std::endl;
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

int main()
{
    std::map<std::string, std::string> config = loadConfig("config.txt");

    std::string templateName = get_templateName();
    std::string configName = get_configName();
    update_xml(get_channelNumbers(), templateName, configName);
    char sendMailChoice;
    std::cout << "Möchten Sie die neue Konfiguration per E-Mail senden? (y/n)" << std::endl;
    std::cin >> sendMailChoice;
    if (sendMailChoice == 'y')
    {
        sendMail(configName);
    }
    return 0;
}
