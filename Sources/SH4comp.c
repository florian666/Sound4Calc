#define __KEYBIOS_H__
#include "fxlib.h"
#define KEY_CHAR_0 71
#define KEY_CHAR_1 72
#define KEY_CHAR_2 62
#define KEY_CHAR_3 52
#define KEY_CHAR_4 73
#define KEY_CHAR_5 63
#define KEY_CHAR_6 53
#define KEY_CHAR_7 74
#define KEY_CHAR_8 64
#define KEY_CHAR_9 54
#define KEY_CHAR_DP 61
#define KEY_CHAR_EXP 51
#define KEY_CHAR_PMINUS 41
#define KEY_CHAR_PLUS 42
#define KEY_CHAR_MINUS 32
#define KEY_CHAR_MULT 43
#define KEY_CHAR_DIV 33
#define KEY_CHAR_FRAC 75
#define KEY_CHAR_LPAR 55
#define KEY_CHAR_RPAR 45
#define KEY_CHAR_COMMA 35
#define KEY_CHAR_STORE 25
#define KEY_CHAR_LOG 66
#define KEY_CHAR_LN 56
#define KEY_CHAR_SIN 46
#define KEY_CHAR_COS 36
#define KEY_CHAR_TAN 26
#define KEY_CHAR_SQUARE 67
#define KEY_CHAR_POW 57
#define KEY_CTRL_EXE 31
#define KEY_CTRL_DEL 44
#define KEY_CTRL_AC 32
#define KEY_CTRL_FD 65
#define KEY_CTRL_EXIT 47
#define KEY_CTRL_SHIFT 78
#define KEY_CTRL_ALPHA 77
#define KEY_CTRL_OPTN 68
#define KEY_CTRL_VARS 58
#define KEY_CTRL_UP 28
#define KEY_CTRL_DOWN 37
#define KEY_CTRL_LEFT 38
#define KEY_CTRL_RIGHT 27
#define KEY_CTRL_F1 79
#define KEY_CTRL_F2 69
#define KEY_CTRL_F3 59
#define KEY_CTRL_F4 49
#define KEY_CTRL_F5 39
#define KEY_CTRL_F6 29
#define KEY_CTRL_MENU 48 
#ifndef OS2Change
#define OS2Change
#ifndef OS2Change_GetOS2
#define OS2Change_GetOS2
typedef int(*sc_i2cp2sip)(char*, char*, short int*, short int*);
const unsigned int sc0015[] = { 0xD201D002, 0x422B0009, 0x80010070, 0x0015 };
#define GlibGetOSVersionInfo (*(sc_i2cp2sip)sc0015)
int OSVersionAsInt(void)
{
  unsigned char mainversion;
  unsigned char minorversion;
  unsigned short release;
  unsigned short build;
  GlibGetOSVersionInfo( &mainversion, &minorversion, &release, &build );
  return ( ( mainversion << 24 ) & 0xFF000000 ) | ( ( minorversion << 16 ) & 0x00FF0000 ) | ( release & 0x0000FFFF );
}
#define isOS2 (OSVersionAsInt() >= 0x02020000)
#define OS2(x,y) ((OSVersionAsInt() >= 0x02020000)?y:x)
#endif
#ifndef OS2Change_Keyboard
#define OS2Change_Keyboard
void delay(void)
{
  char i;
  for (i=0; i<5; i++){};
}
unsigned char CheckKeyRow(unsigned char code)
{
  unsigned char result=0;
  short*PORTB_CTRL=(void*)0xA4000102;
  short*PORTM_CTRL=(void*)0xA4000118;
  char*PORTB=(void*)0xA4000122;
  char*PORTM=(void*)0xA4000138;
  char*PORTA=(void*)0xA4000120;
  short smask;
  char cmask;
  unsigned char column, row;
  column = code>>4;
  row = code &0x0F;
  smask = 0x0003 << (( row %8)*2);
  cmask = ~( 1 << ( row %8) );
  if(row <8)
  {
    *PORTB_CTRL = 0xAAAA ^ smask;
    *PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
    delay();
    *PORTB = cmask;
    *PORTM = (*PORTM & 0xF0 ) | 0x0F;
}
else
{
    *PORTB_CTRL = 0xAAAA;
    *PORTM_CTRL = ((*PORTM_CTRL & 0xFF00 ) | 0x00AA)  ^ smask;
    delay();
    *PORTB = 0xFF;
    *PORTM = (*PORTM & 0xF0 ) | cmask;
}

delay();
result = (~(*PORTA))>>column & 1;
delay();
*PORTB_CTRL = 0xAAAA;
*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x00AA;
delay();
*PORTB_CTRL = 0x5555;
*PORTM_CTRL = (*PORTM_CTRL & 0xFF00 ) | 0x0055;
delay();

return result;
}

unsigned char KeyDown(unsigned char keycode)
{
  unsigned short key[8];
  const unsigned short* keyboardregister = (unsigned short*)0xA44B0000;
  if(isOS2)
  {
    unsigned char row = keycode%10;
    memcpy(key, keyboardregister, sizeof(unsigned short) << 3);
    
    return (0 != (key[row >> 1] & 1 << keycode / 10 - 1 + ((row & 1) << 3)));
}
else
{
    return CheckKeyRow((keycode % 10) + ((keycode / 10 - 1) << 4));
}
}
unsigned char GetKeyMod(unsigned int *key)
{
  unsigned char x, ret;
  
  ret = GetKey(key);
  
  for(x = 0; x < 80; x++)
  {
    if(KeyDown(x))
    {
       *key = x;
       break;
   }
}
return ret;
}
#define IsKeyDown(x) KeyDown(x)
#define IsKeyUp(x) !KeyDown(x)
#define GetKey(x) GetKeyMod(x)
#endif
#endif
