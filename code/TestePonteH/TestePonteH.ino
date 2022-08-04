/*
 * @file: TestePonteH.ino
 * @date: August 03 2022
 * @author: Thales Alves
 * @brief: Control Motor Speed (PWM) through Serial
 */

int throttle;
int pot;
 
void setup() {

  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  pinMode(D3, OUTPUT);
  //pinMode(D1, INPUT);
}

void loop() {
  
pot = analogRead(A0);
  
  while (Serial.available() > 0)
  {
    //Create a place to hold the incoming message
    static char message[12];
    static unsigned int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < 12 - 1) )
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
      throttle = atoi(message);
      //throttle = throttle * 10.23;
      //Serial.println(throttle);
      //Reset for the next message
      message_pos = 0;
    }    
  }

  if(throttle > 0){
    analogWrite(D4, pot);
  }else if(throttle < 0){
    throttle = throttle * -1;
    Serial.println(pot);
    analogWrite(D3, pot);
  }else{
    analogWrite(D4, 0);
    analogWrite(D3, 0);
  }
  
}
