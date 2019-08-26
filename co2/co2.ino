/*
DS-CO2-20二氧化碳溫感測器
 pin1 to arduino 5V
 pin2 to GND
 pin3 to arduino pin10
 pin4 to arduino pin11
 剩下的pin5~pin7不接
 */

#include <SoftwareSerial.h>
SoftwareSerial ss(10, 11); // RX, TX
float time=0;
float time_interval=0;
float time_last=0;
static unsigned int co2 = 0;
static unsigned int ucRxBuffer[10];
char g_zero[4]={
  48,48,48,48};
char G[4]="";
char *co2_send = (char*)malloc(2);
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}

void loop(){
  //ss.listen();
  byte request[] = {
    0x42, 0x4d, 0xe3, 0x00, 0x00, 0x01, 0x72  };
  ss.write(request, 7);
  //Serial.println("send");
  delay(50);
  while (ss.available()>0){
    //Serial.println("recieved");
    for(int i=0; i<12; i=i+1){
      ucRxBuffer[i]=ss.read();
    }
    if((ucRxBuffer[0]!=66) || (ucRxBuffer[1]!=77)){
      strcat(G,g_zero);
      //Serial.print("fail1");
      Serial.print(g_zero); 
      time_last=millis();
    }
    else{
      co2 = ucRxBuffer[4]*256+ucRxBuffer[5];
      sprintf(co2_send, "%04d",co2);

      if((millis()-time_last)>3000){
        strcat(G,g_zero);
        //Serial.print("fail2");
        
        Serial.print(g_zero);
        time_last=millis();
        ss.flush();
      }
      else{
        Serial.print("S");
        Serial.print("S");
        Serial.print(co2_send); 
         //strcat(G,g_zero);
        //Serial.print("fail2");
       
        time_last=millis();
      }

    }
  }
delay(450);  
}


