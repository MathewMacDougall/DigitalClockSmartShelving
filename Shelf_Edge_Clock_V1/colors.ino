#define NUM_COLOR_PAIRS 8

uint32_t white() {
  return stripDownlighter.Color(255, 255, 255);
}

uint32_t purple(int light_sensor_value) {
  return stripDownlighter.Color(
    constrain(map(light_sensor_value, DARK_COLOR_BRIGHTNESS_THRESHOLD, LIGHT_COLOR_BRIGHTNESS_THRESHOLD, 110, 140), 110, 140),
    0,
    255
  );
}

uint32_t pink(int light_sensor_value) {
  return stripDownlighter.Color(255, 0, 110);
}

uint32_t yellow(int light_sensor_value) {
  return stripDownlighter.Color(
    255,
    constrain(map(light_sensor_value, DARK_COLOR_BRIGHTNESS_THRESHOLD, LIGHT_COLOR_BRIGHTNESS_THRESHOLD, 242, 190), 190, 242),
    0
  );
}

uint32_t red() {
  return stripDownlighter.Color(255, 0, 0);
}

uint32_t green() {
  return stripDownlighter.Color(0, 255, 10);
}

uint32_t blue(int light_sensor_value) {
  return stripDownlighter.Color(
    0,
    constrain(map(light_sensor_value, DARK_COLOR_BRIGHTNESS_THRESHOLD, LIGHT_COLOR_BRIGHTNESS_THRESHOLD, 123, 60), 60, 123),
    255
  );
}

uint32_t orange(int light_sensor_value) {
  return stripDownlighter.Color(
    255,
    constrain(map(light_sensor_value, DARK_COLOR_BRIGHTNESS_THRESHOLD, LIGHT_COLOR_BRIGHTNESS_THRESHOLD, 123, 50), 50, 123),
    0
  );
}

void getColorPair(int index, int light_sensor_value, uint32_t& hour_color, uint32_t& minute_color) {
  uint32_t hour_colors[] = {
    // 0:  Blue and White
    blue(light_sensor_value),
    // 1:  Blue and Orange
    blue(light_sensor_value),
    // 2:  Purple and Yellow
    purple(light_sensor_value),
    // 3:  Purple and White
    purple(light_sensor_value),
    // 4:  Red and White
    red(),
    // 5:  Orange and White
    orange(light_sensor_value),
    // 6: Green and White
    green(),
    // 7: Pink and Yellow (Pink Lemonade!)
    pink(light_sensor_value)
  };
  uint32_t minute_colors[] = {
    // 0:  Blue and White
    white(),
    // 1:  Blue and Orange
    orange(light_sensor_value),
    // 2:  Purple and Yellow
    yellow(light_sensor_value),
    // 3:  Purple and White
    white(),
    // 4:  Red and White
    white(),
    // 5:  Orange and White
    white(),
    // 6: Green and White
    white(),
    // 7: Pink and Yellow (Pink Lemonade!)
    yellow(light_sensor_value)
  };
  
  index = index % NUM_COLOR_PAIRS;
  hour_color = hour_colors[index];
  minute_color = minute_colors[index];
}

void getRandomColorPair(int light_sensor_value, uint32_t& hour_color, uint32_t& minute_color) {
  int index = random(0, NUM_COLOR_PAIRS);
  getColorPair(index, light_sensor_value, hour_color, minute_color);
  if(random(0, 2) == 1) {
    uint32_t temp;
    temp = hour_color;
    hour_color = minute_color;
    minute_color = temp;
  }
}

void getDateAwareRandomColorPair(int month, int day, int light_sensor_value, uint32_t& hour_color, uint32_t& minute_color) {
  hour_color = blue(light_sensor_value);
  minute_color = orange(light_sensor_value);
  return;
  if(month == 12 && (random(0, 3) == 0 || day == 25)) {
    // Red and green 33% of the time in December, and on Christmas
    hour_color = green();
    minute_color = red();
    return;
  }
  else if (month == 2 && day == 14) {
    // Pink for Valentine's Day
    hour_color = stripDownlighter.Color(255, 140, 215);
    minute_color = stripDownlighter.Color(255, 0, 175);
    return;
  }
  else if (month == 3 && day == 17) {
    // Green for Saint Patrick's Day
    hour_color = white();
    minute_color = green();
    return;
  }
  else if (month == 7 && day == 1) {
    // Red and White for Canada Day
    hour_color = white();
    minute_color = red();
    return;
  }
  else if (month == 7 && day == 4) {
    // Red and Blue for July 4
    hour_color = stripDownlighter.Color(0, 0, 255);
    minute_color = red();
    return;
  }
  else if (month == 10 && day == 31) {
    // Orange and White for Halloween
    hour_color = white();
    minute_color = orange(light_sensor_value);
    return;
  }
  else if (month == 11 && day == 11) {
    // Red and White for Rememberance Day
    hour_color = white();
    minute_color = orange(light_sensor_value);
    return;
  }

  getRandomColorPair(light_sensor_value, hour_color, minute_color);
}
