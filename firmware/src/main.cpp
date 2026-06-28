#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>


// Temp Sensor at PC0, A0
#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1


void adcInit(){
  // Enable adc, prescaler 128
  ADCSRA = (1 << ADEN) |
             (1 << ADPS2) |
             (1 << ADPS1) |
             (1 << ADPS0);
  ADMUX = (1 << REFS0);
  // PIN ADC0, PC0
}

void uartInit(unsigned int ubrr){
  // set boud rate
  UBRR0H = (unsigned int)(ubrr >> 8);
  UBRR0L = (unsigned int)ubrr;


  UCSR0C = 0;
  UCSR0C |= (0 << UMSEL00) | (0 << UMSEL01); // Asynchronous USART
  UCSR0C |= (0 << UPM00) | (0 << UPM01); // Parity mode - disabled
  UCSR0C |= (0 << USBS0); // Stop bit - 1bit

  UCSR0B |= (1 << UCSZ02); // 8 bit comunnication - 0-7

  UCSR0B |= (1 << TXEN0); // enable TX 

}

uint16_t adcRead(){
  ADCSRA |= (1 << ADSC); // Start conversation
  while(ADCSRA & (1 << ADSC)); // wait for finishing conversation
  return ADC;
}

float adcToTemp(uint16_t adc){
  float v = adc * (5.0 / 1023.0);
  return v * 100.0;
} 

void uartSendChar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uartSendString(const char *s) {
    while (*s) uartSendChar(*s++);
}

void uartSendFloat(float v) {
    char buf[20];
    dtostrf(v, 4, 2, buf);
    uartSendString(buf);
    uartSendString("\r\n");
}

int main(){
  DDRC &= ~(1 << PC0);   // PC0 jako wejście
  PORTC &= ~(1 << PC0);  // wyłącz pull-up

  adcInit();
  uartInit(MYUBRR);
  while(1){
    uint16_t raw = adcRead();
    float temp = adcToTemp(raw);


    char bufor[50];

    sprintf(bufor, "raw: %d\r\n", raw);
    uartSendString(bufor);
    uartSendString("Temp: ");
    uartSendFloat(temp);

    _delay_ms(1000);
  }
}