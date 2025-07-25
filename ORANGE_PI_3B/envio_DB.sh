#!/bin/bash

# 1) Forzar locale numérico (punto decimal)
export LC_NUMERIC=C

# 2) Dispositivo serial
SERIAL_PORT="/dev/ttyACM0"

# 3) Configurar puerto serie
stty -F "$SERIAL_PORT" 9600 cs8 -cstopb -parenb -ixon -crtscts

# 4) Limpiar buffer previo
cat "$SERIAL_PORT" > /dev/null & sleep 1 && kill $!

# 5) Enviar comando al Arduino
echo -n "A" > "$SERIAL_PORT"

# 6) Leer respuesta (timeout 5s)
LINEA=$(timeout 5 cat "$SERIAL_PORT" | head -n 1)

# 7) Verificar lectura
if [ -z "$LINEA" ]; then
  echo "No se recibió dato del puerto serial."
  exit 1
fi

# 8) Separar los valores por coma
IFS=',' read -r TEMP HUM RADIACION PRESION ALTURA <<< "$LINEA"

# 9) Fecha y hora
FECHA=$(date +%F)
HORA=$(date +%T)

# 10) Redondeo con awk (omite decimales)
TEMP_INT=$(awk "BEGIN {printf \"%d\", $TEMP}")
HUM_INT=$(awk "BEGIN {printf \"%d\", $HUM}")
PRESION_INT=$(awk "BEGIN {printf \"%d\", $PRESION}")
# Si RADIACION es por ejemplo 0.21 y quieres almacenarlo como entero 0–255:
RADIACION_INT=$(awk "BEGIN {printf \"%d\", $RADIACION}")

# 11) Insertar en BD local usando usuario dedicado
mysql -u root -p20A024LSALLQA1350T -D SENSORES -e \
"INSERT INTO VALORES (FECHA, HORA, TEMPERATURA, HUMEDAD, PRESION, RADIACION) \
VALUES ('$FECHA', '$HORA', $TEMP_INT, $HUM_INT, $PRESION_INT, $RADIACION_INT);"

# 12) Insertar en BD remota (VPS)
mysql -h 145.223.92.86 -u grafana -pEscuelaNaval2019. -D SENSORES -e \
"INSERT INTO VALORES (FECHA, HORA, TEMPERATURA, HUMEDAD, PRESION, RADIACION) \
VALUES ('$FECHA', '$HORA', $TEMP_INT, $HUM_INT, $PRESION_INT, $RADIACION_INT);"

