/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
#include "fxlib.h"
#include "stdio.h"
#include "stdlib.h"

#include "SH4comp.c"
#include "addresses.h"
#include "Sound4Calc.h"

/*****************************************************************/
/*                                                               */
/*   CASIO fx-9860G SDK Library                                  */
/*                                                               */
/*   File name : [ProjectName].c                                 */
/*                                                               */
/*   Copyright (c) 2006 CASIO COMPUTER CO., LTD.                 */
/*                                                               */
/*****************************************************************/
int AddIn_main(int isAppli, unsigned short OptionNum)
{
    unsigned int key;
    char buffer[50];

    int sleep = 2000;
    int i;
    
    setup();

    while(1)
    {
        Bdisp_AllClr_VRAM();
        
        sprintf(buffer ,"- %d +",sleep);
        PrintMini(1, 1, (const unsigned char*)buffer, 0);

        sprintf(buffer ,"MPU : %d",is_SH4);
        PrintMini(1, 10, (const unsigned char*)buffer, 0);

        GetKey(&key);

        switch(key)
        {
            case KEY_CTRL_RIGHT : sleep+=10; break;
            case KEY_CTRL_LEFT : sleep-=10; break;
            case KEY_CTRL_EXE :
            while(IsKeyDown(KEY_CTRL_EXE))
            {
                Pin();
                for(i=0;i<sleep;i++);
                Pin();
            }
            break;
            case KEY_CTRL_EXIT :
            return 1;
        } 
    }
return 1; // this point is never reached
}

void setup()
{
    is_SH4 = getMPU();

    if(is_SH4)
    {
    // inital value : xxxx 01xx
        *(unsigned char*)SH7305_PJDR |= 0x04;
        *(unsigned char*)SH7305_PJDR &= ~0x08;
    // final value :  xxxx 10xx
    // now, we can do 'xor' to make sound

    // SCIF2 clock on (MSTPCR0.MSTP007) 
        *(unsigned int*)SH7305_MSTPCR0 &= ~0x00000080;
    // switch off SCSMR_2.TE and SCSMR_2.RE 
        *(unsigned short*)SH7305_SCSCR &= ~0x0030;
    // SCIF2 clock off (MSTPCR0.MSTP007)
        *(unsigned int*)SH7305_MSTPCR0 |= 0x00000080;

    // set bit 3 of port U to output mode
        *(unsigned short*)SH7305_PUCR = ( *(unsigned short*)SH7305_PUCR & ~0x00C0 ) | 0x0040;
    // set bit 4 and 5 of port U
        *(unsigned char*)SH7305_PUDR |= 0x0C;

    // set port J bit 2 to output mode  
        *(unsigned short*)SH7305_PJCR = ( *(unsigned short*)SH7305_PJCR & ~0x0030 ) | 0x0010;
    // set port J bit 3 to output mode
        *(unsigned short*)SH7305_PJCR = ( *(unsigned short*)SH7305_PJCR & ~0x00C0 ) | 0x0040;
    }
    else
    {
    // SCIF2 clock on (STBCR3.MSTP31)
        *(unsigned char*)SH7337_STBCR3 &= ~0x02;
    // switch off SCSMR_2.TE and SCSMR_2.RE 
        *(unsigned short*)SH7337_SCSCR2 &= ~0x0030;
    // SCIF2 clock off (STBCR3.MSTP31)
        *(unsigned char*)SH7337_STBCR3 |= 0x02;
    // set bit 6 of port G to output mode
        *(unsigned short*)SH7337_PGCR = ( *(unsigned short*)SH7337_PGCR & ~0x3000 ) | 0x1000;
    // set bit 5 and 6 of port G
        *(unsigned char*)SH7337_PGDR |= 0x60;
    // set port SC bit 0 to output  
        *(unsigned short*)SH7337_SCPCR = ( *(unsigned short*)SH7337_SCPCR & ~0x0003 ) | 0x0001;
    }
    /*
    // set port J bit 2 to output   
    *(unsigned short*)0xA4050110 = ( *(unsigned short*)0xA4050110 & ~0x0030 ) | 0x0010;
    // set port J bit 3 to input    
    *(unsigned short*)0xA4050110 = ( *(unsigned short*)0xA4050110 & ~0x00C0 ) | 0x0080;*/
}

void Pin(void)
{
    if(is_SH4)
    {
        *(unsigned char*)SH7305_PJDR ^= 0x0C;
        //set pin to 0x4B or reset it to 0x47
    }
    else 
    {
        *(unsigned char*)SH7337_SCPDR ^= 0x01;
    }
}


char getMPU(void)
{
    // Port L control register.
    volatile unsigned short *plcr = (unsigned short *)0xa4000114;
    // Saved value for PLCR.
    unsigned short saved_plcr;
    unsigned int tested_plcr;

    saved_plcr = *plcr;
    *plcr = 0xffff;

    tested_plcr = *plcr;
    *plcr = saved_plcr;

    if(tested_plcr == 0x00ff || tested_plcr == 0x0fff)
    {
        return 0; // MPU_SH3
    }

    return 1; // MPU_SH4
}

//****************************************************************************
//**************                                              ****************
//**************                 Notice!                      ****************
//**************                                              ****************
//**************  Please do not change the following source.  ****************
//**************                                              ****************
//****************************************************************************


#pragma section _BR_Size
unsigned long BR_Size;
#pragma section


#pragma section _TOP

//****************************************************************************
//  InitializeSystem
//
//  param   :   isAppli   : 1 = Application / 0 = eActivity
//              OptionNum : Option Number (only eActivity)
//
//  retval  :   1 = No error / 0 = Error
//
//****************************************************************************
int InitializeSystem(int isAppli, unsigned short OptionNum)
{
    return INIT_ADDIN_APPLICATION(isAppli, OptionNum);
}

#pragma section