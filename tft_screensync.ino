#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 320

bool showAlert = true;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500; // Blink every 500ms

void setup() {
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    if (ID == 0x0 || ID == 0xFFFF) {
        ID = 0x9486;
    }
    tft.begin(ID);
    tft.setRotation(1);
    tft.fillScreen(0x0000);
}

void loop() {
    String weather = fetchRealTimeWeather();
    displayWeather(weather);

    if (Serial.available()) {
        String inputText = Serial.readStringUntil('\n');
        inputText.trim();

        // Clear main text area (below weather)
        tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, 0x0000);
        displayText(inputText);
        displayText2("Room Occupant: Arnav Gawas (zxcv)");
        displayText4("Network Goblin Inside!");
    }

    // Handle blinking !!Alert!!
    unsigned long currentMillis = millis();
    if (currentMillis - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = currentMillis;
        showAlert = !showAlert;

        // Clear alert area
        int16_t x, y;
        uint16_t w, h;
        tft.setTextSize(2);
        tft.getTextBounds("!!Alert!!", 0, 0, &x, &y, &w, &h);
        int centerX = (SCREEN_WIDTH - w) / 2;
        int centerY = (SCREEN_HEIGHT - h) / 2;
        tft.fillRect(centerX, centerY, w, h, 0x0000);

        if (showAlert) {
            displayText3("!!Alert!!");
        }
    }

    delay(100);
}

String fetchRealTimeWeather() {
    String weather = "";
    if (Serial.available()) {
        weather = Serial.readStringUntil('\n');
        weather.trim();
    }
    return weather;
}

void displayWeather(String weather) {
    tft.setTextSize(2);
    tft.setTextColor(0xFFFF); // White
    tft.setCursor(10, 10);
    tft.print(weather);
}

void displayText(String text) {
    tft.setTextSize(6);
    tft.setTextColor(0xFFFF); // White

    int16_t x, y;
    uint16_t w, h;

    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
    int centerX = (SCREEN_WIDTH - w) / 2;
    int centerY = (SCREEN_HEIGHT - h - 150) / 2;

    tft.setCursor(centerX, centerY);
    tft.print(text);
}

void displayText2(String text) {
    tft.setTextSize(2);
    tft.setTextColor(0xF800); // Red

    int16_t x, y;
    uint16_t w, h;

    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
    int centerX2 = (SCREEN_WIDTH - w) / 2;
    int centerY2 = (SCREEN_HEIGHT - h + 260) / 2;

    tft.setCursor(centerX2, centerY2);
    tft.print(text);
}

void displayText3(String text) {
    tft.setTextSize(2);
    tft.setTextColor(0xF800); // Red

    int16_t x, y;
    uint16_t w, h;

    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
    int centerX2 = (SCREEN_WIDTH - w) / 2;
    int centerY2 = (SCREEN_HEIGHT - h) / 2;

    tft.setCursor(centerX2, centerY2);
    tft.print(text);
}

void displayText4(String text) {
    tft.setTextSize(3);
    tft.setTextColor(0xF800); // Red

    int16_t x, y;
    uint16_t w, h;

    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);
    int centerX2 = (SCREEN_WIDTH - w) / 2;
    int centerY2 = (SCREEN_HEIGHT - h + 120) / 2;

    tft.setCursor(centerX2, centerY2);
    tft.print(text);
}
