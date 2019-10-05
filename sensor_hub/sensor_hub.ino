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
SoftwareSerial ss_gps(6, 7);
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
void gps(){
 int byteGPS=-1;
char sate[3]="";
char linea[300] = "";
char comandoGPR[7] = "$GNGGA";
char HhMmSs[11]="";
char lat[10]="";
char uLat='0';
char Lon[11]="";
char uLon='0';
int cont=0;
int bien=0;
int conta=0;
int indices[13]; 
int k=0;
int check=1;
for (int i=0;i<300;i++){       // Initialize a buffer for received data
    linea[i]=' ';
  }   
//char FakeGPS[100] = "$GNGGA,033652.000,2500.7940,N,12132.3935,W,1,05,12.1,-7.6,M,0.0,M,,*69\r\n";
 
 for(int l=0;l<100;l++){
    byteGPS=ss_gps.read();
    //Serial.println(byteGPS);
    if (byteGPS == -1) {           // See if the port is empty yet
      delay(100); 
    } 
    if(byteGPS>=24){
 //Serial.println("ok1");
      // note: there is a potential buffer overflow here!
      linea[conta]=byteGPS;        // If there is serial port data, it is put in the buffer
      conta++;

    }                      
    if(byteGPS==13){            // If the received byte is = to 13, end of transmission
      // note: the actual end of transmission is <CR><LF> (i.e. 0x13 0x10) 
      //Serial.println("ok");
      cont=0;
      bien=0;
    }
  }

  // The following for loop starts at 1, because this code is clowny and the first byte is the <LF> (0x10) from the previous transmission.
  for (int i=1;i<=6;i++){     // Verifies if the received command starts with $GPR
    if (linea[i-1]==comandoGPR[i-1]){
      bien++;

      //Serial.println(linea[i-1]);
      //Serial.println(bien++);
    }
  }

  if(bien==6){   
    //Serial.println(bien);
    // If yes, continue and process the data
    for (int i=0;i<100;i++){
      if (linea[i]==','){    // check for the position of the  "," separator
        // note: again, there is a potential buffer overflow here!
        indices[cont]=i;
        //Serial.println( indices[cont]);
        cont++;
      }
      if (linea[i]=='*'){    // ... and the "*"
        indices[12]=i;
        cont++;
      }
    }


for (int i=0;i<7;i++){
 for (int j=indices[i];j<(indices[i+1]-1);j++){
   
 switch(i){
 case 0 :
 HhMmSs[k]=linea[j+1];
 k++;  
 break;
 case 1 :
 lat[k]=linea[j+1];
 k++;   
 break;
 case 2 :
 uLat=linea[j+1];
 k++;  
 break;
 case 3 :
 Lon[k]=linea[j+1];
 
 k++;    
 break;
 case 4:
 uLon=linea[j+1];
 break;
 case 6:
 sate[k]=linea[j+1];
 k++;    
 break;
 }
 } 
 
 k=0; 
 }
 
 }
 int i;
sscanf(sate, "%d", &i);
 if(i>=4){
Serial.println("i:");
Serial.println(i);
Serial.println("S");
Serial.println("G");
Serial.println(HhMmSs);
Serial.println(lat);
Serial.println(uLat);
Serial.println(Lon);
Serial.println(uLon);
Serial.println("E");

}else{ 
  Serial.println("i:");
Serial.println(i);
 Serial.println("S");
 Serial.println("G");  
 Serial.println("0000000000");
 Serial.println("000000000");
 Serial.println("0");
 Serial.println("0000000000");
 Serial.println("0");
 Serial.println("E");
 }
 conta=0;                    // Reset the buffer
 for (int i=0;i<100;i++){    //  
 linea[i]=' ';         
 } 
 memset(HhMmSs, '\0', sizeof(HhMmSs));       
 memset(lat, '\0', sizeof(lat));       
 //memset(uLat, '\0', sizeof(uLat));
 memset(Lon, '\0', sizeof(Lon));       
 //memset(uLon, '\0', sizeof(uLon));       
 
  

}





void setup() {
  Serial.begin(9600);
  ss_pm.begin(9600);
  ss_co2.begin(9600);
  ss_gps.begin(9600);
  
}

void loop() {
  ss_gps.listen();
  delay(50);
  gps();
  ss_gps.flush();
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
  ss_pm.flush();
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
    Serial.println("E");
    time_last=millis();
  }


  delay(10);
  ss_co2.flush();
  



}


