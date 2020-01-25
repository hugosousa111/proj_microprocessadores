; PIC18F4550 Configuration Bit Settings

; Assembly source line config statements

#include "p18f4550.inc"

; CONFIG1L
  CONFIG  PLLDIV = 1            ; PLL Prescaler Selection bits (No prescale (4 MHz oscillator input drives PLL directly))
  CONFIG  CPUDIV = OSC1_PLL2    ; System Clock Postscaler Selection bits ([Primary Oscillator Src: /1][96 MHz PLL Src: /2])
  CONFIG  USBDIV = 1            ; USB Clock Selection bit (used in Full-Speed USB mode only; UCFG:FSEN = 1) (USB clock source comes directly from the primary oscillator block with no postscale)

; CONFIG1H
  CONFIG  FOSC = EC_EC          ; Oscillator Selection bits (EC oscillator, CLKO function on RA6 (EC))
  CONFIG  FCMEN = OFF           ; Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
  CONFIG  IESO = OFF            ; Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

; CONFIG2L
  CONFIG  PWRT = OFF            ; Power-up Timer Enable bit (PWRT disabled)
  CONFIG  BOR = ON              ; Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
  CONFIG  BORV = 3              ; Brown-out Reset Voltage bits (Minimum setting 2.05V)
  CONFIG  VREGEN = OFF          ; USB Voltage Regulator Enable bit (USB voltage regulator disabled)

; CONFIG2H
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  WDTPS = 32768         ; Watchdog Timer Postscale Select bits (1:32768)

; CONFIG3H
  CONFIG  CCP2MX = ON           ; CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
  CONFIG  PBADEN = OFF           ;----PRECISA SER OFF---- PORTB A/D Enable bit (PORTB<4:0> pins are configured as analog input channels on Reset)
  CONFIG  LPT1OSC = OFF         ; Low-Power Timer 1 Oscillator Enable bit (Timer1 configured for higher power operation)
  CONFIG  MCLRE = ON            ; MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

; CONFIG4L
  CONFIG  STVREN = ON           ; Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
  CONFIG  LVP = ON              ; Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
  CONFIG  ICPRT = OFF           ; Dedicated In-Circuit Debug/Programming Port (ICPORT) Enable bit (ICPORT disabled)
  CONFIG  XINST = OFF           ; Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

; CONFIG5L
  CONFIG  CP0 = OFF             ; Code Protection bit (Block 0 (000800-001FFFh) is not code-protected)
  CONFIG  CP1 = OFF             ; Code Protection bit (Block 1 (002000-003FFFh) is not code-protected)
  CONFIG  CP2 = OFF             ; Code Protection bit (Block 2 (004000-005FFFh) is not code-protected)
  CONFIG  CP3 = OFF             ; Code Protection bit (Block 3 (006000-007FFFh) is not code-protected)

; CONFIG5H
  CONFIG  CPB = OFF             ; Boot Block Code Protection bit (Boot block (000000-0007FFh) is not code-protected)
  CONFIG  CPD = OFF             ; Data EEPROM Code Protection bit (Data EEPROM is not code-protected)

; CONFIG6L
  CONFIG  WRT0 = OFF            ; Write Protection bit (Block 0 (000800-001FFFh) is not write-protected)
  CONFIG  WRT1 = OFF            ; Write Protection bit (Block 1 (002000-003FFFh) is not write-protected)
  CONFIG  WRT2 = OFF            ; Write Protection bit (Block 2 (004000-005FFFh) is not write-protected)
  CONFIG  WRT3 = OFF            ; Write Protection bit (Block 3 (006000-007FFFh) is not write-protected)

; CONFIG6H
  CONFIG  WRTC = OFF            ; Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) are not write-protected)
  CONFIG  WRTB = OFF            ; Boot Block Write Protection bit (Boot block (000000-0007FFh) is not write-protected)
  CONFIG  WRTD = OFF            ; Data EEPROM Write Protection bit (Data EEPROM is not write-protected)

; CONFIG7L
  CONFIG  EBTR0 = OFF           ; Table Read Protection bit (Block 0 (000800-001FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR1 = OFF           ; Table Read Protection bit (Block 1 (002000-003FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR2 = OFF           ; Table Read Protection bit (Block 2 (004000-005FFFh) is not protected from table reads executed in other blocks)
  CONFIG  EBTR3 = OFF           ; Table Read Protection bit (Block 3 (006000-007FFFh) is not protected from table reads executed in other blocks)

; CONFIG7H
  CONFIG  EBTRB = OFF           ; Boot Block Table Read Protection bit (Boot block (000000-0007FFh) is not protected from table reads executed in other blocks)

  

RES_VECT  CODE  0x0000	    ;INICIO DOS COMANDOS NA MEMORIA          
    GOTO    START                   
    
INT_HIGH_VECT CODE 0x0008   ;PARTE DA MEMORIA PARA A INTERRUPCAO
    GOTO INT_HIGH
    
VARIAVEIS UDATA_ACS 0	    ;VARIAVEIS
    ATRASO2 RES 1	    
    ATRASO500 RES 1    
    ATRASO1 RES 1
    ESTADO  RES 1	    ;SE FOR 1: CONTANDO, SE FOR 0: PAUSADO
 
MAIN_PROG CODE		    ;let linker place main program

ATRASO_2MS		    ;ATRASO DE 2 MS
    MOVLW .155		    
    MOVWF ATRASO2
LOOP2
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    NOP
    DECFSZ ATRASO2
    GOTO LOOP2
    RETURN
    
ATRASO_500MS		    ;ATRASO DE 500 MS
    MOVLW .247
    MOVWF ATRASO500
LOOP500
    CALL ATRASO_2MS
    DECFSZ ATRASO500
    GOTO LOOP500
    RETURN

ATRASO_1S		    ;ATRASO DE 1 S
    MOVLW .2
    MOVWF ATRASO1
LOOP1
    CALL ATRASO_500MS
    DECFSZ ATRASO1
    GOTO LOOP1
    RETURN

CONTA			    ;ESPERA E ATUALIZA OS SEGUNDOS 
    INCF PORTD, W	    ;*********************
    DAW			    ;PRA CONTAR EM DECIMAL
    MOVWF PORTD		    ;*********************
    RETURN
    
; TODO ADD INTERRUPTS HERE IF USED

START
    CLRF TRISD		    ;COLOCANDO O PORTD COMO SAIDA 
    CLRF PORTD		    ;DEIXANDO A SAIDA ZERADA
    MOVLW 0x03		    
    MOVWF TRISB		    ;Configura RB0 E RB1 como entrada (Push button)
    BSF INTCON,INT0IE	    ;ATIVANDO AS INTERRUPÇÕES INT0
    BSF INTCON3,INT1IE	    ;ATIVANDO AS INTERRUPÇÕES INT1
    BSF INTCON,GIEH	    ;HABILITA INTERRUPÇOES DE ALTA
    BCF INTCON,GIEL	    ;DESABILITA INTERRUPÇÕES DE BAIXA
    MOVLW b'0000000'
    MOVWF ESTADO	    ;DIZ QUE TA PAUSADO
    
INICIO			    ;ESPERA INICIAL DAS INTERRUPCOES
    CALL ATRASO_1S	    ;ESPERA UM SEGUNDO 
    BTFSC ESTADO,0	    ;SE FOR 0, PULA A PROXIMA
    CALL CONTA		    ;COMECA A CONTAR
    BRA INICIO
  
;**************************************************      
; INT0 E INT1 : Ocorre toda vez que pressionar o 
; push button conectado ao pino RB0/INT0 E RB1/INT1
;**************************************************
    
INT_HIGH
    BTFSC INTCON,INT0IF	    ;Verififica se INT0
    BRA ATENDE_INT0	    ;Trata INT0
    BTFSC INTCON3,INT1IF    ;Verififica se INT1
    BRA ATENDE_INT1	    ;Trata INT1
    RETFIE		    ;SENAO RETORNA DA INTERRUPÇÃO
ATENDE_INT0
    BCF INTCON,INT0IF	    ;Limpa flag da INT0
    BTG ESTADO, 0	    ;INVERTE O VALOR DE ESTADO
    RETFIE
ATENDE_INT1
    BCF INTCON3,INT1IF	    ;Limpa flag da INT0
    BCF ESTADO,0	    ;INFORMANDO QUE ESTA PAUSADO
    CLRF PORTD		    ;ZERA O CRONOMETRO
    RETFIE
    
    
    END