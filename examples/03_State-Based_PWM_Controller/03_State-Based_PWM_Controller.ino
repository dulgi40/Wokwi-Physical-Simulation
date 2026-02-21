#include <Arduino.h>;

// Pin Setting
static const int IDLEPin = 4;
static const int LEDPin = 2;
static const int RUNPin = 12;
static const int MENUPin = 15;
static const int RESETPin = 14;

// Status Setting
enum State : uint8_t {IDLE, RUN};
State state = IDLE;

// PWM Setting
const uint8_t pwmIndex[3] = {10,50,90};
uint8_t nowLevel = 0;
const int pwmBit = 8;
const int maxValue = (1<<pwmBit) -1;


// Function When Pushing MENU Button
void pwmUpdate(){
  nowLevel = (nowLevel+1) % 3;
  Serial.print("Duty Is "); Serial.print(pwmIndex[nowLevel]); Serial.println("%");
}

// Boolean for MENU Button Edge Detection
static bool lastMenubutton = HIGH; 

void setup(){
  Serial.begin(115200);

  // IDLE LED Pin & Button Setting
  pinMode(IDLEPin, OUTPUT);
  pinMode(RUNPin, INPUT_PULLUP);
  pinMode(MENUPin, INPUT_PULLUP);
  pinMode(RESETPin, INPUT_PULLUP);

  // PWM Setting for PWM LED Pin
  ledcAttach(LEDPin, 5000, pwmBit);


  Serial.println("PWM System Test");
  Serial.println("Now In IDLE");

}




void loop() {

  // Current MENU Button Status
  bool menuButtonPressed = digitalRead(MENUPin);
  
  // IDLE Status
  if(state == IDLE){

    // IDLE LED Pin ON
    digitalWrite(IDLEPin, HIGH);

    // RUN LED Pin OFF
    ledcWrite(LEDPin, 0);

    // Exception Process for MENU Button Pushing In IDLE
    if(lastMenubutton == HIGH && menuButtonPressed == LOW){
      Serial.println("Cant modify duty value in IDLE");
    }
    
    // Go To RUN Status
    if(digitalRead(RUNPin)==0){
      state = RUN;
      Serial.print("Now RUNNING ===      ");
      Serial.print("Duty Is "); Serial.print(pwmIndex[nowLevel]); Serial.println("%");
    }
    
  }


  // RUN Status
  if(state==RUN){
    
    // When Menu Button Pushing
    if(lastMenubutton == HIGH && menuButtonPressed == LOW){
    pwmUpdate(); // PWM Duty Value Update
    }

    // IDLE LED Pin OFF
    digitalWrite(IDLEPin, LOW);

    // PWM LED Pin ON & Update
    ledcWrite(LEDPin, maxValue * pwmIndex[nowLevel] / 100);

    // RESET Button Process
    if(digitalRead(RESETPin)==LOW){
      state = IDLE; // Go To IDLE
      Serial.println("Now In IDLE");
    }
  }

  // Process for MENU Button Edge Detection
  lastMenubutton = menuButtonPressed;
  delay(2); 
}
