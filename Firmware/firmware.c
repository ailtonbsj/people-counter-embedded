#include <firmware.h>
#include <usb_cdc.h>
#include <stdlib.h>

#define LCD_ENABLE_PIN PIN_D3
#define LCD_RW_PIN PIN_D2
#define LCD_RS_PIN PIN_D1
#define LCD_DATA4 PIN_D4
#define LCD_DATA5 PIN_D5
#define LCD_DATA6 PIN_D6
#define LCD_DATA7 PIN_D7
#define LCD_TYPE 2
#include <lcd.c>
int sensor1=0;
int sensor2=0;
int antsen1=0;
int antsen2=0;
int novo=0;
int32 pessoa=0;
int pcativo=1;
int contKey=0; // setador de coluna no Teclado matricial
char tecla='N';//Guarda tecla pressionada
int ativo=0; //Ativa captura de teclas
int posicao=0; //Guarda posicao de alteracao da String
char numero[16]={"0"};//Guarda numero digitado
int debouncer=20; //usada no debouncer

void conta()
{ //inicio
   if(sensor1==1 && sensor2==0) // verifica se o PRIMEIRO
   {                                               // sensor foi acionado
      if(novo==1) //se variavel novo for 1 entao pessoa ja passou por um sensor
      {
         pessoa-=1; // Decremente pessoa da quantidade total
         pcativo=1; //ativa envio de dados ao PC
         novo=0; // informa que pessoa ja passou
      }
      else
      {
         novo=1; //informa que pessoa passou pelo sensor 1
      }
   }
   if(sensor1==0 && sensor2==1) // verifica se o SEGUNDO
   {                                               // sensor foi acionado
      if(novo==1) //se variavel novo for 1 entao pessoa ja passou por um sensor
      {
         pessoa+=1; // Incremente pessoa da quantidade total
         pcativo=1; //Ativa envio de dados para o PC
         novo=0; // informa que pessoa ja passou
      }
      else
      {
         novo=1; //informa que pessoa passou pelo sensor 2
      }
   }
   //fim
}
#int_TIMER0
void  TIMER0_isr(void) 
{
   //Parte SENSORES
   if(sensor1!=antsen1 || sensor2!=antsen2)
   {
   conta();
   antsen1=sensor1;
   antsen2=sensor2;
   }
   //PARTE TECLADO
   contKey+=1; //INICIO DRIVER TECLADO MATRICIAL
   if(contKey>2) contkey=0;
   switch(contKey)
   {
      case 0:
      output_high(PIN_B0);
      output_low(PIN_B1);
      output_low(PIN_B2);
      break;
      case 1:
      output_high(PIN_B1);
      output_low(PIN_B0);
      output_low(PIN_B2);
      break;
      case 2:
      output_high(PIN_B2);
      output_low(PIN_B1);
      output_low(PIN_B0);
      break;
   }
if(debouncer==0)
{
   if(input_state(PIN_A0))
   {
      switch(contKey)
      {
         case 0:
         tecla='1';
         break;
         case 1:
         tecla='2';
         break;
         case 2:
         tecla='3';
      }
      debouncer=20;
   }
   else if(input_state(PIN_A1))
   {
      switch(contKey)
      {
         case 0:
         tecla='4';
         break;
         case 1:
         tecla='5';
         break;
         case 2:
         tecla='6';
      }
      debouncer=20;
   }
   else if(input_state(PIN_A2))
   {
      switch(contKey)
      {
         case 0:
         tecla='7';
         break;
         case 1:
         tecla='8';
         break;
         case 2:
         tecla='9';
      }
      debouncer=20;
   }
   else if(input_state(PIN_A3))
   {
      switch(contKey)
      {
         case 0:
         tecla='#';
         break;
         case 1:
         tecla='0';
         break;
         case 2:
         tecla='*';
      }
      debouncer=20;
   }
   if(tecla=='#') //Ativa captura de teclas caso # pressionado
   {
      ativo=1; //entra no loop para informa que esta capturando
      posicao=0; // muda para tecla capturada seja inserida no
                 //  primeiro algarismo do numero digitado
   }
   if(tecla=='*') //desativa captura de teclas caso * pressionado
   {
      ativo=0; //sai do loop de captura e volta ao normal
      pessoa=atoi32(numero); // converte string em numero
      pcativo=1; // ativa envio para o PC
   }
   if(tecla!='#' && tecla!='*' && tecla!='N') //caso tecla não seja #, * ou nula
   {                                          //capture e guarde na string
      numero[posicao]=tecla;
      posicao+=1;
      numero[posicao]='\0';
   }
   tecla='N'; // anula tecla capturada
}
if(debouncer>0) debouncer-=1; //FIM DRIVER TECLADO
}
   

#int_RB
void  RB_isr(void) 
{
   if(input_state(PIN_B5))
   sensor1=1;
   else sensor1=0;
   if(input_state(PIN_B4))
   sensor2=1;
   else sensor2=0;
}



void main()
{

   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(INT_RB);
   enable_interrupts(GLOBAL);
   usb_cdc_init();
   usb_init();
   usb_task();
   lcd_init();
   // TODO: USER CODE!!
   while(true)
   {
      printf(lcd_putc,"\fQuantidade:\n%li",pessoa); //Mostra total de pessoas
      while(ativo==1) //loop para teclas pressionadas caso captura ativada
      {
         printf(lcd_putc,"\fAlterar para:\n%s",numero);
         delay_ms(300);
         printf(lcd_putc,"\f"); // delays e printf para efeito de piscar
         delay_ms(300);
      }
      while(pcativo==1) //loop para envio de dados para o PC caso ativado
      {
      printf(usb_cdc_putc,"%li",pessoa);
      pcativo=0;
      }
      delay_ms(1000);
   }
}

