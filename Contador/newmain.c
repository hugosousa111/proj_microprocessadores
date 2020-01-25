// PIC18F4550 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1L
#pragma config PLLDIV = 1       // PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
#pragma config CPUDIV = OSC1_PLL2// System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
#pragma config USBDIV = 1       // USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

// CONFIG1H
#pragma config FOSC = XT_XT     // Oscillator Selection bits (XT oscillator (XT))
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
#pragma config PBADEN = OFF      // PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config ICPRT = OFF      // Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

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

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000 //esse define é para poder usar o delay corretamente com a freq

int d1 = 0; //***********************************************************************************************
int d2 = 0; //essas variaveis recebem o valor que deveria ta no display, devem ser corrigidas para o padrao bcd
int d3 = 0; //***********************************************************************************************
int A;      //variavel contadora da inturrupção
char disp1, disp2, disp3; //são variaveis que vão receber os valores do bcd para mandar pro display

const char bcd2seg[] = {
    //valores correspondentes do display
    0x40, //0
    0x79, //1
    0x24, //2
    0x30, //3
    0x19, //4
    0x12, //5
    0x02, //6
    0x78, //7
    0x00, //8
    0x10, //9
};

void main(void) {
    T2CONbits.T2CKPS = 0b01;    // PRESCALER 1:1
    T2CONbits.TOUTPS = 0b0000;  // POSTSCALER 1:1
    PR2 = 100;                  // Valor do estouro do timer
    TMR2IE = 1;                 // Ativa interrupção do timer
    TMR2ON = 1;                 // Ativa contagem do timer
    TRISD = 0x00;               //PORTD como saida
    TRISB = 0x00;               //PORTB como saida
    GIE = 1;                    // Habilita todas as interrupções
    PEIE = 1;                   // Habilita interrupção dos periféricos

    while (1) {
        //o programa principal fica fazendo a contagem dos segundos 
        disp1 = bcd2seg[d1]; //1
        disp2 = bcd2seg[d2]; //2
        disp3 = bcd2seg[d3]; //3
        //1 é o display da centena
        //2 é o display da dezena
        //3 é o display da unidade
        d3++;
        if (d3 == 10) {
            d2++;
            d3 = 0;
            if (d2 == 10) {
                d1++;
                d2 = 0;
                if (d1 == 10) {
                    d1 = 0;
                }
            }
        }
        __delay_ms(1000); //delay de 1 segundo
    }
}

void interrupt rotina_interrup() {
    if (TMR2IF) {
        TMR2IF = 0; //Zera a flag de TMR2
        PORTDbits.RD0 = 1;  //****************************************
        PORTDbits.RD1 = 1;  //apaga os 3 e dentro dos ifs acende só um 
        PORTDbits.RD2 = 1;  //****************************************
        A++; //Incrementa variável contadora
        if (A == 1) {
            PORTB = disp1;    //joga o padrão q esta dentro da variavel no display
            PORTDbits.RD0 = 0;
        } else if (A == 2) {
            PORTB = disp2;
            PORTDbits.RD1 = 0;
        } else if (A == 3) {
            PORTB = disp3;
            PORTDbits.RD2 = 0;
            A = 0;
        }
    }
}
