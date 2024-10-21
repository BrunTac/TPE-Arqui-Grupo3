#!/bin/bash

# Nombre del contenedor
NOMBRE="TPE-Arqui-Grupo3"
# Nombre del usuario
MI_USER="isaconde"  # Reemplaza "tu_usuario" con tu nombre de usuario

# Iniciar el contenedor
docker start $NOMBRE

# Ejecutar los comandos de compilación con el usuario especificado
docker exec -u $MI_USER -it $NOMBRE make clean -C /root/Toolchain
docker exec -u $MI_USER -it $NOMBRE make clean -C /root/
docker exec -u $MI_USER -it $NOMBRE make -C /root/Toolchain
docker exec -u $MI_USER -it $NOMBRE make -C /root/

# Detener el contenedor
docker stop $NOMBRE
