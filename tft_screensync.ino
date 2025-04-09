#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>

MCUFRIEND_kbv tft;

#define SCREEN_WIDTH  480
#define SCREEN_HEIGHT 320

void setup() {
    Serial.begin(9600); 
    uint16_t ID = tft.readID();
    if (ID == 0x0 || ID == 0xFFFF) {
        ID = 0x9486;  
    }
    tft.begin(ID);  
    tft.setRotation(1);  
    tft.fillScreen(0x07E0);
}

void loop() {
    String weather = fetchRealTimeWeather();
    displayWeather(weather);

    if (Serial.available()) {
        String inputText = Serial.readStringUntil('\n');
        inputText.trim(); 

        tft.fillRect(0, 40, SCREEN_WIDTH, SCREEN_HEIGHT - 40, 0xFFFF);  
        displayText(inputText); 
        displayText2("CHOM 24/7");
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
    tft.setTextColor(0x0000);
    tft.setCursor(10, 10);  
    tft.print(weather);  
}


void displayText(String text) {
    tft.setTextSize(7); 
    tft.setTextColor(0x0000); 

    int16_t x, y;
    uint16_t w, h;

    tft.setCursor(0, 0);
    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);

    int centerX = (SCREEN_WIDTH - w) / 2;
    int centerY = (SCREEN_HEIGHT - h) / 2;  

    tft.setCursor(centerX, centerY); 
    tft.print(text); 
    
}

void displayText2(String text) {
    tft.setTextSize(3); 
    tft.setTextColor(0xF800); 

    int16_t x, y;
    uint16_t w, h;

    tft.setCursor(0, 0);
    tft.getTextBounds(text, 0, 0, &x, &y, &w, &h);

    int centerX2 = (SCREEN_WIDTH - w) / 2;
    int centerY2 = (SCREEN_HEIGHT - h + 180) / 2;

    tft.setCursor(centerX2, centerY2);
    tft.print(text);
    
}
