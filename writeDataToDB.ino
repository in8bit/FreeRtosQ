
// localhost/sensors/dht11.php?temp_c=20&temp_f=66&humidity=33
void writeDataToDB(void * pvParameters)
{
  (void) pvParameters;

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
       queueRecieveStatus = xQueueReceive(dataQ, &currentQueueVal, portMAX_DELAY); // ( TickType_t ) 10000 );
     // xSemaphoreGive(xMutex);
   // }else{
    //  Serial.println("DB task not able to read the queue, mutext not available.");
  //  }
      // Read structure elements from queue and check if data received successfully
      // if (xQueueReceive(dataQ, &currentQueueVal, portMAX_DELAY) == pdPASS)
     
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
            Serial.print(currentQueueVal.b);
            Serial.println ();
          } else if (currentQueueVal.structName == 'd') {
            Serial.println ("Distance");
            Serial.print(" a: ");
            Serial.print(currentQueueVal.a);
            Serial.print(" b: ");
            Serial.print(currentQueueVal.b);
            Serial.print(" c: ");
            Serial.print(currentQueueVal.b);
            Serial.println ();
          }

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
        } else {
          Serial.println ("connection failure");
        }
      } else {
        Serial.println ("queue recieve failure");
      }
    
    taskYIELD(); // terminate the task and inform schulder about it
  }
}
