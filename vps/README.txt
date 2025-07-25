
Informe Técnico – Configuración de VPS como Servidor de Visualización IoT
Sistema Operativo: Debian/Ubuntu Server
------------------------------------------------------------

Este VPS se utiliza para centralizar la visualización remota de datos IoT provenientes de sensores conectados a una Orange Pi 3B. Para ello, se instaló un servidor de base de datos (MySQL/MariaDB) y la herramienta de visualización Grafana.

Instalación de MySQL (MariaDB)
-------------------------------
MariaDB es un sistema de gestión de bases de datos relacional que ofrece compatibilidad con MySQL.

Instalación:

$ sudo apt update
$ sudo apt install mariadb-server

Configuración inicial:

$ sudo mysql_secure_installation

Durante este proceso se recomienda:
- Establecer una contraseña para el usuario root.
- Eliminar usuarios anónimos.
- Restringir el acceso remoto al usuario root.
- Eliminar la base de datos de prueba.
- Recargar los privilegios.

Crear base de datos y usuario:
------------------------------

$ sudo mariadb -u root -p

CREATE DATABASE sensores;
CREATE USER 'grafana'@'%' IDENTIFIED BY 'clave_segura';
GRANT ALL PRIVILEGES ON sensores.* TO 'grafana'@'%';
FLUSH PRIVILEGES;
EXIT;

Permitir conexiones remotas:
----------------------------

Editar el archivo de configuración de MariaDB:

$ sudo nano /etc/mysql/mariadb.conf.d/50-server.cnf

Modificar la línea:

bind-address = 0.0.0.0

Reiniciar el servicio:

$ sudo systemctl restart mariadb

Instalación de Grafana
----------------------

Agregar el repositorio de Grafana:

$ sudo apt install -y software-properties-common
$ sudo apt install -y gnupg2 curl

$ curl -fsSL https://packages.grafana.com/gpg.key | sudo gpg --dearmor -o /etc/apt/trusted.gpg.d/grafana.gpg
$ echo "deb [signed-by=/etc/apt/trusted.gpg.d/grafana.gpg] https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list

Instalar Grafana:

$ sudo apt update
$ sudo apt install grafana

Habilitar e iniciar el servicio:

$ sudo systemctl enable grafana-server
$ sudo systemctl start grafana-server

Acceso a Grafana
-----------------
Una vez instalado, se puede acceder desde un navegador web ingresando la IP pública del VPS seguido del puerto 3000:

http://<IP_del_VPS>:3000

Credenciales por defecto:
Usuario: admin
Contraseña: admin

Luego de iniciar sesión se recomienda cambiar la contraseña y configurar el acceso a la base de datos `sensores` con el usuario `grafana`.
