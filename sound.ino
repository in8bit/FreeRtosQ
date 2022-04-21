
// http://localhost/sensors/sound.php?digital=0&analog=665

//// Analog
//void getAnalogSoundSensorReading(void *pvParameters){
// (void) pvParameters;
// 
// TickType_t xLastWakeTime;
// const TickType_t xFrequency = perSecond;
// xLastWakeTime = xTaskGetTickCount();
// 
//  while(1){
//    analogSoundSensorValue = analogRead(digitalSoundSensorPin);
//     //if(analogSoundSensorValue > 635){
//        Serial.println(analogSoundSensorValue, DEC); //634 seems to be the fan value from the laptop  
//     // } 
//     vTaskDelayUntil(&xLastWakeTime, xFrequency); // more precise delay
//  }
//  
//}
//
//// Digital
//void getDigitalSoundSensorReading(void *pvParameters){
// (void) pvParameters;
// 
// TickType_t xLastWakeTime;
// const TickType_t xFrequency = perSecond;
// xLastWakeTime = xTaskGetTickCount();
// 
//  while(1){
//   digitalSoundSensorValue = digitalRead(analogSoundSensorPin); 
//     
//     if(digitalSoundSensorValue == HIGH){
//        Serial.println(digitalSoundSensorValue, DEC); 
//     }else{
//        Serial.println(digitalSoundSensorValue, DEC);
//     }
//   
//   vTaskDelayUntil(&xLastWakeTime, xFrequency);
//  } 
//}
