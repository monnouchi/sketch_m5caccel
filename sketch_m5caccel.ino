#include <M5StickC.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

int outputMode = 0;
int isSending = false;

float accX = 0;
float accY = 0;
float accZ = 0;

void setup() {
  pinMode(GPIO_NUM_10, OUTPUT);
  digitalWrite(GPIO_NUM_10, HIGH); // OFF
  
  Serial.begin(115200);
  SerialBT.begin("ESP32");

  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("MPU6886 SEND");
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.println("  X       Y       Z      ");
  M5.MPU6886.Init();

}

void loop() {
  M5.MPU6886.getAccelData(&accX,&accY,&accZ);

  float margedAcc = sqrt(pow(accX,2.0)+pow(accY,2.0)+pow(accZ,2.0));

  switch(outputMode) {
    case 1: // only X
      accY = 0.0f;
      accZ = 0.0f;
      break;
      
    case 2: // only Y
      accX = 0.0f;
      accZ = 0.0f;
      break;
      
    case 3: // only X
      accX = 0.0f;
      accY = 0.0f;
      break;
      
    case 4: // only Synth
      accX = 0.0f;
      accY = 0.0f;
      accZ = 0.0f;
      break;
      
    case 0: // ALL
    default:
      break;
  }
  
  M5.Lcd.setCursor(0, 45);  
  M5.Lcd.printf("%.2f   %.2f   %.2f   g\nSynth: %.2f   g    ",
    accX,
    accY,
    accZ,
    margedAcc);
  M5.Lcd.setCursor(140, 45);

  if(isSending) {
    SerialBT.printf("%.8f,%.8f,%.8f,%.8f\n",
      accX,
      accY,
      accZ,
      margedAcc);
  }

  delay(10);
  M5.update();
  if (M5.BtnA.wasPressed() ){
    toggleSenser();
  }
  if (M5.BtnB.wasPressed() ){
    changeMode();
  }
}

void changeMode() {
  outputMode += 1;
  if( outputMode > 4 ){
    outputMode = 0;
  }
}
void toggleSenser() {
  isSending = !isSending;
  if(isSending) {
    digitalWrite(GPIO_NUM_10, LOW);  // ON
    
    SerialBT.printf("X,Y,Z,Synth\n");
        
  }else{
    digitalWrite(GPIO_NUM_10, HIGH); // OFF
  }
}
