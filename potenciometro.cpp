#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

int pinPotenciometro = 34;
int ledPin = 14; // Pin del LED

// URL de la API en Heroku
String serverName = "https://herokuiot-4e5cc906e754.herokuapp.com/dispositivos";

unsigned long lastTime = 0;
unsigned long timerDelay = 500;
int lastValorPotenciometro = -1;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // Conectar a la red WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la red WiFi con la dirección IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Temporizador configurado a 5 segundos (variable timerDelay), tomará 5 segundos antes de publicar la primera lectura.");
}

void loop() {
  // Leer el valor del potenciómetro
  int valorPotenciometro = analogRead(pinPotenciometro);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Crear una URL para la API en Heroku, incluyendo el valor del potenciómetro
    String serverPathPotenciometro = serverName + "/2/" + String(valorPotenciometro);

    // Iniciar la conexión HTTP para el potenciómetro
    http.begin(serverPathPotenciometro.c_str());
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Enviar una solicitud HTTP PUT con el valor del potenciómetro
    int httpResponseCodePotenciometro = http.PUT("");

    if (httpResponseCodePotenciometro > 0) {
      Serial.print("Código de respuesta HTTP (Potenciómetro): ");
      Serial.println(httpResponseCodePotenciometro);
      Serial.print("Valor del potenciómetro enviado: ");
      Serial.println(valorPotenciometro);
    } else {
      Serial.print("Código de error (Potenciómetro): ");
      Serial.println(httpResponseCodePotenciometro);
      Serial.print("Valor del potenciómetro no enviado. Error.");
    }

    // Liberar recursos para la solicitud del potenciómetro
    http.end();
  }
}


