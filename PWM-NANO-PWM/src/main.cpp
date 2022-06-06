#include <Arduino.h>
#include  <avr/io.h>
#include  <util/delay.h>
#include  <avr/interrupt.h>
#include   <avr/wdt.h>
#include  "C:/Users/Ingenieria/Documents/GitHub/JMfunciones/Jfunciones.hpp"
#define VUELTASPWM 50
#define LIMIT_SUP   95 //%
#define LIMIT_INF   75 //%
volatile uint32_t contador=0,ocrInferior,ocrSuperior;
uint8_t sumando = true;
int main(void){
  highBit(DDRB,PB1);
  highBit(DDRB,PB2);
  setTimer1WGM(15);
  setTimer1PRE(1);
  //setOC1A(3);
  setOC1B(2);
  TIMSK1 |= (1<<TOIE1);
  OCR1A = 1024;
  ocrInferior = OCR1A * LIMIT_INF / 100;
  ocrSuperior = OCR1A * LIMIT_SUP / 100;
  OCR1B = ocrInferior;
  sei();
  while(1){  
   }
}
ISR (TIMER1_OVF_vect){
  contador++;
  if (contador>VUELTASPWM){
    if(sumando){
      OCR1B++;
      if(OCR1B > ocrSuperior){
        sumando=false;
      }
    }else{
      OCR1B--;
      if(OCR1B < ocrInferior){
        sumando=true;
      }      
    }
    if (OCR1B > OCR1A) OCR1B=0;
    contador = 0;
  } 
}
void setOC1A(uint8_t nibble){
  if (nibble > 3) nibble = 0;
  TCCR1A |= (((nibble&2)>>1)<<COM1A1) | ((nibble&1)<<COM1A0) ;
}
void setOC1B(uint8_t nibble){
  if (nibble > 3) nibble = 0;
  TCCR1A |= (((nibble&2)>>1)<<COM1B1) | ((nibble&1)<<COM1B0) ;
}
void setTimer1PRE(uint8_t nibble){
  if (nibble > 7) nibble = 0;
  TCCR1B |= (((nibble&2)>>1)<<CS12) | (((nibble&4)>>2)<<CS11) | (nibble&1<<CS10);
}
void setTimer1WGM(uint8_t nibble){
  if (nibble > 15) nibble = 0;
  TCCR1B |= (((nibble&8)>>3)<<WGM13) | (((nibble&4)>>2)<<WGM12);
  TCCR1A |= (((nibble&2)>>1)<<WGM11) | ((nibble&1)<<WGM10);
}