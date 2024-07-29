/*
connections between Arduino and the sensors
 //soil moisture sensor
sensor   | ARDUINO
 VCC    ->  3.3 v
 GND    ->  gnd
 AOUT   ->  A0

 Relay   | ARDUINO
  DC+   -> 5V
  DC-   -> GND
  CH1   -> 8

  LED    | ARDUINO
  RED    -> 9
  GREEN  -> 10
 NB:: connect AREF pin on the Arduino board to the 3.3 v line
coonect

*/

#define SCL A5
#define SDA A4
#define RELAY 8
#define  ONLED 9
#define  MOTORLED 10
bool state = false ;
int moisturesensor =  A0;


const int Airvalue = 680;
const int Watervalue = 350 ;
int SoilMoisturevalue =0;
int SoilMoisturePercentage = 0;

unsigned long now = 0;
unsigned long previous = 0;
int interval = 5000 ;       //variable to hold the value time for hoew long motor should be on
void setup() {
  //set analogread function reference at 3.3V
  analogReference(EXTERNAL);
  //initializing the pins to be used in the program
 Serial.begin(9600);
 pinMode(ONLED,OUTPUT);
 pinMode(MOTORLED , OUTPUT);
 pinMode(RELAY, OUTPUT);
 pinMode( moisturesensor, INPUT);

//LED is put HIGH to indicate that the system has been activated
 digitalWrite(ONLED, HIGH);

 //printing a string on the terminal displaying system activation text
 Serial.println("Water Monitoring System Activated ! !");

 previous = millis();
}

void loop() {
  // main loop
  // calling the Sensor_Reading function
  Sensor_Reading();
  //determining at percentage the Motor should go on 
  if ((SoilMoisturePercentage <= 40 ) && (state == false)) {
     //MOTORLED is set HIGH to indicate watering has sterted
     digitalWrite(MOTORLED, HIGH);
     //put the Relay high to statrt watering 
     digitalWrite(RELAY,  HIGH);
     state = true ;
  }else if ((SoilMoisturePercentage >= 80 ) && (state == true)) {
     //MOTORLED is set LOW to indicate watering has stopped
     digitalWrite(MOTORLED, LOW);
     //put the Relay high to statrt watering 
     digitalWrite(RELAY,  LOW);
     state = false ;
  }

 
 //set the motor on for the specified time
 now = millis();
 if (now - previous >= interval && state == true) {
  previous = millis();
    //MOTORLED and RELAY are set LOW 
     digitalWrite(MOTORLED, LOW);
     digitalWrite(RELAY,  LOW);
     state = false ;
 }


}
//function to take reading from the soil moisture sensor
void Sensor_Reading(void){
  //taking readings from the sensor using analogRead function
  SoilMoisturevalue = analogRead(moisturesensor);
  
  //converting the raw values into percentage
  SoilMoisturePercentage = map(SoilMoisturevalue, Airvalue, Watervalue, 0, 100);

//keep the values between 0 to 100
  SoilMoisturePercentage = constrain(SoilMoisturePercentage, 0, 100);
  //display the value on the terminal
  Serial.println("Moisture Percentage : ");
  Serial.print(String(SoilMoisturePercentage) + " %");
  Serial.println(SoilMoisturevalue);
  Serial.println();

}
