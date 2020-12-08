#include <PCF8574.h>

PCF8574 pcf20(0x20);

const byte PcfButton1 = 0;

const int PCFIRQPIN = 3;
volatile bool pcf_flag = false;

