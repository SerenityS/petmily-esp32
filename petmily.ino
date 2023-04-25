void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  
  startBLE();
}

void loop() {
  if(Serial2.available() > 0){
    String hex = String(Serial2.read());
    Serial.print(hex);
  }
  
  loopBLE();
}