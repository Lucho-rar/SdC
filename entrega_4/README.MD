![portada](../entrega%202/img/portada.png)
# Universidad Nacional de Córdoba
## FCEFyN - Sistemas de Computación 2024
## _Entrega #4: Modulos de kernel_

Autores:
- Mangin, Matias Eduardo.
- Robles, Karen Yésica.
- Rodriguez, Luciano Ariel


## Desafio 1
### Ejemplo printf con makefile

```make
all:
	gcc -Wall -o ejemplo_printf ejemplo_printf.c 

clean:
	rm ejemplo_printf
```

![printf](img/ejemploprintf.png)

### run con strace
![strace1](img/strace1.png)
![strace2](img/strace2.png)
![stracett1](img/stracett1.png)
![stracett2](img/stracett2.png)
![strace-c](img/strace-c.png)

### /proc
![prockalls](img/prockallsyms.png)
![prockallsymsusb](img/prockallsymsusb.png)

## Desafio 2
![makemimodulo](img/ko.png)
![modinfoko](img/modinfoko.png)
![insmod](img/insmod.png)
![dmsg1](img/demsg.png)
![dmsg2](img/dmsg2.png)
![ver modulo con proc](img/moduloconproc.png)

## Desafio 3 (pdf csroom)
![modulos que tengo cargados](img/moduloscargados.png)
![modulos que cargados kernel](img/moduloscargados-kernel.png)
![crpyto](img/crypto.png)
![desgeneric](img/desgeneric.png)
![desgeneric firma](img/des_generic_firma.png)
![mimodulofirmado](img/mimodulofirmado.png)