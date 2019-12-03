bool direction = true; //forward/reverse
bool running = true; // activate the outputs

#define A    A0
#define B    A1
#define Z    A2


int speed = 64; // RPM for Z-Pulses
volatile int PPR = 16; //PPR default of encoder
volatile long periodLength = 100; //default period length
unsigned long lastA = millis();
int pulseCount = 0;

void setup() {
  Serial.begin(115200);
  periodLength = _periodLength();
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Z, OUTPUT);
  delay(1);
  Serial.print("Period Length: ");
  Serial.println(periodLength);
  delay(1000);
  lastA=millis();
}

int _periodLength(){
  int n = 1.0/((PPR * speed/60.0))*1000.0; // time in ms for a complete period
  Serial.println(int(.25*n));
  Serial.println(.5*n);
  Serial.println(.75*n);
  return n;
}

void loop() {
 if(running) doEncoderThing();
} 

void doEncoderThing(){
  if(millis()>=(lastA + periodLength)){
    digitalWrite(A, direction);
    pulseCount++;
    lastA=millis(); 
  }
  else if(millis()>=(lastA + int(.75*periodLength)) && digitalRead(B) == direction){
    digitalWrite(B, !direction);
    if(digitalRead(Z) == direction){
      digitalWrite(Z, !direction); 
    }
  }
  else if(millis()>=(lastA + int(.5*periodLength)) && digitalRead(A) == direction){
    digitalWrite(A, !direction);
  }
  else if(millis()>=(lastA + int(.25*periodLength)) && digitalRead(B) == !direction && digitalRead(A) == direction){
    digitalWrite(B, direction);
    if(pulseCount >= PPR) {
      digitalWrite(Z, direction);
      pulseCount = 0;
    } 
  }
}
