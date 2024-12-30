int count=0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  count++;
  if (count>100) {
    count=1;
  }
  int sensorValue = analogRead(A0);
  //Serial.println(sensorValue);
  if (sensorValue<200) {
    Serial.println(count);
  }
  delay(1);
}