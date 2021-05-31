/*
 * 3D printed smart shelving with a giant hidden digital clock in the front edges of the shelves - DIY Machines

==========

More info and build instructions: https://www.youtube.com/watch?v=8E0SeycTzHw

3D printed parts can be downloaded from here: https://www.thingiverse.com/thing:4207524

You will need to install the Adafruit Neopixel library which can be found in the library manager.

This project also uses the handy DS3231 Simple library:- https://github.com/sleemanj/DS3231_Simple   Please follow the instruction on installing this provided on the libraries page

Before you install this code you need to set the time on your DS3231. Once you have connected it as shown in this project and have installed the DS3231_Simple library (see above) you
 to go to  'File' >> 'Examples' >> 'DS3231_Simple' >> 'Z1_TimeAndDate' >> 'SetDateTime' and follow the instructions in the example to set the date and time on your RTC

==========


 * SAY THANKS:

Buy me a coffee to say thanks: https://ko-fi.com/diymachines
Support us on Patreon: https://www.patreon.com/diymachines

SUBSCRIBE:
■ https://www.youtube.com/channel/UC3jc4X-kEq-dEDYhQ8QoYnQ?sub_confirmation=1

INSTAGRAM: https://www.instagram.com/diy_machines/?hl=en
FACEBOOK: https://www.facebook.com/diymachines/

*/


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

// Recording values to help tune
// * sunny at 5:20 pm   light sensor=389,  normalized value = 60
#define CLOCK_FACE_MIN_BRIGHTNESS 255
#define CLOCK_FACE_MAX_BRIGHTNESS 255
#define LIGHT_SENSOR_MIN_VALUE 70 // When it's bright
#define LIGHT_SENSOR_MAX_VALUE 800 // When it's dark
#define LIGHT_SENSOR_DARK_COLOR_THRESHOLD 50
#define LIGHT_SENSOR_LIGHT_COLOR_THRESHOLD 80

//(red * 65536) + (green * 256) + blue ->for 32-bit merged colour value so 16777215 equals white
// or 3 hex byte 00 -> ff for RGB eg 0x123456 for red=12(hex) green=34(hex), and green=56(hex) 
// this hex method is the same as html colour codes just with "0x" instead of "#" in front
uint32_t clockMinuteColour = 0x800000; // pure red 
uint32_t clockHourColour = 0x008000;   // pure green

// Declare our NeoPixel objects:
Adafruit_NeoPixel hoursClock(LED_HOURS_COUNT, LED_HOURS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutesClock(LED_MINUTES_COUNT, LED_MINUTES_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripDownlighter(LED_DOWNLIGHT_COUNT, LED_DOWNLIGHT_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

//Smoothing of the readings from the light sensor so it is not too twitchy
#define PHOTORESISTOR_BUFFER_SIZE 2

int readings[PHOTORESISTOR_BUFFER_SIZE];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading


void setup() {
  Serial.begin(9600);
  Clock.begin();

  hoursClock.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  hoursClock.setBrightness(255); // Set inital BRIGHTNESS (max = 255)
  hoursClock.show();            // Turn OFF all pixels ASAP
 
  minutesClock.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  minutesClock.setBrightness(255); // Set inital BRIGHTNESS (max = 255)
  minutesClock.show();            // Turn OFF all pixels ASAP
  
  stripDownlighter.begin();           // INITIALIZE NeoPixel stripClock object (REQUIRED)
  stripDownlighter.setBrightness(100); // Set BRIGHTNESS (max = 255)
  stripDownlighter.show();            // Turn OFF all pixels ASAP
  
  // initialize all the readings to 0:
  for (int i = 0; i < PHOTORESISTOR_BUFFER_SIZE; i++) {
    readings[i] = 0;
  } 
}

uint32_t xx = 0;
void loop() { 
  updateAndPrintCurrentTime();
  int lightSensorValue = getLightSensorValue();
  lightSensorValue = 100;
  int clockFaceBrightness = constrain(map(lightSensorValue, 0, 100, CLOCK_FACE_MIN_BRIGHTNESS, CLOCK_FACE_MAX_BRIGHTNESS), 0, 255);
  Serial.print("Mapped brightness value = ");
  Serial.println(clockFaceBrightness);
  uint32_t hour_color;
  uint32_t minute_color;
  getDateAwareRandomColorPair(datetime.Month, datetime.Day, lightSensorValue, hour_color, minute_color);

  minutesClock.clear();
  hoursClock.clear();
  displayCurrentTime(hour_color, minute_color);
  minutesClock.setBrightness(clockFaceBrightness);
  hoursClock.setBrightness(clockFaceBrightness);
  minutesClock.show();
  hoursClock.show();

  stripDownlighter.fill(stripDownlighter.Color(255, 255, 255), 0, LED_DOWNLIGHT_COUNT);
  stripDownlighter.show();
  
  delay(DELAY_MS);
}

int getLightSensorValue() {
  //Record a reading from the light sensor and add it to the array
  readings[readIndex] = analogRead(A0); //get an average light level from previouse set of samples
  Serial.print("Light sensor value added to array = ");
  Serial.println(readings[readIndex]);
  
  readIndex = (readIndex + 1) % PHOTORESISTOR_BUFFER_SIZE; // advance to the next position in the array:

  //now work out the sum of all the values in the array
  int sumBrightness = 0;
  for (int i=0; i < PHOTORESISTOR_BUFFER_SIZE; i++)
    {
        sumBrightness += readings[i];
    }
  Serial.print("Sum of the brightness array = ");
  Serial.println(sumBrightness);

  // and calculate the average: 
  int lightSensorValue = sumBrightness / PHOTORESISTOR_BUFFER_SIZE;
  Serial.print("Average light sensor value = ");
  Serial.println(lightSensorValue);

  int normalized_value = constrain(map(lightSensorValue, LIGHT_SENSOR_MIN_VALUE, LIGHT_SENSOR_MAX_VALUE, 100, 0), 0, 100);
  Serial.print("Normalized light sensor value = ");
  Serial.println(normalized_value);

  return normalized_value;
}

void updateAndPrintCurrentTime(){
  // Ask the clock for the data.
  datetime = Clock.read();
  
  // And use it
  Serial.println("");
  Serial.print("Time is: ");   Serial.print(datetime.Hour);
  Serial.print(":"); Serial.print(datetime.Minute);
  Serial.print(":"); Serial.println(datetime.Second);
  Serial.print("Date is: 20");   Serial.print(datetime.Year);
  Serial.print(":");  Serial.print(datetime.Month);
  Serial.print(":");    Serial.println(datetime.Day);
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

void ones_displayNumber(Adafruit_NeoPixel& leds, int digitToDisplay, int offsetBy, uint32_t colourToUse){
    switch (digitToDisplay){
    case 0:
    ones_digitZero(leds, offsetBy, colourToUse);
      break;
    case 1:
    ones_digitOne(leds, offsetBy, colourToUse);
      break;
    case 2:
    ones_digitTwo(leds, offsetBy, colourToUse);
      break;
    case 3:
    ones_digitThree(leds, offsetBy, colourToUse);
      break;
    case 4:
    ones_digitFour(leds, offsetBy, colourToUse);
      break;
    case 5:
    ones_digitFive(leds, offsetBy, colourToUse);
      break;
    case 6:
    ones_digitSix(leds, offsetBy, colourToUse);
      break;
    case 7:
    ones_digitSeven(leds, offsetBy, colourToUse);
      break;
    case 8:
    ones_digitEight(leds, offsetBy, colourToUse);
      break;
    case 9:
    ones_digitNine(leds, offsetBy, colourToUse);
      break;
    default:
     break;
  }
}

void tens_displayNumber(Adafruit_NeoPixel& leds, int digitToDisplay, int offsetBy, uint32_t colourToUse){
    switch (digitToDisplay){
    case 0:
    tens_digitZero(leds, offsetBy, colourToUse);
      break;
    case 1:
    tens_digitOne(leds, offsetBy, colourToUse);
      break;
    case 2:
    tens_digitTwo(leds, offsetBy, colourToUse);
      break;
    case 3:
    tens_digitThree(leds, offsetBy, colourToUse);
      break;
    case 4:
    tens_digitFour(leds, offsetBy, colourToUse);
      break;
    case 5:
    tens_digitFive(leds, offsetBy, colourToUse);
      break;
    case 6:
    tens_digitSix(leds, offsetBy, colourToUse);
      break;
    case 7:
    tens_digitSeven(leds, offsetBy, colourToUse);
      break;
    case 8:
    tens_digitEight(leds, offsetBy, colourToUse);
      break;
    case 9:
    tens_digitNine(leds, offsetBy, colourToUse);
      break;
    default:
     break;
  }
}
