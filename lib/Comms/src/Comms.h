#ifndef COMMS_H
#define COMMS_H

#include <WiFi.h>
#include <HTTPClient.h>

class Comms
{
public:
    // Construtor que define o SSID e senha do Wi-Fi e o token de API do TagoIO
    Comms(const char *ssid, const char *password, const char *apiToken);

    // Conecta ao Wi-Fi
    void connectWiFi();

    // Envia dados dos sensores (exemplo: umidade, temperatura)
    void sendSensorData(int humidity, int temperature);

    // Envia uma foto em formato base64
    void sendPhoto(const String &photoBase64);

private:
    const char *_ssid;
    const char *_password;
    const char *_apiToken;
    HTTPClient _httpClient;
};

#endif
