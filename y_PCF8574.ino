

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

    boolean pcf_btn_state1 = pcf20.readButton(PcfButton1);
    boolean pcf_pir_state1 = pcf20.readButton(PcfPIR1);

    pcf_flag = false;

    Serial.println(" PCF!!");

    if (PcfButton1state != pcf_btn_state1) {
      pub_btn1(pcf_btn_state1);
      PcfButton1state = pcf_btn_state1;
    }

    if (pcf_btn_state1 == false) {
      // Button Pressed
      Serial.println(" PCF Btn !");
    } else {
      // Button released
      //Serial.println(" PCF Btn false");
    }


    if (PcfPIR1state != pcf_pir_state1) {
      pub_pir1(pcf_pir_state1);
      PcfPIR1state = pcf_pir_state1;
    }

    if (pcf_pir_state1 == false) {
      // PIR inactive
    } else {
      // PIR active
      Serial.println(" PCF PIR !");
    }

    uint32_t now = millis();
    int x = pcf20.read8();
    Serial.print("READ:\t");
    Serial.print('\t');
    Serial.print(now);
    Serial.print('\t');
    Serial.println(x, HEX);


  }




}


