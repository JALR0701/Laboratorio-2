#include <stdint.h>
#include <pic16f887.h>
#include "Multiplexar.h"

void initMultiplex(unsigned int valor){
    unsigned char const SEGMENT_MAP[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0X39, 0X5E, 0X79, 0X71}; //Mapeo de valores en hexadecimal para el display 7 segmentos.
            if(PORTDbits.RD0 == 1){
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 0;
                PORTC = SEGMENT_MAP[valor];
                PORTDbits.RD1 = 1;
            }else {
                PORTDbits.RD1 = 0;
                PORTDbits.RD0 = 0;
                PORTC = SEGMENT_MAP[valor];
                PORTDbits.RD0 = 1;
            }
}