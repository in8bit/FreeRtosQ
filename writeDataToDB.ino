
// localhost/sensors/dht11.php?temp_c=20&temp_f=66&humidity=33
// localhost/sensors/distance.php?distance_cm=10&distance_inch=2.3&distance_mm=0
void writeDataToDB(void * pvParameters)
{
  (void) pvParameters;

   TickType_t xLastWakeTime;
   const TickType_t xFrequency = 3000; // perMinuteDelay; // perSecond; 
   xLastWakeTime = xTaskGetTickCount();
   
  struct queueValue
  {
    char structName;
    float a;
    float b;
    float c;
  };

  while (1)
  {
    struct dhtVal currentDhtVal; // structure to hold receiv data
    struct distVal distanceVal;
    struct queueValue currentQueueVal;
    BaseType_t queueRecieveStatus;
    
   // if (xSemaphoreTake(xMutex, 0) == pdTRUE) {
       queueRecieveStatus = xQueueReceive(dataQ, &currentQueueVal, ( TickType_t ) 1000 ); // portMAX_DELAY // ( TickType_t ) 10000 );
     // xSemaphoreGive(xMutex);
   // }else{
    //  Serial.println("DB task not able to read the queue, mutext not available.");
   // }
      // Read structure elements from queue and check if data received successfully 
      if(queueRecieveStatus == pdPASS)
      {
        //connection check
        if (client.connect(server, 80)) {
          if (currentQueueVal.structName == 't') {
            Serial.println ("Temperature");
            Serial.print(" a: ");
            Serial.print(currentQueueVal.a);
            Serial.print(" b: ");
            Serial.print(currentQueueVal.b);
            Serial.print(" c: ");
            Serial.print(currentQueueVal.c);
            Serial.println ();
           
            if (isnan(currentQueueVal.b)|| isnan(currentQueueVal.c) || isnan(currentQueueVal.a)){
              currentQueueVal.a = 0.0;
              currentQueueVal.b = 0.0;
              currentQueueVal.c = 0.0;
            }
              client.print("GET /sensors/dht11.php?temp_c=");
              client.print(currentQueueVal.c);
              client.print("&temp_f=");
              client.print(currentQueueVal.b);
              client.print("&humidity=");
              client.print(currentQueueVal.a);
              client.print(" ");      //SPACE BEFORE HTTP/1.1
              client.print("HTTP/1.1");
              client.println();
              client.println("Host: 192.168.3.25");
              client.println("Connection: close");
              client.println();
              
          } else if (currentQueueVal.structName == 'd') {
            Serial.println ("Distance");
            Serial.print(" a: ");
            Serial.print(currentQueueVal.a);
            Serial.print(" b: ");
            Serial.print(currentQueueVal.b);
            Serial.print(" c: ");
            Serial.print(currentQueueVal.c);
            Serial.println ();
            
            if (isnan(currentQueueVal.b)|| isnan(currentQueueVal.c) || isnan(currentQueueVal.a)){
             currentQueueVal.a = 0.0;
             currentQueueVal.b = 0.0;
             currentQueueVal.c = 0.0;
            }
              client.print("GET /sensors/distance.php?distance_cm=");
              client.print(currentQueueVal.a);
              client.print("&distance_inch=");
              client.print(currentQueueVal.b);
              client.print("&distance_mm=");
              client.print(currentQueueVal.c);
              client.print(" ");      //SPACE BEFORE HTTP/1.1
              client.print("HTTP/1.1");
              client.println();
              client.println("Host: 192.168.3.25");
              client.println("Connection: close");
              client.println();
          }
//          }else if (currentQueueVal.structName == 's') {
//            Serial.println ("Sound");
//            Serial.print(" a: ");
//            Serial.print(currentQueueVal.a);
//            Serial.print(" b: ");
//            Serial.print(currentQueueVal.b);
//
//            Serial.println ();
//          }
          
        } else {
          Serial.println ("connection failure");
        }
      } else {
        Serial.println ("queue recieve failure");
      }
     // vTaskDelayUntil(&xLastWakeTime, xFrequency);
    // vTaskDelay(3000);
     taskYIELD(); // terminate the task and inform schulder about it
  }
}
