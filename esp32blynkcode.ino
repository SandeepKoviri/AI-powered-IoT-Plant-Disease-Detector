#define BLYNK_TEMPLATE_ID "TMPL3cUsEpof5"
#define BLYNK_TEMPLATE_NAME "SANDEEP"
#define BLYNK_AUTH_TOKEN "Qrkf9sE1KHke8HMsdQo0wivLPfnaGpdd"


#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// COREXY
#define STEP_PIN_1 12
#define DIR_PIN_1 13
#define STEP_PIN_2 14
#define DIR_PIN_2 27
#define FORWARD HIGH
#define BACKWARD LOW

// peripherals
#define FERTILIZER_PUMP_PIN 25
#define WATER_PUMP_PIN 26
#define RED_LED_PIN 22
#define BLUE_LED_PIN 21
#define BUZZER_PIN 23
#define LIMIT_RIGHT 32
#define LIMIT_LEFT 33
#define LIMIT_TOP 34
#define LIMIT_BOTTOM 35
#define SOIL_MOISTURE_PIN 39

#define DHTPIN 0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Blynk
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "A";
char pass[] = "111111111";

bool autoModeActive = false;
bool manualModeActive = false;
bool sirenActive = false;
int currentX = 0, currentY = 0;

// Option A: static IP
const char* cam_url_detect = "http://10.69.251.154/detect";
const char* cam_url_stream = "http://10.69.251.154/stream";

// Option B: mDNS name (if you enabled MDNS on ESP32-CAM and your router supports it)
// const char* cam_url_detect = "http://esp32cam.local/detect";
// const char* cam_url_stream = "http://esp32cam.local/stream";


void verifyHomePosition() {
  Serial.println("Verifying Home Position...");

  digitalWrite(DIR_PIN_1, HIGH); digitalWrite(DIR_PIN_2, LOW);
  while (digitalRead(LIMIT_BOTTOM) != LOW) {
    digitalWrite(STEP_PIN_1, HIGH); digitalWrite(STEP_PIN_2, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP_PIN_1, LOW); digitalWrite(STEP_PIN_2, LOW);
    delayMicroseconds(800);
  }
  delay(300);

  digitalWrite(DIR_PIN_1, LOW); digitalWrite(DIR_PIN_2, LOW);
  while (digitalRead(LIMIT_LEFT) != LOW) {
    digitalWrite(STEP_PIN_1, HIGH); digitalWrite(STEP_PIN_2, HIGH);
    delayMicroseconds(800);
    digitalWrite(STEP_PIN_1, LOW); digitalWrite(STEP_PIN_2, LOW);
    delayMicroseconds(800);
  }
  currentX = 0; currentY = 0;
  Serial.println("✅ Home verified");
}

void moveTo(int x, int y) {
  int dx = x - currentX;
  int dy = y - currentY;

  if (dx != 0) {
    digitalWrite(DIR_PIN_1, dx > 0 ? BACKWARD : FORWARD);
    digitalWrite(DIR_PIN_2, dx > 0 ? FORWARD : BACKWARD);
    for (int i = 0; i < abs(dx); i++) {
      digitalWrite(STEP_PIN_1, HIGH); digitalWrite(STEP_PIN_2, HIGH);
      delayMicroseconds(800);
      digitalWrite(STEP_PIN_1, LOW); digitalWrite(STEP_PIN_2, LOW);
      delayMicroseconds(800);
    }
  }

  if (dy != 0) {
    digitalWrite(DIR_PIN_1, dy > 0 ? FORWARD : BACKWARD);
    digitalWrite(DIR_PIN_2, dy > 0 ? FORWARD : BACKWARD);
    for (int i = 0; i < abs(dy); i++) {
      digitalWrite(STEP_PIN_1, HIGH); digitalWrite(STEP_PIN_2, HIGH);
      delayMicroseconds(800);
      digitalWrite(STEP_PIN_1, LOW); digitalWrite(STEP_PIN_2, LOW);
      delayMicroseconds(800);
    }
  }

  currentX = x; currentY = y;
  delay(500);
}

bool isDiseasedLeaf() {
  HTTPClient http;
  http.begin(cam_url_detect);
  int httpCode = http.GET();
  if (httpCode == 200) {
    String payload = http.getString();
    return payload.indexOf("whiteDot's") >= 0;
  }
  return false;
}

void checkPlant(int x, int y, String name) {
  if (!autoModeActive) return;

  moveTo(x, y);
  delay(2000);

  if (isDiseasedLeaf()) {
    Serial.println(name + " diseased, fertilizing");
    Blynk.virtualWrite(V4, name + " diseased (getting fertilizer)");

    // Fertilizer pump ON
    digitalWrite(FERTILIZER_PUMP_PIN, LOW);

    // Start siren pulse for 4 seconds
    unsigned long startTime = millis();
    bool ledState = false;

    while (millis() - startTime < 4000) {
      ledState = !ledState;
      digitalWrite(RED_LED_PIN, ledState ? HIGH : LOW);
      digitalWrite(BLUE_LED_PIN, ledState ? LOW : HIGH);
      digitalWrite(BUZZER_PIN, ledState ? HIGH : LOW);
      Blynk.virtualWrite(V11, ledState);  // Blynk siren indicator
      delay(300);
    }

    // Stop fertilizer, siren, buzzer
    digitalWrite(FERTILIZER_PUMP_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(BLUE_LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Blynk.virtualWrite(V11, 0);

  } else {
    Serial.println(name + " healthy");
    Blynk.virtualWrite(V4, name + " healthy");
  }
}


// BLYNK callbacks
BLYNK_WRITE(V20) {  // auto mode
  autoModeActive = param.asInt();
  if (autoModeActive) {
    manualModeActive = false;
  }
}
BLYNK_WRITE(V21) {  // manual mode
  manualModeActive = param.asInt();
  if (manualModeActive) {
    autoModeActive = false;
  }
}
BLYNK_WRITE(V10) {  // image capture
  if (manualModeActive && param.asInt()) {
    Blynk.virtualWrite(V30, cam_url_stream);
  }
}
BLYNK_WRITE(V12) { if(manualModeActive && param.asInt()) moveTo(currentX,currentY+200); }
BLYNK_WRITE(V13) { if(manualModeActive && param.asInt()) moveTo(currentX,currentY-200); }
BLYNK_WRITE(V14) { if(manualModeActive && param.asInt()) moveTo(currentX-200,currentY); }
BLYNK_WRITE(V15) { if(manualModeActive && param.asInt()) moveTo(currentX+200,currentY); }
BLYNK_WRITE(V16) { if(manualModeActive) digitalWrite(FERTILIZER_PUMP_PIN, !param.asInt()); }
BLYNK_WRITE(V17) { if(manualModeActive) digitalWrite(WATER_PUMP_PIN, !param.asInt()); }
BLYNK_WRITE(V18) { // emergency stop
  digitalWrite(STEP_PIN_1, LOW); digitalWrite(STEP_PIN_2, LOW);
  digitalWrite(FERTILIZER_PUMP_PIN, HIGH);
  digitalWrite(WATER_PUMP_PIN, HIGH);
}

void soilMoistureTask(void *pv) {
  for (;;) {
    int val = analogRead(SOIL_MOISTURE_PIN);
    Blynk.virtualWrite(V0, val);
    Blynk.virtualWrite(V1, val > 2000 ? "Soil dry" : "Soil wet");
    digitalWrite(WATER_PUMP_PIN, val > 2000 ? LOW : HIGH);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}

void dhtTask(void *pv) {
  for(;;) {
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    Blynk.virtualWrite(V2, t);
    Blynk.virtualWrite(V3, h);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(STEP_PIN_1, OUTPUT); pinMode(DIR_PIN_1, OUTPUT);
  pinMode(STEP_PIN_2, OUTPUT); pinMode(DIR_PIN_2, OUTPUT);
  pinMode(FERTILIZER_PUMP_PIN, OUTPUT); digitalWrite(FERTILIZER_PUMP_PIN, HIGH);
  pinMode(WATER_PUMP_PIN, OUTPUT); digitalWrite(WATER_PUMP_PIN, HIGH);
  pinMode(RED_LED_PIN, OUTPUT); pinMode(BLUE_LED_PIN, OUTPUT); pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LIMIT_LEFT, INPUT_PULLUP); pinMode(LIMIT_BOTTOM, INPUT_PULLUP);

  dht.begin();
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  verifyHomePosition();

  xTaskCreatePinnedToCore(soilMoistureTask,"soilTask",4096,NULL,1,NULL,1);
  xTaskCreatePinnedToCore(dhtTask,"dhtTask",4096,NULL,1,NULL,1);
}

void loop() {
  Blynk.run();

  if (autoModeActive) {
    checkPlant(0, 200, "P1");
    checkPlant(900, 200, "P2");
    checkPlant(1900, 200, "P3");
    checkPlant(0, 1500, "P4");
    checkPlant(900, 1500, "P5");
    checkPlant(1900, 1500, "P6");
    moveTo(0,0);
    Blynk.virtualWrite(V4,"Auto cycle complete");
    delay(10000);
  }
}
