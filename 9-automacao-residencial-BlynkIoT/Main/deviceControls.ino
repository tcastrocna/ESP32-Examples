void deviceControl(int opc) {
  switch (opc) {
    //Iluminação 1
    case 1:
      if (POWER_STATE_1 == LOW) {
        digitalWrite(RELAY_PIN_1, LOW);
        POWER_STATE_1 = 1;
        pref.putBool("Relay1", POWER_STATE_1);
        Blynk.virtualWrite(VIRTUAL_PIN_1, POWER_STATE_1);
        Serial.println("Iluminação 1 Ligado");
      } else {
        digitalWrite(RELAY_PIN_1, HIGH);
        POWER_STATE_1 = 0;
        pref.putBool("Relay1", POWER_STATE_1);
        Blynk.virtualWrite(VIRTUAL_PIN_1, POWER_STATE_1);
        Serial.println("Iluminação 1 Desligado");
      }
      break;
    //Iluminação 2
    case 2:
      if (POWER_STATE_2 == LOW) {
        digitalWrite(RELAY_PIN_2, LOW);
        POWER_STATE_2 = 1;
        pref.putBool("Relay2", POWER_STATE_2);
        Blynk.virtualWrite(VIRTUAL_PIN_2, POWER_STATE_2);
        Serial.println("Iluminação 2 Ligado");
      } else {
        digitalWrite(RELAY_PIN_2, HIGH);
        POWER_STATE_2 = 0;
        pref.putBool("Relay2", POWER_STATE_2);
        Blynk.virtualWrite(VIRTUAL_PIN_2, POWER_STATE_2);
        Serial.println("Iluminação 2 Desligado");
      }
      break;

    //Ventilador
    case 3:
      if (currSpeed == 0) {
        digitalWrite(RELAY_PIN_3, LOW);
        POWER_STATE_3 = 1;
        currSpeed = 1;
        pref.putInt("Fan", currSpeed);
        Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);

      } else if ((currSpeed == 1 || (currSpeed == 2))) {
        digitalWrite(RELAY_PIN_3, HIGH);
        digitalWrite(RELAY_PIN_4, HIGH);
        POWER_STATE_3 = 0;
        POWER_STATE_4 = 0;
        currSpeed = 0;
        pref.putInt("Fan", currSpeed);
        Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
      }
      break;

    //Velocidade 0 ventilador
    case 4:
      digitalWrite(RELAY_PIN_3, HIGH);
      digitalWrite(RELAY_PIN_4, HIGH);
      POWER_STATE_3 = 0;
      POWER_STATE_4 = 0;
      currSpeed = 0;
      pref.putInt("Fan", currSpeed);
      Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
      break;

    //Velocidade 1 Ventilador
    case 5:
      delay(100);
      digitalWrite(RELAY_PIN_3, LOW);
      delay(100);
      digitalWrite(RELAY_PIN_4, HIGH);
      POWER_STATE_3 = 1;
      POWER_STATE_4 = 0;
      currSpeed = 1;
      pref.putInt("Fan", currSpeed);
      Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
      break;
    //Velocidade 2 ventilador
    case 6:
      delay(100);
      digitalWrite(RELAY_PIN_3, HIGH);
      delay(100);
      digitalWrite(RELAY_PIN_4, LOW);
      POWER_STATE_3 = 0;
      POWER_STATE_4 = 1;
      currSpeed = 2;
      pref.putInt("Fan", currSpeed);
      Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
      break;
    default: break;
  }
}

// Função para desligar todos os dispositivos simultaneamente
void offAllDevices() {
  digitalWrite(RELAY_PIN_1, HIGH);
  POWER_STATE_1 = 0;
  Blynk.virtualWrite(VIRTUAL_PIN_1, POWER_STATE_1);
  delay(100);
  digitalWrite(RELAY_PIN_2, HIGH);
  POWER_STATE_2 = 0;
  Blynk.virtualWrite(VIRTUAL_PIN_2, POWER_STATE_2);
  delay(100);
  digitalWrite(RELAY_PIN_3, HIGH);
  digitalWrite(RELAY_PIN_4, HIGH);
  POWER_STATE_3 = 0;
  POWER_STATE_4 = 0;
  currSpeed = 0;
  pref.putInt("Fan", currSpeed);
  Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
  delay(100);
  Serial.println("Todos os dispositivos Desligado");
  delay(100);
  Blynk.virtualWrite(VIRTUAL_PIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VIRTUAL_PIN_HUMIDITY, humidity1);
}