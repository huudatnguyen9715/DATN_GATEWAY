#include <Ethernet.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <SPI.h>
#include <LoRa.h>

// Led
#define LED1      2    // GPIO2, GREEN
#define LED2      15    // GPIO15, BLUE

// Button
#define BTN       39    // SENSOR_VN / GPIO39 / Input only

// Speaker
#define SPK       32    // GPIO32

// USB RESET
#define CP2012_RST    33

// SHT1x
#define SHT_SCK       23
#define SHT_DATA      22

// SPI Lora
#define LORA_RST      13
#define LORA_SS       14
#define LORA_MOSI     27
#define LORA_MISO     26
#define LORA_SCK      25
#define LORA_DIO012   12


int freq = 2400;
int channel = 0;
int resolution = 8;
int number = 0 ; 
String stringLoRa ;
char LoRaData[10] ;

int id_tam = 1 ;
int reload = 0 ;

char Temperature[10];
char Humidity_air[10];

char Humidity_soil[10];

char Lux[10];

char DO[10];

char id[10];
float temperature;
float humidity_air;
float humidity_soil;
float lux;

int tamp;

char device_data[300] ;
char ctrl_device[10];
char ctrl_device_temp[10];

unsigned int  id_temp[10];
String data;
String tstr;
String c ;
String PH_temp ;
String DO_temp ;

const char* ssid     = "BeHome NHT";

const char* password = "0968377014behome";

const char* host = "datnhuudatnguyen.000webhostapp.com";
// char* host = "192.168.1.9";
// char* host = "192.168.1.12";

const int httpPort = 80;
WiFiClient client;
//WiFiServer server(80);
void setup() {
    Serial.begin(115200);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(BTN, INPUT);
    
    pinMode(16,OUTPUT);
    pinMode(17,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(18,OUTPUT);


 //   Serial.print("Connecting to ");
 //   Serial.println(ssid);


    WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


   Serial.println("");
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());
   Serial.print("Netmask: ");
   Serial.println(WiFi.subnetMask());
   Serial.print("Gateway: ");
   Serial.println(WiFi.gatewayIP());

    
   Serial.printf("Raw Lora Receiver!\n");
    
    // test Leds, flash leds in 10s
    for(uint8_t i=0; i<10; i++) {
      if(i%2) {
        digitalWrite(LED1, !digitalRead(LED1));
      } else {
        digitalWrite(LED2, !digitalRead(LED2));
      }
      delay(300);
    }

    // test SPK
    ledcWriteTone(channel, 2400);
    delay(1000);
    ledcWrite(channel, 0);

    // setting SPI
    SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_SS);
    if (!LoRa.begin(434E6)) {
      Serial.println("Starting LoRa failed!");
      
      Serial.println("Failed");
      
      while (1);
    }

      Serial.println("Passed");
      
    Serial.println("LoRa Begin");
    LoRa.setSpreadingFactor(8);
    LoRa.setSignalBandwidth(20.8E3);
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(10);
    LoRa.enableCrc();

    Serial.println("Lora Ready");


}

void loop() {
  //  Transmit Lora_packet
  String ctrl = (String)ctrl_device[id_tam-1];
  String id_node = (String)id_tam;
  String outgoing =id_node + ctrl ;

   for(reload=0;reload<2;reload++)
   {
        sendLoRaMessage(outgoing);
   }
delay(100);
   reload=0;
   int i=0;
 while( i<32000)
 {
  if(LoRa.parsePacket())
    break;
    i++;
  }


//Serial.println(i);
   
     // received a packet   
//      int packetSize = LoRa.parsePacket();
//      Serial.print("PacketSize: ");
//      Serial.println(packetSize);

     int id_node_=id_tam;
    id_tam += 1 ;

    // read packet
    if (LoRa.available()) 
    {
      Serial.print("Received packet: ");
      stringLoRa = LoRa.readString();  
      Serial.println(stringLoRa); 

    digitalWrite(LED1, HIGH);   // OFF
    ledcWrite(channel, 127);
    delay(80);
    digitalWrite(LED1, LOW);    // ON
    ledcWrite(channel, 0);
    

     // Read data 
     
   stringLoRa.toCharArray(LoRaData, 100)  ;
   
        Serial.print("LoRaData: "); 
        Serial.println(LoRaData); 

  strncpy(id, LoRaData + 0, 1) ;
      id[1] = '\0';
      
     Serial.print("Node address : ");  
     Serial.println(id); 

    id_temp[0] = id[0]-48;



       
///------------------------------------------------------------------------------
switch (id_temp[0]) 
{
    case 1:                                 // DS18B20
    strncpy(Temperature, LoRaData + 1, 10) ;
//    sscanf(Temperature,"%d",&tamp);
//    temperature=((float)tamp)/16;
//    sprintf(Temperature,"%0.2f",temperature);



    Serial.print("nhiet do khong khi: ");
    Serial.println(Temperature);
//    Serial.println(temperature);


    sendTemperature(Temperature);
    number = number + 1 ;
    Serial.print("...index:");
    Serial.println(number);
      break; 
    
    case 2:                                //sensor soil
    strncpy(Humidity_soil, LoRaData + 1, 2) ; 
//    sscanf(Humidity_soil,"%d",&tamp);
//    if(tamp <=1600){
//      tamp=1600;
//    }
//    if(tamp >=3590){
//      tamp=3590;
//    }
//    humidity_soil=((float)tamp-3590.0)/(1600.0-3590.0)*100.0;
//        if(humidity_soil <= 0)
//        {
//          humidity_soil=0;
//          }
//        if(humidity_soil >= 100){
//          humidity_soil=100;
//      }
//    sprintf(Humidity_soil,"%0.2f",humidity_soil);
    Serial.print("do am dat: ");
    Serial.println(Humidity_soil);
//    Serial.println(humidity_soil);

    sendHumidity_soil(Humidity_soil);
    number = number + 1 ;  
    Serial.print("...index:");
    Serial.println(number); 
    break;
    
    case 3:                                  //BH1750
    strncpy(Lux, LoRaData + 1, 10) ;
//    sscanf(Lux,"%d",&tamp);
//    lux=((float)tamp)/1.2;
//    sprintf(Lux,"%0.2f",lux);
    
    Serial.print("do sang: ");
    Serial.println(Lux);
//    Serial.println(lux);

    sendLux(Lux); 
    number = number + 1 ; 
    Serial.print("...index:");
    Serial.println(number);
    break;
}

    }
    
        else  {
         switch(id_node_)
         {
          case 1:
           Serial.println("node_1_error");
          break; 
          case 2:
           Serial.println("node_2_error");
          break; 
          case 3:
           Serial.println("node_3_error");
          break; 
         }
   
    }


     pinStatus();
     delay(100);
      while (client.available()) 
      {
//          Serial.println("read request");
        String line = client.readStringUntil('\n');
         tstr+=line ;
        }      
   
       parseGet(tstr,ctrl_device); 
       tstr = "";
       stringLoRa ="";

       if(id_tam==4){
        id_tam = 1 ;}
        

  if(client.connected()) 
  {     
  client.stop();  // DISCONNECT FROM THE SERVER
  Serial.println("closing connection");
  } 

   // ngoặc mới của "if (packetSize) {"
  }


// CT CON////////////////////////////////////////////////////////////////////////////////


//--------------------------------------------------------------------------

void sendTemperature(String temp_x) {
if (!client.connect(host, httpPort)) {
    
    Serial.println("connection failed");
    return;
  }

 // String url1 = "/DATN_SERVER/temp/includes/write_temp.php?temp=" + String(temp) ; // localhost
//  String url1 = "/DATN_SERVER/hum/includes/write_hum.php?humidity=" + String(temp_x) ; // localhost
  String url1 = "/hum/includes/write_hum.php?humidity=" + String(temp_x) ; // global host 


//  Serial.print(temp_x);
// 
//  Serial.print("1");
  
  client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

//  temp = temp + 1 ;
if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
    
  }
}

//...............................

void sendHumidity_air(String humidity_air_x) {
if (!client.connect(host, httpPort)) {
    
    Serial.println("connection failed");
    return;
  }

 // String url1 = "/DATN_SERVER/temp/includes/write_temp.php?temp=" + String(temp) ; // localhost
  String url2 = "/hum/includes/write_hum.php?humidity=" + String(humidity_air_x) ; // global host 

//  Serial.print(humidity_air_x);
// 
//  Serial.print("1");
  
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

//  temp = temp + 1 ;
if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}

//................................................

void sendHumidity_soil(String humidity_soil_x) {
if (!client.connect(host, httpPort)) {
    
    Serial.println("connection failed");
    return;
  }

 // String url1 = "/DATN_SERVER/temp/includes/write_temp.php?temp=" + String(temp) ; // localhost
  String url3 = "/hum4/includes/write_hum.php?humidity=" + String(humidity_soil_x) ; // global host 
    
//  Serial.print(humidity_soil_x);
// 
//  Serial.print("1");
  
  client.print(String("GET ") + url3 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

//  temp = temp + 1 ;
if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}

//...........................................................

void sendLux(String lux_x) {
if (!client.connect(host, httpPort)) {
    
    Serial.println("connection failed");
    return;
  }

 // String url1 = "/DATN_SERVER/temp/includes/write_temp.php?temp=" + String(temp) ; // localhost
  String url4 = "/hum3/includes/write_hum.php?humidity=" + String(lux_x) ; // global host 

//  Serial.print(lux_x);
// 
//  Serial.print("1");
  
  client.print(String("GET ") + url4 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

//  temp = temp + 1 ;
if (client.connected()) {
    client.stop();  // DISCONNECT FROM THE SERVER
  }
}



//-------------------------------------------------------------------------chương trình cũ------------------------------------------



//-------------------------------------------------------------------------------------------------------------------------------------------

//
//void pinStatus() {
//  if (!client.connect(host, httpPort)) {
//    
//    Serial.println("connection failed");
//    return;
//  }          
//   
//  // String request = "/DATN_SERVER/pc/ajax/pin_status_device.php?" ; //+ String(pin_27);
//    String request = "/DATN_SERVER/pc/ajax/pin_status_device.php?" ;  
//    
//      client.print(String("GET ") + request + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" + 
//               "Connection: close\r\n\r\n");      
//     delay(500) ;
//}

void pinStatus() {
  if (!client.connect(host, httpPort)) {
    
    Serial.println("connection failed");
    return;
  }          
   
  // String request = "/DATN_SERVER/pc/ajax/pin_status_device.php?" ; localhost
    String request = "/pc/ajax/pin_status_device.php?" ;   // global host 
    
      client.print(String("GET ") + request + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");      
     delay(100);
}

// Control Device 
void parseGet(String str,char ctrl_device[10]) {
char device_data[300] ;
str.toCharArray(device_data, 300);
Serial.print("String_temp:");
Serial.println(device_data);
strncpy(ctrl_device, device_data +209 , 4) ;
      ctrl_device[3] = '\0'; 
   Serial.print("[Device]=> ");
   Serial.println(ctrl_device);
//     Serial.println(ctrl_device[0]);  
//     Serial.println(ctrl_device[1]);  
//     Serial.println(ctrl_device[2]); 

  if(ctrl_device[0]!= ctrl_device_temp[0])
  {
    id_tam = 1 ;
  } else if (ctrl_device[1]!= ctrl_device_temp[1])
  {
    id_tam = 2 ;
  }else if (ctrl_device[2]!= ctrl_device_temp[2])
  {
    id_tam = 3 ;
  }
  ctrl_device_temp[0] = ctrl_device[0] ;
  ctrl_device_temp[1] = ctrl_device[1] ;
  ctrl_device_temp[2] = ctrl_device[2] ;

 

}

// Send Lora message 
void sendLoRaMessage(String outgoing) {
  
  LoRa.beginPacket();                   // start packet
  /*
  msgCount++;                           // increment message ID
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  */
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it 
  Serial.println("[Transmit]=> Sending packet: " + outgoing);
//    digitalWrite(LED1, HIGH);   // OFF
//    ledcWrite(channel, 127);
//    delay(200);
//    digitalWrite(LED1, LOW);    // ON
//    ledcWrite(channel, 0);
//    delay(2000);   
}
