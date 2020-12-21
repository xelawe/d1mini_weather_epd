#include <PCF8574.h>

PCF8574 pcf20(0x20);

const byte PcfButton1 = 0;
const byte PcfPIR1 = 1;

boolean PcfButton1state;
boolean PcfPIR1state;

const int PCFIRQPIN = 3;
volatile bool pcf_flag = false;

