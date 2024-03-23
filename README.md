# Sistemas de Computación - Entrega #1
## _Time Profiling_

[![N|Solid](https://cldup.com/dTxpPi9lDf.thumb.png)](https://nodesource.com/products/nsolid)

## Introducción
Se utiliza la herramienta gprof. Para ello es necesario:
- Habilitar profiling durante la compilación.
- Ejecutar el código del programa para producir los datos de perfil.
- Ejecutar la herramienta gprof en el archivo de datos de generación de perfiles.

Este último paso produce un archivo legible para las personas. Este archivo, contiene tablas (perfil plano y gráfico de llamadas) además de más información. 
El perfil plano da una descripción general de tiempo de las funciones, la cantidad de veces que se llaman, etc.
El gráfico de llamadas es información de cantidad de llamadas dentro de una X función, etc. (subrutinas)

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

Esto genera un gmon.out.

## Paso 2: Ejecutar el código

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


## Profiling con linux perf
Perf es una herramienta para crear perfiles de programas, usa perfiles estadisticos donde sondea el programa y ve qué funcion está funcionando. Es menos preciso pero tiene menos impacto en el rendimiento. 





