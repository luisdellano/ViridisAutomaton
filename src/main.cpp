#include <Arduino.h>
#include "Pump.h"

int RELAY_PIN = 5;

Pump pump(RELAY_PIN);

void setup()
{
  // Inicializa a comunicação serial e o módulo de irrigação
  Serial.begin(115200);
  pump.start();
  Serial.println("Sistema de Irrigação Iniciado.");
}

void loop()
{
  // Logica de irrigação baseada em tempo (exemplo: liga por 5 segundos a cada 20 segundos)
  static unsigned long tempoUltimaIrrigacao = 0;
  unsigned long tempoAtual = millis();

  if (tempoAtual - tempoUltimaIrrigacao >= 20000)
  { // A cada 20 segundos
    Serial.println("Ligando a bomba...");
    pump.turnOn();
    delay(5000); // Liga a bomba por 5 segundos
    Serial.println("Desligando a bomba...");
    pump.turnOff();

    tempoUltimaIrrigacao = tempoAtual; // Atualiza o tempo da última irrigação
  }
}
