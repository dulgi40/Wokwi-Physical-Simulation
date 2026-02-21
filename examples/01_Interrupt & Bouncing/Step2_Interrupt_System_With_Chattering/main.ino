// --- Pin Configuration ---
const int pushButton = 12;      // Input Pin (Software Debouncing Applied)
const int pushButtonNC = 14;    // Input Pin (Raw Signal / Chattering occurs)

// --- Global Variables for Debouncing ---
int cnt = 0;                    // Event counter (Stable)
unsigned long lastTime = 0;     // Last trigger time for debouncing
bool lastStatus = HIGH;         // Previous button state

// --- Global Variables for Raw Signal ---
int cntNC = 0;                  // Event counter (Unstable)
bool lastStatusNC = HIGH;       // Previous button state

void setup() {
  Serial.begin(115200);

  // Initialize digital pins as input with internal pull-up resistors
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(pushButtonNC, INPUT_PULLUP);

  Serial.println("========= What is BOUNCE? ========");
}

void loop() {
  unsigned long nowTime = millis();

  // ========================= DEBOUNCED SYSTEM ==========================
  bool nowStatus = digitalRead(pushButton);

  if (nowStatus == LOW && lastStatus == HIGH) {
    // Check if 100ms has passed since the last valid press
    if (nowTime - lastTime > 100) {
      cnt++;
      Serial.print("Push Button Count (Stable): "); 
      Serial.println(cnt);
      lastTime = nowTime;
    }
  }
  lastStatus = nowStatus;


  // ========================= RAW SYSTEM (CHATTERING) ==========================
  bool nowStatusNC = digitalRead(pushButtonNC);

  // Logic operator fixed: '&' changed to '&&' for logical AND
  if (nowStatusNC == LOW && lastStatusNC == HIGH) {
    cntNC++;
    Serial.print("Push Button Count (Unstable): "); 
    Serial.println(cntNC);
  }
  lastStatusNC = nowStatusNC;
}
