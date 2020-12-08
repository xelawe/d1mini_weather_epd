

void  ICACHE_RAM_ATTR pcf_irq()
{
  pcf_flag = true;
}


void init_pcf8574() {
  pcf20.begin();

  pinMode(PCFIRQPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PCFIRQPIN), pcf_irq, FALLING);
}

void check_pcf8574() {

  static bool state;
  unsigned int currentMillis = millis();

  if (pcf_flag == true) {
    Serial.println(" PCF!!");

    boolean pcf_btn_state1 = pcf20.readButton(PcfButton1);
    pub_btn1(pcf_btn_state1);
    if (pcf_btn_state1 == false) {
      // Button Pressed
      Serial.println(" PCF Btn !");
    } else {
      // Button released
      //Serial.println(" PCF Btn false");
    }

    uint32_t now = millis();
    int x = pcf20.read8();
    Serial.print("READ:\t");
    Serial.print('\t');
    Serial.print(now);
    Serial.print('\t');
    Serial.println(x, HEX);

    pcf_flag = false;
  }




}


