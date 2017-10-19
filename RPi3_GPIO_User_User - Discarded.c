/** This is a program in userspace that gives instructions to GPIO through userspace **/

/********************************************************************************/
/** The idea is using default BCM numbering without WiringPi                   **/
/** It turns out that WiringPi also supports default BCM numbering             **/
/** So this idea is discarded. The file is still used for documentary purposes **/
/********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define GPIO_DEVICE1 "17"
#define GPIO_DEVICE2 "27"
#define GPIO_DEVICE3 "22"
#define GPIO_BUTTON1 "23"
#define GPIO_BUTTON2 "24"
#define GPIO_BUTTON3 "25"
#define GPIO17_PATH "/sys/class/gpio/gpio17"
#define GPIO27_PATH "/sys/class/gpio/gpio27"
#define GPIO22_PATH "/sys/class/gpio/gpio22"
#define GPIO23_PATH "/sys/class/gpio/gpio23"
#define GPIO24_PATH "/sys/class/gpio/gpio24"
#define GPIO25_PATH "/sys/class/gpio/gpio25"
#define GPIO_SYSFS "/sys/class/gpio/"

static unsigned int buttonCounter1 = 0;
static unsigned int buttonCounter2 = 0;
static unsigned int buttonCounter3 = 0;
static unsigned int totalCounter = 0;
static bool devOn1 = 0;
static bool devOn2 = 0;
static bool devOn3 = 0;

int main(int argc, char* argv[]){
    while True{
        writeGPIO(GPIO_SYSFS "export", GPIO_DEVICE1);
        writeGPIO(GPIO17_PATH "direction", "out");
        writeGPIO(GPIO_SYSFS "export", GPIO_DEVICE2);
        writeGPIO(GPIO27_PATH "direction", "out");
        writeGPIO(GPIO_SYSFS "export", GPIO_DEVICE3);
        writeGPIO(GPIO22_PATH "direction", "out");
        writeGPIO(GPIO_SYSFS "export", GPIO_BUTTON1);
        writeGPIO(GPIO23_PATH "direction", "in");
        writeGPIO(GPIO_SYSFS "export", GPIO_BUTTON2);
        writeGPIO(GPIO24_PATH "direction", "in");
        writeGPIO(GPIO_SYSFS "export", GPIO_BUTTON3);
        writeGPIO(GPIO25_PATH "direction", "in");

        if(){

        }
};
