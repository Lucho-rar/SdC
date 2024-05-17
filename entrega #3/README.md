# Sistemas de Computación
# _Entrega  3_


## Commands
_Necesario_
```
sudo apt install qemu-system-x86
```
_Creacion de una img booteable_
```
//Create
printf '\364%509s\125\252' > main.img

//Run image
qemu-system-x86_64 --drive file=main.img,format=raw index=0,media=disk

```

_Para trabajar con el repositorio de ejemplos_
```
git clone
https://gitlab.unc.edu.ar/javierjorge/protected-mode-sdc.git

git submodule init 

git submodule update

//esto agrega una carpeta como submodulo de git con ejemplos

// para correr los ejemplos de prueba dentro del submodulo

./run bios_hello_world
./run protected_mode

```

Podemos modificar estos ejemplos 

```
// Dentro de x86-bare-metal-examples
vim bios_hello_world.S 
```
```
/* https://github.com/cirosantilli/x86-bare-metal-examples#bios-hello-world */

#include "common.h"

BEGIN
    mov $msg, %si
    mov $0x0e, %ah
loop:
    lodsb
    or %al, %al
    jz halt
    int $0x10
    jmp loop
halt:
    hlt
msg:
    .asciz "TP3 - Modo Protegido - SDC 2024"
```

_Para grabar la img en HW real_

