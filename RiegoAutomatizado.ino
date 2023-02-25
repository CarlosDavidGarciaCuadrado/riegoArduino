
//--------------Librerias------------------//
    #include <ESP8266HTTPClient.h>                                             //Librería para la conexión a la red en modo cliente.
    #include "DHT.h"                                                           //Librería para el dispositivo DTH que captura la temperatura y humedad del ambiente.
    #include "ConexionEthernet.h"                                              //Clase para la conexión a internet.


//-------------variables------------------//
    #define DHTTYPE DHT11
    #define dht_dpin 0
    DHT dht(dht_dpin, DHTTYPE);
    ConexionEthernet Conexion;
    int humT = 0;
    int humA = 0;
    int tempA = 0;
    String url = "http://192.168.1.13:1020/IrrigationSystem/Datos/save";
    //String url = "http://192.168.1.70:1020/IrrigationSystem/Datos/save";  

//-----------Inicializador---------------//
void setup() {
  Serial.begin(9600);
  Conexion.conexion();
  dht.begin();
  Serial.println("Humedad y Temperatura\n\n");
  delay(700);
}


//-----------código bucle-----------------//
void loop() {
  int hum = analogRead(A0);
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int humedad = map(hum, 1023, 0, 0, 100);
  
  HTTPClient http;
  WiFiClient client;
  http.begin(client, url);
  http.addHeader("Content-Type","application/x-www-form-urlencoded");                                                          //Esta linea no se modifica.

  String postData = "tempAmbiente="+ String(t)+"&humTerreno="+String(humedad)+"&humAmbiente="+ String(h)+"&phTerreno=0";
  int httpCode = http.POST(postData);                                                                                          //Se envia solicitud al servidor.

  if(httpCode > 0){
    Serial.println("el servidor respondio: ");
    if(httpCode == 200){
      String respuesta = http.getString();
      Serial.println(respuesta);
      }else{
        Serial.println(httpCode);
        }
    }                                                                                         //Se obtiene respuesta.

  Serial.print("la temperatura es: ");
  Serial.println(t);
  Serial.print("la humedad del ambiente es: ");
  Serial.println(h); 
  Serial.print("la humedad del suelo es: ");
  Serial.println(humedad);

  http.end();                                                                 //Se finaliza la conexión.
  variablesAmbiente(humedad, h, t);
  delay(15000);

}

  void variablesAmbiente(int humTerreno, int humAmbiente, int tempAmbiente){
     if((humT != humTerreno)||(humA != humAmbiente)||(tempA != tempAmbiente)){
       humT = humTerreno;
       humA = humAmbiente;
       tempA = tempAmbiente;
      }
      if(tempA > 25 && humT < 50){
          Serial.println("Regando Terreno del Cultivo de Tomates....");
        }else{
            Serial.println("Variables de ambiente optimas....");  
          }
  }
