
//-----------------------------------------Librerias-------------------------------------------//

    #include "PeticionHttp.h"                                                                                                                   //Librería para la conexión a la red en modo cliente.
    #include "DHT.h"                                                                                                                                 //Librería para el dispositivo DTH que captura la temperatura y humedad del ambiente.
    #include "ConexionEthernet.h"                                                                                                                    //Clase para la conexión a internet.

//------------------------------------------variables------------------------------------------//

    #define DHTTYPE DHT11
    #define dht_dpin 0                                                                                                                               //D3
    DHT dht(dht_dpin, DHTTYPE);
    ConexionEthernet Conexion;
    PeticionHttp Peticion;
    int grifo = 14;                                                                                                                                  //D5
    int humT = 0;
    int humA = 0;
    int tempA = 0;
    boolean controlGrifo = false;
    String url = "http://192.168.1.13:1020/IrrigationSystem/Datos/save";
    unsigned long inicioRiego, finalRiego;

//--------------------------------------Inicializador-----------------------------------------//

  void setup() {
    Serial.begin(9600);
    pinMode(grifo, OUTPUT);
    digitalWrite(grifo, LOW);
    Conexion.conexion();
    dht.begin();
    Serial.println("Humedad y Temperatura\n\n");
    delay(700);
  }

//------------------------------------------bucle---------------------------------------------//

  void loop() {
    int humedad = analogRead(A0);
    int humedadAmbiente = dht.readHumidity();
    int temperaturaAmbiente = dht.readTemperature();
    int humedadTerreno = map(humedad, 1023, 0, 0, 100);
    Serial.print("la temperatura es: ");
    Serial.println(temperaturaAmbiente);
    Serial.print("la humedad del ambiente es: ");
    Serial.println(humedadAmbiente);
    Serial.print("la humedad del suelo es: ");
    Serial.println(humedadTerreno);
    leerVariablesAmbiente(url, humedadTerreno, humedadAmbiente, temperaturaAmbiente, grifo, controlGrifo);
    delay(15000);
  }
  
//------------------metodo para leer variables y enviarlas por metodo http---------------------//

  void leerVariablesAmbiente(String url, int humTerreno, int humAmbiente, int tempAmbiente, int grifo, boolean controlGrifo){
     String estadoRiego = "";
     if((humT != humTerreno)||(tempA != tempAmbiente)){
       humT = humTerreno;
       tempA = tempAmbiente;
       if(tempA > 25 && humT < 50 && !controlGrifo){
          controlGrifo = true;
          controlRiego(controlGrifo, grifo);
          inicioRiego = millis();
          estadoRiego = "Inicio";
          Peticion.peticionPost(url, tempA, humT, humA, estadoRiego, 0);
        }else if(tempA <= 25 && humT >= 50 && controlGrifo){
            controlGrifo = false;
            controlRiego(controlGrifo, grifo);
            finalRiego = millis();
            estadoRiego = "Final";
            long tiempoRiego = finalRiego-inicioRiego;
            Peticion.peticionPost(url, tempA, humT, humA, estadoRiego, tiempoRiego);
          }
      }
  }
  
//---------------------metodo para el control de abrir y cerrar el grifo-----------------------//

  void controlRiego(boolean controlGrifo, int grifo){
      if(controlGrifo){ 
          digitalWrite(grifo, HIGH);                                                                                                                   //abrir grifo
          Serial.println("Regando Terreno del Cultivo de Tomates....");
        }else{
            digitalWrite(grifo, LOW);                                                                                                                 //cerrar grifo
            Serial.println("Riego detenido....");             
          }
    }
