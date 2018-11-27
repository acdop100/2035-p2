// This header has all the (extern) declarations of the globals.
// "extern" means "this is instantiated somewhere, but here's what the name
// means.
#include "globals.h"
#include "hardware.h"

// We need to actually instantiate all of the globals (i.e. declare them once
// without the extern keyword). That's what this file does!

// Hardware initialization: Instantiate all the things!

uLCD_4DGL uLCD(p9, p10, p11);  // LCD Screen (tx, rx, reset)
Serial pc(USBTX, USBRX);       // USB Console (tx, rx)
MMA8452 acc(p28, p27, 100000); // Accelerometer (sda, sdc, rate)
double ax, ay, az;

//AnalogOut DACout(p19); // AUX output
//wave_player waver(&DACout);

SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD Card(mosi, miso, sck, cs)

DigitalIn button1(p21); // Pushbuttons (pin)
DigitalIn button2(p22);
DigitalIn button3(p23);
DigitalIn button4(p24);

// Some hardware also needs to have functions called before it will set up
// properly. Do that here.
int hardware_init()
{

    FILE *file;
    // Access sounds file on SD card

    file = fopen("/sd/save_data.txt", "w");
    if (file == NULL)
    {
        error("ERROR: Could not open file!\n\r");
        return -1;
    } 

    // Crank up the speed
    uLCD.baudrate(3000000);
    pc.baud(115200);
    

    //Initialize pushbuttons
    button1.mode(PullUp);
    button2.mode(PullUp);
    button3.mode(PullUp);
    button4.mode(PullUp);

    return ERROR_NONE;
}

GameInputs read_inputs()
{
    // Read accelerometer values
    acc.readXYZGravity(&ax, &ay, &az);
    //pc.printf("x:%lf   y:%lf z:%lf\r\n", ax, ay, az);

    GameInputs newInputs; // Initialize new input struct

    newInputs.b1 = button1;
    newInputs.b2 = button2;
    newInputs.b3 = button3;
    newInputs.b4 = button4;

    newInputs.ax = ax;
    newInputs.ay = ay;
    newInputs.az = az;

    return (newInputs);
}
