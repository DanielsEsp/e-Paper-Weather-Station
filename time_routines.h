



String jourTxt[8] = { "", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche" };
String moisTxt[13] = { "", "Janvier", "Fevrier", "Mars", "Avril", "Mai", "Juin", "Juillet", "Aout", "Septembre", "Octobre", "Novembre", "Décembre" };
String dateTxt, oldDateTxt, timeTxt, oldTimeTxt, heureTxt, minuteTxt;


void setTimezone(String timezone) {
  
     Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
     setenv("TZ",timezone.c_str(),1); 
     tzset();
     
}


void initTime(String timezone) {
  
     struct tm timeinfo;
     Serial.println("  Setting up time");
     configTime(0, 0, "pool.ntp.org");    // First connect to NTP server, with 0 TZ offset
     if (!getLocalTime(&timeinfo)) {
         Serial.println("  Failed to obtain time");
         return;
        }
     Serial.println("  Got the time from NTP");
     // Now we can set the real timezone
     setTimezone(timezone);
     
}


void printLocalTime() {
 
     struct tm timeinfo;
     if (!getLocalTime(&timeinfo)) {
         Serial.println("Failed to obtain time 1");
         return;
        }

     char timeHour[3];
     strftime(timeHour,3, "%H", &timeinfo);
     String heures = timeHour;

     char timeMinute[3];
     strftime(timeMinute,3, "%M", &timeinfo);
     String minutes = timeMinute;

     char timeInfoDay[2];
     strftime(timeInfoDay,2, "%u", &timeinfo);    
     int timeDay = String(timeInfoDay).toInt();     

     char timeInfoJour[3];
     strftime(timeInfoJour,3, "%e", &timeinfo);    
     int timeJour = String(timeInfoJour).toInt();    

     char timeInfoMois[3];
     strftime(timeInfoMois,3, "%m", &timeinfo);     
     int timeMois = String(timeInfoMois).toInt();     

     dateTxt = jourTxt[timeDay] + " " + String(timeJour) + " " + moisTxt[timeMois];
//     Serial.println(dateTxt);
     timeTxt = heures + ":" + minutes;
//     Serial.println(timeTxt);
     
}


void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  
     struct tm tm;
            tm.tm_year = yr - 1900;   // Set date
            tm.tm_mon = month-1;
            tm.tm_mday = mday;
            tm.tm_hour = hr;      // Set time
            tm.tm_min = minute;
            tm.tm_sec = sec;
            tm.tm_isdst = isDst;  // 1 or 0
            time_t t = mktime(&tm);
            Serial.printf("Setting time: %s", asctime(&tm));

     struct timeval now = { .tv_sec = t };
     settimeofday(&now, NULL);
     
}
