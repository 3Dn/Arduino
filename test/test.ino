#include <Wire.h>
#include <ArduinoJson.h>
 
#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

JsonObject& prepareResponse(JsonBuffer& jsonBuffer) {
  JsonObject& root = jsonBuffer.createObject();

  JsonArray& analogValues = root.createNestedArray("analog");
  for (int pin = 0; pin < 6; pin++) {
    int value = analogRead(pin);
    analogValues.add(value);
  }

  JsonArray& digitalValues = root.createNestedArray("digital");
  for (int pin = 0; pin < 14; pin++) {
    int value = digitalRead(pin);
    digitalValues.add(value);
  }

  return root;
}

void setup() {
 pinMode(13, OUTPUT);

 // initialize i2c as slave
 Wire.begin(SLAVE_ADDRESS);
 
 // define callbacks for i2c communication
 Wire.onReceive(receiveData);
 Wire.onRequest(sendData);
}
 
void loop() {
 delay(100);
}
 
// callback for received data
void receiveData(int byteCount){
 
 while(Wire.available()) {
  number = Wire.read();
 
  if (number == 1){
   if (state == 0){
    digitalWrite(13, HIGH); // set the LED on
    state = 1;
   } else{
    digitalWrite(13, LOW); // set the LED off
    state = 0;
   }
  }
 
  if(number==2) {
   //number = (int)temp;
  }
 }
}
 
// callback for sending data
void sendData(){
 //Wire.write(number);
 StaticJsonBuffer<500> jsonBuffer;
 JsonObject& json = prepareResponse(jsonBuffer);
 json.printTo(Wire);
}
