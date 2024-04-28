# Sistemas de Computación
# _Entrega  3_


## Commands
_Needs_
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