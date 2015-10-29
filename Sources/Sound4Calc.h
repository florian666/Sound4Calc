#ifndef _SOUND4CALC
#define _SOUND4CALC

char is_SH4 = 1; // 1 => SH4 (default); 0 => Sh3

void setup(void); // configure ports to output

void SetPin(void); // put to 1 the state of the pin
void ResetPin(void); // reset the state of the pin to 0 

void getMPU(void);

#endif