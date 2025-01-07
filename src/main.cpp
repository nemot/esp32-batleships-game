#include <Arduino.h>
#include <vector>
#include <esp_system.h>
#include "WiFi.h"
#include <ESPmDNS.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "FontsRus/CourierCyr8.h"
#include "types.h"
#include "Grid.h"
#include "ships.h"
#include "images.h"

#define WIFI_SSID "Finetwork_q79F"
#define WIFI_PASSWORD "HNfY4H5R"
#define PRINTER_HOST "hpe073e7fd8c1c.local"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define RED_PIN 19
#define BLUE_PIN 21

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Grid my_grid;
Grid opponents_grid;

void connect_to_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print(String("Connecting to wifi: ") + WIFI_SSID + " .");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.println(WiFi.localIP());
}

void place_random_ship(std::vector<Ship>& ships, short quantity ) {
  Ship random_ship;
  Coordinate random_coordinate;
  short ships_placed = 0;
  auto empty_coordinates = my_grid.empty_cells();
  
  while(ships_placed < quantity) {
    random_coordinate = empty_coordinates[esp_random() % empty_coordinates.size()];
    random_ship = ships[esp_random() % ships.size()];
    if(my_grid.place_figure(random_coordinate, random_ship)) { ships_placed ++; }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(RED_PIN, INPUT_PULLUP);
  pinMode(BLUE_PIN, INPUT_PULLUP);

  Wire.begin(23, 22);
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  oled.clearDisplay();
  oled.drawBitmap(0, 0, yes_no_bmp, 128, 64, WHITE);
  oled.setTextSize(1);
  oled.setTextColor(WHITE);
  oled.setCursor(20, 15);
  oled.setFont(&CourierCyr8pt8b); // выбор шрифта
  oled.println("Утопил?");
  oled.display();

  my_grid.empty();
  place_random_ship(four_cells_ships, 1);
  place_random_ship(three_cells_ships, 2);
  place_random_ship(two_cells_ships, 3);
  place_random_ship(one_cells_ships, 4);
  my_grid.inspect();


  Serial.println("");
  Serial.println("");

  opponents_grid.acknowledge_cell({2,2}, CELL_FULL);
  opponents_grid.acknowledge_cell({3,2}, CELL_FULL);

  opponents_grid.inspect();
  connect_to_wifi();
}

bool blue_button_prev_state = HIGH;
bool red_button_prev_state = HIGH;

void loop() {

  bool red_button_state = digitalRead(RED_PIN);
  if(red_button_prev_state == HIGH && red_button_state == LOW) {
    oled.clearDisplay();
    oled.drawBitmap(0, 0, yes_bmp, 128, 64, WHITE);
    oled.display();

    delay(300);

    oled.clearDisplay();
    oled.drawBitmap(0, 0, yes_no_bmp, 128, 64, WHITE);
    oled.setCursor(20, 15);
    oled.println("RED");
    oled.display();
  }
  red_button_prev_state = red_button_state;

  bool blue_button_state = digitalRead(BLUE_PIN);
  if(blue_button_prev_state == HIGH && blue_button_state==LOW) {
    oled.clearDisplay();
    oled.drawBitmap(0, 0, no_bmp, 128, 64, WHITE);
    oled.display();

    delay(300);

    oled.clearDisplay();
    oled.drawBitmap(0, 0, yes_no_bmp, 128, 64, WHITE);
    oled.setCursor(20, 15);
    oled.println("BLUE");
    oled.display();
  }
  blue_button_prev_state = blue_button_state;

  

}
