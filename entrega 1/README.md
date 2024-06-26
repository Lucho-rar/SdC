# Sistemas de Computación - Entrega #1
## _Time Profiling_

[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](https://nodesource.com/products/nsolid)

## Introducción
Se utiliza la herramienta gprof. Para ello es necesario:
- Habilitar profiling durante la compilación.
- Ejecutar el código del programa para producir los datos de perfil.
- Ejecutar la herramienta gprof en el archivo de datos de generación de perfiles.

Este último paso finaliza con la generación de un archivo legible para las personas. Este archivo, contiene un par de tablas (perfil plano y gráfico de llamadas) así como otros detalles adicionales. 
El perfil plano proporciona una descripción general de tiempo de las funciones, la cantidad de veces que se llaman, etc.
El gráfico de llamadas muestra para cada función, que funciones la llaman, que funciones llama y cuantas veces.

Se usa un código de ejemplo test_gprof.c
```
#include<stdio.h>

void new_func1(void);

void func1(void)
{
    printf("\n Inside func1 \n");
    int i = 0;

    for(;i<0xffffffff;i++);
    new_func1();

    return;
}

static void func2(void)
{
    printf("\n Inside func2 \n");
    int i = 0;

    for(;i<0xffffffaa;i++);
    return;
}

int main(void)
{
    printf("\n Inside main()\n");
    int i = 0;

    for(;i<0xffffff;i++);
    func1();
    func2();

    return 0;
}

void new_func1(void)
{
    printf("\n Inside new_func1()\n");
    int i = 0;

    for(;i<0xffffffee;i++);

    return;
}

```
##  Paso 1: creación de perfiles habilitada durante la compilación
En este primer paso, debemos asegurarnos de que la generación de perfiles esté habilitada cuando se complete la compilación del código. Esto es posible al agregar la opción '-pg' en el paso de compilación.

- pg : Generate extra code to write profile information suitable for the analysis program gprof. You must use this option when compiling the source files you want data about, and you must also use it when linking.


`
$ gcc -Wall -pg test_gprof.c test_gprof_new.c -o test_gprof
`

## Paso 2: Ejecutar el código
`
./test_gprof
`

```
Inside main()
Inside func1 
Inside new_func1()
Inside func2 
```

Esto genera un archivo llamado gmon.out en el directorio de trabajo actual

## Paso 3: Ejecutar la herramienta gprof

En este paso, la herramienta gprof se ejecuta con el nombre del ejecutable y el 'gmon.out' generado anteriormente como argumento. Esto produce un archivo de análisis que contiene toda la información de perfil deseada.
`
gprof test_gprof gmon.out > analysis.txt
`
```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 34.26      7.90     7.90        1     7.90    15.56  func1
 33.22     15.56     7.66        1     7.66     7.66  new_func1
 32.18     22.98     7.42        1     7.42     7.42  func2
  0.35     23.06     0.08                             main

```
```
		     Call graph (explanation follows)


granularity: each sample hit covers 4 byte(s) for 0.04% of 23.06 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.08   22.98                 main [1]
                7.90    7.66       1/1           func1 [2]
                7.42    0.00       1/1           func2 [4]
-----------------------------------------------
                7.90    7.66       1/1           main [1]
[2]     67.5    7.90    7.66       1         func1 [2]
                7.66    0.00       1/1           new_func1 [3]
-----------------------------------------------
                7.66    0.00       1/1           func1 [2]
[3]     33.2    7.66    0.00       1         new_func1 [3]
-----------------------------------------------
                7.42    0.00       1/1           main [1]
[4]     32.2    7.42    0.00       1         func2 [4]
-----------------------------------------------
```


#### Customizar la info con flags
##### _Suprima la impresión de funciones declaradas estáticamente (privadas) usando -a_
`
gprof -a test_gprof gmon.out > analysis.txt
`

```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 66.44     15.32    15.32        2     7.66    11.49  func1
 33.22     22.98     7.66        1     7.66     7.66  new_func1
  0.35     23.06     0.08                             main

		     Call graph (explanation follows)


granularity: each sample hit covers 4 byte(s) for 0.04% of 23.06 seconds

index % time    self  children    called     name
                                                 <spontaneous>
[1]    100.0    0.08   22.98                 main [1]
               15.32    7.66       2/2           func1 [2]
-----------------------------------------------
               15.32    7.66       2/2           main [1]
[2]     99.7   15.32    7.66       2         func1 [2]
                7.66    0.00       1/1           new_func1 [3]
-----------------------------------------------
                7.66    0.00       1/1           func1 [2]
[3]     33.2    7.66    0.00       1         new_func1 [3]
-----------------------------------------------

```

Se observa que no hay info de func2 que está declarada como estática.

##### _Elimine los textos detallados usando -b e imprimir solo perfil plano con -p_
`
gprof -p -b test_gprof gmon.out > analysis.txt
`
```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
 34.26      7.90     7.90        1     7.90    15.56  func1
 33.22     15.56     7.66        1     7.66     7.66  new_func1
 32.18     22.98     7.42        1     7.42     7.42  func2
  0.35     23.06     0.08                             main
```

##### _Imprimir información relacionada con funciones específicas en perfil plano_
`
gprof -pfunc1 -b test_gprof gmon.out > analysis.txt
`
```
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls   s/call   s/call  name    
100.00      7.90     7.90        1     7.90     7.90  func1
```

#### Interpretación de la salida gprof
El perfil plano muestra la cantidad total de tiempo dedicado a la ejecución del programa de cada funcion.Las funciones se ordenan por orden decreciente por tiempo de ejecución empleado en ellas. En el encabezado aparece una declaración que indica cuanto tiempo equivale una muestra.Significado de los campos:
- ‘% time’ Este es el porcentaje del tiempo total de ejecución que el programa dedicó a cada función. Estos deben sumar el 100% entre todos.
- ‘cumulative seconds’ Este es el número total de segundos acumulados que el ordenador dedicó a ejecutar estas funciones, más el tiempo dedicado a todas las funciones anteriores a esta en esta tabla.
- ‘self seconds’ Este es el número de segundos contabilizados a esta función sola. El listado del perfil plano se ordena por este número.
- ‘calls’ Este es el número total de veces que se invocó la función. Si la función no se llamó nunca, o el número de veces que fue llamado no se puede determinar (probablemente porque la función no se compiló con el perfilado habilitado), el campo de las llamadas (‘calls’) permanecerá en blanco.
- ‘self ms/call’ Esta cifra representa el número de milisegundos de media gastados en esta función por cada llamada, si esta función es perfilada. De lo contrario, este campo permanecerá en blanco para esta función.
- ‘total ms/call’ total de ms / llamada Est cifra representa el número de milisegundos de media gastados en esta función y sus descendientes por cada llamada.

El grafo de llamadas muestra qué funciones son llamadas por qué otras, y cuanto tiempo cada función usó cuando fue llamada por éstas.
Las líneas de guiones dividen esta tabla en ‘entradas’, una para cada función. Cada entrada tiene una o más líneas.
En cada entrada, la línea principal es la que comienza con un número de índice entre corchetes. Al final de esta línea aparece el nombre de la función a la que esta entrada se refiere. Las líneas anteriores de la entrada describen a los llamadores de esta función y las líneas siguientes describen sus subrutinas (también llamados ‘hijos’ cuando se habla del grafo de llamadas). Las entradas se ordenan por el tiempo gastado en la función y sus subrutinas.
- ‘index’Las entradas están numeradas con enteros consecutivos. Por lo tanto, cada función tiene un número de índice, que aparece al principio de su línea principal.
- ‘% time’ Este es el porcentaje del tiempo total que se gastó en esta función, incluido el tiempo dedicado en las subrutinas llamadas desde esta función.
- ‘self’ Esta es la cantidad total de tiempo dedicado a esta función. Esta debe ser idéntico al número impreso en el campo ‘seconds’ (segundos) de esta función en el perfil plano.
- ‘children’ Esta es la cantidad total de tiempo dedicado a las llamadas a subrutinas realizadas por esta función. Esta debe ser igual a la suma de todos los campos ‘self’ y ‘children’ de todos los hijos que aparecen justo debajo de esta función
- ‘called’Este es el número de veces que se llama a la función.

#### Genere un gráfico 

gprof2dot es una herramienta que puede crear una visualización de la salida de gprof. 
Install's:
`
pip install gprof2dot
`
`
sudo apt install graphviz 
`
Generar el .dot 
`
gprof -b test_gprof gmon.out | python3 -m gprof2dot -n0 -e0 -o output.dot
`
Usar graphviz para el png
`
dot -Tpng output.dot -o output.png
`
![Descripción de la imagen](img/output.png)

# Profiling con linux perf
Perf es una herramienta para crear perfiles de programas, usa perfiles estadisticos donde sondea el programa y ve qué funcion está funcionando. Es menos preciso pero tiene menos impacto en el rendimiento. 

##  Paso 1: Instalación de perf
El programa perf no viene preinstalado en los sistemas Linux,puede instalarlo como se muestra a continuación en Ubuntu:

`
sudo apt install linux-tools-common
`

`
sudo apt install linux-tools-5.15.0-100-generic
`

##  Paso 2: Registrar ciclos de CPU
El comando perf de forma predeterminada requiere provilegios sudo para permitir que los usuarios lo utilicen.
- perf record:escribir eventos en un archivo.
- perf report:para explorar el archivo grabado.
  
`
sudo perf record #con path abs completo test_gprof
`

La grabación guarda los datos en un archivo perf.date

## Paso 3: Ver resultados de rendimiento
Para ver los resultados de rendimiento del archivo perf.date

`
sudo perf report
`

Podemos observar las diferencias de los reportes

![Descripción de la imagen](img/perf.png)

El comando ayuda a leer el archivo perf.date, mostrando todos los eventos y estadisticas recopiladas

