// 13/Dez/2015 - Luzes ano novo

void setup() {
  Serial.begin(9600);
  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
}

void loop() {
  blinkClear();
  blinkPair(800);
  blinkPair(800);
  blinkPair(800);

  blinkClear();
  blinkClock(500);
  blinkClock(300);

  blinkClear();
  blinkCounterClock(500);
  blinkCounterClock(300);
}

void blinkPair(int T){  // P = pin 1 or 2;
  digitalWrite(2, LOW); // T = delay period;
  digitalWrite(4, LOW);
  digitalWrite(3, HIGH);
  digitalWrite(5, HIGH);
  delay(T);
  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  delay(T);
}

void blinkClear(){ // Turn off all lights
  for (int Li = 2; Li < 6; Li++){
    digitalWrite(Li, HIGH);  
  }
  delay(200);
}

void blinkAll(){ // Turn on all lights
  for (int Li = 2; Li < 6; Li++){
    digitalWrite(Li, LOW);  
  }
  delay(200);
}

void blinkClock(int T){ // Clockwise sequential blink
  for (int Ci = 2; Ci < 6; Ci++){
    digitalWrite(Ci, LOW);
    delay(T);
    digitalWrite(Ci, HIGH);
  }
}

void blinkCounterClock(int T){ // Counter-clockwise sequential blink
  for (int CCi = 5; CCi > 1; CCi--){
    digitalWrite(CCi, LOW);
    delay(T);
    digitalWrite(CCi, HIGH);
  }
}

