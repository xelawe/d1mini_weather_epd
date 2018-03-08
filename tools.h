
float payload_to_float( byte* payload, unsigned int length) {
  float lv_float = 0;
  float lv_mul = 1;
  float lv_fact = 0;
  
  for (int i = 0; i < length; i++) {
    DebugPrint((char)payload[i]);
  }

  for (int i = 0; i < length; i++) {
    if ( payload[i] != 46 ) { // "."
      lv_float = (lv_float * lv_mul) + payload[i] - 48;
      lv_mul = lv_mul * 10;
      lv_fact = lv_fact * 10;
    } else {
      lv_fact = 1;
    }
  }

  if (lv_fact > 0) {
    lv_float = lv_float / lv_fact;
  }

  DebugPrintln("");
  DebugPrintln(lv_float);
  
  return lv_float;
}

// utility function for digital clock display: prints leading 0
String twoDigits(int digits) {
  if (digits < 10) {
    String i = '0' + String(digits);
    return i;
  }
  else {
    return String(digits);
  }
}
