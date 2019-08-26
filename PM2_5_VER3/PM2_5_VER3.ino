#include <SoftwareSerial.h>
#include <Arduino.h>
int check=0;
int flag=0;
//char g[10]="";
char G[10]="";
String G_last="";
float time=0;
float time_interval=0;
float time_last=0;
char g_zero[10]={48,48,48,48,48,48,48,48,48,48};
SoftwareSerial ss(10, 11); // RX, TX
void retrievepm25(){
  //char g[10]="";
  int count = 0;
  //String g="";
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
  while (ss.available()) { 
    c = ss.read();
    if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
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
      //Serial.print("temp");
      //Serial.println(temperature);
    }
    else if(count == 27)
    {          
      humandity = (256*high + c)/10;
      
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
  while(ss.available()) 
  ss.flush();
  if(temperature!=0){
  
  strcpy(G,g);
  time_last=millis();
  }else{
   
    strcat(G,g_zero);
}
  
  }
void setup() {
  Serial.begin(9600);
  ss.begin(9600);
}

void loop() {
  retrievepm25();
  //time=millis();
  time_interval=millis()-time_last;
  if(time_interval<2000){
  Serial.print("S");
  Serial.print("S");  
  Serial.print(G);
  //Serial.println(time_last);
  memset(G, '\0', sizeof(G));}else{
  Serial.print("S");
  Serial.print("S");  
  strcat(G,g_zero);
  Serial.print(G); 
  }
  //Serial.print("G3:");
  //Serial.println(G);
  //}else{
  //Serial.println("wrong");
  //}
  
  //Serial.print(pmat10_result);
  //Serial.print(pmat25_result);
  //Serial.print(pmat100_result);
  //Serial.print(temp_result);
  //Serial.println(hum_result);


  delay(1000);
  //display.clearDisplay();


}


