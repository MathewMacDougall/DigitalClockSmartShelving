#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#endif

#include <DS3231_Simple.h>
DS3231_Simple Clock;

// Create a variable to hold the time data 
DateTime datetime;

// LED setup
#define LEDS_PER_SEGMENT 10
#define LED_MINUTES_PIN    4
#define LED_MINUTES_COUNT 140
#define LED_HOURS_PIN    5
#define LED_HOURS_COUNT 90
#define LED_DOWNLIGHT_PIN    6
#define LED_DOWNLIGHT_COUNT 12

#define HOURS_TENS_DIGIT_OFFSET 7 * LEDS_PER_SEGMENT
#define HOURS_ONES_DIGIT_OFFSET 0
#define MINUTES_TENS_DIGIT_OFFSET 7 * LEDS_PER_SEGMENT
#define MINUTES_ONES_DIGIT_OFFSET 0

#define DELAY_MS 1000
#define DELAY_S (DELAY_MS / 1000.0)

// Mapped to 0 (dark) to 100 (bright)
#define LIGHT_SENSOR_RAW_MIN_VALUE 100 // When it's bright
#define LIGHT_SENSOR_RAW_MAX_VALUE 1023 // When it's dark

#define CLOCK_FACE_MIN_BRIGHTNESS 100
#define CLOCK_FACE_MAX_BRIGHTNESS 255
#define CLOCK_FACE_MIN_BRIGHTNESS_THRESHOLD 25
#define CLOCK_FACE_MAX_BRIGHTNESS_THRESHOLD 50
#define CLOCK_FACE_OFF_BRIGHTNESS_THRESHOLD 1

#define DARK_COLOR_BRIGHTNESS_THRESHOLD 30
#define LIGHT_COLOR_BRIGHTNESS_THRESHOLD 80

#define PHOTORESISTOR_BUFFER_SIZE 10

Adafruit_NeoPixel hoursClock(LED_HOURS_COUNT, LED_HOURS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutesClock(LED_MINUTES_COUNT, LED_MINUTES_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter(LED_DOWNLIGHT_COUNT, LED_DOWNLIGHT_PIN, NEO_GRB + NEO_KHZ800);

int readings[PHOTORESISTOR_BUFFER_SIZE];
int readIndex = 0;

int previous_day = 0;
uint32_t hour_color;
uint32_t minute_color;


void setup() {
  Serial.begin(9600);

  // Sample random seed from analog noise
  randomSeed(analogRead(2));
  
  Clock.begin();

  hoursClock.begin();
  hoursClock.setBrightness(255);
  hoursClock.show();
 
  minutesClock.begin();
  minutesClock.setBrightness(255);
  minutesClock.show();
  
  stripDownlighter.begin();
  stripDownlighter.setBrightness(100);
  stripDownlighter.show();
  
  for (int i = 0; i < PHOTORESISTOR_BUFFER_SIZE; i++) {
    readings[i] = 0;
  } 
}

void loop() { 
  minutesClock.clear();
  hoursClock.clear();
  
  updateAndPrintCurrentTime();
  int lightSensorValue = getLightSensorValue();
  if (lightSensorValue >= CLOCK_FACE_OFF_BRIGHTNESS_THRESHOLD) {
    int clockFaceBrightness = constrain(map(lightSensorValue, CLOCK_FACE_MIN_BRIGHTNESS_THRESHOLD, CLOCK_FACE_MAX_BRIGHTNESS_THRESHOLD, CLOCK_FACE_MIN_BRIGHTNESS, CLOCK_FACE_MAX_BRIGHTNESS), CLOCK_FACE_MIN_BRIGHTNESS, CLOCK_FACE_MAX_BRIGHTNESS);
    Serial.print("Mapped brightness value = ");
    Serial.println(clockFaceBrightness);

    if(shouldChangeColor()) {
      getDateAwareRandomColorPair(datetime.Month, datetime.Day, lightSensorValue, hour_color, minute_color);  
    }

    displayCurrentTime(hour_color, minute_color);
    minutesClock.setBrightness(clockFaceBrightness);
    hoursClock.setBrightness(clockFaceBrightness);
    stripDownlighter.fill(stripDownlighter.Color(255, 255, 255), 0, LED_DOWNLIGHT_COUNT);
  }else {
      stripDownlighter.clear();
  }

  minutesClock.show();
  hoursClock.show();
  stripDownlighter.show();
  
  delay(DELAY_MS);
}

int getLightSensorValue() {
  readings[readIndex] = analogRead(A0);
  Serial.print("Light sensor value added to array = ");
  Serial.println(readings[readIndex]);
  
  readIndex = (readIndex + 1) % PHOTORESISTOR_BUFFER_SIZE;

  int sumBrightness = 0;
  for (int i=0; i < PHOTORESISTOR_BUFFER_SIZE; i++)
    {
        sumBrightness += readings[i];
    }
  Serial.print("Sum of the brightness array = ");
  Serial.println(sumBrightness);

  int lightSensorValue = sumBrightness / PHOTORESISTOR_BUFFER_SIZE;
  Serial.print("Average light sensor value = ");
  Serial.println(lightSensorValue);

  int normalized_value = constrain(map(lightSensorValue, LIGHT_SENSOR_RAW_MIN_VALUE, LIGHT_SENSOR_RAW_MAX_VALUE, 100, 0), 0, 100);
  Serial.print("Normalized light sensor value = ");
  Serial.println(normalized_value);

  return normalized_value;
}

void displayCurrentTime(uint32_t hour_color, uint32_t minute_color) {
  int minute_ones_digit = datetime.Minute % 10;
  ones_displayNumber(minutesClock, minute_ones_digit, MINUTES_ONES_DIGIT_OFFSET, minute_color);
  
  int minute_tens_digit = floor(datetime.Minute / 10);
  tens_displayNumber(minutesClock, minute_tens_digit, MINUTES_TENS_DIGIT_OFFSET, minute_color);

  int current_hour = datetime.Hour;
  if (current_hour > 12) {
    current_hour -= 12;
  }
  // Show midnight as 12:00 instead of 0:00
  if (current_hour == 0) {
    current_hour = 12;
  }
  
  int hour_ones_digit = current_hour % 10; 
  ones_displayNumber(hoursClock, hour_ones_digit, HOURS_ONES_DIGIT_OFFSET, hour_color);

  int hour_tens_digit = floor(current_hour / 10);
  if (hour_tens_digit > 0) {
    hoursClock.fill(hour_color, HOURS_TENS_DIGIT_OFFSET, 2 * LEDS_PER_SEGMENT); 
  }
}

void updateAndPrintCurrentTime(){
  datetime = Clock.read();
  
  Serial.println("");
  Serial.print("Time is: ");   Serial.print(datetime.Hour);
  Serial.print(":"); Serial.print(datetime.Minute);
  Serial.print(":"); Serial.println(datetime.Second);
  Serial.print("Date is: 20");   Serial.print(datetime.Year);
  Serial.print(":");  Serial.print(datetime.Month);
  Serial.print(":");    Serial.println(datetime.Day);
}

bool shouldChangeColor() {
  if(datetime.Day != previous_day) {
    previous_day = datetime.Day;
    return true;
  }else {
    return false;
  }
}
