//-----------------------------
// by Fahad Alduraibi 2015
// http://www.fadvisor.net
//
// How it works:
// ------------
// 
// The Android app RoboRC sends three bytes in every iteration, the first byte is always "102" (ascii 'f')
// which is used to detect a new transmission or packet. The two other bytes represents the two sliders
// from the RoboRC app adn the range for each is from 0 to 100 (should not use any range higher than 102)
// the middle value 50 means it is in STOP, 51~100 is FORWARD and 0~49 is BACKWARD.
// The arduino app maps these ranges to 0~255 and use it with PWM pins to control speed.
//
// A reset counter is used to stop the robot/car in case it drove far away from bluetooth range and cannot
// receive any more packets.
//-----------------------------

#define M0F 3   // Motor 0 Forward
#define M0B 5   // Motor 0 Backward
#define M1F 6   // Motor 1 Forward
#define M1B 9   // Motor 1 Backward

#define LED 13  // LED to blink if no bluetooth connection/packets

byte pos;
unsigned int reset=0;  // A counter to stop motors if it didn't receive bluetooth packates after some time (MAX for uint = 65,535)
byte M0;  // store Motor 0 value
byte M1;  // store Motor 1 value

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(M0F, OUTPUT);
  pinMode(M0B, OUTPUT);
  pinMode(M1F, OUTPUT);
  pinMode(M1B, OUTPUT);

  pinMode(LED, OUTPUT);

  digitalWrite(M0F, LOW);
  digitalWrite(M0B, LOW);
  digitalWrite(M1F, LOW);
  digitalWrite(M1B, LOW);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  byte b;
  if (Serial.available())
  {
    b = Serial.read();
    if (b == 'f') {
      reset = 40000;  // Timeout counter to stop motors if it didn't receive new packets (out of range), adjust the value to something suitable
      digitalWrite(LED, LOW);  // TUrn off LED since we have received a packet
      pos = 0;
    } 
    else {
      if (pos == 0) {
        M0 = b;
        pos = 1;
      } 
      else {
        M1 = b;

        if (M0 == 50) {
          stopM0();
        } 
        else if (M0 < 50) {
          int speed = map(M0, 49, 0, 0, 255);
          digitalWrite(M0F, LOW);
          analogWrite(M0B, speed);  
        } 
        else if (M0 > 50) {
          int speed = map(M0, 51, 100, 0, 255);
          digitalWrite(M0B, LOW);
          analogWrite(M0F, speed);
        }

        if (M1 == 50) {
          stopM1();
        } 
        else if (M1 < 50) {
          int speed = map(M1, 49, 0, 0, 255);
          digitalWrite(M1F, LOW);
          analogWrite(M1B, speed);
        } 
        else if (M1 > 50) {
          int speed = map(M1, 51, 100, 0, 255);
          digitalWrite(M1B, LOW);
          analogWrite(M1F, speed);
        }
      }
    }
  }

  if ( reset <= 0 ) {
    stop();
    digitalWrite(LED, !digitalRead(LED));
    delay(50);
  } 
  else {
    reset--;
  }
}

void stop() {
  stopM0();
  stopM1();
}

void stopM0() {
  digitalWrite(M0F, LOW);
  digitalWrite(M0B, LOW);
}

void stopM1() {
  digitalWrite(M1F, LOW);
  digitalWrite(M1B, LOW);
}


