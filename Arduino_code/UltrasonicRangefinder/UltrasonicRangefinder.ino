/* This sketch is a combination of the following two tutorial 
   sketches, plus additional code written by the CSUB Fab Lab to
   incorporate a button. Also configured to use the HC-SR04
   rangefinder module instead of the Parallax PING))) module.

   http://www.arduino.cc/en/Tutorial/Ping
   http://www.arduino.cc/en/Tutorial/LiquidCrystal

   
   The above example code is in the public domain.

 */
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// these constant won't change.  They're the pin numbers
// of the sensor's input/output and the button:
const int pingRxPin = 7;
const int pingTxPin = 8;
const int button = 9;

void setup() {
  // initialize LCD, set up display:
  lcd.begin(16, 2);
  lcd.print("    in");
  lcd.setCursor(0, 1);
  lcd.print("    cm");

  // set up pin modes, turn on button pin's pullup resistor
  pinMode(pingTxPin, OUTPUT);
  pinMode(pingRxPin, INPUT);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);

  // begin serial for debugging purposes
  Serial.begin(9600);
  
}

void loop() {
  
  // while the button is held, measure distance
  while (digitalRead(button) == LOW) { 

    // establish variables for duration of the ping,
    // and the distance result in inches and centimeters:
    long duration, inches, cm;
  
    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    
    digitalWrite(pingTxPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingTxPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingTxPin, LOW);
  
    // A HIGH pulse whose duration is the time (in microseconds) from 
    // the sending of the ping to the reception of its echo off of an object.
  
    duration = pulseIn(pingRxPin, HIGH);
  
    // convert the time into a distance
    inches = microsecondsToInches(duration);
    cm = microsecondsToCentimeters(duration);
  
    // move to LCD's first line, blank old measurement, then overwrite
    // with new measurement. Repeat for second line.
    lcd.setCursor(0, 0);
    lcd.print("   ");
    lcd.setCursor(0, 0);
    lcd.print(inches);

    lcd.setCursor(0, 1);
    lcd.print("   ");
    lcd.setCursor(0, 1);
    lcd.print(cm);

    // print distance to serial, for debugging
    Serial.print("in: ");
    Serial.println(inches);
  
    delay(100);
  }
}

long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
