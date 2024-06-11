<p align="center">
  <img src="../entrega%202/img/portada.png"><br>
</p>

# Universidad Nacional de Córdoba
## FCEFyN - Sistemas de Computación 2024
## _Entrega #5: Drivers_

Autores:
- Mangin, Matias Eduardo.
- Robles, Karen Yésica.
- Rodriguez, Luciano Ariel


## Consigna

Para superar este TP tendrán que diseñar y construir un CDD que permita sensar dos señales externas con un periodo de UN segundo. Luego una aplicación a nivel de usuario deberá leer UNA de las dos señales y graficarla en función del tiempo. La aplicación tambien debe poder indicarle al CDD cuál de las dos señales leer. Las correcciones de escalas de las mediciones, de ser necesario, se harán a nivel de usuario. Los gráficos de la señal deben indicar el tipo de señal que se
está sensando, unidades en abcisas y tiempo en ordenadas. Cuando se cambie de señal el gráfico se debe "resetear" y acomodar a la nueva medición.

## ¿Qué es un driver?
Es un software que permite al sistema operativo interactuar con un periférico, creando una abtracción del hardware y proporcionando una interfaz para poder usarlo. Generalmente son construidos por los mismos fabricantes pero también se encuentran controladores libres, por ejemplo en los SO libres. En este ultimo caso, no son gente de los fabricantes pero puede haber cooperación lo que facilita el desarrollo.

## ¿ Cuantos tipos podemos encontrar ?
Comparando con un conductor en la vida real, el conductor puede ser una persona, o también un sistema automático. De la misma manera, un "Device Driver" puede ser pieza de software de otro periférico, posiblemente controlado por un software; en tal caso, se lo llama "Device Controller"

Un Device Controller es un device en sí mismo, y por lo tanto muchas veces también necesita un driver, comúnmente llamado "bus controller".

## Drivers & Buses
Los "bus drivers" nos dan una interfaz específica de hardware para los protocolos de hardware correspondientes. Además son las capas de software más bajas del SO (horizontales).

Sobre ellos se encuentran los "devices drivers" que operan sobre los dispositivos subyacentes usando las interfaces de capa horizontal, y por ende, son específicos del dispostivo.

<p align="center">
  <img src="./img/graph1.png"><br>
  <em>Fig 1. Drivers & Buses.</em>
</p>

<p align="center">
  <img src="./img/graph2.png"><br>
  <em>Fig 2. Una mirada horizontal.</em>
</p>

Basado en la interfaz específica del sistema operativo de un "driver", en Linux un driver se clasifica generalmente en tres verticales:
- Orientado a paquetes - Vertical "Network"
- Orientado a bloques - Vertical "Storage"
- Orientado a bytes - Vertical "Character"

<p align="center">
  <img src="./img/graph3.png"><br>
  <em>Fig 3. Una mirada vertical.</em>
</p>

El directorio /dev contiene los archivos de dispositivos especiales para todos los dispositivos hardware.

<p align="center">
  <img src="./img/barra-dev.png"><br>
  <em>Fig 4. /dev.</em>
</p>


## Character Devices

Ya sabemos qué son los drives y para que sirven. ¿Que tienen de especial los character devices? En resumen, que la mayoría de los controladores de dispositivos son de caracteres (recordemos que son orientados a bytes), por ejemplo, seriales, audio, video, cámara, etc.

## Nuestro primer CDD
Cualquier driver de Linux consta de un _constructor_ y un _destructor._
Se llama al constructor de un módulo cada vez que _insmod_ logra cargar el módulo en el núcleo. Se llama al descructor cada vez que _rmmod_ logra descargar el módulo del núcleo.

Se implementan dos funciones habituales en el driver con las macros _module_init()_ y _module_exit()_ incluidas en _module.h_.

```c
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init drv1_init(void) /* Constructor */
{
    printk(KERN_INFO "SdeC: drv1 Registrado exitosamente..!!\n");

    return 0;
}

static void __exit drv1_exit(void) /* Destructor */
{
    printk(KERN_INFO "SdeC: drv1 dice Adios mundo cruel..!!\n");
}

module_init(drv1_init);
module_exit(drv1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anil Kumar Pugalia <email@sarika-pugs.com>");
MODULE_DESCRIPTION("Nuestro primer driver de SdeC");
```
<p align="center">
  <img src="./img/makeinsmoddrv1.png"><br>
  <img src="./img/dmesgdrv1.png"><br>
  <img src="./img/dmesgdrv1.png"><br>
  <img src="./img/rmmoddrv1.png"><br>
  <em>Fig 4. insmod & rmmod drv1.</em>
</p>

## Número mayor y menor
El vínculo entre _APPLICATION_ y _CDF_ se basa en el nombre del archivo del dispositivo. En caso del vínculo _CDF - DD_ se basa en el número del archivo de dispositivo (NO en el nombre).

De esta manera, una aplicación de espacio de usuario tiene cualquier nombre para el CDF y permite que el núcleo tenga un enlace trivial CDF-DD basado en un índice.

Este índice se conoce como el par <major,minor > del archivo del dispositivo. 

A partir del kernel 2.6 en adelante hubo más recursos para el par:
- Tipos especiales: dev_t (contiene ambos numeros)
- Macros específicas:
  - MAJOR(dev_t dev) - El mayor de dev_t
  - MINOR(dev_t dev) - El menor de dev_t
  - MKDEV(int major, int minor) - Crea dev a partir de major y minor.

```c
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first; 

static int __init drv2_init(void) /* Constructor */
{
    int ret;

    printk(KERN_INFO "SdeC_drv2 Registrado exitosamente..!!");

    // alloc_chrdev_region calcula (¿ obtiene ?) dinámicamente un MAJOR libre (se devuelve en 
    // first) y registra 3 números de device MINOR a partir de <el mayor libre, primer menor>, 
    // con el nombre SdeC_drv2. Si devuelve 0, está todo bien..!!

    if ((ret = alloc_chrdev_region(&first, 0, 3, "SdeC_Driver2")) < 0)
    {
        return ret;
    }
    printk(KERN_INFO "<Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
    return 0;
}

static void __exit drv2_exit(void) /* Destructor */
{
    unregister_chrdev_region(first, 3);
    printk(KERN_INFO "SdeC_drv2 dice Adios mundo cruel..!!\n");
}

module_init(drv2_init);
module_exit(drv2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cátedra Sistemas de Computación");
MODULE_DESCRIPTION("Nuestro segundo driver de SdeC");
```

<p align="center">
  <img src="./img/insmoddrv2.png"><br>
  <img src="./img/majorminor.png"><br>
  <img src="./img/devdrv2.png"><br>
  <img src="./img/catdev.png"><br>
  <em>Fig 4. insmod drv2.</em>
</p>

## Creación automática de los CDF
Hasta el kernel 2.4, la creación de los archivos de dispositivos fue realizada automáticamente por el mismo kernel, usando api's de devfs. A medida que el kernel evolucionó, los desarrolladores se dieron cuenta que los DF son una cuestión más de espacio de usuario y que solo ellos deberían tratarlo, y no el kernel.

Con esa idea, el núcleo solo completa la clase de dispositivo y la información del dispositivo en _sys/class_. Luego el espacio de usuario necesita interpretarlo y tomar una acción apropiada. 

<p align="center">
  <img src="./img/lssys.png"><br>
  <em>Fig 4. lsys.</em><br>
  <img src="./img/wifi.png"><br>
  <em>Fig 4. interfaz wifi.</em><br>
  <img src="./img/bateria.png"><br>
  <em>Fig 4. capacidad de la bateria.</em><br>
  <img src="./img/temperatura.png"><br>
  <em>Fig 4. temperatura.</em><br>
</p>

La clase de dispositivo se crea con:
- struct class *cl=class_create(THIS_MODULE, "<device class name>");
La información del dispositivo se completa con:
- device_create(cl, NULL, first, NULL, "<device name format>", …);
- first es tipo dev_t con su correspondiente major, minor.
Las llamadas complementarias o inversas:
- device_destroy(c1,first)
- class_destroy(c1)

## Driver 3


<p align="center">
  <img src="./img/driver3.png"><br>
  <em>Fig 4. driver3.</em><br>
</p>

Análisis de driver3:
- Variables globales:
  ```c
  static dev_t first; 		// Número de dispositivo (major y minor)
  static struct cdev c_dev; 	// Estructura del dispositivo de caracteres
  static struct class *cl; 	// Clase de dispositivo para sysfs
  ```
- Implementación de las funciones de operación, que son los puntos de entrada para las operaciones de archivo en el dispositivo
  ```c
  static int my_open(struct inode *i, struct file *f)
  {
      printk(KERN_INFO "Driver3_SdeC: open()\n");
      return 0;
  }
  static int my_close(struct inode *i, struct file *f)
  {
      printk(KERN_INFO "Driver3_SdeC: close()\n");
      return 0;
  }
  static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
  {
      printk(KERN_INFO "Driver3_SdeC: read()\n");
      return 0;
  }
  static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
  {
      printk(KERN_INFO "Driver3_SdeC: write()\n");
      return len;
  }
  ```
- La estructura file operations contiene las declaración de las funciones que se desarrollaron
  ```c
  static struct file_operations pugs_fops =
  {
      .owner = THIS_MODULE,
      .open = my_open,
      .release = my_close,
      .read = my_read,
      .write = my_write
  };
  ```
- Además tenemos el init y exit que veniamos tratando.

#### _Análisis de my_read_
La funcion debería copiar los datos del device al espacio de usuario. Vemos que retorna 0 lo que indicaría que no leyó ningun byte. En una implementación real debería retornar un lenght de bytes leídos.
#### _Análisis de my_write_
Esta función debería copiar desde el espacio de usuario al dispositivo. Debe retornar un lenght cuando se escribió exitosamente y otra cosa en caso de error (número negativo).

## Driver 4
Observamos como se modifican las funciones de lectura y escritura.
- my_read:
  ```c
  static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
  {
      printk(KERN_INFO "SdeC_drv4: read()\n");

      if (*off == 0)
      {
          if ( copy_to_user(buf, &c, 1) != 0)
              return -EFAULT;
          else
          {
              (*off)++;
              return 1;
          }
      }
      else
          return 0;

  }
  ```
- my_write:
  ```c
  static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
  {
      printk(KERN_INFO "SdeC_drv4: write()\n");

      if ( copy_from_user(&c, buf + len - 1, 1) != 0 )
          return -EFAULT;
      else
          return len;
  }
  ```

<p align="center">
  <img src="./img/driver4.png"><br>
  <em>Fig 4. driver4.</em><br>
</p>

## Módulo Clipboard
Para este caso, trabajamos con un módulo que crea una entrada en /proc, no es un CDD. 
```c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Clipboard Kernel Module - FDI-UCM");
MODULE_AUTHOR("Juan Carlos Saez");

#define BUFFER_LENGTH       PAGE_SIZE

static struct proc_dir_entry *proc_entry;
static char *clipboard;                     // Space for the "clipboard"

static ssize_t clipboard_write(struct file *filp, const char __user *buf, size_t len, loff_t *off) {
  int available_space = BUFFER_LENGTH-1;
  
  if ((*off) > 0) /* The application can write in this entry just once !! */
    return 0;
  
  if (len > available_space) {
    printk(KERN_INFO "clipboard: not enough space!!\n");
    return -ENOSPC;
  }
  
  /* Transfiere data desde el espacio de usuario al espacio de kernel */
  /* echo un mensaje > /proc/clipboard                                */

  if (copy_from_user( &clipboard[0], buf, len ))  
    return -EFAULT;

  clipboard[len] = '\0';  /* Add the `\0' */  
  *off+=len;              /* Update the file pointer */
  
  return len;
}

static ssize_t clipboard_read(struct file *filp, char __user *buf, size_t len, loff_t *off) {
  
  int nr_bytes;
  
  if ((*off) > 0) /* Tell the application that there is nothing left to read */
      return 0;
    
  nr_bytes=strlen(clipboard);
    
  if (len<nr_bytes)
    return -ENOSPC;
  
    /* Transfiere data desde el espacio de kernel al espacio de usuario */ 
    /* cat /proc/clipboard                                              */

  if (copy_to_user(buf, clipboard, nr_bytes))
    return -EINVAL;
    
  (*off)+=len;  /* Update the file pointer */

  return nr_bytes; 
}

static const struct proc_ops proc_entry_fops = {
    .proc_read = clipboard_read,
    .proc_write = clipboard_write,    
};



int init_clipboard_module( void )
{
  int ret = 0;
  clipboard = (char *)vmalloc( BUFFER_LENGTH );

  if (!clipboard) {
    ret = -ENOMEM;
  } else {

    memset( clipboard, 0, BUFFER_LENGTH );
    proc_entry = proc_create( "clipboard", 0666, NULL, &proc_entry_fops);
    if (proc_entry == NULL) {
      ret = -ENOMEM;
      vfree(clipboard);
      printk(KERN_INFO "Clipboard: No puede crear entrada en /proc..!!\n");
    } else {
      printk(KERN_INFO "Clipboard: Modulo cargado..!!\n");
    }
  }
  return ret;
}


void exit_clipboard_module( void )
{
  remove_proc_entry("clipboard", NULL);
  vfree(clipboard);
  printk(KERN_INFO "Clipboard: Modulo descargado..!!\n");
}


module_init( init_clipboard_module );
module_exit( exit_clipboard_module );
```

<p align="center">
  <img src="./img/clipboard.png"><br>
  <em>Fig 4. clipboard.</em><br>
</p>
