#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Fonts/FreeSans12pt7b.h>
#include <NewPing.h>

const byte TRIGGER_PIN = D7;
const byte ECHO_PIN = D8;

const int SCREEN_WIDTH = 128; // OLED display width (pixels)
const int SCREEN_HEIGHT = 32; // OLED display height (pixels)
const int OLED_RESET = -1;  // Reset pin (or -1 if sharing Arduino reset pin)

#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int MAX_DISTANCE = 400;
const int TEMPERATURE = 15;
const int HUMIDITY = 80;

// the speed of sound is 340 m/s when air temp is 15 degrees celsius
int SOUND_SPEED = 331.4 + (0.606 * TEMPERATURE) + (0.0124 * HUMIDITY);

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, distance;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LED_BUILTIN, LOW);
  OLED.begin();
  OLED.setRotation(2);
  OLED.clearDisplay();
  OLED.setTextWrap(true);
  OLED.setTextSize(2);
  OLED.setTextColor(WHITE);
  OLED.setCursor(0, 0);
  OLED.println("Ultrasonic ruler");
  OLED.display();
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  OLED.setTextSize(3);
  OLED.display();
}

void loop() {
  const int ITERATIONS = 10;
  duration = sonar.ping_median(ITERATIONS);
  distance = (duration / 2) * SOUND_SPEED / 10000;

  OLED.clearDisplay();
  OLED.setCursor(0, 0);

  int distance_int = static_cast<int>(distance);

  if (distance > 0) {
    if (distance >= MAX_DISTANCE || distance <= 2) {
      Serial.println("Out of range");
      OLED.println("------");
    } else {
      Serial.println(distance);
      OLED.print(distance_int);
      OLED.println(" cm");
    }

    OLED.display();
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
