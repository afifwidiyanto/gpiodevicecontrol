/***********************************************************************/
/** This is a program in userspace that gives instructions to GPIO    **/
/** through userspace using WiringPi with default BCM GPIO numberings **/
/***********************************************************************/

#include<stdio.h>    // For the printf() statements
#include<stdlib.h>   // (I literally don't know so let it be for a while)
#include<wiringPi.h> // To include WiringPi library

/** These are the macros, their values will be the same, everywhere in the program **/
#define GPIO_DEVICE1 "17"
#define GPIO_DEVICE2 "27"
#define GPIO_DEVICE3 "22"
#define GPIO_BUTTON1 "23"
#define GPIO_BUTTON2 "24"
#define GPIO_BUTTON3 "25"
//#define GPIO17_PATH "/sys/class/gpio/gpio17"
//#define GPIO27_PATH "/sys/class/gpio/gpio27"
//#define GPIO22_PATH "/sys/class/gpio/gpio22"
//#define GPIO23_PATH "/sys/class/gpio/gpio23"
//#define GPIO24_PATH "/sys/class/gpio/gpio24"
//#define GPIO25_PATH "/sys/class/gpio/gpio25"
//#define GPIO_SYSFS "/sys/class/gpio/"

/** These are the counters **/
static unsigned int buttonCounter1 = 0;
static unsigned int buttonCounter2 = 0;
static unsigned int buttonCounter3 = 0;
static unsigned int totalCounter = 0;

/** And these are the variables to flip the devices' on and off state**/
static bool devOn1 = 0;
static bool devOn2 = 0;
static bool devOn3 = 0;

int main()
{
    // Setting up before the actual mechanism
    wiringPiSetupGPIO(); // Initializing wiringPi using the BCM pin numbering

    pinMode(GPIO_DEVICE1, OUTPUT);
    pinMode(GPIO_DEVICE2, OUTPUT);
    pinMode(GPIO_DEVICE3, OUTPUT);
    pinMode(GPIO_BUTTON1, INPUT);
    pinMode(GPIO_BUTTON2, INPUT);
    pinMode(GPIO_BUTTON3, INPUT);
    //pullUpDnControl(GPIO_BUTTON1,PUD_UP); //There is actually an internal pull-up/down mechanism in RPi3 GPIO
    //pullUpDnControl(GPIO_BUTTON2,PUD_UP);
    //pullUpDnControl(GPIO_BUTTON2,PUD_UP);
    digitalWrite(GPIO_DEVICE1, devOn1);
    digitalWrite(GPIO_DEVICE2, devOn2);
    digitalWrite(GPIO_DEVICE3, devOn3);

    printf("Program to control devices via GPIO is running. Press CTRL+C to exit. \n");

    // Loop while the program is on
    while (1)
    {
        if(digitalRead(GPIO_BUTTON1) = 1)
        {
            devOn1 = !devOn1; // Flip device state
            digitalWrite(GPIO_DEVICE1, devOn1); //Execute the flip
            buttonCounter1++;
            totalCounter++;
            printf("Button 1 has been pressed %d times, while in total all buttons has been pressed %d times.\n",buttonCounter1,totalCounter);
            delay(1000); //delay 1000ms for testing purposes & can act as debounce timer too. :D
        }
        if(digitalRead(GPIO_BUTTON2) = 1)
        {
            devOn2 = !devOn2;
            digitalWrite(GPIO_DEVICE2, devOn2);
            buttonCounter2++;
            totalCounter++;
            printf("Button 2 has been pressed %d times, while in total all buttons has been pressed %d times.\n",buttonCounter2,totalCounter);
            delay(1000);
        }
        if(digitalRead(GPIO_BUTTON3) = 1)
        {
            devOn3 = !devOn3;
            digitalWrite(GPIO_DEVICE3, devOn3);
            buttonCounter3++;
            totalCounter++;
            printf("Button 3 has been pressed %d times, while in total all buttons has been pressed %d times.\n",buttonCounter3,totalCounter);
            delay(1000);
        }
};
