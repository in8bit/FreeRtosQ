// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
// Include queue support
#include <queue.h>
#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>

//physical mac address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 

//Temperature Sensor Globals
#define Dht11 DHT11
int temperaturePin = 6;
DHT HT(temperaturePin, Dht11);

// Define a struct that hold the sensor values
struct dhtVal 
{
 float humidity;
 float tempC;
 float tempF;
};

char server[] = "192.168.3.25";
IPAddress ip(192,168,3,125); 
IPAddress myDns(192, 168, 3, 1);
EthernetClient client; 

// delay values
int perHalfSecondDelay = 500;
int perSecond = 1000;

//Distance Sensor Globals
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

//Sensor Function Headers
void getTempSensorReading(void *pvParameters);
void getDistanceSensorReading(void *pvParameters);
void writeDataToDB(void *pvParameters);

// Task handles
TaskHandle_t taskTemperatureSensorReading;
TaskHandle_t taskDistanceSensorReading;
TaskHandle_t taskWriteDataToDB;

// dataQ
QueueHandle_t dataQ;

void setup() 
{

  Serial.begin(9600); 
  
  // set up DHT11 (humidity and temperature)
  HT.begin();
  delay(perHalfSecondDelay);
  
  //ethernet connection
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
  Ethernet.begin(mac, ip);
  }else {
    Serial.print(" DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  //A second to initialize the shield:
  delay(perSecond);
  
  // create Structure Queue
  dataQ = xQueueCreate(5, sizeof(struct dhtVal));// 5 items of size== dhtVal 
  
  if (dataQ != NULL) { //implies that the queue is created.
    
    //Create task that consumes the queue if it was created.
    xTaskCreate(writeDataToDB, // Task function
                "WriteDataToDB", // A name just for humans
                1000,  // This stack size can be checked & adjusted by reading the Stack Highwater
                NULL, 
                1, // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
                &taskWriteDataToDB);

  //Temperature sensor reading
  xTaskCreate(getTempSensorReading, // Task function
              "TempSensorReading", // Task name
              500, // Stack size 
              NULL, 
              3, // Priority
              &taskTemperatureSensorReading); // Task handler 

//  //Distance sensor reading
//  xTaskCreate(getDistanceSensorReading, // Task function
//              "DistanceSensorReading", // Task name
//              500, // Stack size 
//              NULL, 
//              1, // Priority
//              &taskDistanceSensorReading); // Task handler
    
  }

  vTaskStartScheduler();
 
}

void loop() 
{
  
}

// Temperature value and adc number writing
void getTempSensorReading(void *pvParameters)
{
  (void) pvParameters;
  struct dhtVal previousDhtVal; // to store previous values
  while (1)
  {
   
    struct dhtVal currentDhtVal;  // define a structure of type dhtVal 
    currentDhtVal.humidity = HT.readHumidity();
    currentDhtVal.tempC = HT.readTemperature();
    currentDhtVal.tempF = HT.readTemperature(true);
    
    if(currentDhtVal.humidity != previousDhtVal.humidity || 
                  currentDhtVal.tempC != previousDhtVal.tempC || 
                  currentDhtVal.tempF != previousDhtVal.tempF )
    {
    
      xQueueSend(dataQ, &currentDhtVal, portMAX_DELAY); //write data  to queue

    }

    previousDhtVal.humidity = previousDhtVal.tempF;
    previousDhtVal.tempC = currentDhtVal.tempC;
    previousDhtVal.tempF =currentDhtVal.tempF;
    
    taskYIELD(); //terminate the task and inform schulder about it
  }
}

//// distance sensor
//void getDistanceSensorReading(void *pvParameters)
//{
//  (void) pvParameters;
//  
//  while (1)
//  {   
//    struct dhtVal currentDhtVal;  // define a structure of type dhtVal 
//     // localhost/sensors/distance.php?distance=200
//    currentDhtVal.humidity = 10.00;
//    currentDhtVal.tempC = 10.00;
//    currentDhtVal.tempF =10.00;
//    
//    xQueueSend(dataQ, &currentDhtVal, portMAX_DELAY); //write data  to queue
//     Serial.println("...distance val..."); 
//     taskYIELD(); //terminate the task and inform schulder about it
//  }
//}


void writeDataToDB(void * pvParameters)
{
  (void) pvParameters;
  struct dhtVal previousDhtVal; // to store previous values
  while (1) 
  {
    struct dhtVal currentDhtVal; // structure to hold receiv data
    
    // Read structure elements from queue and check if data received successfully 
    if (xQueueReceive(dataQ, &currentDhtVal, portMAX_DELAY) == pdPASS)
    {
      //connection check
      if (client.connect(server, 80)) {
        
        Serial.print ("connected to ");
        Serial.print (server);
        Serial.println ();
        
        if(currentDhtVal.humidity != previousDhtVal.humidity || 
                  currentDhtVal.tempC != previousDhtVal.tempC || 
                  currentDhtVal.tempF != previousDhtVal.tempF )
    {
    
        Serial.print("Humidity: ");
        Serial.print(currentDhtVal.humidity);
        Serial.print (" Temperature in Celsius: ");
        Serial.print (currentDhtVal.tempC);
        Serial.print (" Temperature in Farenheit: ");
        Serial.print (currentDhtVal.tempF);
        Serial.println ();
      // xQueueReset( dataQ );

    // localhost/sensors/dht11.php?temp_c=20&temp_f=66&humidity=33
     
      
//    client.print("GET /sensors/dht11.php?temp_c=");     
//    client.print(currentDhtVal.tempC);
//    client.print("&temp_f=");
//    client.print(currentDhtVal.tempF);
//    client.print("&humidity=");
//    client.print(currentDhtVal.humidity);
//    client.print(" ");      //SPACE BEFORE HTTP/1.1
//    client.print("HTTP/1.1");
//    client.println();
//    client.println("Host: 192.168.3.25");
//    client.println("Connection: close");
//    client.println();

    }
    previousDhtVal.humidity = previousDhtVal.tempF;
    previousDhtVal.tempC = currentDhtVal.tempC;
    previousDhtVal.tempF =currentDhtVal.tempF;
   
      }else {
         Serial.println ("connection failure");
      }
    }else{
        Serial.println ("queue recieve failure");
    }
    // vTaskDelay(1000);
     taskYIELD(); // terminate the task and inform schulder about it
    }
  }
