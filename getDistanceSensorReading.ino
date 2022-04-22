
// distance sensor
void getDistanceSensorReading(void *pvParameters)
{
  (void) pvParameters;

   TickType_t xLastWakeTime;
   const TickType_t xFrequency = perSecond; // perMinuteDelay;
   xLastWakeTime = xTaskGetTickCount();
  
  while (1)
  {   
    struct distVal currentDistVal;  // define a structure of type dhtVal 
    long duration; // variable for the duration of sound wave travel
    float dist_cm; // variable for the distance measurement
    float dist_inch;
    float dist_mm;
      
      // Clear the trigPin condition
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      
      // Set the trigPin high/active for 10 microseconds
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      
      // Reads the echoPin, returns the sound wave travel time in microseconds
      duration = pulseIn(echoPin, HIGH);
     
      // Calculating the distance
      dist_cm = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back) & speed of sound is 343 meters per second
      dist_inch = dist_cm / 2.54;
      dist_mm = dist_cm*10;
      
      currentDistVal.inCM = dist_cm;
      currentDistVal.inInch = dist_inch;
      currentDistVal.inMM = dist_mm;

       if(currentDistVal.inCM != previousDistVal.inCM || 
                  currentDistVal.inInch != previousDistVal.inInch || 
                  currentDistVal.inMM != previousDistVal.inMM ){

        // mutex prevents anyother task to use our queue
        if(xSemaphoreTake(xMutex, 0) == pdTRUE){
           xQueueSend(dataQ, &currentDistVal, portMAX_DELAY); //write data  to queue
           xSemaphoreGive(xMutex);
         }else{
          Serial.println("Distance Sensor not able to update queue, mutext not available.");
          }
         previousDistVal.inCM = currentDistVal.inCM;
         previousDistVal.inInch = currentDistVal.inInch;
         previousDistVal.inMM = currentDistVal.inMM ; 
       }
       vTaskDelayUntil(&xLastWakeTime, xFrequency);
      }
      
     // taskYIELD(); //terminate the task and inform schulder about it
}
