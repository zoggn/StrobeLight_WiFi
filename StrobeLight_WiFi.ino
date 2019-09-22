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

#define _cLEDpin 13
#define _mLEDpin 9

#define _cDuty 16
#define _minFreq 10 //*0.1Hz
#define _maxFreq  1200 //*0.1Hz
#define _maxOn 1000 // *0.1ms
#define _Pause 100 //ms

#include <LiquidCrystal.h>
// initialize the LCD library with interface pins order
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#include <avr/io.h>

void setTimer2(){ 
  //Prescaller->Divider: 
  //7->1024;6->256;5->128;4->64;3->32;2->8;1->1;0->Stop
  //Counter=255 -> Fmin; Counter=0 -> Fmax 
  // Fout=Fclk/(Divider*(OCR2+1))
  
  // ATmega8 only!!! Needs change Timer Registers for ATmega168
  // TCCR2->TCCR2A; OCR2->OCR2A;  
  TCCR2 = 2<<CS20; //Prescaller=2 -> Devider=8
  TCCR2 |= (1<<WGM21); //CTC-mode - Clear Timer on Compare 
  //COM20,COM21=0 -> normal mode
  OCR2=199; // Output Compare Match Registr 
  // Fout=Fclk/(16*(199+1))=16MHz/1600=10KHz 
  TIMSK |= 1<<OCIE2; //Enable Interrupt
}// setTimer2 
//-----------------------------------------------------------------
//Devider=1; OCR2=0 ->Fmax=75KHz WRONG!!!, OCR2=255 ->Fmin=62.5KHz 
//Devider=8; OCR2=31 ->Fmax=62.5KHz, OCR2=255 ->Fmin=7.8KHz
//Devider=8; OCR2=199 ->Fmax=10KHz

unsigned int count=0;
unsigned int cPeriod=200; //*0.1ms
unsigned int cOn=cPeriod/_cDuty; 
  
ISR(TIMER2_COMP_vect){ // -------- Interupt routine ----------------
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

int aS;
void setup(){
  digitalWrite(_cLEDpin,LOW); //LED Off 
  digitalWrite(_mLEDpin,LOW); //LED Off 
  pinMode(_cLEDpin,OUTPUT);
  pinMode(_mLEDpin,OUTPUT);

  setTimer2();
  aS=analogRead(0);
  
  // set up the LCD's number of rows and columns: 
  lcd.begin(8, 2); // 8 columns, 2 rows

}//setup  

#define _aMin 0  
#define _aMax 1023  

void loop(){
  char p=0; 
  // ----------  Check analog control and set Frequencie -------
  int a=analogRead(0);
  aS=(aS+a)>>1; //aS=(aS+a)/2 averaging
  int Freq10=map(aS,_aMin,_aMax,_minFreq,_maxFreq); //*0.1Hz
  unsigned int Period=100000/Freq10; //*0.1ms 
  unsigned int On=Period/_cDuty; //*0.1ms  
  if (On<_maxOn) cOn=On; else cOn=_maxOn;
  cPeriod=Period;
  
  // ---------------- LCD --------------------
  lcd.clear();
  lcd.setCursor(0,0); //column, row
  lcd.print("Freq, Hz"); 
  // Right adjusment
  if (Freq10>=1000) p=2;
  else if (Freq10>=100) p=3;
  else p=4;
  lcd.setCursor(p,1); //second line
  LCDprintHz(Freq10); 
    
  delay(_Pause);  
}//loop  

void LCDprintHz(int dHz){
  int Hz=dHz/10;
  int rHz=dHz-Hz*10;
  lcd.print(Hz,DEC); lcd.print('.'); lcd.print(rHz,DEC); 
}//LCDprintHz   

