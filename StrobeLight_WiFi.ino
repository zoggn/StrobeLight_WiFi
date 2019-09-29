/* 
  Stoboscopee generator with analog control and LCD
  Kubov V.I. 2012
  Digital outputs: 
   Control LED output: digital pin 13, Gnd
   Main LED output : digital pin 9, Gnd
   The LCD circuit:
   * LCD RS pin to digital pin 7
   * LCD En pin to digital pin 6
   * LCD D4 pin to digital pin 5
   * LCD D5 pin to digital pin 4
   * LCD D6 pin to digital pin 3
   * LCD D7 pin to digital pin 2
   * LCD VO pin to Ground
   * Gnd.
  Frequencie control: Variable Resistor 1K - 100K.
   * Vcc;
   * AnalogIn_0 - VR middle point; 
   * Gnd.
*/

#define _cLEDpin 5
#define _mLEDpin 2

#define _cDuty 16
#define _minFreq 10 //*0.1Hz
#define _maxFreq  1200 //*0.1Hz
#define _maxOn 1000 // *0.1ms
#define _Pause 100 //ms

unsigned int count=0;
unsigned int cPeriod=200; //*0.1ms
unsigned int cOn=cPeriod/_cDuty; 

void ICACHE_RAM_ATTR onTimerISR(){ // -------- Interupt routine ----------------
  if (count==0){ 
    digitalWrite(_cLEDpin,HIGH); //LED On 
    digitalWrite(_mLEDpin,HIGH); //LED On 
  }//  
  if (count>=cOn){ // 
    digitalWrite(_cLEDpin,LOW); //LED Off 
    digitalWrite(_mLEDpin,LOW); //LED Off 
  }//
  count++;
  if (count>=cPeriod) count=0;
}//ISR 

void setTimer2(){ 
      timer1_isr_init();      
      timer1_attachInterrupt(onTimerISR);
      timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
      timer1_write(500);
      // Fout = (80mHz / 16) / 500 = 10kHz
}// setTimer2 
  

void setup(){
  pinMode(_cLEDpin,OUTPUT);
  pinMode(_mLEDpin,OUTPUT);
  digitalWrite(_cLEDpin,LOW); //LED Off 
  digitalWrite(_mLEDpin,LOW); //LED Off 

  setTimer2();
  
  // set up the LCD's number of rows and columns: 

}//setup  

#define _aMin 0  
#define _aMax 1023  

void loop(){
  // ----------  Check analog control and set Frequencie -------
  int aS = 2; 
  int Freq10=map(aS,_aMin,_aMax,_minFreq,_maxFreq);
  unsigned int Period=100000/Freq10; //*0.1ms 
  unsigned int On=Period/_cDuty; //*0.1ms  
  if (On<_maxOn) cOn=On; else cOn=_maxOn;
  cPeriod=Period;
  
  // ---------------- LCD --------------------
    
  delay(_Pause);  
}//loop  
