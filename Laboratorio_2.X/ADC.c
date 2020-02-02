/*
 * File:   ADC.c
 * Author: jorge
 *
 * Created on 1 de febrero de 2020, 06:30 PM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000

char change = 0;
unsigned char const SEGMENT_MAP[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0X39, 0X5E, 0X79, 0X71};

void __interrupt() ISR (void){
    INTCONbits.GIE = 0;
    INTCONbits.RBIE = 0;
    INTCONbits.T0IE = 0;
    if(INTCONbits.T0IF == 1){
        TMR0 = 4;
        change = 1;
    }
    if(INTCONbits.RBIF == 1 && PORTBbits.RB0 == 1){
        PORTA++;
    }
    if(INTCONbits.RBIF == 1 && PORTBbits.RB1 == 1){
        PORTA--;
    }
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
}

void main(void) {
    TRISA = 0;
    TRISB = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0;
    TRISD = 0;
    
    ANSEL = 0;
    ANSELH = 0;
    
    INTCON = 0;
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    
    OPTION_REG = 0;

    TMR0 = 4;
    
    IOCB = 0;
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    PORTA = 0;
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    while (1){
        if(change == 1){
            if(PORTDbits.RD0 == 1){
                PORTDbits.RD0 = 0;
                PORTDbits.RD1 = 0;
                PORTC = SEGMENT_MAP[10];
                PORTDbits.RD1 = 1;
            }else {
                PORTDbits.RD1 = 0;
                PORTDbits.RD0 = 0;
                PORTC = SEGMENT_MAP[11];
                PORTDbits.RD0 = 1;
            }
            change = 0;
        }
        
    }
    
    return;
}