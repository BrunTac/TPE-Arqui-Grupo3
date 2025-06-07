#!/bin/bash

# Nombre del contenedor
NOMBRE="TPE_SO"

# Iniciar el contenedor
docker start $NOMBRE

# Ejecutar los comandos de compilación con el usuario especificado
docker exec -u root -it $NOMBRE make clean -C /root/Toolchain
docker exec -u root -it $NOMBRE make clean -C /root/
docker exec -u root -it $NOMBRE make -C /root/Toolchain

# Check if buddy argument is provided
if [ "$1" = "buddy" ]; then
    docker exec -u root -it $NOMBRE make buddy -C /root/
else
    docker exec -u root -it $NOMBRE make -C /root/
fi


