/*****************************************************************/
/** This is a program in userspace that read GPIO and do fwrite **/
/** to file named /proc/devState everytime the button is pushed **/
/*****************************************************************/

#include <stdio.h>
#include <wiringPi.h>

//#define GPIO_DEVICE1    17
//#define GPIO_DEVICE2    27
//#define GPIO_DEVICE3    22
#define GPIO_BUTTON1    23
#define GPIO_BUTTON2    24
#define GPIO_BUTTON3    25
#define DEBOUNCE_TIME   200

static unsigned long lastISRTime1 = 0;
static unsigned long lastISRTime2 = 0;
static unsigned long lastISRTime3 = 0;

void device1(void) {
    static int x = 1;   //counter of presses & Interrupt service routine
                        //uses static to retain multiple calls
    unsigned long currentISRTime1 = millis();
    if (currentISRTime1 - lastISRTime1 > DEBOUNCE_TIME)
    {
        char str1[] = "001"; // means button1 is pressed
        fwrite(str1, 1, sizeof(str1), fp);
        x++;
    }
    lastISRTime1 = currentISRTime1;
}

void device2(void) {
    static int y = 1;
    unsigned long currentISRTime2 = millis();
    if (currentISRTime2 - lastISRTime2 > DEBOUNCE_TIME)
    {
        char str2[] = "010";
        fwrite(str2, 1, sizeof(str2), fp);
        y++;
    }
    lastISRTime2 = currentISRTime2;
}

void device3(void) {
    static int z = 1;
    unsigned long currentISRTime3 = millis();
    if (currentISRTime3 - lastISRTime3 > DEBOUNCE_TIME)
    {
        char str3[] = "100";
        fwrite(str3, 1, sizeof(str3), fp);
        z++;
    }
    lastISRTime3 = currentISRTime3;
}

int main()
{
    wiringPiSetupGpio();
    FILE *fp;
    char buffer[4]; // potentially need 5 bytes -> 3 numbers, 1 null terminator
    size_t bytes_read;

    while(1)
    {
        fp = fopen("/proc/devState","r+");

        wiringPiISR(GPIO_BUTTON1, INT_EDGE_RISING, &device1); // if there is a trigger from GPIO_BUTTON1 at rising edge, then call fuction device1()
        wiringPiISR(GPIO_BUTTON2, INT_EDGE_RISING, &device2);
        wiringPiISR(GPIO_BUTTON3, INT_EDGE_RISING, &device3);

        fclose(fp);
    }

    /** jadi nanti aplikasi akan write kalau ada input high dari button, berarti akan digital read, lalu fwrite ke devState. fread jika diperlukan untuk cek.
    *** nanti di kernel akan proc read **/
}
