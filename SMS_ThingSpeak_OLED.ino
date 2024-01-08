/* 

The objective of this code if to sense the soil moisture through a capacitive soil moisture sensor and send it to acloud service platform "ThingSpeak" while showing the result on an OLED screen



/*information and pin connections:
- The LCD screen Address is 0x27

- The I2C bus for the 1st BMP180 sensor and the LCD screen is: 
                                   SCL ----> 22
                                   SDA ----> 21
                                   

- the capacitive soil moisture sensor is connected to pin number            
*/

#include "ThingSpeak.h" //This is the Library for the cloud data website "ThingSpeak"
#include "secrets.h"  //this file contains the WiFi network information
#include <WiFi.h> //The library for Wifi connection 

#include <Wire.h>         //adding libraries, by default wire library SCl--22 and SDA--21, so we declared just the secound SDA and the SCL 
#include <Adafruit_Sensor.h>      


#include <SPI.h>
#include <Adafruit_GFX.h>  // library for the OLED screen
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(-1);


char ssid[] = SECRET_SSID;   // your network SSID (WiFi network name) from secrets
char password[] = SECRET_PASS;   // your network password from secrets

WiFiClient  client; 

unsigned long myChannelNumber = SECRET_CH_ID; //ThisngSpeak channel ID added to secrets
const char * myWriteAPIKey = SECRET_WRITE_APIKEY; //ThisngSpeak channel Write API key in Secrets

int LED = 16; //difine the digital pin connected to the LED
int button = 17;
int SMS = 34;
int SMS_Aalog;
int SMS_Val;
int interval = 1000;


void setup() {
  Serial.begin(115200);
  delay(1000); //Take some time to open up the Serial Monitor

   pinMode (LED, OUTPUT);
   pinMode(SMS, INPUT),

   WiFi.mode(WIFI_STA);   //seting the ESP32 to station status
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  
 if(WiFi.status() != WL_CONNECTED){  //this whole part is to check if the ESP32 is connected to the WiFi network
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(3000);     
    } 
    Serial.println("\nConnected.");
 }
  digitalWrite(LED, LOW);

  //initialize with the I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  
  // Clear the buffer.
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30,10); //the first number represent the column and the second is the row
  display.println("SMS");
  display.display();
  delay(2000);
  display.clearDisplay();

}

void loop() { 
 SMS_Aalog= analogRead(SMS); 
 SMS_Val= map( SMS_Aalog, 3056, 1050, 0, 100);

  // Display Text
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(5,10);
   display.print("Soil: ");
  display.print(SMS_Val);
  display.println("%");
  display.display();
  delay(3000);
  digitalWrite(LED, LOW);

  Serial.print("Analog Read from the sensor = ");
  Serial.println(SMS_Aalog);
  Serial.println("----------------------------------------");
  Serial.println("----------------------------------------");

 ThingSpeak.setField(2, SMS_Val); //send the data to the selected field in ThingSpeak if you have more than one data point set the field below in each line
      int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); //convert evrything to integer and send it.
  delay(interval);
  display.clearDisplay();


  if (WiFi.status()!= WL_CONNECTED){
  digitalWrite(LED, HIGH);
    delay(3000);
 
    ESP.restart();
}
 

}
