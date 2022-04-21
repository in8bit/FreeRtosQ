// Include Arduino FreeRTOS library
#include <Arduino_FreeRTOS.h>
// Include queue support
#include <queue.h>
#include "semphr.h"
#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>

// Ethernet set up and physical mac address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
char server[] = "192.168.3.25";
IPAddress ip(192,168,3,125); 
IPAddress myDns(192, 168, 3, 1);
EthernetClient client;

// Temperature Sensor Globals
#define Dht11 DHT11
int temperaturePin = 6;
DHT HT(temperaturePin, Dht11);

// Define a struct that hold the sensor values
struct dhtVal 
{
 char  structName = 't';
 float humidity;
 float tempC;
 float tempF;
};

struct dhtVal previousDhtVal; // to store previous temperature values

//Distance Sensor Globals
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// Define a struct that hold the sensor values
struct distVal 
{
 char  structName = 'd';
 float inCM;
 float inInch;
 float inMM;
};

struct distVal previousDistVal; // to store previous distance values

// Sound Sensor Globals
int digitalSoundSensorPin = 8; //digital signal
int analogSoundSensorPin = A0; // analog signal
int digitalSoundSensorValue; //TODO : consider going local?
int analogSoundSensorValue; //TODO: consider going local? 

// Function Headers
void getTempSensorReading(void *pvParameters);
void getDistanceSensorReading(void *pvParameters);
void writeDataToDB(void *pvParameters);
void getAnalogSoundSensorReading(void *pvParameters);
void getDigitalSoundSensorReading(void *pvParameters);

// Task handles
TaskHandle_t taskTemperatureSensorReading;
TaskHandle_t taskDistanceSensorReading;
TaskHandle_t taskWriteDataToDB;
TaskHandle_t taskAnalogSoundSensorReading;
TaskHandle_t taskDigitalSoundSensorReading;

// dataQ
QueueHandle_t dataQ; // TODO: define Queue data structure -for commnality

//mutex handle
SemaphoreHandle_t  xMutex;

// delay values
int perMinuteDelay = 60000;
int perHalfSecondDelay = 500;
int perSecond = 1000;

void setup() 
{

  Serial.begin(9600); 
  
  //creating mutex and assigning to its handle
  xMutex = xSemaphoreCreateMutex();
  if(xMutex == NULL){
    Serial.print("Failed to create mutex");
  }
  
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
  dataQ = xQueueCreate(10, sizeof(struct dhtVal));// 10 items of size== dhtVal 
  
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
              500, 
              NULL, 
              3, // highest Priority
              &taskTemperatureSensorReading); 

  //Distance sensor reading
  xTaskCreate(getDistanceSensorReading, // Task function
              "DistanceSensorReading", // Task name
              500,  
              NULL, 
              3, // Priority
              &taskDistanceSensorReading); 
    
  }

  vTaskStartScheduler();
 
}

void loop() 
{
  // nothing goes here 
}
