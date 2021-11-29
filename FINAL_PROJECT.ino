#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 96

// You can use any (4 or) 5 pins
#define SCLK_PIN 19
#define MOSI_PIN 18
#define DC_PIN   31
#define CS_PIN   30
#define RST_PIN  32

// Color definitions
#define  BLACK           0x0000
#define BLUE            0x001F
#define RED             0xF800
#define GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include "BetterButton.h"

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

float p = 3.1415926;

int centX = 64;
int centY = 48;

uint8_t sides1 = 1;
uint8_t sides2 = 1;

unsigned long lastStepTime = 0;
int currentStep = 0;
int stepLength = 2000;
int tempo = 120;

bool buttonState[4] = { LOW, LOW, LOW, LOW };
bool lastButtonState[4] = { LOW, LOW, LOW, LOW };

int buttonArray[4] = { 33, 34, 35, 36 };

BetterButton button1(buttonArray[0], 0);
BetterButton button2(buttonArray[1], 1);
BetterButton button3(buttonArray[2], 2);
BetterButton button4(buttonArray[3], 3);

void setup(void) {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++) {
    pinMode(buttonArray[i], INPUT);
  }

  button1.pressHandler(onPress);
  button1.releaseHandler(onRelease);
  button2.pressHandler(onPress);
  button2.releaseHandler(onRelease);
  button3.pressHandler(onPress);
  button3.releaseHandler(onRelease);
  button4.pressHandler(onPress);
  button4.releaseHandler(onRelease);

  tft.begin();
  Serial.println("init");
  drawThings();
  playNotes();
}

void loop() {
  button1.process();
  button2.process();
  button3.process();
  button4.process();
  playNotes();
}

void onPress(int val) {
  if (val == 0) if (sides1 < 12) sides1 = sides1 + 1;
  if (val == 1) if (sides1 > 1) sides1 = sides1 - 1;
  if (val == 2) if (sides2 > 1) sides2 = sides2 - 1;
  if (val == 3) if (sides2 < 12) sides2 = sides2 + 1;
  stepLength = 2000/(sides1*sides2);
  drawThings();
}

void onRelease() {
}

void playNotes() {
  if (millis() > lastStepTime + stepLength) {
    lastStepTime = millis();
    currentStep = currentStep + 1;
    Serial.println(currentStep);
    if (currentStep >= (sides1*sides2)) {
      currentStep = 0;
    }
    for (int i = 0; i < (sides1*sides2); i++) {
      if (currentStep == (i * sides2)) {
        usbMIDI.sendNoteOff(45, 0, 1);
        Serial.println("dum");
        usbMIDI.sendNoteOn(45, 127, 1);
      }
      if (currentStep == (i * sides1)) {
        usbMIDI.sendNoteOff(47, 0, 1);
        Serial.println("da");
        usbMIDI.sendNoteOn(47, 127, 1);
      }
    }
  }
}


void drawThings() {
  Serial.println(sides1);
  tft.fillScreen(BLACK);
  tft.drawCircle(64, 48, 40, RED);
  polygon(sides1, GREEN, 4);
  polygon(sides2, BLUE, 2);
  tft.setCursor(centX - 26, centY - 10);
  tft.setTextColor(RED);
  tft.setTextSize(3);
  tft.print(tempo);
}

void polygon(uint8_t sides, uint16_t color, uint8_t cSize) {
  for (uint8_t i = 0; i < sides; i++) {
    float deg = i * 360 / sides;
    uint8_t x = 64 + 40 * cos((deg + 90) * p / 180);
    uint8_t y = 48 + 40 * sin((deg - 90) * p / 180);
    tft.drawCircle(x, y, cSize, color);
  }
}
