#include "twilio.hpp"

#define PD_PIN 5
#define TD_PIN 18

static const char *ssid = "Ganesh D";
static const char *password = "timetrack@1234";

static const char *from_number = "+17817982801";

String to_numbers[] = {"+918777653929", "+918910084161"};

uint8_t PD_STATE = false, TD_STATE = false, stats = false;
uint64_t lastmillis = 0;

Twilio *twilio;

void setup()
{
  Serial.begin(115200);
  pinMode(PD_PIN, INPUT);
  pinMode(TD_PIN, INPUT);
  pinMode(2, OUTPUT);
  
  setupWiFiAndTwilio();
}

void loop()
{
  if(stats)
  {
    boolean success = false;
  
    Serial.printf("\nTD: %d  PD: %d Intrude: %d", TD_STATE, PD_STATE, checkMovement());
  
    if (checkMovement())
    {
      String response;
      //success = twilio->send_message(to_numbers[1], from_number, "Movement Alert", response);
      success = twilio->send_message(to_numbers[0], from_number, "Movement Alert", response);
      
      
      for(int i=0;i<20;i++)
      {
        digitalWrite(2, LOW);
        delay(300);
        digitalWrite(2, HIGH);
        delay(100);     
      }
      digitalWrite(2, LOW);
      delay(5000);
      //stats = false;
    }
  }
Serial.println(digitalRead(0));
  if(!digitalRead(0))
  {
    for(int i=15;i>=0;i--)
    {
      digitalWrite(2, LOW);
      delay(i*50);
      digitalWrite(2, HIGH);
      delay(i*50);     
    }
      digitalWrite(2, LOW);
    stats = 1;
  }
}

boolean checkMovement()
{
  boolean intrude = false;

  PD_STATE = digitalRead(PD_PIN);
  TD_STATE = !digitalRead(TD_PIN);
  
  if (TD_STATE)
  {
    if (millis() - lastmillis > 2000)
      intrude = true;
  }

  else
  {
    intrude = false;
    lastmillis = millis();
  }

  return intrude;
}


void setupWiFiAndTwilio()
{
  static const char *account_sid = "AC8d124f051460d10ffeca8d93e23067c5";
  static const char *auth_token = "f53ece252b5e7ce0449a1caa777927c5";

  Serial.print("Connecting to WiFi network ;");
  Serial.print(ssid);
  Serial.println("'...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("Connected!");

  twilio = new Twilio(account_sid, auth_token);
}
