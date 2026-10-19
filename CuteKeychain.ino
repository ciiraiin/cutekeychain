#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>
#include "bitmapcake.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define TOUCH_PIN 4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

int mode = 0;
bool lastTouch = LOW;
uint8_t cakeIdx = 0;
unsigned long cakeTimer = 0;
int scrollX = 0;
unsigned long scrollTimer = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TOUCH_PIN, INPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED tidak ditemukan!");
    for(;;);
  }
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
  roboEyes.setAutoblinker(ON, 2, 2); // Start auto blinker animation cycle -> bool active, int interval, int variation -> turn on/off, set interval between each blink in full seconds, set range for random interval variation in full seconds
  roboEyes.setIdleMode(ON, 1, 1); // Start idle animation cycle (eyes looking in random directions) -> turn on/off, set interval between each eye repositioning in full seconds, set range for random time interval variation in full seconds
  roboEyes.setWidth(36, 36); // byte leftEye, byte rightEye
  roboEyes.setHeight(36, 36); // byte leftEye, byte rightEye
  roboEyes.setBorderradius(8, 8); // byte leftEye, byte rightEye
  roboEyes.setSpacebetween(10); // int space -> can also be negative
  roboEyes.setPosition(DEFAULT);
}

void loop() {
  bool touch = digitalRead(TOUCH_PIN);
  // Mendeteksi perubahan LOW -> HIGH
  if (touch == HIGH && lastTouch == LOW) {
    mode++;
    if (mode > 6) {
      mode = 0;
    }
    aturMode();
    Serial.print("Mode: ");
    Serial.println(mode + 1);
    delay(100);
  }
  lastTouch = touch;

  if (mode <= 4) {
    roboEyes.update();
  }
  else if (mode == 5) {
    updateCakeGIF();
  }

  else if (mode == 6) {
    updateScrollText();
  }
}

// FUNGSI MENAMPILKAN GAMBAR
void aturMode() {
  if (mode == 0) {       //default
    display.stopscroll();
    roboEyes.setMood(DEFAULT);
    roboEyes.setCuriosity(OFF);
  }
  else if (mode == 1) {  //happy
    roboEyes.anim_laugh();
    roboEyes.setMood(HAPPY);
    roboEyes.setCuriosity(OFF);
  }
  else if (mode == 2) {  //curious
    roboEyes.setMood(DEFAULT);
    roboEyes.setCuriosity(ON);
  }
  else if (mode == 3) {  //tired
    roboEyes.setMood(TIRED);
    roboEyes.setCuriosity(OFF);
  }
  else if (mode == 4) {  //angry
    roboEyes.setMood(ANGRY);
    roboEyes.setCuriosity(ON);
  }
  else if (mode == 5) {  //gif birthday cake
    updateCakeGIF();
  }
  else if (mode == 6) {  //scroll text
    updateScrollText();
  }
}

void updateCakeGIF() {
  const uint8_t* cakes[] = {
    cake0, cake1, cake2, cake3, cake4, cake5,cake6, cake7, cake8, cake9, cake10, cake11, cake12, cake13, 
    cake14, cake15, cake16, cake17, cake18, cake19, cake20, cake21, cake22, cake23,cake24, cake25, cake26, cake27
  };

  if (millis() - cakeTimer >= 100) {
    cakeTimer = millis();
    display.clearDisplay();
    display.drawBitmap(0,0,cakes[cakeIdx], 128, 64,SSD1306_WHITE);
    display.display();
    cakeIdx++;
    if (cakeIdx >= 28) {
      cakeIdx = 0;
    }
  }
}

void updateScrollText() {
  if (millis() - scrollTimer >= 15) {
    scrollTimer = millis();
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(SSD1306_WHITE);
    int16_t x1, y1;
    uint16_t textWidth, textHeight;
    display.getTextBounds(
      "PRETTY", 0, 20, &x1, &y1, &textWidth, &textHeight);

    // PRETTY pertama
    display.setCursor(scrollX, 20);
    display.print("PRETTY");

    // PRETTY kedua
    display.setCursor(scrollX + textWidth + 20, 20);
    display.print("PRETTY");

    // PRETTY ketiga
    display.setCursor(scrollX + (textWidth + 20) * 2, 20);
    display.print("PRETTY");
    display.fillRect(0, 40, 128, 24, BLACK);
    display.display();

    // Bergerak ke kiri
    scrollX--;

    // Setelah satu jarak teks bergeser,
    // ulangi posisi
    if (scrollX <= -(textWidth + 20)) {
      scrollX = 0;
    }
  }
}