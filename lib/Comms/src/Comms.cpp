#include "Comms.h"

Comms::Comms(const char *ssid, const char *password, const char *apiToken)
    : _ssid(ssid), _password(password), _apiToken(apiToken) {}

void Comms::connectWiFi()
{
    WiFi.begin(_ssid, _password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
}

void Comms::sendSensorData(int humidity, int temperature)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        _httpClient.begin("https://api.tago.io/data");
        _httpClient.addHeader("Content-Type", "application/json");
        _httpClient.addHeader("Device-Token", _apiToken);

        // Cria o JSON com os dados de umidade e temperatura
        String payload = "[{\"variable\": \"humidity\", \"value\": " + String(humidity) + "},";
        payload += "{\"variable\": \"temperature\", \"value\": " + String(temperature) + "}]";

        int httpResponseCode = _httpClient.POST(payload);
        if (httpResponseCode > 0)
        {
            Serial.println("Sensor data sent successfully.");
        }
        else
        {
            Serial.print("Error sending data: ");
            Serial.println(_httpClient.errorToString(httpResponseCode));
        }

        _httpClient.end();
    }
    else
    {
        Serial.println("WiFi not connected. Unable to send sensor data.");
    }
}

void Comms::sendPhoto(const String &photoBase64)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        _httpClient.begin("https://api.tago.io/data");
        _httpClient.addHeader("Content-Type", "application/json");
        _httpClient.addHeader("Device-Token", _apiToken);

        // Cria o JSON com a foto em formato base64
        String payload = "[{\"variable\": \"photo\", \"value\": \"" + photoBase64 + "\"}]";

        int httpResponseCode = _httpClient.POST(payload);
        if (httpResponseCode > 0)
        {
            Serial.println("Photo sent successfully.");
        }
        else
        {
            Serial.print("Error sending photo: ");
            Serial.println(_httpClient.errorToString(httpResponseCode));
        }

        _httpClient.end();
    }
    else
    {
        Serial.println("WiFi not connected. Unable to send photo.");
    }
}
