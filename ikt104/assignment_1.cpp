#include "mbed.h"

#define WAIT_TIME_MS 500 
DigitalOut led1(D15);
BufferedSerial serial_port(USBTX, USBRX,115200);

int main()
{
    int buf[10] = {0};
    while (true) {
        char buf[1];
        serial_port.read(buf, 1);
        if (buf[0] == '0') led1= false;
        if (buf[0] == '1') led1= true;
        thread_sleep_for(WAIT_TIME_MS);
    }
}
