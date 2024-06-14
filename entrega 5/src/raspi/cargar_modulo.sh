#!/bin/bash

# Cargar el módulo del kernel si es necesario
insmod gpio_handler.ko

# Crear el archivo de dispositivo /dev/gpiohandler con mknod
mknod /dev/gpiohandler c $(awk '/gpiohandler/ {print $1}' /proc/devices) 0

# Establecer permisos para permitir lectura/escritura a todos los usuarios
chmod o+rw /dev/gpiohandler
