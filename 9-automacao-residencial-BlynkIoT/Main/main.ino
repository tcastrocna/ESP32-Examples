/* Preencha seu ID de modelo (somente se estiver usando Blynk.Cloud) */
#define BLYNK_TEMPLATE_ID "xxxxxxxx"
#define BLYNK_DEVICE_NAME "xxxxxxxxxxxxx"
#define BLYNK_AUTH_TOKEN "i-xxxxxxxxxxxxxxxxxxxxxxxxxxx"

char auth[] = BLYNK_AUTH_TOKEN;

#define BLYNK_PRINT Serial
#define BAUD_RATE 115200

// *** Bibliotecas necessárias para compilação do código ***
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiManager.h>
#include <BlynkSimpleEsp32.h>
#include <Preferences.h>
#include <DHT.h>
#include <IRremote.hpp>
#include <Ticker.h>

Preferences pref;

// *** Atualiza o código HEX dos botões IR Remote 0x<HEX CODE> ***
#define IR_BUTTON_1 0xEA15BD00  //Tecla 01 - Iluminação 1
#define IR_BUTTON_2 0xE916BD00  //Tecla 02 - Iluminação 2
#define IR_BUTTON_3 0xE817BD00  //Tecla 03 - Ventilador
#define IR_FAN_UP 0xF30CBD00    //tecla seta cima (velocidade +)
#define IR_FAN_DOWN 0xEF10BD00  //tecla seta cima (velocidade -)

#define IR_All_OFF 0xFE01BD00     //Tecla Off - Desligar Tudo
#define IR_RESET_WIFI 0xF708BD00  //Tecla Input - Reseta rede WiFi


// *** Altera os pinos virtuais de acordo com o estado da nurvem ***
#define VIRTUAL_PIN_1 V1    //Iluminação 1
#define VIRTUAL_PIN_2 V2    //Iluminação 2
#define VIRTUAL_PIN_FAN V3  //Ventilador

#define VIRTUAL_PIN_BUTTON_C V4     //Desliga Todos
#define VIRTUAL_PIN_TEMPERATURE V5  //Temperatura do ambiente
#define VIRTUAL_PIN_HUMIDITY V6     //Umidade do Ar
#define VIRTUAL_PIN_LUMINOSITY V7   //Luminosidade do Ambiente

// *** Define o GPIO conectado com Relays, switches e sensores ***
#define RELAY_PIN_1 18  //Iluminação 1
#define RELAY_PIN_2 19  //Iluminação 2
#define RELAY_PIN_3 23  //Velocidade 1 Ventilador
#define RELAY_PIN_4 05  //Velocidade 2 Ventilador

#define BUTTON_PIN_1 13  //Botão Touch Iluminação 1
#define BUTTON_PIN_2 12  //Botão Touch Iluminação 2
#define BUTTON_PIN_3 14  //Botão Touch Ventilador (Liga/Desliga e Controle de velocidade)

#define WIFI_LED_PIN 34      //Led indicador de rede WiFi
#define RESET_BUTTON_PIN 35  //Reseta a rede Wifi

#define IR_RECEIVER 25  //Pino 35 (pino do receptor IR)
#define DHT_SENSOR 27   //Pino 26 conectado com DHT
#define LDR_SENSOR 26   //Pino do sensor LDR (Luminosidade do ambiente)

// *** Descomente o modelo e tipo do sensor de temperatura que você esteja usando! ***
#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301

DHT dht(DHT_SENSOR, DHTTYPE);

int currSpeed = 0;

// *** Estado de energia dos relés, devem iniciar desligados devido ao módulo !POWER_STATE_1 ***
bool POWER_STATE_1 = LOW;  // Estado de energia dos relé 1, deve iniciar LOW (desligado NO)
bool POWER_STATE_2 = LOW;  // Estado de energia dos relé 2, deve iniciar LOW (desligado NO)
bool POWER_STATE_3 = LOW;  // Estado de energia dos relé 3, deve iniciar LOW (desligado NO)
bool POWER_STATE_4 = LOW;  // Estado de energia dos relé 4, deve iniciar LOW (desligado NO)
bool POWER_STATE_5 = LOW;  // Estado de energia do Led Wifi, deve iniciar LOW

float temperature1 = 0, humidity1 = 0, luminosity = 0;
int Valor_Ldr = 0;

int wifiFlag = 0;
bool fetch_blynk_state = true;  // true ou false
BlynkTimer timer;

Ticker ticker;
void tick() {
  // Alterna estado do led
  int state = digitalRead(WIFI_LED_PIN);  // Obtém o estado atual do pino led wifi
  digitalWrite(WIFI_LED_PIN, !state);     // Define o pino para o estado oposto
}

// *** Função para controle manual da iluminação e ventilador através de botões touch ***
void manualControl() {
  int capacitanciaMaxima = 20;  //valor que nos da a certeza de toque
  int mediaB1 = 0, mediaB2 = 0, mediaB3 = 0;

  //Serial.print("Valor Botão 1"); Serial.println(touchRead(BUTTON_PIN_1));
  //Serial.print("Valor Botão 2"); Serial.println(touchRead(BUTTON_PIN_2));
  //Serial.print("Valor Botão 3"); Serial.println(touchRead(BUTTON_PIN_3));

  //Faz 100 leituras de cada sensor touch e calcula a média do valor lido
  for (int i = 0; i < 100; i++) {
    mediaB1 += touchRead(BUTTON_PIN_1);
    mediaB2 += touchRead(BUTTON_PIN_2);
    mediaB3 += touchRead(BUTTON_PIN_3);
  }

  mediaB1 = mediaB1 / 100;
  mediaB2 = mediaB2 / 100;
  mediaB3 = mediaB3 / 100;

  //Iluminação 1
  if (mediaB1 < capacitanciaMaxima) {
    deviceControl(1);
    delay(100);
  }
  //Iluminação 2
  else if (mediaB2 < capacitanciaMaxima) {
    deviceControl(2);
    delay(100);
  }
  //Ventilador
  else if (mediaB3 < capacitanciaMaxima) {
    deviceControl(3);
    delay(100);
  }

  delay(100);
}

// *** Função para controle através do controle remoto infravermelho ***
void remoteControl() {
  if (IrReceiver.decode()) {
    switch (IrReceiver.decodedIRData.decodedRawData) {
      //Iluminação 1
      case IR_BUTTON_1:
        deviceControl(1);
        delay(100);
        break;
      //Iluminação 2
      case IR_BUTTON_2:
        deviceControl(2);
        delay(100);
        break;
      //Ventilador
      case IR_BUTTON_3:
        deviceControl(3);
        break;
      //Ventilador Velocidade +
      case IR_FAN_UP:
        if (currSpeed < 4) {
          currSpeed = currSpeed + 1;
          if (currSpeed == 1) {
            deviceControl(5);
          } else if (currSpeed == 2) {
            deviceControl(6);
          }
        }
        delay(100);
        break;
      //Ventilador Velocidade -
      case IR_FAN_DOWN:
        if (currSpeed > 0) {
          currSpeed = currSpeed - 1;
          if (currSpeed == 0) {
            deviceControl(4);
          } else if (currSpeed == 1) {
            deviceControl(5);
          }
        }
        delay(100);
        break;
      //Desligar Todos
      case IR_All_OFF: offAllDevices(); break;  //Desligar Todos
      default: break;
    }
    Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
    IrReceiver.resume();
  }
}

// *** Controle dos dispositivos através do app Blynk IoT ***
//Iluminação 1
BLYNK_WRITE(VIRTUAL_PIN_1) {
  deviceControl(1);
}
//Iluminação 2
BLYNK_WRITE(VIRTUAL_PIN_2) {
  deviceControl(2);
}
//Ventilador
BLYNK_WRITE(VIRTUAL_PIN_FAN) {
  if (param.asInt() == 0) {
    deviceControl(4);
  } else if (param.asInt() == 1) {
    deviceControl(5);
  } else if (param.asInt() == 2) {
    deviceControl(6);
  }
}
//Desligar Todos
BLYNK_WRITE(VIRTUAL_PIN_BUTTON_C) {
  offAllDevices();
}

// *** Função para leitura e envio de temperatura e umidade ***
void dhtSensor() {
  temperature1 = dht.readTemperature();
  humidity1 = dht.readHumidity();

  if (isnan(temperature1) || isnan(humidity1)) {
    Serial.println("Falha ao ler do sensor DHT!");
    delay(100);
    return;
  }
  //Você pode enviar qualquer valor a qualquer momento.
  //Por favor, não envie mais que 10 valores por segundo.
  Blynk.virtualWrite(VIRTUAL_PIN_TEMPERATURE, temperature1);
  Blynk.virtualWrite(VIRTUAL_PIN_HUMIDITY, humidity1);
}

// *** Função para leitura do sensor de luminosidade LDR ***
void ldrSensor() {
  Valor_Ldr = analogRead(LDR_SENSOR);
  luminosity = map(Valor_Ldr, 0, 4095, 0, 100);
  Blynk.virtualWrite(VIRTUAL_PIN_LUMINOSITY, luminosity);
  Serial.print(luminosity);
  Serial.println("%");
  delay(100);
}

// *** Chamado a cada 3 segundos pelo SimpleTimer ***
void checkBlynkStatus() {
  bool isconnected = Blynk.connected();
  if (isconnected == false) {
    wifiFlag = 1;
    Serial.println("Blynk Não Conectado :(");
  }
  if (isconnected == true) {
    wifiFlag = 0;
    Serial.println("Blynk Conectado :)");
  }
}

// *** Atualiza o estado mais recente para o servidor ***
BLYNK_CONNECTED() {
  Blynk.virtualWrite(VIRTUAL_PIN_1, POWER_STATE_1);
  Blynk.virtualWrite(VIRTUAL_PIN_2, POWER_STATE_2);
  Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);

  Blynk.syncVirtual(VIRTUAL_PIN_TEMPERATURE);
  Blynk.syncVirtual(VIRTUAL_PIN_HUMIDITY);
}

void getRelayState() {
  //Serial.println("reading data from NVS");
  POWER_STATE_1 = pref.getBool("Relay1", 0);
  digitalWrite(RELAY_PIN_1, !POWER_STATE_1);
  Blynk.virtualWrite(VIRTUAL_PIN_1, POWER_STATE_1);
  delay(200);
  POWER_STATE_2 = pref.getBool("Relay2", 0);
  digitalWrite(RELAY_PIN_2, !POWER_STATE_2);
  Blynk.virtualWrite(VIRTUAL_PIN_2, POWER_STATE_2);
  delay(200);
  currSpeed = pref.getInt("Fan", 0);
  if (currSpeed == 0) {
    deviceControl(4);
  }else if(currSpeed == 1){
    deviceControl(5);
  }else if(currSpeed == 2){
    deviceControl(6);
  }
  Blynk.virtualWrite(VIRTUAL_PIN_FAN, currSpeed);
  delay(200);
}

void setup() {
  Serial.begin(BAUD_RATE);
  //Open namespace in read-write mode
  pref.begin("Relay_State", false);

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(RELAY_PIN_4, OUTPUT);

  pinMode(WIFI_LED_PIN, OUTPUT);
  pinMode(LDR_SENSOR, INPUT);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  //Durante a partida todos os relés devem iniciar DESLIGADOS
  digitalWrite(RELAY_PIN_1, !POWER_STATE_1);
  digitalWrite(RELAY_PIN_2, !POWER_STATE_2);
  digitalWrite(RELAY_PIN_3, !POWER_STATE_3);
  digitalWrite(RELAY_PIN_4, !POWER_STATE_4);
  digitalWrite(WIFI_LED_PIN, POWER_STATE_5);

  IrReceiver.begin(IR_RECEIVER, ENABLE_LED_FEEDBACK);  // Ativando o sensor IR
  dht.begin();                                         // Ativando o sensor DHT

  ticker.attach(0.6, tick);  // Inicia o ticker com 0.5 porque iniciamos no modo AP e tentamos conectar

  wifiConfigESP();
  Blynk.config(auth);
  timer.setInterval(2000L, checkBlynkStatus);  // Verifica se o servidor Blynk está conectado a cada 2 segundos
  timer.setInterval(1000L, dhtSensor);         // Enviando dados do sensor DHT para Blynk Cloud a cada 1 segundo
  timer.setInterval(1000L, ldrSensor);         // Enviando dados do sensor LDR para Blynk Cloud a cada 1 segundo
  delay(1000);

  getRelayState();  //fetch data from NVS Flash Memory
  //delay(1000);
}

void loop() {
  resetNetwork();
  manualControl();
  remoteControl();
  ldrSensor();
  Blynk.run();
  timer.run();
}