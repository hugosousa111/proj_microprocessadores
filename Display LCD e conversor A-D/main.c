
#include <xc.h>
#include <stdio.h>
#include "lcd.h"

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = INTOSC_HS        // Oscillator Selection bits (HS oscillator (HS))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = ON         // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Reset Voltage bits (Minimum setting 2.05V)
#pragma config VREGEN = OFF     // USB Voltage Regulator Enable bit (USB voltage regulator disabled)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = ON      // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)


// para escrever caracteres no lcd com printf()

void putch(char data) {
    escreve_lcd(data);
}

#define servo1 PORTCbits.RC0                       

unsigned char duty = 0x00;

//void OpenADC( unsigned int ReadADC config, unsigned char config2, unsigned char portconfig)

unsigned int ADC_Read() {
    unsigned int aux = 0;
    ADCON0bits.GO_DONE = 1;
    while (ADCON0bits.GO_DONE);

    return ADRES; //Return result
}

void main(void) {
    OSCCON = 0b01100000;
    GIE = 0x01;
    PEIE = 0x01;
    T2CON = 0b01111101; // 1111  1101
    TMR2IE = 1;
    ADCON1 = 0x8E;
    ADCON0 = 0x80;
    ADCON2 = 0b10010110;
    ADCON0bits.ADON = 1;
    TRISA = 0x01;
    TRISC = 0x00;
    TRISD = 0x00;
    PORTD = 0;
    PORTA = 0x00;
    PORTC = 0x00;
    duty = 127;
    TMR2ON = 1;

    inicializa_lcd();
    limpa_lcd();
    //mesmo código do exemplo LCD_PWM, as unicas alterações são while(1)
    //que faz a conversão do duty em uma voltagem 
    
    
    while (1) {
        __delay_us(500);
        duty = (unsigned char) (ADC_Read() / 4); //Variável adc recebe valor adc do AN0

        caracter_inicio(1, 3); //define o ponto de inicio da frase na primeira linha
        
        /*Unica alteração do código é a linha a seguir, 
         fazendo uma regra de 3 com os valores do duty 
         que saiam na simulação, percebe-se que quando coloca 
         5V o duty ficava 255, então, para transformar em voltz só 
         dividir por 51.0 (.0 para fica o numero float)*/
        
        printf("Voltz = %.2f", duty / 51.0); 

    }
}

void interrupt tmr2(void) {
    if (TMR2IF) {
        TMR2IF = 0x00;
        if (servo1 == 0) { //Saída servo1 em high? Sim...
            PR2 = duty;
            TMR2 = 0; //TMR0 recebe valor atual do duty
            servo1 = 0x01; //Saída do servo1 em low
        } else {
            PR2 = 255 - duty;
            TMR2 = 0; //TMR0 recebe valor máximo menos valor do duty
            servo1 = 0x00; //Saída do servo1 em high
        }
    }
}

