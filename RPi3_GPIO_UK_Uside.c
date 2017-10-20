/** Sebelum dilanjutkan perlu diingat bahwa paradigmanya adalah komputasi di kernel module & di userspace tinggal switching saja **/

/*****************************************************************/
/** This is a program in userspace that read GPIO and do fwrite **/
/** to file named /proc/devState everytime the button is pushed **/
/*****************************************************************/

#include <stdio.h>
#include <wiringPi.h>

//#define GPIO_DEVICE1    23
//#define GPIO_DEVICE2    24
//#define GPIO_DEVICE3    25
#define GPIO_BUTTON1    17
#define GPIO_BUTTON2    27
#define GPIO_BUTTON3    22
#define DEBOUNCE_TIME   200

static unsigned long lastISRTime1 = 0;
static unsigned long lastISRTime2 = 0;
static unsigned long lastISRTime3 = 0;
//static char buffer[3];  // initiation of the buffer
static char str1[3];    // the arrays to write into the procfile
//static char str2[3];
//static char str3[3];
//static int loop;
static int loopPrint;

/** Untuk 3 fungsi ini gimana kalau fread() dulu lalu jadikan array. Fungsi tertentu cuma ngganti entry tertentu pada array itu. Lalu baru fwrite() **/

void device1(void) {
    static int x = 1;   //counter of presses & Interrupt service routine
                        //uses static to retain multiple calls
    unsigned long currentISRTime1 = millis();
    if(currentISRTime1 - lastISRTime1 > DEBOUNCE_TIME){
//        fseek(fp, 0, SEEK_SET); // Seek start of file

//        fread(buffer, 1, 3, fp);
//        loop = 0;
//        for(loop = 0; loop < 3; loop++){
//            str1[loop] = buffer[loop];
//        }

//        fread(str1, 1, 3, fp);

        if(str1[0] = "1"){
            str1[0] = "0";
        }else{
            str1[0] = "1";
        }

        loopPrint = 0;
        for(loopPrint = 0; loopPrint < 3; loopPrint++){
                printf("Userspace lampu %d bernilai %d\n", loopPrint, str1[loopPrint]);
        }

        fwrite(str1, 1, sizeof(str1), fp);
        x++;
    }
    lastISRTime1 = currentISRTime1;
}

void device2(void) {
    static int y = 1;
    unsigned long currentISRTime2 = millis();
    if(currentISRTime2 - lastISRTime2 > DEBOUNCE_TIME){
//        fseek(fp, 0, SEEK_SET); // Seek start of file
//        fread(buffer, 1, 3, fp);
//        loop = 0;
//        for(loop = 0; loop < 3; loop++){
//            str1[loop] = buffer[loop];
//        }

        if(str1[1] = "1"){
            str1[1] = "0";
        }else{
            str1[1] = "1";
        }

        loopPrint = 0;
        for(loopPrint = 0; loopPrint < 3; loopPrint++){
                printf("Userspace lampu %d bernilai %d\n", loopPrint, str1[loopPrint]);
        }

        fwrite(str1, 1, sizeof(str1), fp);
        y++;
    }
    lastISRTime2 = currentISRTime2;
}

void device3(void) {
    static int z = 1;
    unsigned long currentISRTime3 = millis();
    if(currentISRTime3 - lastISRTime3 > DEBOUNCE_TIME){
//        fseek(fp, 0, SEEK_SET); // Seek start of file
//        fread(buffer, 1, 3, fp);
//        loop = 0;
//        for(loop = 0; loop < 3; loop++){
//            str1[loop] = buffer[loop];
//        }

        if(str1[2] = "1"){
            str1[2] = "0";
        }else{
            str1[2] = "1";
        }

        loopPrint = 0;
        for(loopPrint = 0; loopPrint < 3; loopPrint++){
                printf("Userspace lampu %d bernilai %d\n", loopPrint, str1[loopPrint]);
        }

        fwrite(str1, 1, sizeof(str1), fp);
        z++;
    }
    lastISRTime3 = currentISRTime3;
}

int main(){
    wiringPiSetupGpio();
    FILE *fp;
    char buffer[4]; // potentially need 5 bytes -> 3 numbers, 1 null terminator
    size_t bytes_read;

    while(1){
        fp = fopen("/proc/devState","r+");
        //fseek(fp, 0, SEEK_SET); // Seek start of file, buat jaga-jaga kalau nanti dipakai.
        fread(str1, 1, 3, fp);

        wiringPiISR(GPIO_BUTTON1, INT_EDGE_RISING, &device1); // if there is a trigger from GPIO_BUTTON1 at rising edge, then call fuction device1()
        wiringPiISR(GPIO_BUTTON2, INT_EDGE_RISING, &device2);
        wiringPiISR(GPIO_BUTTON3, INT_EDGE_RISING, &device3);

        fclose(fp);
    }

    /** jadi nanti aplikasi akan write kalau ada input high dari button, berarti akan digital read, lalu fwrite ke devState. fread jika diperlukan untuk cek.
    *** nanti di kernel akan proc read **/
}
