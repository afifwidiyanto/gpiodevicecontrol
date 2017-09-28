/***********************************************************/
/** This is a program in userspace that gives syscall for **/
/** controlling the on/off state of devices through GPIO  **/
/***********************************************************/

/*******************************************/
/** Put these lines first in kernel/sys.c **/
/** is it the same location for raspbian? **/
/*******************************************/

SYSCALL_DEFINE3(flipstate, int, gpioButton, int, gpioDevice, int, devOn)
{
    // copy interrupt from kernel here?
    // should the flipping function be put in kernel/sys.c?
    // or in kernel module?
}

/** And then compile the kernel **/

/*********************************************************************/
/** alternatively create a way to access kernel module with ioctl() **/
/*********************************************************************/
