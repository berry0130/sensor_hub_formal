#include <SoftwareSerial.h>
#include <Arduino.h>

char G_co2[4]="";
char G_pm[10]="";
String G_last="";
float time=0;
float time_interval=0;
float time_last=0;
float time_last_pm=0;
char g_zero[10]={
  48,48,48,48,48,48,48,48,48,48};
char g_zero_co2[4]={
  48,48,48,48};
SoftwareSerial ss_pm(10, 11); // RX, TX
SoftwareSerial ss_co2(8, 9); // RX, TX

void retrievepm25(){

  int count = 0;

  char g[10]="";

  long pmat10 = 0;
  long pmat25 = 0;
  long pmat100 = 0;
  unsigned int temperature = 0;
  unsigned int humandity = 0;
  char *pmat10_result = (char*)malloc(2);
  char *pmat25_result = (char*)malloc(2);
  char *pmat100_result = (char*)malloc(2);
  char *temp_result = (char*)malloc(2);
  char *hum_result = (char*)malloc(2);
  unsigned char c;
  unsigned char high;


  while (ss_pm.available()>0) { 
    c = ss_pm.read();
    //Serial.println("got data"); 
    if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
      //Serial.println("ok1");
      break;
    }
    if(count > 27){ 
      //Serial.println("complete");
      break;
    }
    else if(count == 10 || count == 12 || count == 14 || count == 24 || count == 26) {
      high = c;
    }
    else if(count == 11){
      pmat10 = 256*high + c;
    }
    else if(count == 13){
      pmat25 = 256*high + c;
    }
    else if(count == 15){
      pmat100 = 256*high + c;
    }
    else if(count == 25){        
      temperature = (256*high + c)/10;

    }
    else if(count == 27)
    {          
      humandity = (256*high + c)/10;
      //Serial.println("ok1");
      sprintf(temp_result, "%02X",temperature);
      sprintf(pmat25_result, "%02X", pmat25);
      sprintf(pmat10_result, "%02X", pmat10);
      sprintf(hum_result, "%02X",humandity);
      sprintf(pmat100_result, "%02X", pmat100);
      strcat(g,pmat10_result);
      strcat(g,pmat25_result);
      strcat(g,pmat100_result);
      strcat(g,temp_result);
      strcat(g,hum_result);
      //Serial.println(g);
      //strcpy(G,g);
    }

    count++;
  }
  //  while(ss_pm.available()>0) 
  //    ss_pm.flush();

  if(temperature!=0){
    strcpy(G_pm,g);
    //Serial.print("pm:");
    //Serial.println(G_pm);
    time_last_pm=millis();
  }
  else{
    strcat(G_pm,g_zero);
    //Serial.println(G_pm);
    time_last_pm=millis();
  }

}

void co2sensor(){
  char *co2_send = (char*)malloc(2);
  unsigned int co2 = 0;
  unsigned int ucRxBuffer[10];
  //static unsigned int co2 = 0;
  byte request[] = {
    0x42, 0x4d, 0xe3, 0x00, 0x00, 0x01, 0x72          };
  ss_co2.write(request, 7);
  delay(50);
  memset(G_co2, '\0', sizeof(G_co2));
  while (ss_co2.available()>0){

    for(int i=0; i<12; i=i+1){
      ucRxBuffer[i]=ss_co2.read();
    }}
    if((ucRxBuffer[0]!=66) || (ucRxBuffer[1]!=77)){
    strcat(G_co2,g_zero_co2);
    }
    else{
      co2 = ucRxBuffer[4]*256+ucRxBuffer[5];
      sprintf(co2_send, "%04d",co2);

      strcat(G_co2,co2_send);
      time_last=millis();
 
    }

}
void setup() {
  Serial.begin(9600);
  ss_pm.begin(9600);
  ss_co2.begin(9600);
}

void loop() {
  ss_pm.listen();
  delay(250);
  retrievepm25();
  time_interval=millis()-time_last_pm;
  if(time_interval<2000){
    Serial.print("S");
    Serial.print("P");  
    Serial.print(G_pm);
     Serial.print("E");
    //Serial.println(time_last);
    memset(G_pm, '\0', sizeof(G_pm));
  }
  else{
    Serial.print("S");
    Serial.print("P");  
    strcat(G_pm,g_zero);
    Serial.print(G_pm); 
     Serial.print("E");
  }
  ss_co2.listen();
  co2sensor();
  if((millis()-time_last)>500){
     memset(G_co2, '\0', sizeof(G_pm));
    strcat(G_co2,g_zero_co2);
    Serial.print("S");
    Serial.print("C");
    Serial.print(G_co2);
     Serial.print("E");
    time_last=millis();
  }
  else{
    Serial.print("S");
    Serial.print("C");
    Serial.print (G_co2); 
    Serial.print("E");
    time_last=millis();
  }


  delay(10);



}






