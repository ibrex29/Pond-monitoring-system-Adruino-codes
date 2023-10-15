void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int turbidity = analogRead(A0);
float volt = turbidity * (5.0/1024.0) *3 ;
Serial.println("sensor output");
Serial.println(volt);
Serial.println();
delay(1000);
}
