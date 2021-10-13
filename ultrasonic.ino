#include <NewPing.h>

#include "math.h"

#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 400
#define PING_DELAY   33

#define DURATION_INIT_PHASE 5000
#define TRIGGER_DISTANCE_DELTA 20
#define TRIGGER_PERCENTAGE 80

#define TRIGGER_COUNT     1
#define TRIGGER_DISTANCE  100
#define TRIGGER_COOLDOWN  2000
#define TRIGGER_LED_PULSE 100

#define SIGNAL_PIN       4
#define SIGNAL_PIN_PULSE 10

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

static uint32_t triggerDistance = 0;

void setup()
{
  uint32_t startInitPhase = 0;
  uint32_t ulDistance = 0;
  uint32_t countMeasurements = 0;

  Serial.begin(115200);
  pinMode(SIGNAL_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(SIGNAL_PIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);

  /* Initialization phase */
  Serial.println("Start initialization phase");
  startInitPhase = millis();
  for (;;)
  {
    uint32_t ulDuration;
    uint32_t ulStart = millis();

    ulDistance += sonar.ping_cm();

    if ((uint32_t)(millis() - startInitPhase) < DURATION_INIT_PHASE)
    {
      countMeasurements++;
    }
    else
    {
      break;
    }

    ulDuration = millis() - ulStart;
    if (ulDuration < PING_DELAY)
    {
      delay(PING_DELAY - ulDuration);
    }
  }

  /* Measure the trigger distance from the init phase */
  triggerDistance = ulDistance / countMeasurements;
  Serial.println("Initializeation phase completed");
  Serial.print("Took ");
  Serial.print(countMeasurements);
  Serial.print(" measurements with a median of ");
  Serial.print(triggerDistance);
  Serial.println(" cm");
  
  triggerDistance = (triggerDistance * TRIGGER_PERCENTAGE) / 100;
  Serial.print("Trigger distance of ");
  Serial.print(triggerDistance);
  Serial.println(" cm configured");
}

void loop()
{
  static uint32_t ulCount = 0;
  uint32_t ulDuration;
  uint32_t ulStart = millis();
  uint32_t ulDistance = sonar.ping_cm();

  if (ulDistance == 0)
  {
    /* Invalid measurement */
  }
  else if (ulDistance < triggerDistance)
  {
    ulCount++;
    if (ulCount > TRIGGER_COUNT)
    {
      /* Generate a pulse on the signal pin */
      digitalWrite(SIGNAL_PIN, HIGH);
      delayMicroseconds(SIGNAL_PIN_PULSE);
      digitalWrite(SIGNAL_PIN, LOW);

      /* Blink the LED */
      digitalWrite(LED_BUILTIN, HIGH);
      delay(min(TRIGGER_LED_PULSE, TRIGGER_COOLDOWN));
      digitalWrite(LED_BUILTIN, LOW);

      Serial.print("Trigger distance: ");
      Serial.print(ulDistance);
      Serial.println(" cm");

      /* Trigger cooldown */
      if (TRIGGER_LED_PULSE < TRIGGER_COOLDOWN)
      {
        delay(TRIGGER_COOLDOWN - TRIGGER_LED_PULSE);
      }
      ulCount = 0;
    }
  }
  else
  {
    ulCount = 0;
  }

  ulDuration = millis() - ulStart;
  if (ulDuration < PING_DELAY)
  {
    delay(PING_DELAY - ulDuration);
  }
}