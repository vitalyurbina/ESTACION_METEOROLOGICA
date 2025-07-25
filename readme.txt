Sistema distribuido de monitoreo ambiental con Orange Pi, Arduino Nano ESP32 y VPS
Este proyecto está dividido en tres componentes principales, cada uno con una función específica dentro de un sistema IoT diseñado para recolectar, transmitir, almacenar y visualizar datos de sensores ambientales en tiempo real.

📌 Estructura del proyecto
1. Arduino Nano ESP32 – Captura de datos
El Arduino tiene conectados tres sensores:

DHT11 – Mide temperatura y humedad.

Sensor UV – Mide el índice de radiación ultravioleta.

Sensor de presión – Mide presión atmosférica.

Cada 5 segundos, el ESP32 toma una lectura de todos los sensores y la envía por USB (Serial) a la Orange Pi en formato de texto separado por comas (CSV). Ejemplo:
24.5,60.2,1.3,1012.7

2. Orange Pi 3B – Recepción y reenvío
La Orange Pi se encarga de:

Leer los datos del ESP32 a través del puerto /dev/ttyACM0.

Procesar la cadena recibida.

Insertar los valores en una base de datos MySQL remota, alojada en un VPS.

Este paso se ejecuta mediante un script en BASH, no se usö python porque es ineficiente, y este srcript corre en segundo plano.

3. VPS (Servidor en la nube) – Almacenamiento y visualización
El VPS ejecuta:

MySQL – Base de datos donde se registran las mediciones con un timestamp.

Grafana – Herramienta de visualización conectada a MySQL, que muestra los datos en tiempo real mediante dashboards personalizables.

💡 Finalidad del proyecto
Este sistema permite monitorear condiciones ambientales desde cualquier parte, con bajo consumo de energía, bajo costo y usando herramientas de software libre. Puede ser útil para proyectos de clima local,
estaciones meteorológicas caseras o entornos de monitoreo ambiental urbano o rural.

