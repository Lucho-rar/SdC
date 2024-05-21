#!/bin/bash

# Imprimir un mensaje de inicio
printf "¡Ejecutando el script para compilar y ejecutar!\n"

# Compilar el archivo assembly
nasm -f elf32 sumarUno.asm -o sumarUno.o -g

# Imprimir un mensaje intermedio
printf "Archivo assembly compilado exitosamente.\n"

# Compilar el archivo C principal
gcc -o result sumarUno.o main.c -m32 -g

# Imprimir un mensaje intermedio
printf "Programa principal compilado exitosamente.\n"

# Compilar la biblioteca compartida
gcc -shared -W -o libsuma.so sumarUno.c sumarUno.o -m32

# Imprimir un mensaje final
printf "Biblioteca compartida compilada exitosamente.\n"

gdb result
