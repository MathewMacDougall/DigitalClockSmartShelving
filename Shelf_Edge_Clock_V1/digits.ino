/*
 * The digits for the "ones" column of the hour and minute numbers are wired
 * differently than the "tens" column, so they have separate functions.
 * The "ones" start going down, while the "tens" start going up.
 */

void ones_digitZero(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 3 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 3 * LEDS_PER_SEGMENT);
}

void ones_digitOne(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
}

void ones_digitTwo(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (1 * LEDS_PER_SEGMENT + offset), 5 * LEDS_PER_SEGMENT);
}

void ones_digitThree(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 3 * LEDS_PER_SEGMENT);
}

void ones_digitFour(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (6 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
}

void ones_digitFive(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (5 * LEDS_PER_SEGMENT + offset), 2 * LEDS_PER_SEGMENT);
}

void ones_digitSix(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 4 * LEDS_PER_SEGMENT);
  leds.fill(colour, (5 * LEDS_PER_SEGMENT + offset), 2 * LEDS_PER_SEGMENT);
}

void ones_digitSeven(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 2 * LEDS_PER_SEGMENT);
}

void ones_digitEight(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 7 * LEDS_PER_SEGMENT);
}

void ones_digitNine(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 4 * LEDS_PER_SEGMENT);
}

//=============================================================================//

void tens_digitZero(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 3 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 3 * LEDS_PER_SEGMENT);
}

void tens_digitOne(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
}

void tens_digitTwo(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (5 * LEDS_PER_SEGMENT + offset), 2 * LEDS_PER_SEGMENT);
}

void tens_digitThree(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (3 * LEDS_PER_SEGMENT + offset), 3 * LEDS_PER_SEGMENT);
}

void tens_digitFour(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 1 * LEDS_PER_SEGMENT);
  leds.fill(colour, (2 * LEDS_PER_SEGMENT + offset), 3 * LEDS_PER_SEGMENT);
}

void tens_digitFive(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (1 * LEDS_PER_SEGMENT + offset), 5 * LEDS_PER_SEGMENT);
}

void tens_digitSix(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (1 * LEDS_PER_SEGMENT + offset), 6 * LEDS_PER_SEGMENT);
}

void tens_digitSeven(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 2 * LEDS_PER_SEGMENT);
  leds.fill(colour, (4 * LEDS_PER_SEGMENT + offset), 1 * LEDS_PER_SEGMENT);
}

void tens_digitEight(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 7 * LEDS_PER_SEGMENT);
}

void tens_digitNine(Adafruit_NeoPixel& leds, int offset, uint32_t colour) {
  leds.fill(colour, (0 + offset), 5 * LEDS_PER_SEGMENT);
}

//=============================================================================//

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
