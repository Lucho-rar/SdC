# Sistemas de Computacion
Trabajo practico N°2
## Stack frame 
Integrantes:
- Robles, Karen Yesica
- Rodriguez, Luciano
- Mangin, Matias
## Objetivo
Se debe diseñar e implementar una interfaz que muestre el índice GINI. La capa superior recuperará la información del banco mundial [WorldBank API](https://api.worldbank.org/v2/en/country/all/indicator/SI.POV.GINI?format=json&date=2011:2020&per_page=32500&page=1&country=%22Argentina%22)
Se recomienda el uso de API Rest y Python. Los datos de consulta realizados deben ser entregados a un programa en C(capa intermedia) que convocará rutinas en ensamblador para que hagan los cálculos deconversión de float a enteros y devuelva el índice de un país como Argentina u otro
sumando uno (+1). Luego el programa en C o python mostrará los datos obtenidos.
Se debe utilizar el stack para convocar, enviar parámetros y devolver resultados. O sea utilizar las convenciones de llamadas de lenguajes de alto nivel a bajo nivel.-
En una primera iteración resolverán todo el trabajo práctico usando c con python sin ensamblador. En la siguiente iteración usarán los conocimientos de ensamblador para
completar el tp.
IMPORTANTE: en esta segunda iteración deberán mostrar los resultados con gdb, para ello pueden usar un programa de C puro. Cuando depuren muestran el estado
del área de memoria que contiene el stack antes, durante y después de la función.

## Desarrollo
 Requerimientos:
- Sistema Operativo Linux.
- Editor de texto como Codium o Sublime Text cualquier otro editor.
- Netwide Assembler: Ensamblador libre para X86.
- Linkeditor, generalmente viene instalado por default en el SO.
- Compilador de lenguaje C, también viene instalado por default
- Depurador


Para la primera fase del proyecto (antes de la implementación del ensamblador), se ha elaborado el siguiente diagrama de secuencias. 
Este diagrama es fundamental para comprender el flujo de interacción entre los diferentes componentes del sistema en esta etapa inicial 
del desarrollo. Además de proporcionar una visión clara de cómo se comunican los distintos elementos del software:


![Diagrama de Secuencia](/practico2/img/diagram.jpg "Fig1.Diagrama de Secuencia")


El código en Python tiene las siguientes funciones:
 
* show_menu(): Imprime un menú en la consola con dos opciones: seleccionar un país o salir.
* get_url(pais): Toma el nombre de un país como entrada y devuelve una URL formateada para hacer una solicitud a la API de World Bank. Esta URL solicita datos sobre el índice de GINI para el país dado, para los años 2011-2020.
* get_suma(num): Esta función llama a una función definida en un archivo compartido de C (libsuma.so),. La función get _suma () toma un número en punto flotante (float) como entrada, lo convierte a un entero (int), y luego le suma 1 al entero resultante y devuelve el resultado de la función C _suma().
* option_1(): Solicita al usuario un país, obtiene la URL correspondiente usando la función get_url(), realiza una solicitud GET a esa URL utilizando la biblioteca requests, procesa la respuesta JSON y muestra las fechas y valores del índice GINI para el país dado. Para cada valor, llama a get_suma() para realizar alguna operación en el valor y luego muestra el resultado junto con la fecha.
