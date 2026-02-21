#include <string.h> // for strcmp Function
#include <stdlib.h> // for atoi Function

char rxBuf[32]; // Input Text By UART
uint8_t rxLen = 0; // Input Text Index By UART


// PWM Setting
const int pwmValue[3] = {10, 50, 90};
uint8_t pwmIndex = 0;
const uint8_t pwmBit = 8;
const uint8_t pwmIndexSize = sizeof(pwmValue)/sizeof(pwmValue[0]);
int pwmMax = (1<<pwmBit) -1;


// Status Setting
enum State : uint8_t {IDLE, RUN};
State state = IDLE;



void setup() {
  Serial.begin(115200);

  // PWM Setup
  ledcAttach(12, 5000, pwmBit);


  Serial.println("UART Test System (if you don't know commands, type 'help')");
}


// UART Communication By Serial Text Input
void handleLine(char* line){

  // Go to RUN
  if (strcmp(line, "run") ==0){
    if(state == IDLE){
      state = RUN;
      Serial.println("Now RUNNING");
    }
    else{
      Serial.println("Already RUN");
    }
  }
  
  // Go to IDLE
  else if (strcmp(line, "idle")==0){
    if(state == RUN){
      state = IDLE;
      Serial.println("Now IDLE");

    }
    else{
      Serial.println("Already IDLE");
    }
  }

  // Set Duty 
  else if (strncmp(line, "duty ",5)==0){
    int value = atoi(&line[5]);
    bool found = false;
    for (int i=0;i<pwmIndexSize;i++){
      if(value==pwmValue[i]){
        pwmIndex = i;
        found = true;
        Serial.print("Duty Set To "); Serial.print(value); Serial.println("%");
        break;
      }
    }
    if(found == false){
      Serial.println("Doesn't Exist Value (must input 'duty 10/50/90')");
    }

  }

  // Show Current Status
  else if (strcmp(line, "status")==0){
    if(state==IDLE){
      Serial.println("Now IDLE");
    }
    else {Serial.println("Now RUNNING");}
    Serial.print("Duty: "); Serial.print(pwmValue[pwmIndex]); Serial.println("%");
  }

  // Show Commands
  else if (strcmp(line, "help")==0){
    Serial.println("Available Commands:");
    Serial.println(" run");
    Serial.println(" idle");
    Serial.println(" duty 10 / 50 / 90");
    Serial.println(" status");
  }

  // else : Unknown Command
  else{
    Serial.println("Unknown Command");
  }
}

void loop() {

  // UART Communication
  while (Serial.available()>0){
    char c = Serial.read();


    // When Taking Enter
    if (c=='\n'){
      rxBuf[rxLen] = '\0';
      handleLine(rxBuf);
      rxLen = 0;

    }

    // When Taking Another
    else{
      if(c=='\r') continue;

      else if(rxLen < sizeof(rxBuf) -1){
        rxBuf[rxLen] = c;
        rxLen++;
      }


    }

  }
  
  // In IDLE
  if (state==IDLE){
    ledcWrite(12, 0);
  }


  // In RUN
  else if (state==RUN){
    ledcWrite(12, pwmMax * pwmValue[pwmIndex] / 100);

  }


  delay(2); //
}

