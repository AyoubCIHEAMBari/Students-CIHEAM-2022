
#include <LiquidCrystal_I2C.h>  // a library for controlling LCD Screens  

int M_Sensor = 34;  //The soil moisture sensor was attached to pin no 34
int value_M;        // a variable to store the sensor reading 
int value_M_soil;   // another variable to map the analog reading of the sensor between 0-100. Note: this do not represent the actual soil moisture it needs calibration 
int LED = 19;       // an LED indicator was attached to pin no 19
int relay = 17;     // The relay oin was attached to pin no 17
int button = 16;    // a button was attached to pin 16
int button_val;     // a variable to store the button value

int pot = 35;       // the pot was attached to pin 35
int pot_val;        // a variable to store the pot value
int pot_val_2 ;     // another variable to approximate the pot value. 
int RESET = 18;     // pin no 18 was attached to the reset pin on the ESP32 (this to reset the code from the program when needed.

LiquidCrystal_I2C lcd (0x27, 16,2);  // Creating the lcd object address 0x3F and 16 columns x 2 rows 


void setup() {
  // put your setup code here, to run once:
  
pinMode(M_Sensor, INPUT);  // Declaring the soil moisture pin as an INPUT 
pinMode(LED, OUTPUT);      // Declaring the LED pin as OUTPUT
pinMode(relay, OUTPUT);    // Declaring the relay pin as an putput 
pinMode(button, INPUT_PULLUP); // declaring the button pin as an INPU_PULLUP
pinMode (pot, INPUT);          // declaring the pot as an input 
pinMode (RESET, INPUT);        // declaring the RESET pin as an input 

digitalWrite (relay, LOW);    // starting the relay as low 


digitalWrite (RESET, HIGH);  // startig the reset as High

Serial.begin(115200);      // starting the serial communication 

 
lcd. begin ();  // Initialize the LCD connected 
lcd. backlight ();  // Turn on the backlight on LCD. 

lcd. print ( "PRORGRAM START" );  // print this Message on the LCD. 
delay(1000); // wait fro one second
lcd.clear(); // clear the message
}


void loop() {
  // put your main code here, to run repeatedly:

button_val = digitalRead (button); // read the button digital value and store it in button_val
value_M = analogRead(M_Sensor); // read the analog value of the sensor and store it in value_M 
value_M_soil = map (analogRead (M_Sensor), 3530, 1450, 10, 100); // mapping the analog value between 10-100. 
pot_val = map(analogRead (pot), 0, 4095, 1000, 10000); // reading the pot value and mapping it between 1000-10000 to represent milliseconds
pot_val_2 = (pot_val/1000) * 1000; // just to get rid of the fraction so the step will be 1000, 2000, 3000 ...., 10000. 
                                   // remeber the variable is an integer it can not store fractions for example 1458 will be 1000 or 3985 will be 3000,  etc.
                                    
lcd. setCursor (0, 0); //set the cursor in the begining of the LCD screen
lcd. print ( "moisture: " ); // print the word "moisture: " 
lcd. print (value_M_soil); // then add the soil moisture value
lcd. print  (" %" );       // then add %
lcd. setCursor (0, 1); // place the cursor on first position (col: 0) of the second line (row: 1) 
lcd. print ( "seconds: " ); // then print the word "seconds: " 
lcd. print (pot_val_2);     // print the milliseconds 


Serial.print ("moisture: ");       
Serial.print (value_M); Serial.print (", button value: ");
Serial.print (button_val);
Serial.print (", irrigation time: ");
Serial.println (pot_val_2);

/*----------------------------------------------------------------------------------------------BEHAVIOR_01-----------------------------------------------------------------------------------------------------
// in this behavior, once the soil is dry the system will start for a certian time then it will stop and read the sensor again.
//if the soil still dry it will start again until it finishs the irrigation. 
if (value_M > 2700) { // the soil is dry  
  digitalWrite (relay, LOW); // start the pump
  delay(pot_val_2);               // for three seconds
  digitalWrite (relay, HIGH); // stop the pump
  delay(5000);                // for four seconds
}
//---------------------------------------------------------------------------------------------END_BEHAVIOR_1--------------------------------------------------------------------------------------------------*/


//----------------------------------------------------------------------------------------------BEHAVIOR_02-----------------------------------------------------------------------------------------------------
// in this behavior the sensor will read the moisture and the user will start the pump for a certian time using a button. in this cas a reset button had to be pressed to read the moisture again.

if (value_M > 1600 && button_val == 0) { // the soil is dry and the button was switched on 
  digitalWrite (relay, HIGH);   // start the pump
  digitalWrite (LED, HIGH);
  delay(pot_val_2);                // for three seconds
  digitalWrite (LED, LOW);
  digitalWrite (relay, LOW);// stop the pump
  lcd.clear();
  lcd. print ( "Finished" );
  delay(3000);
  lcd.clear();
  ESP.restart();
}
//---------------------------------------------------------------------------------------------END_BEHAVIOR_1--------------------------------------------------------------------------------------------------*/
}
