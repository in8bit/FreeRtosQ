
// http://localhost/sensors/sound.php?digital=0&analog=665

// Analog
void getAnalogSoundSensorReading(void *pvParameters){
 (void) pvParameters;
 
 TickType_t xLastWakeTime;
 const TickType_t xFrequency = perSecond;
 xLastWakeTime = xTaskGetTickCount();
 
  while(1){
    struct soundVal currentVal;
    analogSoundSensorValue = analogRead(digitalSoundSensorPin);
    
     if(analogSoundSensorValue > acceptableVal.analog){
        Serial.println(analogSoundSensorValue, DEC); //634 seems to be the fan value from the laptop  
      currentVal.analog = analogSoundSensorValue;
     //if(xSemaphoreTake(xMutex, 0) == pdTRUE){
         xQueueSend(dataQ, &currentVal, portMAX_DELAY); 
        // xSemaphoreGive(xMutex);
    // }else{
      //   Serial.println("Analog sound Sensor not able to update queue, mutext not available.");
    // }
     }
     vTaskDelayUntil(&xLastWakeTime, xFrequency); // more precise delay
  }
  
}

// Digital
void getDigitalSoundSensorReading(void *pvParameters){
 (void) pvParameters;
 
 TickType_t xLastWakeTime;
 const TickType_t xFrequency = perSecond;
 xLastWakeTime = xTaskGetTickCount();
 
  while(1){
   digitalSoundSensorValue = digitalRead(analogSoundSensorPin); 
     
     if(digitalSoundSensorValue == HIGH){
        Serial.println(digitalSoundSensorValue, DEC); 
     }else{
        Serial.println(digitalSoundSensorValue, DEC);
     }
   
   vTaskDelayUntil(&xLastWakeTime, xFrequency);
  } 
}
