#include <LiquidCrystal.h>
#define NUM_SAMPLES 10

float sum = 0;
unsigned char sample_count = 0;
float voltage = 0.0;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int PT = A0;
const int CT = A1;
float Cur;
float Vol;
float Power;


//float pulsewidth=0;
//float powerfactor=0;
//float phase=0;
int pin = 8;
float rads = 57.29577951; // 1 radian = approx 57 deg.
float degree = 360;
float frequency = 50;
float nano = 1 * pow (10,-6); // Multiplication factor to convert nano seconds into seconds

// Define floats to contain calculations

float pf;
float angle;
float pf_max = 0;
float angle_max = 0;
int ctr;


int mVperAmp = 100; // 185 = ACS712 5A, 100 = ACS712 20A, 66 = ACS712-30A
int ACSoffset = 2500; 


double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;


void setup() {

  pinMode(PT, INPUT);
  pinMode(CT, INPUT);
  pinMode(pin, INPUT);
  lcd.begin(20, 4);
  lcd.setCursor(1,1);
  lcd.print("EEE_428 PROJECT");
  delay(1000);
  lcd.clear();
  
  Constants();
}

void loop() {
  lcd.setCursor(0,2);
  ShowVoltage(9,0,PT);
  Vol = voltage;


 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707;  //root 2 is 0.707
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 Cur = AmpsRMS;
 lcd.setCursor(9,1);
 lcd.print(Cur);
 // ShowCurrent(9,1,CT);
  
  Power = Vol*Cur;
  lcd.setCursor(7,2);
  lcd.print(Power);
  
  //pulsewidth=pulseIn(8,HIGH);
  //phase=2*180*50*pulsewidth/1000000;
  //phase=180-phase;
  //powerfactor=cos(phase*3.1416/180);
 for (ctr = 0; ctr <=2; ctr++) // Perform 4 measurements then reset
  {
  // 1st line calculates the phase angle in degrees from differentiated time pulse
  // Function COS uses radians not Degree's hence conversion made by dividing angle / 57.2958
   angle = ((((pulseIn(pin, HIGH)) * nano)* degree)* frequency);
  // pf = cos(angle / rads);
  
   if (angle > angle_max) // Test if the angle is maximum angle
     {
      angle_max = angle; // If maximum record in variable "angle_max"
      pf_max = cos(angle_max / rads); // Calc PF from "angle_max"
     }
   }
   if (angle_max > 360) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }
   if (angle_max == 0) // If the calculation is higher than 360 do following...
   {
    angle_max = 0; // assign the 0 to "angle_max"
    pf_max = 1; // Assign the Unity PF to "pf_max"
   }



  

  //lcd.setCursor(7,3);
  //lcd.print(angle_max);
  lcd.setCursor(14,3);
  lcd.print(pf_max);

  angle = 0; // Reset variables for next test
  angle_max = 0;
  

}

void Constants() {
  lcd.setCursor(0,0);
  lcd.print("Voltage: ");
  lcd.setCursor(0,1);
  lcd.print("Current: ");
  lcd.setCursor(0,2);
  lcd.print("Power: ");
  lcd.setCursor(0,3);
  lcd.print("power_factor: ");
  
  lcd.setCursor(17,0);
  lcd.print("V");
  lcd.setCursor(14,1);
  lcd.print("A");
  lcd.setCursor(14,2);
  lcd.print("W");

  lcd.setCursor(1,3);
  //lcd.print("== Energy Meter ==");  
}


 
  

void ShowVoltage (int x, int y, unsigned int value)
{
  while (sample_count < NUM_SAMPLES)
  {
    sum += analogRead(value);
    sample_count++;
    delay(1);
  }

  voltage = ((float)sum / (float)NUM_SAMPLES * 300) / 1023.0;
  lcd.setCursor(x,y);
  lcd.print(voltage);
  sample_count = 0;
  sum = 0;  
}


float getVPP()
{
  float result;
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();
   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(CT);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the minimum sensor value*/
           minValue = readValue;
       }
   }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5.0)/1024.0;
      
   return result;
 }
