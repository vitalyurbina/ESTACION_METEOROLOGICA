🔌 Código para Arduino Nano ESP32 – Lectura de Sensores Ambientales
Este sketch está diseñado para ejecutarse en un Arduino Nano ESP32. Su función principal es leer datos ambientales desde tres sensores conectados, procesarlos y enviarlos por el puerto serial cuando se recibe una señal de activación externa.

📡 Sensores utilizados
DHT11

Mide temperatura (°C) y humedad relativa (%).

Conectado al pin digital D2.

Sensor de Radiación UV (salida analógica)

Mide el nivel de radiación ultravioleta.

Conectado al pin A0.

Se mide como voltaje (0–3.3 V).

BMP280

Mide presión atmosférica (hPa) y calcula altitud (m).

Conectado por I2C (SDA, SCL).

⚙️ Funcionamiento del código
Al iniciar, el ESP32 configura todos los sensores y el puerto serial.

Permanece a la espera de que le envíen por serial el carácter 'A'.

Al recibirlo:

Lee los valores de temperatura y humedad del DHT11.

Lee el voltaje del sensor UV desde el pin analógico A0 (con resolución de 8 bits).

Lee presión y altitud desde el BMP280.

Combina todos los datos en una sola línea de texto CSV.

Ejemplo de salida por serial:
25.60,52.30,1.21,1011.85,64.37
Orden: temperatura, humedad, voltaje UV, presión (hPa), altitud (m)

🧪 Estructura del flujo
void loop() {
  if (Serial.available()) {
    char letra = Serial.read();
    if (letra == 'A') {
      // Lectura de sensores
      radiacionUV();
      Altura_Presion();
      temperatura_humedad();

      // Envío por Serial
      String datos = String(temperatura) + "," + String(humedad) + "," + String(uv) + "," + String(presion) + "," + String(altitud);
      Serial.println(datos);
    }
  }
}
🛠️ Requisitos
Librerías Arduino necesarias
Adafruit_BMP280

DHT sensor library

Adafruit Unified Sensor

Instálalas desde el Library Manager del IDE de Arduino.

📝 Notas
El valor del sensor UV está calibrado para una referencia de 3.3 V y ADC de 8 bits.

El sistema espera una orden para leer sensores, lo que permite sincronizarlo con un sistema externo como una Orange Pi o una Raspberry Pi.

Ideal para aplicaciones IoT, estaciones meteorológicas, agricultura inteligente o monitoreo ambiental en tiempo real.

