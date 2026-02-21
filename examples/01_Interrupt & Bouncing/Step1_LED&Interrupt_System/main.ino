int ledPins[] = {12, 14, 27, 26};
const int pushButton = 13;
int lengthofledPins = sizeof(ledPins) / sizeof(ledPins[0]);

// Flag to indicate interrupt occurrence
volatile bool pushON = false;
unsigned long lastTime = 0;

// Interrupt Service Routine (ISR) - Stored in RAM for faster execution
void IRAM_ATTR handleButton(){
  pushON = true;
}

void setup(){
  Serial.begin(115200);

  // Initialize LED Pins as Output
  for (int i = 0; i < lengthofledPins; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  
  // Initialize Push Button with Internal Pull-up Resistor
  pinMode(pushButton, INPUT_PULLUP);

  // Register Interrupt: Trigger handleButton on Falling Edge
  attachInterrupt(digitalPinToInterrupt(pushButton), handleButton, FALLING);

  Serial.println("========= LED Interrupt Simulation System ========");
}

void loop(){
  for (int i = 0; i < lengthofledPins; i++){
    // Check if interrupt flag is set
    if(pushON){
      runInterrupt();
      i = 0; // Reset sequence after interrupt task
    }
    
    // Default LED Chasing Sequence
    digitalWrite(ledPins[i], HIGH);
    digitalWrite(ledPins[(i + 3) % 4], LOW);
    delay(500);
  }
}

// Function to execute when interrupt is triggered
void runInterrupt(){
  Serial.println("Interrupt!!");
  
  // Blink all LEDs 3 times
  for (int i = 0; i < 3; i++){
    for (int j = 0; j < lengthofledPins; j++){
      digitalWrite(ledPins[j], HIGH);
    }
    delay(500);
    for (int k = 0; k < lengthofledPins; k++){
      digitalWrite(ledPins[k], LOW);
    }
    delay(500);
    
    pushON = false; // Reset the interrupt flag
  }
}
