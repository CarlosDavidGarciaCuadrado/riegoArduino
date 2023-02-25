

const char* host = "192.168.1.69";  // IP serveur - Server IP
const int   port = 80;            // Port serveur - Server Port
const int   watchdog = 5000;        // Fréquence du watchdog - Watchdog frequency
unsigned long previousMillis = millis();


class envioGet {

  public:
    void metodoGet(int t, int h) {

      unsigned long currentMillis = millis();

      if ( currentMillis - previousMillis > watchdog ) {
        previousMillis = currentMillis;
        WiFiClient client;

        if (!client.connect(host, port)) {
          Serial.println("Fallo al conectar");
          return;
        }

        String url = "/RiegoAutomatizado/index.php?temp=";
        url += t;
        url += "&hum=";
        url += h;

        // Enviamos petición al servidor
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n");
        unsigned long timeout = millis();
        while (client.available() == 0) {
          if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
          }
        }

        // Leemos la respuesta del servidor
        while (client.available()) {
          String line = client.readStringUntil('\r');
          Serial.print(line);
        }
      }
      delay(60000);
    }
};
