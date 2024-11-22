#include <Arduino.h>
#include "HumiditySensor.h"
#include "Comms.h"
#include "Camera.h"

// Configurações de hardware e credenciais
const int PIN_SENSOR_HUMIDITY = 34;
const char *SSID = "YOUR_SSID";
const char *PASSWORD = "YOUR_PASSWORD";
const char *API_TOKEN = "YOUR_TAGOIO_API_TOKEN";

// Intervalos de envio (em milissegundos)
const unsigned long SENSOR_DATA_INTERVAL = 30000;    // Intervalo de leitura e envio dos sensores
const unsigned long PHOTO_CAPTURE_INTERVAL = 300000; // Intervalo de captura e envio de fotos

// Instâncias dos módulos
HumiditySensor humiditySensor(PIN_SENSOR_HUMIDITY);
Comms comms(SSID, PASSWORD, API_TOKEN);
Camera camera;

void setup()
{
  Serial.begin(115200);

  // Inicializa módulos
  humiditySensor.start();
  comms.connectWiFi();

  // Inicializa a câmera e verifica se foi bem-sucedida
  if (camera.initialize())
  {
    Serial.println("Camera initialized successfully.");
  }
  else
  {
    Serial.println("Camera initialization failed.");
  }
}

void loop()
{
  static unsigned long lastSensorDataTime = 0;
  static unsigned long lastPhotoTime = 0;
  unsigned long currentTime = millis();

  // Envio de dados dos sensores no intervalo definido
  if (currentTime - lastSensorDataTime >= SENSOR_DATA_INTERVAL)
  {
    int humidity = humiditySensor.getNormalizedValue();
    int temperature = 25; // Exemplo, pode adicionar um sensor de temperatura real

    comms.sendSensorData(humidity, temperature);
    lastSensorDataTime = currentTime;
  }

  // Envio de fotos no intervalo definido
  if (currentTime - lastPhotoTime >= PHOTO_CAPTURE_INTERVAL)
  {
    String photoBase64 = camera.capturePhotoBase64();
    if (photoBase64 != "")
    {
      comms.sendPhoto(photoBase64);
    }
    lastPhotoTime = currentTime;
  }

  delay(1000); // Intervalo de espera antes da próxima iteração
}
