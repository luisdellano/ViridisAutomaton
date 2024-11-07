#ifndef CAMERA_H
#define CAMERA_H

#include <Arduino.h>
#include "esp_camera.h"

class Camera
{
public:
    // Construtor da câmera
    Camera();

    // Inicializa a câmera com as configurações específicas do ESP32-CAM
    bool initialize();

    // Captura uma foto e retorna como uma string base64
    String capturePhotoBase64();

private:
    String convertToBase64(uint8_t *data, size_t length);
};

#endif
