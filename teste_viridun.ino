#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"
#include "base64.h"

// Configuração dos pinos para o modelo AI-Thinker ESP32-CAM
#define PWDN_GPIO_NUM    32
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

// Credenciais Wi-Fi
const char* ssid = "Galaxy S20 FE 5GBF37";
const char* password = "jayb8149";

// Configuração do TagoIO
const char* TAGOIO_URL = "https://api.tago.io/data";
const char* TAGOIO_DEVICE_TOKEN = "73da4536-45cd-4052-a498-83a511d49cc7";

// Configurações da Câmera
int jpeg_quality = 15; // Qualidade inicial do JPEG
const int MAX_CHUNK_SIZE = 5000; // Tamanho máximo de cada chunk em Base64

// Configurações adicionais
#define SOIL_SENSOR_PIN 14  // Pino do sensor de umidade capacitivo
#define RELAY_PIN 12        // Pino do relé
#define FLASH_PIN 4         // Pino do flash embutido (LED embutido no AI-Thinker ESP32-CAM)
int soilThreshold = 500;    // Limite para ativação do relé

// Inicializa a câmera
bool initCamera() {
  Serial.println("Inicializando a câmera...");

  // Garante que o flash está desligado antes da inicialização
  digitalWrite(FLASH_PIN, LOW);

  // Configuração da câmera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA; // 320x240
  config.jpeg_quality = jpeg_quality;
  config.fb_count = 1;

  // Inicializa a câmera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Erro ao inicializar a câmera: 0x%x\n", err);
    return false;
  }

  // Garante que o flash permanece desligado após inicialização
  digitalWrite(FLASH_PIN, LOW);

  Serial.println("Câmera inicializada com sucesso!");
  return true;
}

// Envia a imagem dividida em partes ao TagoIO
void sendPhotoToTagoIO(const uint8_t* imageData, size_t imageLength) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Converte a imagem para Base64
    String imageBase64 = base64::encode(imageData, imageLength);
    int totalChunks = (imageBase64.length() + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE; // Calcula o número total de partes

    for (int i = 0; i < totalChunks; i++) {
      // Extrai um chunk da imagem em Base64
      int start = i * MAX_CHUNK_SIZE;
      int end = start + MAX_CHUNK_SIZE;
      if (end > imageBase64.length()) end = imageBase64.length();

      String chunk = imageBase64.substring(start, end);

      // Monta o payload JSON para a parte atual
      String variableName = String("photo_part_") + String(i + 1);
      String payload = "[{\"variable\": \"" + variableName + "\", \"value\": \"" + chunk + "\"}]";

      // Configura o HTTP Client
      http.begin(TAGOIO_URL);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("Device-Token", TAGOIO_DEVICE_TOKEN);

      // Envia o POST
      int httpResponseCode = http.POST(payload);

      // Verifica o código de resposta HTTP
      if (httpResponseCode > 0) {
        Serial.printf("Chunk %d enviado com sucesso! Código HTTP: %d\n", i + 1, httpResponseCode);
      } else {
        Serial.printf("Erro ao enviar chunk %d. Código HTTP: %d\n", i + 1, httpResponseCode);
      }

      http.end();
      delay(200); // Evita sobrecarregar o servidor com requisições rápidas
    }

    Serial.println("Todas as partes da imagem foram enviadas!");
  } else {
    Serial.println("Wi-Fi desconectado. Não foi possível enviar a foto.");
  }
}

// Envia o valor do sensor de umidade ao TagoIO
void sendSoilMoistureToTagoIO(int soilMoisture) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Monta o payload JSON para enviar o valor do sensor
    String payload = "[{\"variable\": \"soil_moisture\", \"value\": " + String(soilMoisture) + "}]";

    // Configura o HTTP Client
    http.begin(TAGOIO_URL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Device-Token", TAGOIO_DEVICE_TOKEN);

    // Envia o POST
    int httpResponseCode = http.POST(payload);

    // Verifica o código de resposta HTTP
    if (httpResponseCode > 0) {
      Serial.printf("Umidade do solo enviada com sucesso! Código HTTP: %d\n", httpResponseCode);
    } else {
      Serial.printf("Erro ao enviar umidade do solo. Código HTTP: %d\n", httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi desconectado. Não foi possível enviar o valor do sensor de umidade.");
  }
}

void setup() {
  Serial.begin(115200);

  // Configuração do pino do flash para mantê-lo desligado
  pinMode(FLASH_PIN, OUTPUT);
  digitalWrite(FLASH_PIN, LOW);

  // Configuração do pino do relé
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // Configuração do pino do sensor de umidade
  pinMode(SOIL_SENSOR_PIN, INPUT);

  // Conecta ao Wi-Fi
  Serial.printf("Conectando ao Wi-Fi: %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");

  // Inicializa a câmera
  if (!initCamera()) {
    Serial.println("Erro ao inicializar a câmera. Reiniciando...");
    ESP.restart();
  }
}

void loop() {
  // Lê o sensor de umidade
  int soilMoisture = analogRead(SOIL_SENSOR_PIN);
  Serial.printf("Umidade do solo: %d\n", soilMoisture);

  // Envia o valor do sensor de umidade para o TagoIO
  sendSoilMoistureToTagoIO(soilMoisture);

  // Controla o relé e o flash com base no limite de umidade
  if (soilMoisture < soilThreshold) {
    digitalWrite(RELAY_PIN, HIGH); // Ativa o relé
    digitalWrite(FLASH_PIN, HIGH); // Liga o flash
    Serial.println("Relé e Flash ativados!");
  } else {
    digitalWrite(RELAY_PIN, LOW); // Desativa o relé
    digitalWrite(FLASH_PIN, LOW); // Desliga o flash
    Serial.println("Relé e Flash desativados!");
  }

  // Lógica da câmera permanece inalterada
  Serial.println("Capturando imagem...");
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Falha ao capturar imagem");
    return;
  }

  Serial.printf("Imagem capturada com sucesso! Tamanho: %d bytes\n", fb->len);

  // Envia a imagem dividida para o TagoIO
  sendPhotoToTagoIO(fb->buf, fb->len);

  // Libera o frame buffer
  esp_camera_fb_return(fb);

  // Aguarda 10 segundos antes da próxima leitura
  delay(10000);
}
