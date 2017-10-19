/**********************************************************/
/** This is a kernel module that reads a proc filesystem **/
/** which is under control of the program in user space  **/
/** for controlling the state of devices through GPIO    **/
/**********************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Afif Widiyanto Musthofa");
MODULE_DESCRIPTION("Button test driver for User-Kernel configuration");
MODULE_VERSION("0.0.1");

#define PROCFS_NAME "devState"
#define MAXBUFFSIZE 256

static unsigned int gpioDevice1 = 23; // GPIO23
static unsigned int gpioDevice2 = 24;
static unsigned int gpioDevice3 = 25;
static unsigned int gpioButton1 = 17; // GPIO17
static unsigned int gpioButton2 = 27;
static unsigned int gpioButton3 = 22;
static bool devOn1 = 0; // Initial state of devices
static bool devOn2 = 0;
static bool devOn3 = 0;

static struct proc_dir_entry *Our_Proc_File;

static char procfs_buffer[MAXBUFFSIZE];
static unsigned long procfs_buffer_size = 0;

int procfie_read(char *buffer, char** buffer_location, off_t offset, int buffer_length, int *eof, void *data)
{
    int ret;

    printk(KERN_INFO "procfile_read (/proc/%s) called \n", PROCFS_NAME);

    if (offser > 0){
        ret =0;
    }
    else{
        memcpy(buffer, procfs_buffer,procfs_buffer_size);
        ret = procfs_buffer_size;
    }
    return ret;
}

static int __init rpi3_gpio_UK_init(void){
    Our_Proc_File = proc_create_data(PROCFS_NAME,.....)'
    }
