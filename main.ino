#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS D1
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
BlynkTimer timer;

    float temp;
    float temp1;
    int pinValue_V0;
    int pinValue_V6;
    int pinValue_V7;
    int switcher = D2;
    int check_phase = D5;
    int temp_diff_define = 2.00;
    int temp_diff_current = 0.00;
    int phase_status = 0;

    char auth[] = "qOmpm-Kb8r2Zc6zcVyqeDPWyQeSdkOYs";
    char ssid[] = "xxxxx";
    char pass[] = "xxxxx";

   
    void setup()
    {
      Serial.begin(115200);
      sensors.begin();
      Blynk.begin(auth, ssid, pass);
      timer.setInterval(3000, sendTemps);
      timer.setInterval(3000, phase);
      pinMode(switcher, OUTPUT);
      pinMode(check_phase, INPUT);
         
    }
  
    void sendTemps()
    {
      sensors.requestTemperatures(); 
      temp  = sensors.getTempCByIndex(0); 
      temp1  = sensors.getTempCByIndex(1); 
      Blynk.virtualWrite(V1, temp); 
      Blynk.virtualWrite(V2, temp1); 
    }

    BLYNK_WRITE(V0) 
    {
      pinValue_V0 = param.asInt(); 
      
      if(pinValue_V7 == 0)
      {
        if (pinValue_V0 == 1){
        digitalWrite(switcher, HIGH);  
        delay(100);
        } else if (pinValue_V0 == 0) {
        digitalWrite(switcher, LOW); 
        delay(100);
        }
      }
    }

    BLYNK_WRITE(V6) 
    {
      pinValue_V6 = param.asInt(); 
    }

    BLYNK_WRITE(V7) 
    {
      pinValue_V7 = param.asInt(); 

    }

    void phase() 
    {
      phase_status = digitalRead(check_phase);
      Blynk.virtualWrite(V5, phase_status);  
    }
  
    void loop()
    {
      if (Blynk.connected())
          { Blynk.run();
            Blynk.syncAll();
          }
      timer.run();
      
      if (pinValue_V7 == 1)
        {
          if (temp < pinValue_V6)
            {
              digitalWrite(switcher, HIGH);  
              Blynk.virtualWrite(V0, 1);  
              delay(100);
            }
          else 
            {
              digitalWrite(switcher, LOW);   
              Blynk.virtualWrite(V0, 0);  
              delay(100);
            }
        }
      temp_diff_current = temp - temp1;
      if (temp_diff_current == temp_diff_define )
        {
          Blynk.logEvent("warning");
        }
           
    }
