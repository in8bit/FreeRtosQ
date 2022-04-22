
// Temperature Reading
void getTempSensorReading(void *pvParameters)
{
  (void) pvParameters;
  
  TickType_t xLastWakeTime;
  const TickType_t xFrequency = perSecond; // perMinuteDelay;
  xLastWakeTime = xTaskGetTickCount();
  
  while (1)
  {
   
    struct dhtVal currentDhtVal;  // define a structure of type dhtVal 
    currentDhtVal.humidity = HT.readHumidity();
    currentDhtVal.tempC = HT.readTemperature();
    currentDhtVal.tempF = HT.readTemperature(true);

    // write data to queue if the previous readings are different from the current readings.
    if(currentDhtVal.humidity != previousDhtVal.humidity || 
                  currentDhtVal.tempC != previousDhtVal.tempC || 
                  currentDhtVal.tempF != previousDhtVal.tempF )
    {
       if(xSemaphoreTake(xMutex, 0) == pdTRUE){
         xQueueSend(dataQ, &currentDhtVal, portMAX_DELAY); 
         xSemaphoreGive(xMutex);

       previousDhtVal.humidity = currentDhtVal.humidity;
       previousDhtVal.tempC = currentDhtVal.tempC;
       previousDhtVal.tempF = currentDhtVal.tempF;
      }else{
          Serial.println("Temperature Sensor not able to update queue, mutext not available.");
        }
      
    }
    vTaskDelayUntil(&xLastWakeTime, xFrequency);
    // vTaskDelay(perMinuteDelay);
   // taskYIELD(); //terminate the task and inform schulder about it
  }
}
