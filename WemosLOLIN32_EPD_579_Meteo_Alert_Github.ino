



//        open-meteo   Free API sans API_KEY
//        weatherapi   Free API avec limite de calls at API_KEY

//        mapping of Waveshare ESP32 Driver Board
//        BUSY -> 25, RST -> 26, DC -> 27, CS -> 33, CLK -> 13, DIN -> 14
//            Violet,     Blanc,     Vert,   Orange,     Jaune,      Bleu 
//     hspi.begin(13, 12, 14, 33); // ESP32         remap hspi for EPD (swap pins) !IMPORTANT
//     hspi.begin(14, 12, 13, 5);  // WemosLOLIN32  remap hspi for EPD (swap pins) !IMPORTANT


#include <GxEPD2_BW.h>
#define   GxEPD2_DISPLAY_CLASS GxEPD2_BW

#define   USE_HSPI_FOR_EPD
#define   EPD_CS    5//33  > orange  
#define   EPD_DC   15//27  > vert 
#define   EPD_RST  16//26  > blanc
#define   EPD_BUSY 04//25  > gris = violet 
// SCL/SCK/CLK     14//13  > jaune
// SDA/DIN/MOSI    13//14  > bleu

#define   GxEPD2_DRIVER_CLASS GxEPD2_579_GDEY0579T93 // GDEY0579T93 792x272, SSD1683 (FPC-E004 22.04.13)
#define   GxEPD2_579_GDEY0579T93_IS_BW true
          GxEPD2_BW<GxEPD2_579_GDEY0579T93, GxEPD2_579_GDEY0579T93::HEIGHT> epd(GxEPD2_579_GDEY0579T93(EPD_CS, EPD_DC, EPD_RST, EPD_BUSY)); // GDEY0579T93 792x272, SSD1683 (FPC-E004 22.04.13)
          SPIClass hspi(HSPI);

#include <Adafruit_GFX.h>  
#include <U8g2_for_Adafruit_GFX.h>
          U8G2_FOR_ADAFRUIT_GFX affiche;

#include <WiFi.h>
          const char * ssid="VOTRE_SSID";
          const char * wifipw="VOTRE_PASSWORD";
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "time.h"
#include "time_routines.h"
#include "clock.h"
     bool firstFlag = true;

#include "bitmap_icones.h"
#include "icones_grille.h"
      int pointeur;
const unsigned char* icone;

//        Varialbles WEATHER DATAS LOAD
   String current = ("https://api.open-meteo.com/v1/forecast?latitude=VOTRE_LAT&longitude=VOTRE_LONG&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,precipitation,weather_code,surface_pressure,wind_speed_10m,wind_direction_10m,wind_gusts_10m&timezone=auto");    
   String url;
   String current_date;
   String last_weather_update;
   String temperature;
   String ressenti;
   String humidite;
   String pression;
   String vent_vitesse;
   String vent_rafale;
      int vent_direction, Direction;
   String vient_De; 
   String is_day;
     bool grande_icone = true;
   String weather_code;
   String weather_code_txt;
      int meteoCall = (5*60)*1000;//    pour 5 minutes en microsecondes
      int lastMeteoCall;
      int pressionCall = (60*60)*1000;//  pour 1 heure en microsecondes
      int lastPressionCall;
   String oldWeatherCode, oldTemperatureFormatTxt, oldHumiditeFormatTxt, oldPressionFormatTxt, oldPressionDisplay, oldVentVitesse;
    float temperatureFormat, ressentiFormat, pressionFormat;
      int humiditeFormat;
   String temperatureFormatTxt, ressentiFormatTxt, humiditeFormatTxt, pressionFormatTxt;
   String description;
const char degree_symbol[] = "\u00B0C";

    String siAlerte;
       int numeroAlerte, nombreAlertes;
    String evenementAlerte[7];



void startWifi() {
 
     WiFi.begin(ssid, wifipw);
     Serial.println("Connecting Wifi");
     while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(500);
           }
     Serial.print("Wifi RSSI=");
     Serial.println(WiFi.RSSI());
     Serial.println();
     
}



void setup() {
 
     Serial.begin(115200); Serial.println();

     startWifi();
     initTime("CET-1CEST,M3.5.0,M10.5.0/3");   // Set for Berlin
//     WiFi.disconnect();    
     
//     hspi.begin(13, 12, 14, 33);  // ESP32      remap hspi for EPD (swap pins) !IMPORTANT
     hspi.begin(14, 12, 13, 5);  // WemosLOLIN32  remap hspi for EPD (swap pins) !IMPORTANT
     epd.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));

     epd.init(115200, true, 50, false);
     epd.setRotation(0); 
     epd.fillScreen(GxEPD_BLACK);
     epd.setTextColor(GxEPD_WHITE);
     
     affiche.begin(epd); // connect u8g2 procedures to Adafruit GFX
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_fub35_tr);
             
}



void loop() {

     if (WiFi.status() != WL_CONNECTED) {
         ESP.restart(); delay(50);
        }    
     
     printLocalTime(); 
     
     if (oldDateTxt != dateTxt || firstFlag == true) {
         oldDateTxt = dateTxt;         
         epd.drawPaged(drawFull, 0);//               Grille de base
         epd.drawPaged(drawPartial1, 0);//           Affiche heure
         current_datas_load(); delay(50); 
         alert_datas_load(); delay(50);
         if (firstFlag == true) { oldPressionDisplay =  pressionFormatTxt; firstFlag = false; }            
         epd.drawPaged(drawPartial2, 0);//           Affiche icone
         epd.drawPaged(drawPartial3, 0);//           Affiche description
         epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas
         epd.drawPaged(drawPartial5, 0);//           Affiche vent datas 
        }

     if (oldTimeTxt != timeTxt) {
         oldTimeTxt = timeTxt;
         epd.drawPaged(drawPartial1, 0);               
        }

     if ((millis() - lastMeteoCall) > meteoCall) {
         lastMeteoCall = millis();
         current_datas_load(); delay(50);              

         if (weather_code != oldWeatherCode) {
             oldWeatherCode = weather_code;
             epd.drawPaged(drawPartial2, 0);//           Affiche icone              
             epd.drawPaged(drawPartial3, 0);//           Affiche description
            }
         if (temperatureFormatTxt != oldTemperatureFormatTxt) {
             oldTemperatureFormatTxt = temperatureFormatTxt;
             epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas              
            }
         if (humiditeFormatTxt != oldHumiditeFormatTxt) {
             oldHumiditeFormatTxt = humiditeFormatTxt;
             epd.drawPaged(drawPartial4, 0);//           Affiche temp. datas              
            }
         if (vent_vitesse != oldVentVitesse) {
             oldVentVitesse = vent_vitesse;
             epd.drawPaged(drawPartial5, 0);//           Affiche vent datas 
            }

        }

     if ((millis() - lastPressionCall) > pressionCall) {
         lastPressionCall = millis();
         current_datas_load(); delay(50);            
         oldPressionDisplay = oldPressionFormatTxt;
         oldPressionFormatTxt = pressionFormatTxt;
         epd.drawPaged(drawPartial4, 0);//                Affiche temp. datas              
         alert_datas_load();
        }
                
     delay(100);
     epd.hibernate();
     delay(50);
     epd.powerOff();  
     delay(100);
     esp_sleep_enable_timer_wakeup(55*1000000);
     esp_light_sleep_start();
     delay(50);
    
}



void drawFull(const void* pv) { //                                   Grille de base
  
     epd.setFullWindow();
     epd.fillRoundRect(5, 0, 780, 60, 10, GxEPD_BLACK);
     epd.setTextColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_fub35_tr);
     affiche.setCursor(20, 50);
     affiche.print(dateTxt);
     epd.drawBitmap(590, 12, epd_bitmap_clock, 40, 40, GxEPD_WHITE);
     epd.drawRoundRect(5, 65, 150, 130, 10, GxEPD_BLACK); //         Grille icone
     epd.drawRoundRect(160, 65, 625, 35, 10, GxEPD_BLACK); //        Grille description
     epd.drawRoundRect(160, 105, 625, 90, 10, GxEPD_BLACK); //       Grille temp. datas
     epd.drawRoundRect(5, 200, 780, 70, 10, GxEPD_BLACK); //         Grille vent datas
   
}


void drawPartial1(const void* pv) { //            Affiche heure
 
     epd.setPartialWindow(640, 10, 160, 45);
     epd.fillRoundRect(0, 0, 785, 60, 10, GxEPD_BLACK);
     affiche.setBackgroundColor(GxEPD_BLACK);  
     affiche.setForegroundColor(GxEPD_WHITE);
     affiche.setFont(u8g2_font_fub35_tr);
     affiche.setCursor(645, 50);
     affiche.print(timeTxt); 
  
}


void drawPartial2(const void* pv) { //           Affiche icone
  
     epd.setPartialWindow(10, 70, 140, 120);
     epd.fillRect(10, 70, 140, 120, GxEPD_WHITE);
     epd.drawBitmap(10, 70, icone_name[pointeur], 140, 120, GxEPD_BLACK);
 
}


void drawPartial3(const void* pv) { //           Affiche description
  
     epd.setPartialWindow(170, 70, 500, 25);
     epd.fillRect(170, 70, 600, 25, GxEPD_WHITE);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(175, 90);
     affiche.print(data_description[pointeur]);
  
}


void drawPartial4(const void* pv) { //           Affiche temp. datas

     epd.setPartialWindow(170, 110, 610, 80);
     epd.fillRect(170, 110, 610, 80, GxEPD_WHITE);
     affiche.setBackgroundColor(GxEPD_WHITE);  
     affiche.setForegroundColor(GxEPD_BLACK);
     affiche.setFont(u8g2_font_10x20_mf);
     affiche.setCursor(195, 125);
     affiche.print("Température");    
     affiche.setCursor(360, 125);
     affiche.print("Ressenti");    
     affiche.setCursor(505, 125);
     affiche.print("Humidité");    
     affiche.setCursor(665, 125);
     affiche.print("Pression");    

     affiche.setFont(u8g2_font_logisoso46_tf);
     affiche.setCursor(170, 185);
     affiche.print(temperatureFormatTxt + "\u00B0");     
     affiche.setCursor(320, 185);
     affiche.print(ressentiFormatTxt + "\u00B0");     
     affiche.setCursor(485, 185);
     affiche.print(humiditeFormatTxt);    
     affiche.setFont(u8g2_font_profont29_mf);
     affiche.setCursor(580, 155);
     affiche.print("%");
     affiche.setFont(u8g2_font_logisoso24_tf); 
     affiche.setCursor(635, 155);
     affiche.print("Act." + pressionFormatTxt);    
     affiche.setCursor(635, 185);   
     affiche.print("Old." + oldPressionDisplay);    
                  
}


void drawPartial5(const void* pv) { //            Affiche vent datas

     if (siAlerte == "[]") {      
//       Affiche vent
         epd.setPartialWindow(10, 205, 770, 60);
         epd.fillRect(15, 205, 755, 60, GxEPD_WHITE);
         affiche.setBackgroundColor(GxEPD_WHITE);  
         affiche.setForegroundColor(GxEPD_BLACK);
         affiche.setFont(u8g2_font_profont29_mf);
         affiche.setCursor(15, 230);
         affiche.print("Vitesse actuelle du vent "); affiche.print(vent_vitesse); affiche.print(" km/h "); affiche.println(vient_De); 
         affiche.print(" avec des rafales de ");affiche.print(vent_rafale); affiche.print(" km/h");
        } else {         
//       Affiche alerte
         epd.setPartialWindow(10, 205, 770, 60);
         epd.fillRoundRect(8, 205, 772, 60, 10, GxEPD_BLACK);
         affiche.setBackgroundColor(GxEPD_BLACK);  
         affiche.setForegroundColor(GxEPD_WHITE);
         affiche.setFont(u8g2_font_profont29_mf);
         affiche.setCursor(15, 230);
         affiche.println("  " + evenementAlerte[1]);
         affiche.println("   " + evenementAlerte[2]);       
        }
          
}


//
