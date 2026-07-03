#include <RMaker.h>
#include <WiFi.h>
#include <WiFiProv.h>
#include <SimpleTimer.h>
#include <IRremote.hpp>
#include <DHT.h>

#include "SettingBoard.h"

// Define a estrutura dos dispositivos como interruptor, lâmpada, ventilador, sensor de temperatura.
static Switch my_switch1(DEVICE_NAME_1, &RELAY_PIN_1);
static Switch my_switch2(DEVICE_NAME_2, &RELAY_PIN_2);
static Switch my_switch3(DEVICE_NAME_3, &RELAY_PIN_3);
static Switch my_switch4(DEVICE_NAME_4, &RELAY_PIN_4);
static TemperatureSensor temperature("Temperatura");
static TemperatureSensor humidity("Umidade");
static TemperatureSensor ldr("Luminosidade");

DHT dht(DHT_SENSOR, DHTTYPE);
bool WifiConnected = 0;

IRrecv irrecv(IR_RECEIVER);
decode_results results;

void sysProvEvent(arduino_event_t *sys_event) {
  switch (sys_event->event_id) {
    case ARDUINO_EVENT_PROV_START:
#if CONFIG_IDF_TARGET_ESP32
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on BLE\n", service_name, pop);
      printQR(service_name, pop, "ble");
#else
      Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
      printQR(service_name, pop, "softap");
#endif
      break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      Serial.printf("\nConnected to Wi-Fi!\n");
      digitalWrite(WIFI_LED_PIN, true);
      WifiConnected = 1;
      break;
  }
}

/* Função para controle e alternancia através do app no smartphone, Assistente.*/
void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx) {
  const char *device_name = device->getDeviceName();
  const char *param_name = param->getParamName();

  if (strcmp(device_name, DEVICE_NAME_1) == 0) {

    Serial.printf("Lightbulb = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      POWER_STATE_1 = val.val.b;
      (POWER_STATE_1 == false) ? digitalWrite(RELAY_PIN_1, HIGH) : digitalWrite(RELAY_PIN_1, LOW);
      param->updateAndReport(val);
    }

  } else if (strcmp(device_name, DEVICE_NAME_2) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      POWER_STATE_2 = val.val.b;
      (POWER_STATE_2 == false) ? digitalWrite(RELAY_PIN_2, HIGH) : digitalWrite(RELAY_PIN_2, LOW);
      param->updateAndReport(val);
    }

  } else if (strcmp(device_name, DEVICE_NAME_3) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      POWER_STATE_3 = val.val.b;
      (POWER_STATE_3 == false) ? digitalWrite(RELAY_PIN_3, HIGH) : digitalWrite(RELAY_PIN_3, LOW);
      param->updateAndReport(val);
    }

  } else if (strcmp(device_name, DEVICE_NAME_4) == 0) {

    Serial.printf("Switch value = %s\n", val.val.b ? "true" : "false");

    if (strcmp(param_name, "Power") == 0) {
      Serial.printf("Received value = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      POWER_STATE_4 = val.val.b;
      (POWER_STATE_4 == false) ? digitalWrite(RELAY_PIN_4, HIGH) : digitalWrite(RELAY_PIN_4, LOW);
      param->updateAndReport(val);
    }
  }
}

/* Função de configuração principal */
void setup() {
  uint32_t chipId = 0;
  Serial.begin(BAUD_RATE);

  dht.begin();
  irrecv.enableIRIn(); // Start the receiver

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(WIFI_LED_PIN, OUTPUT);

  pinMode(LDR_SENSOR, INPUT);

  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  //Durante a partida todos os relés devem DESLIGAR
  digitalWrite(RELAY_PIN_1, !POWER_STATE_1);
  digitalWrite(RELAY_PIN_2, !POWER_STATE_2);
  digitalWrite(RELAY_PIN_3, !POWER_STATE_3);
  digitalWrite(RELAY_PIN_4, !POWER_STATE_4);
  digitalWrite(WIFI_LED_PIN, LOW);

  Node my_node;
  my_node = RMaker.initNode(NAME_DEVICE);

  //Dispositivo de comutação padrão
  my_switch1.addCb(write_callback);
  my_switch2.addCb(write_callback);
  my_switch3.addCb(write_callback);
  my_switch4.addCb(write_callback);

  //Adiciona dispositivo switch ao nó
  my_node.addDevice(my_switch1);
  my_node.addDevice(my_switch2);
  my_node.addDevice(my_switch3);
  my_node.addDevice(my_switch4);
  my_node.addDevice(temperature);
  my_node.addDevice(humidity);
  my_node.addDevice(ldr);

  //Isso é opcional
  RMaker.enableOTA(OTA_USING_PARAMS);
  //Se você deseja habilitar o agendamento, defina o fuso horário para sua região usando setTimeZone().
  //A lista de valores disponíveis é fornecida aqui https://rainmaker.espressif.com/docs/time-service.html
  // RMaker.setTimeZone("Ásia/Xangai");
  // Como alternativa, habilite o serviço Timezone e deixe que os aplicativos de telefone definam o fuso horário apropriado
  RMaker.enableTZService();
  RMaker.enableSchedule();

  //Nome do Serviço
  for (int i = 0; i < 17; i = i + 8) {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  Serial.printf("\nID do Chip:  %d Nome do Serviço: %s\n", chipId, service_name);
  Serial.printf("\nIniciando ESP-RainMaker\n");
  RMaker.start();

  WiFi.onEvent(sysProvEvent);
#if CONFIG_IDF_TARGET_ESP32
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_BLE, WIFI_PROV_SCHEME_HANDLER_FREE_BTDM, WIFI_PROV_SECURITY_1, pop, service_name);
#else
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
#endif

  my_switch1.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch2.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch3.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
  my_switch4.updateAndReportParam(ESP_RMAKER_DEF_POWER_NAME, false);
}

/* Função principal de loop */
void loop() {
  //Lê GPIO  (botão externo para resetar o dispositivo)
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {  //Botão pressionado
    Serial.printf("Botão de reset pressionado!\n");
    //Manipulação de debounce de chave
    delay(100);
    int startTime = millis();
    while (digitalRead(RESET_BUTTON_PIN) == LOW) delay(50);
    int endTime = millis();

    if ((endTime - startTime) > 10000) {
      //Se a tecla for pressionada por mais de 10 segundos, reseta tudo
      Serial.printf("Redefinir para as configurações de fábrica.\n");
      RMakerFactoryReset(2);
    } else if ((endTime - startTime) > 3000) {
      Serial.printf("Redefinir Wi-Fi.\n");
      //Se a tecla for pressionada por mais de 3 segundos, mas menos de 10, redefina o Wi-Fi
      RMakerWiFiReset(2);
    }
  }
  delay(200);

  //Verifica o estado da rede WiFi
  if (WifiConnected != 1) {
    Serial.println("WiFi Não Conectado");
    digitalWrite(WIFI_LED_PIN, false);
  } else {
    Serial.println("WiFi Conectado");
    digitalWrite(WIFI_LED_PIN, true);
    Serial.println("Enviando dados do sensor");
    sendSensor();
    ldrSensor();
    delay(500);
  }

  manualControl();
  remoteControl();
}