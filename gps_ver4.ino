#include <SoftwareSerial.h>

static const int RXPin = 6, TXPin = 7;
static const uint32_t GPSBaud = 9600;
int byteGPS=-1;

char linea[300] = "";
char comandoGPR[7] = "$GNGGA";
char HhMmSs[11]="";
char lat[10]="";
char uLat='0';
char Lon[12]="";
char uLon='0';
int cont=0;
int bien=0;
int conta=0;
int indices[13]; 
int k=0;
int check=1;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);


char FakeGPS[100] = "$GNGGA,033652.000,2500.7940,N,12132.3935,E,1,06,12.1,-7.6,M,0.0,M,,*69\r\n";

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  for (int i=0;i<300;i++){       // Initialize a buffer for received data
    linea[i]=' ';
  }   
}

void loop(){
  //byteGPS=ss.read();         // Read a byte of the serial port
  //ss.println(  byteGPS);
  for(int l=0;l<100;l++){
    byteGPS=FakeGPS[l];
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


for (int i=0;i<5;i++){
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
 
 }
 } 
 
 k=0; 
 }
 
 }
 Serial.println( HhMmSs);
 Serial.println( lat);
 Serial.println( uLat);
 Serial.println( Lon);
 Serial.println( uLon);
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
 
 



