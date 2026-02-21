#include <Arduino.h>

// ===== pin setting =====
static const int LED_PIN    = 2;
static const int START_PIN  = 12;
static const int SENSOR_PIN = 14;
static const int RESET_PIN  = 27;

// ===== 1ms timer tick =====
hw_timer_t *g_timer = NULL;
volatile uint32_t g_ms = 0; // ms counter, updated in ISR

// timer ISR (1ms)
void IRAM_ATTR onTimer() { 
  g_ms++; 
}

// ===== FSM =====
enum State : uint8_t { IDLE, RUN, FAULT };
State state = IDLE;

uint32_t lastSensorMs = 0; // last valid sensor time (for timeout)

// ===== button debounce (edge) =====
struct DebouncedButton {
  int pin;
  bool lastLevel;          // last read level
  uint32_t lastAcceptedMs; // last accepted press time
};

DebouncedButton btnStart  {START_PIN,  HIGH, 0};
DebouncedButton btnSensor {SENSOR_PIN, HIGH, 0};
DebouncedButton btnReset  {RESET_PIN,  HIGH, 0};

// read pin level (HIGH/LOW)
static inline bool readLevel(int pin) {
  return digitalRead(pin);
}

// falling edge + debounce ok
bool pollPressed(DebouncedButton &b, uint32_t now, uint32_t debounceMs = 50) {
  bool level = readLevel(b.pin);

  // falling edge: HIGH -> LOW (INPUT_PULLUP)
  bool fallingEdge = (b.lastLevel == HIGH && level == LOW);
  b.lastLevel = level;

  if (!fallingEdge) return false;

  // ignore bounce (within debounceMs)
  if (now - b.lastAcceptedMs < debounceMs) return false;

  b.lastAcceptedMs = now;
  return true;
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(START_PIN, INPUT_PULLUP);
  pinMode(SENSOR_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);

  // timer setup:
  // timer freq = 1MHz (1 tick = 1us)
  // alarm = 1000us -> 1ms interrupt
  g_timer = timerBegin(1000000);
  timerAttachInterrupt(g_timer, &onTimer);
  timerAlarm(g_timer, 1000, true, 0);

  Serial.println("=== 1ms Timer + FSM + Timeout FAULT ===");
}

void loop() {
  uint32_t now = g_ms; // take snapshot

  // ===== input events =====
  if (pollPressed(btnStart, now)) {
    if (state == IDLE) {
      state = RUN;
      lastSensorMs = now; // init timeout base
      Serial.println("[FSM] IDLE -> RUN");
    }
  }

  if (pollPressed(btnSensor, now)) {
    if (state == RUN) {
      lastSensorMs = now; // refresh timeout
      Serial.printf("[SENSOR] trigger %lu ms\n", (unsigned long)now);
    }
  }

  if (pollPressed(btnReset, now)) {
    if (state == FAULT) {
      state = IDLE;
      Serial.println("[FSM] FAULT -> IDLE (reset)");
    }
  }

  // ===== timeout check =====
  if (state == RUN) {
    if (now - lastSensorMs > 3000) { // 3 sec no sensor -> fault
      state = FAULT;
      Serial.println("[FAULT] Timeout -> FAULT");
    }
  }

  // ===== LED show state =====
  static uint32_t lastBlink = 0;

  if (state == IDLE) {
    if (now - lastBlink >= 500) { // slow
      lastBlink = now;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  } else if (state == RUN) {
    if (now - lastBlink >= 150) { // fast
      lastBlink = now;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  } else { // FAULT
    digitalWrite(LED_PIN, HIGH); // steady on
  }

  delay(2); // small delay for polling
}
