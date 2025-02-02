



void alert_datas_load() {
  
     if (WiFi.status() == WL_CONNECTED) {
         HTTPClient http; 
         String url = String("https://api.weatherapi.com/v1/alerts.json?key=VOTRE_KEY&q=VOTRE_VILLE");      
         http.begin(url);
         int httpCode = http.GET(); 
         Serial.println(httpCode);

         if (httpCode > 0) {
             if (httpCode == HTTP_CODE_OK) {
                 String payload = http.getString();
                 Serial.println("Request information:");
                 Serial.println(payload); Serial.println();
                 StaticJsonDocument<1024> doc;
                 DeserializationError error = deserializeJson(doc, payload);

                 if (!error) {
                              JsonObject location = doc["location"];
                              const char* location_name = location["name"];
                              siAlerte = String(doc["alerts"]["alert"]);
                              if (siAlerte == "[]") { evenementAlerte[1] = ""; evenementAlerte[2] = ""; numeroAlerte = 0; return; } 
                              for (JsonObject alerts_alert_item : doc["alerts"]["alert"].as<JsonArray>()) {
                                   numeroAlerte ++;
                                   const char* alerts_alert_item_event = alerts_alert_item["event"];
                                   evenementAlerte[numeroAlerte] = alerts_alert_item_event;
                                  }
                               nombreAlertes = numeroAlerte; 
                              } else {
                               Serial.print("deserializeJson() failed: ");
                               Serial.println(error.c_str());
                              }

                }
           } else {
            Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
            delay(5000);
            Serial.printf("Reset !");           
            esp_restart(); delay(100);
           }
       http.end(); // Close connection
      } else {
       Serial.println("Not connected to Wi-Fi");
      }
  
}


//
