#!/bin/bash

# Imprimir un mensaje de inicio
printf "¡Ejecutando el script para compilar y ejecutar!\n"


gcc -c exampleCyPy/suma.c -o exampleCyPy/suma.o

gcc -shared -W -o exampleCyPy/libsuma.so exampleCyPy/suma.o

# Ejecutar el script de Python
python3 exampleCyPy/req.py

# Imprimir un mensaje de finalización
printf "¡Script completado con éxito!\n"
