#include <WiFi.h>
#include "DHT.h"
#include "HTTPClient.h"

//--- Configuracoes Wi-Fi---
const char* ssid = "SUA_REDE";       // Nome da rede Wi-Fi
const char* password = "SUA_SENHA";  // Senha da rede Wi-Fi

//--- Configuracoes ThingSpeak---
String apiKey = "SUA_WRITE_API_KEY";  // Chave de escrita do canal
String server = "http://api.thingspeak.com/update";

//--- Configuracao do sensor DHT22---
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  //Conectar ao Wi-Fi
  WiFi.brgin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");]
  }
  Serial.println("\nWi-Fi conectado com sucesso!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP())
}

void loop() {
  if (Wifi.status() == WL_CONNECTED) {
    // Leitura do sensor
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t) && !isnan(h)) {
      // Monta URL de envio
      HTTPClient http;
      String url = server + "?api_key=" + apiKey + "&field1=" + String(t, 2) + "&field2=" + String(h, 2);
      http.begin(url);

      // Envia requisicao GET
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.printf("Dados enviados! HTTP Code: %d\n", httpCode);
      } else {
        Serial.printf("Erro HTTP: %d\n", httpCode);
      }
      http.end();
    } else {
      Serial.println("Falha na leitura do DHT22.");
    }
  } else {
    Serial.println("Wi-Fi desconectado. Tentando reconectar...");
    WiFi.begin(ssid, password);
  }

  delay(20000);  // Intervalo de envio (>= 15 s)
}