#include "lcd.h" 

// Escreve no LCD os 4 bits menos significativos de dado
void envia_nibble_lcd(char dado)
{ 
  lcd_db4 = dado & 0x01;
  lcd_db5 = (dado >> 1) & 0x01;
  lcd_db6 = (dado >> 2) & 0x01;
  lcd_db7 = (dado >> 3) & 0x01;
  //Gera um pulso de enable
  lcd_enable = 1;
  __delay_us(25); // Recomendado para estabilizar o LCD
  lcd_enable = 0; 
}

// Esta rotina envia um dado ou um comando para o LCD
// endereco = 0 -> a vari�vel dado ser� enviada como uma instru��o
// endereco = 1 -> a vari�vel dado ser� enviada como um caractere

 void envia_byte_lcd(char endereco,char dado)
{
 lcd_rs = endereco; // Seta o bit RS para instru��o ou caractere
 __delay_us(100);   // Aguarda 100 us para o LCD estabilizar
 lcd_enable = 0;    // Desativa a linha ENABLE
 envia_nibble_lcd(dado>>4); // Envia ao LCD os MSBs de dado
 envia_nibble_lcd(dado & 0x0f); // Envia os LSBs de dado
 __delay_us(40); // Aguarda 40us para estabilizar o LCD                                                       // Retorna ao ponto de chamada da fun��o
}

// Esta rotina foi implementada como uma forma conveniente 
// de se escrever um caractere no display

 inline void escreve_lcd(char c){envia_byte_lcd(1,c);};

// Fun��o que envia instru��o para limpar a tela do LCD
void limpa_lcd(void)
{
  envia_byte_lcd(0,0x01); // 0x01 -> instru��o para limpar LCD 
  __delay_ms(2);  // Aguarda 2ms para estabilizar o LCD                                                         // Retorna ao ponto de chamada da fun��o
}

// Fun��o de inicializa��o do LCD
void inicializa_lcd(void)
{
 lcd_db4 = 0;  // Garante que os pino DB4-DB7 est�o em 0 (low)
 lcd_db5 = 0;
 lcd_db6 = 0;
 lcd_db7 = 0;
 __delay_ms(5); // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x03); // Envia comando de inicializa��o
 __delay_ms(5); // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x03); // Envia comando de inicializa��o
 __delay_ms(5); // Aguarda 5ms para estabilizar o LCD
 envia_nibble_lcd(0x02); // Comando para zerar o
 //contador de caracteres e retornar o cursor � posi��o inicial (0x80).
 __delay_ms(1); // Aguarda 1ms para estabilizar o LCD
 envia_byte_lcd(0,0x28); // Comando que configura o LCD para 4 bits, 2 linhas
 // e fonte 5X7.
 envia_byte_lcd(0,0x0c); // Display ligado, sem cursor
 limpa_lcd(); // Limpa tela do LCD
 envia_byte_lcd(0,0x06); // Posiciona o cursor � direita
}

// Esta fun��o define a posi��o da tela do LCD (linha e coluna)
// a partir da qual ser�o realizadas futuras escritas de caracteres. 

 void caracter_inicio(char linha,char coluna)
{
 char posicao=0;
 if(linha == 1)
{
 posicao=0x80; // Endere�o inicial da linha 1 (0x80)
}
 if(linha == 2)
{
 posicao=0xc0; // Endere�o inicial da linha 2 (0xc0)
}

posicao=posicao+coluna; //soma o n�mero da coluna ao endere�o inicial                                                  //subtrai 1 para corrigir posi��o

envia_byte_lcd(0,posicao); // Envia commando
}