/* Configurações de rede WiFi, Reseta as configurações caso necessario alterar as credenciais de rede*/
WiFiManager wm;
WiFiManagerParameter custom_field;
int customFieldLength = 40;
bool wm_nonblocking = false, res;



String getParam(String name) {
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

// Função principal de configuração da rede.
void wifiConfigESP() {
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);
  delay(3000);
  Serial.println("\n Iniciando");

  if (wm_nonblocking) wm.setConfigPortalBlocking(false);

  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str);
  std::vector<const char*> menu = { "wifi", "info", "param", "sep", "restart", "exit" };

  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(200);
  res = wm.autoConnect("ESP32_CONFIG", "12345678");

  //Entrou no modo de configuração, faz o led alternar mais rápido
  ticker.attach(0.2, tick);

  if (!res) {
    Serial.println("Falha ao conectar ou o tempo limite esgotou :(");
  } else {
    Serial.println("Conectado...yeey :)");
    ticker.detach();
    digitalWrite(WIFI_LED_PIN, HIGH);
  }
}

// Função de resetar a rede, ao precionar o botão ou acionar tecla do controle remoto o acesso point é ativado
void resetNetwork() {
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    Serial.println("Botão Precionado");
    delay(200);
    if (digitalRead(RESET_BUTTON_PIN) == LOW) {
      Serial.println("Botão retido");
      Serial.println("Apagando Configurações, restaurando ...");
      wm.resetSettings();
      ESP.restart();
    }

  } else if (IrReceiver.decode()) {
    switch (IrReceiver.decodedIRData.decodedRawData) {
      case IR_RESET_WIFI:
        Serial.println("Apagando Configurações de rede, restaurando ...");
        wm.resetSettings();
        ESP.restart();
        break;
      default: break;
    }
  }
}