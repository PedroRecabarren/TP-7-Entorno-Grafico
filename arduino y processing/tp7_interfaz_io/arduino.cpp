const int PIN_E1 = 2;
const int PIN_E2 = 3;
const int PIN_L1 = 8;
const int PIN_L2 = 9;

bool estadoL1 = false;
bool estadoL2 = false;

int lastRawE1 = HIGH;
int lastRawE2 = HIGH;
int stableE1 = LOW; // 1 = activo, 0 = inactivo
int stableE2 = LOW;

unsigned long lastDebounceE1 = 0;
unsigned long lastDebounceE2 = 0;
unsigned long lastSend = 0;
const unsigned long DEBOUNCE_MS = 35;
const unsigned long SEND_INTERVAL_MS = 100;

void actualizarLeds() {
  digitalWrite(PIN_L1, estadoL1 ? HIGH : LOW);
  digitalWrite(PIN_L2, estadoL2 ? HIGH : LOW);
}

void enviarEstado() {
  Serial.print("E:");
  Serial.print(stableE1);
  Serial.print(",");
  Serial.print(stableE2);
  Serial.print(";S:");
  Serial.print(estadoL1 ? 1 : 0);
  Serial.print(",");
  Serial.println(estadoL2 ? 1 : 0);
}

void leerEntradas() {
  unsigned long now = millis();

  int rawE1 = digitalRead(PIN_E1);
  if (rawE1 != lastRawE1) {
    lastDebounceE1 = now;
    lastRawE1 = rawE1;
  }
  if ((now - lastDebounceE1) > DEBOUNCE_MS) {
    int nuevoE1 = (rawE1 == LOW) ? 1 : 0;
    if (nuevoE1 != stableE1) {
      stableE1 = nuevoE1;
      // Punto 3: al pulsar/cambiar entrada fisica, activa/alterna salida correspondiente
      if (stableE1 == 1) {
        estadoL1 = !estadoL1;
        actualizarLeds();
      }
    }
  }

  int rawE2 = digitalRead(PIN_E2);
  if (rawE2 != lastRawE2) {
    lastDebounceE2 = now;
    lastRawE2 = rawE2;
  }
  if ((now - lastDebounceE2) > DEBOUNCE_MS) {
    int nuevoE2 = (rawE2 == LOW) ? 1 : 0;
    if (nuevoE2 != stableE2) {
      stableE2 = nuevoE2;
      if (stableE2 == 1) {
        estadoL2 = !estadoL2;
        actualizarLeds();
      }
    }
  }
}

void leerComandosSerial() {
  while (Serial.available() > 0) {
    char cmd = Serial.read();
    // Punto 2: activar/alternar salidas al clickear en Processing
    if (cmd == '1') {
      estadoL1 = !estadoL1;
      actualizarLeds();
    } else if (cmd == '2') {
      estadoL2 = !estadoL2;
      actualizarLeds();
    }
  }
}

void setup() {
  pinMode(PIN_E1, INPUT_PULLUP);
  pinMode(PIN_E2, INPUT_PULLUP);
  pinMode(PIN_L1, OUTPUT);
  pinMode(PIN_L2, OUTPUT);
  actualizarLeds();
  Serial.begin(9600);
}

void loop() {
  leerEntradas();
  leerComandosSerial();

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;
    enviarEstado();
  }
}