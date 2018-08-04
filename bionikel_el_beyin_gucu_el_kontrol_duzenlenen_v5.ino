
#include <Servo.h>

#define BAUDRATE 57600
#define DEBUGOUTPUT 0
#define powercontrol 10

// kontrol degiskenleri
byte generatedChecksum = 0;
byte checksum = 0; 
int payloadLength = 0;
byte payloadData[64] = {
  0};
byte poorQuality = 0;
byte attention = 0;
byte meditation = 0;

// sistem_degiskenleri
long lastReceivedPacket = 0;
boolean bigPacket = false;


Servo servo_bas1;  
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;



//pin tanımlamaları

void setup() {
  Serial.begin(BAUDRATE);           // USB

  
   servo_bas1.attach(8);  
   myservo2.attach(9);
   myservo3.attach(10);
   myservo4.attach(11);
   myservo5.attach(12);
}

////////////////////////////////
// Read data from Serial UART //
////////////////////////////////
byte ReadOneByte() {
  int ByteRead;

  while(!Serial.available());
  ByteRead = Serial.read();

#if DEBUGOUTPUT  
  Serial.print((char)ByteRead);   
#endif

  return ByteRead;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {


  // Look for sync bytes
  if(ReadOneByte() == 170) {
    if(ReadOneByte() == 170) {

      payloadLength = ReadOneByte();
      if(payloadLength > 169)                     
          return;

      generatedChecksum = 0;        
      for(int i = 0; i < payloadLength; i++) {  
        payloadData[i] = ReadOneByte();            
        generatedChecksum += payloadData[i];
      }   

      checksum = ReadOneByte();                      
      generatedChecksum = 255 - generatedChecksum;  

        if(checksum == generatedChecksum) {    

        poorQuality = 200;
        attention = 0;
        meditation = 0;

        for(int i = 0; i < payloadLength; i++) {    
          switch (payloadData[i]) {
          case 2:
            i++;            
            poorQuality = payloadData[i];
            bigPacket = true;            
            break;
          case 4:
            i++;
            attention = payloadData[i];                        
            break;
          case 5:
            i++;
            meditation = payloadData[i];
            break;
          case 0x80:
            i = i + 3;
            break;
          case 0x83:
            i = i + 25;      
            break;
          default:
            break;
          } // switch
        } // for loop

#if !DEBUGOUTPUT

       
        if(bigPacket) {
          Serial.print("PoorQuality: ");
          Serial.print(poorQuality, DEC);
          Serial.print(" Attention: ");
          Serial.print(attention, DEC);
          Serial.print(" Time since last packet: ");
          Serial.print(millis() - lastReceivedPacket, DEC);
          lastReceivedPacket = millis();
          Serial.print("\n");



//Servo acılarının ayarlanması
   
    int servoAng1=(attention / 10)*20;
    if(servoAng1>180){servoAng1=180;}    
    servo_bas1.write(servoAng1);
    
    int servoAng2=(attention / 10)*20;
    if(servoAng2>180){servoAng2=180;}    
    myservo2.write(servoAng2);
    
    int servoAng3=(attention / 10)*20;
    if(servoAng3>180){servoAng3=180;}    
    myservo3.write(servoAng3);
    
    int servoAng4=(attention / 10)*20;
    if(servoAng4>180){servoAng4=180;}    
    myservo4.write(servoAng4);
    
    int servoAng5=(attention / 10)*20;
    if(servoAng5>180){servoAng5=180;}    
    myservo5.write(servoAng5);
    
    Serial.print(servoAng1);
    Serial.print(servoAng2);
    Serial.print(servoAng3);
    Serial.print(servoAng4);
    Serial.print(servoAng5);
    Serial.print("");

   
                
        }
#endif        
        bigPacket = false;        
      }
      else {
        // Checksum Error
      }  // end if else for checksum
    } // end if read 0xAA byte
  } // end if read 0xAA byte
}



