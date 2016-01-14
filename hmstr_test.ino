/*
Adafruit Arduino - Lesson 15. Bi-directional Motor
*/

#include <SoftwareSerial.h>

int left_enable = 11;
int in1Pin = 10;
int in2Pin = 9;
int potPin = 0;
int right_forward = 5;
int right_reverse = 6;
int right_enable = 3;
int bluetoothTx = 12;  // TX-O pin of bluetooth mate, Arduino D12
int bluetoothRx = 13;  // RX-I pin of bluetooth mate, Arduino D13

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  Serial.begin(9600);  // Begin the serial monitor at 9600bps
  //bluetooth.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  //bluetooth.print("$");  // Print three times individually
  //bluetooth.print("$");
  //bluetooth.print("$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  //bluetooth.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  bluetooth.begin(9600);  // Start bluetooth serial at 9600
  //bluetooth.listen();
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(right_forward, OUTPUT);
  pinMode(right_reverse, OUTPUT);
  pinMode(right_enable, OUTPUT);
  pinMode(left_enable, OUTPUT);
}

void loop()
{
  
  //int speed = 0;
  //boolean reverse = false;
  //setMotor(speed, reverse);
  /*
  while (bluetooth.available() > 0) {
    // read value of character
    int c = bluetooth.read();
    if (c == 255)
    {
      Serial.print("Shit, no data");
    }
    if (c == 97)
    {
      Serial.print("A received");
    }
    // write value to serial port
    Serial.print(c);
  }
*/
  if(bluetooth.available())  // If the bluetooth sent any characters
  {
    // Send any characters the bluetooth prints to the serial monitor
    byte message = (byte) bluetooth.read();
    
    if (message & 0x80)
    {
      if (message & 0x40)
      {
        Serial.print("Backward  ");
        Serial.println(message & 0x3F);
        setMotor((message & 0x3F) << 2, 0);
      }
      else
      {
        Serial.print("Forward   ");
        Serial.println(message & 0x3F);
        setMotor((message & 0x3F) << 2, 1);
      }
    }
    else
    {
      if (message & 0x40)
      {
        Serial.print("Right     ");
        Serial.println(message & 0x3F);
      }
      else
      {
        Serial.print("Left      ");
        Serial.println(message & 0x3F);
      }
    }
  }
  if(Serial.available())  // If stuff was typed in the serial monitor
  {
    // Send any characters the Serial monitor prints to the bluetooth
    bluetooth.print((char)Serial.read());
  }
  // and loop forever and ever!
 
}

void setMotor(int speed, boolean reverse)
{
  analogWrite(left_enable, speed);
  analogWrite(right_enable, speed);
  digitalWrite(in1Pin, ! reverse);
  digitalWrite(in2Pin, reverse);
  digitalWrite(right_forward, !reverse);
  digitalWrite(right_reverse, reverse);
}
