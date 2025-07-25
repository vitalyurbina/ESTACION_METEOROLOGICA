#include <Wire.h>                // Librería para comunicación I2C
#include <SPI.h>                 // Librería para comunicación SPI (no usada aquí pero buena tenerla)
#include <Adafruit_BMP280.h>     // Librería para el sensor BMP280 (presión y temperatura)
#include "DHT.h"                 // Librería para el sensor DHT11 (temperatura y humedad)

// --- Configuración del DHT11 ---
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// --- Configuración del sensor BMP280 ---
Adafruit_BMP280 bmp; // Se usa interfaz I2C por defecto
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();
#define SEALEVELPRESSURE_HPA (1013.25)  // Presión a nivel del mar usada para calcular altitud

// --- Variables para almacenar los datos ---
float temperatura = 0;
float humedad = 0;
float uv = 0;
float presion = 0;
float altitud = 0;

// --- Prototipos de funciones ---
void radiacionUV();
void Altura_Presion();
void temperatura_humedad();

void setup() {
  Serial.begin(9600);        // Inicia comunicación serial
  dht.begin();               // Inicia sensor DHT11
  analogReadResolution(8);   // Resolución ADC de 8 bits para leer voltajes analógicos

  // --- Inicialización del sensor BMP280 ---
  unsigned status;
  status = bmp.begin();      // Inicia el sensor BMP280
  if (!status) {
    while (1) delay(10);     // Si falla, se queda en bucle
  }

  // Configura los parámetros de muestreo del BMP280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                  Adafruit_BMP280::SAMPLING_X2,
                  Adafruit_BMP280::SAMPLING_X16,
                  Adafruit_BMP280::FILTER_X16,
                  Adafruit_BMP280::STANDBY_MS_500);

  // Imprime detalles del sensor (opcional)
  bmp_temp->printSensorDetails();
}

void loop() {
  // Espera a recibir el carácter 'A' desde el puerto serial
  if(Serial.available()){
    char letra = Serial.read();
    if(letra == 'A'){
      // Llama a las funciones para tomar datos de los sensores
      radiacionUV();
      Altura_Presion();
      temperatura_humedad();

      // Construye y envía la cadena CSV por el puerto serial
      String datos = String(temperatura) + "," + String(humedad) + "," + String(uv) + "," + String(presion) + "," + String(altitud);
      Serial.println(datos);
    }
  }
  delay(500); // Espera antes de seguir leyendo
}

// --- Función para leer el voltaje del sensor UV ---
void radiacionUV(){
  float VoltajeUV;
  VoltajeUV = analogRead(A0);              // Lee valor analógico del pin A0
  uv = VoltajeUV * (3.3/255);              // Convierte a voltaje (asumiendo Vref de 3.3V y resolución 8 bits)
}

// --- Función para leer la presión y la altitud ---
void Altura_Presion(){
  float Pressure = bmp.readPressure();     // Lee presión absoluta en Pa
  altitud = bmp.readAltitude(SEALEVELPRESSURE_HPA);  // Calcula altitud
  presion = Pressure / 100.0;              // Convierte a hPa
  delay(1000);                             // Espera para estabilidad
}

// --- Función para leer temperatura y humedad del DHT11 ---
void temperatura_humedad(){
  humedad = dht.readHumidity();            // Lee humedad relativa
  temperatura = dht.readTemperature();     // Lee temperatura en grados Celsius
}
