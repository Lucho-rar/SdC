#include <linux/cdev.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kdev_t.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <asm/atomic.h>

#define MAX_BUFFER_SIZE 1024
#define RPI3_GPIO_BASE 0x3F200000
#define RPI4_GPIO_BASE 0xfe200000
static char kernel_buffer[MAX_BUFFER_SIZE + 1] = {0};
static unsigned int *gpio_base = NULL;
static unsigned int active_gpio_pin = 18;




/**
 * @brief Configures the specified GPIO pin for input/output.
 *
 * @param pin The GPIO pin number to configure.
 */
static void configure_gpio_pin(unsigned int pin) {
    unsigned int fsel_index = pin / 10;
    unsigned int fsel_bitpos = pin % 10;
    unsigned int *gpio_fsel = gpio_base + fsel_index;

    *gpio_fsel &= ~(7 << (fsel_bitpos * 3));
    active_gpio_pin = pin;

    return;
}

/**
 * @brief Reads the value of the specified GPIO pin.
 *
 * @param pin The GPIO pin number to read.
 * @return The value of the GPIO pin (0 or 1).
 */
static int read_gpio_value(unsigned int pin) {
    unsigned int lev_index = pin / 32;
    unsigned int lev_bitpos = pin % 32;
    volatile unsigned int* gpio_lev = (volatile unsigned int*)((char*)gpio_base + 0x34 + (lev_index * 4));

    return (*gpio_lev & (1 << lev_bitpos)) ? 1 : 0;
}

/**
 * @brief Reads the state of the GPIO pin and copies it to the user buffer.
 *
 * @param file The file structure pointer.
 * @param user_buf The user buffer to copy the GPIO state to.
 * @param count The size of the user buffer.
 * @param f_pos The file position pointer.
 * @return The number of bytes read, or an error code on failure.
 */
ssize_t gpio_read(struct file *file, char __user *user_buf, size_t count, loff_t *f_pos)
{
    printk("Reading GPIO state\n");

    if(*f_pos > 0) {
        return 0; 
    }

    int gpio_state = read_gpio_value(active_gpio_pin);
    udelay(1000);
    if(copy_to_user(user_buf, &gpio_state, sizeof(gpio_state)))
    {
        return -EFAULT;
    }
    return sizeof(gpio_state);
}

/**
 * @brief Writes the specified GPIO pin number to the kernel buffer.
 *
 * @param file The file structure pointer.
 * @param user_buf The user buffer containing the GPIO pin number.
 * @param count The size of the user buffer.
 * @param off The file position pointer.
 * @return The number of bytes written, or an error code on failure.
 */
ssize_t gpio_write(struct file *file, const char __user *user_buf, size_t count, loff_t *off)
{
    unsigned int pin = 18;

    memset(kernel_buffer, 0x0, sizeof(kernel_buffer));

    if (count > MAX_BUFFER_SIZE) {
        count = MAX_BUFFER_SIZE;
    }

    if (copy_from_user(kernel_buffer, user_buf, count))
        return 0;

    printk("Kernel buffer: %s\n", kernel_buffer);

    if (sscanf(kernel_buffer, "%d", &pin) != 1) {
        printk("Invalid format in user data\n");
        return count;
    }

    if (pin > 27 || pin < 0) {
        printk("Invalid GPIO pin number\n");
        return count;
    }

    printk("Selected GPIO Pin: %d\n", pin);
    configure_gpio_pin(pin);

    return count;
}

struct file_operations gpio_fops = {
	.owner =     THIS_MODULE,
	.read =	     gpio_read,
    .write =     gpio_write
};

/**
 * @brief Initializes the GPIO handler driver.
 *
 * @return 0 on success, or an error code on failure.
 */
static int __init gpio_driver_init(void) {

    int ret;
    dev_t dev = MKDEV(major_number,0);

    printk("Initializing GPIO handler driver\n");
    ret = alloc_chrdev_region(&dev, 0, 1, "gpiohandler");
    major_number = MAJOR(dev);

    if (ret < 0) {
        pr_alert("DriverGpio: Error allocating character device region\n");
        return ret;
    }

    gpio_character_device = cdev_alloc();
    gpio_character_device->ops = &gpio_fops;
    ret = cdev_add(gpio_character_device, dev, 1);
    if (ret < 0) {
        printk("DriverGpio: Error adding character device\n");
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    gpio_base = (int *)ioremap(RPI3_GPIO_BASE, PAGE_SIZE);
    if (gpio_base == NULL) {
        printk("Failed to map GPIO memory for Raspberry Pi\n");
        return -1;
    }

    printk("GPIO handler driver loaded successfully\n");

    return 0;
}

/**
 * @brief Cleans up and removes the GPIO handler driver.
 */
static void __exit gpio_driver_exit(void)
{
    printk("Removing GPIO handler driver\n");
    dev_t dev = MKDEV(major_number,0);
    cdev_del(gpio_character_device);
    unregister_chrdev_region(dev, 1);
}

module_init(gpio_driver_init);
module_exit(gpio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lucho, Yesi, Mati");
MODULE_DESCRIPTION("Driver to interact with a specific GPIO pin on Raspberry Pi");
MODULE_VERSION("2.0");
