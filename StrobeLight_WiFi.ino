#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"


ESP8266WebServer server(80);

#define _cLEDpin 5
#define _mLEDpin 2

#define _cDuty 50
#define _minFreq 10 //*0.1Hz
#define _maxFreq  1200 //*0.1Hz
#define _maxOn 1000 // *0.1ms
#define _Pause 100 //ms

unsigned int count=0;
unsigned int cPeriod=200; //*0.1ms
unsigned int cOn=cPeriod/_cDuty; 

int strobeFreq = 1;

/*
* Change the state of the LED
*/
void ICACHE_RAM_ATTR onTimerISR(){ // -------- Interupt routine ----------------
  if (count==0){ 
    digitalWrite(_cLEDpin,HIGH); //LED On 
    digitalWrite(_mLEDpin,HIGH); //LED On 
  }
  if (count>=cOn){ 
    digitalWrite(_cLEDpin,LOW); //LED Off 
    digitalWrite(_mLEDpin,LOW); //LED Off 
  }
  count++;
  if (count>=cPeriod) count=0;
}

/*
* Set interrupt timer for ESP8266
*/
void setTimer2(){ 
  timer1_isr_init();      
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
  timer1_write(500);
      // Fout = (80mHz / 16) / 500 = 10kHz
}

/**
* Arduino Setup function.
*/
void setup(){
  pinMode(_cLEDpin,OUTPUT);
  pinMode(_mLEDpin,OUTPUT);
  digitalWrite(_cLEDpin,LOW); //LED Off 
  digitalWrite(_mLEDpin,LOW); //LED Off 
  Serial.begin(115200);
  
  WiFi.softAP("StrobeLight", "");
  
   server.on("/", mainHTML);
   server.on("/ctl", timeControl); 
   server.begin();

   setTimer2();
}

/**
* Default Arduino Function
*/
void loop(){   
  server.handleClient(); 
  delay(_Pause);  
}

// Include web page header
#include "src/web_page.h"

/**
* Displays the start page
*/
void mainHTML(){ 
 server.send(200,"text/html",formHTML());
}

/**
 * Control led blinking frequency.
 */
void timeControl(){
   strobeFreq = server.arg("freq").toInt();
   Serial.println(strobeFreq);
   strobeFreq *= 10;
   

  unsigned int Period=100000/strobeFreq; //*0.1ms 
  unsigned int On=Period/_cDuty; //*0.1ms  
  if (On<_maxOn) cOn=On; else cOn=_maxOn;
  cPeriod=Period;
  
  server.send(200,"text/html",formHTML());
}
