void SetPin()
{
    if(is_SH4)
    {
        *(unsigned char*)SH7305_PJDR |= 0x04;
        *(unsigned char*)SH7305_PJDR &= ~0x08;
        //set pin to 0x4B
    }
    else 
    {
        *(unsigned char*)SH7337_SCPDR |= 0x01;
    }
}

void ResetPin()
{
if(is_SH4)
    {
        *(unsigned char*)SH7305_PJDR &= ~0x04;
        *(unsigned char*)SH7305_PJDR |= 0x08;
        // set the pin to 0x47
    }
    else 
    {

        *(unsigned char*)SH7337_SCPDR &= ~0x01;
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