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

static char mm_buff[MAXBUFFSIZE];

static unsigned int gpioDevice1 = 17; // GPIO17
static unsigned int gpioDevice2 = 27;
static unsigned int gpioDevice3 = 22;
static unsigned int gpioButton1 = 23; // GPIO23
static unsigned int gpioButton2 = 24;
static unsigned int gpioButton3 = 25;
static bool devOn1 = 0; // Initial state of devices
static bool devOn2 = 0;
static bool devOn3 = 0;

static struct proc_dir_entry *Our_Proc_File;

/*******************************************/
/** Put these lines first in kernel/sys.c **/ /** <--- all below this line is not used. LEL **/
/** is it the same location for raspbian? **/
/*******************************************/

SYSCALL_DEFINE3(flipstate, int, gpioButton, int, gpioDevice, int, devOn)
{
    // copy interrupt from kernel here?
    // should the flipping function be put in kernel/sys.c?
    // or in kernel module?
}

/** And then compile the kernel **/

/**********************************************************************************/
/** OR alternatively use ioctl() to access kernel module, but that's not syscall **/
/**********************************************************************************/

// fopen( nama part)
// fwrite()
// keyword how to access kern module using fopen() in raspberrypi
