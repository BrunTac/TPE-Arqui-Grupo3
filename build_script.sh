#!/bin/bash

# Nombre del contenedor
NOMBRE="TPE-Arqui-Grupo3"

# Iniciar el contenedor
docker start $NOMBRE

# Ejecutar los comandos de compilación con el usuario especificado
docker exec -u root -it $NOMBRE make clean -C /root/Toolchain
docker exec -u root -it $NOMBRE make clean -C /root/
docker exec -u root -it $NOMBRE make -C /root/Toolchain
docker exec -u root -it $NOMBRE make -C /root/

# Detener el contenedor
docker stop $NOMBRE
