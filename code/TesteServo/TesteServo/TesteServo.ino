#include <Servo.h>

Servo servo;
unsigned int angle;
const unsigned int MAX_MESSAGE_LENGTH = 12;

void setup() {

  servo.attach(D4, 500, 2400);; //D4 
  Serial.begin(115200);

}

void loop() {

  
  
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
    
    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
    
    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
      }
      //Full message received...
      else
      {
      //Add null character to string
      message[message_pos] = '\0';
      
      //Print the message (or do other things)
      Serial.println(message);
      angle = atoi(message);
      Serial.println(angle);
      //Reset for the next message
      message_pos = 0;
    }
  }

  servo.write(angle);
  delay(200);
}
