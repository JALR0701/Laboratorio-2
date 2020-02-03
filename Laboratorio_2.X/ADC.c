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
#include "Multiplexar.h"
#define _XTAL_FREQ 4000000

char change = 0; //Bandera para multiplexar
unsigned int numero_ = 0;
uint8_t contador1 = 0;
unsigned int d1 = 0, d2 = 0;

void __interrupt() ISR (void){
    INTCONbits.GIE = 0; //deshabilitación de interrupciones
    INTCONbits.RBIE = 0;
    INTCONbits.T0IE = 0;
    INTCONbits.PEIE = 0;
    if(INTCONbits.T0IF == 1){ // interrupción del timer0
        TMR0 = 99; //Define el valor precagado del timer0
        change = 1; //Activa el cambio de transistor
    }
    if(INTCONbits.RBIF == 1 && PORTBbits.RB1 == 1){ //interrupción del puerto b
        contador1++;
    }
    if(INTCONbits.RBIF == 1 && PORTBbits.RB0 == 1){
        contador1--;
    }
    if(PIR1bits.ADIF == 1){
        d1 = ADRESH * (0B00001111);
        d2 = ADRESH * (0B11110000)/16;
    }
    INTCONbits.GIE = 1; //Reinicio de banderas y habilitación de interrupciones
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.PEIE = 1;
    PIR1bits.ADIF = 0;
}

void main(void) {
    TRISA = 0; //I/O de puertos
    TRISB = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISC = 0;
    TRISD = 0;
    
    ANSEL = 0; //Pines digitales
    ANSELH = 0;
    
    INTCON = 0; //Habilitación de interrupciones y reinicio de banderas
    INTCONbits.GIE = 1;
    INTCONbits.RBIE = 1;
    INTCONbits.RBIF = 0;
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    
    OPTION_REG = 0; //configuracion para desborde del timer 0
    OPTION_REGbits.PS2 = 1;

    TMR0 = 99; //Precarga del timer 0
    
    IOCB = 0; //Bits del puerto B que provocan interrupciones
    IOCBbits.IOCB0 = 1;
    IOCBbits.IOCB1 = 1;
    
    PORTA = 0; //valor inicial de los puertos
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTDbits.RD1 = 1;
    
    contador1 = 0;
    
    INTCONbits.PEIE = 1;
    PIE1 = 0B01000000;
    PIR1 = 0;
    ANSELHbits.ANS8 = 1;
    ADCON0 = 0B01100001;
    ADCON1 = 0;
    
    while (1){ //Loop
        __delay_ms(10);
        ADCON0bits.GO_DONE = 1;
        __delay_ms(10);
        if(change == 1){ //Cambio de transistor para cada interrupcion del timer 0
            initMultiplex(d1, d2);
            change = 0; //Reinicio de bandera.
        }
        PORTA = contador1;
    }
    
    return; 
}