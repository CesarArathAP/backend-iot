#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const int ledPin = 14; // Pin para controlar el LED

String serverName = "https://herokuiot-4e5cc906e754.herokuapp.com/dispositivos/1"; // Cambia esto por tu URL de la base de datos

unsigned long lastTime = 0;
unsigned long timerDelay = 100; // Intervalo de tiempo para consultar el estado en milisegundos (10 segundos)

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.println("Conectando a WiFi...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red WiFi con dirección IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer configurado a 5 segundos (variable timerDelay), tardará 5 segundos antes de publicar la primera lectura.");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;

      http.begin(serverName.c_str());
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        String payload = http.getString();
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        Serial.print("Payload: ");
        Serial.println(payload);

        // Verificar el valor obtenido y controlar el LED
        int index = payload.indexOf("\"valor\":\"0\"");
        if (index != -1) {
          digitalWrite(ledPin, LOW); // Apaga el LED si el valor es "0"
          Serial.println("LED apagado");
        } else {
          digitalWrite(ledPin, HIGH); // Enciende el LED si el valor no es "0"
          Serial.println("LED encendido");
        }
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("WiFi Desconectado");
    }
    lastTime = millis();
  }
}
