#include "Camera.h"
#include "esp_camera.h"
#include "base64.h" // Biblioteca para codificação em base64

Camera::Camera() {}

bool Camera::initialize()
{
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
    config.pin_xclk = XCLK_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;

    // Inicializar câmera
    if (psramFound())
    {
        config.frame_size = FRAMESIZE_UXGA; // 1600x1200
        config.jpeg_quality = 10;
        config.fb_count = 2;
    }
    else
    {
        config.frame_size = FRAMESIZE_SVGA; // 800x600
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }

    // Inicializa e verifica se houve erro
    if (esp_camera_init(&config) != ESP_OK)
    {
        Serial.println("Failed to initialize camera");
        return false;
    }
    Serial.println("Camera initialized");
    return true;
}

String Camera::capturePhotoBase64()
{
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return "";
    }

    // Converte a foto para base64
    String photoBase64 = convertToBase64(fb->buf, fb->len);

    // Libera o frame buffer
    esp_camera_fb_return(fb);

    return photoBase64;
}

String Camera::convertToBase64(uint8_t *data, size_t length)
{
    return base64::encode(data, length); // Usa a biblioteca base64
}
