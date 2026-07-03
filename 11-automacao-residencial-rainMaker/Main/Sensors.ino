/*** Funções para leitura e envio dos sensores de temperatura, umidade e luminosidade ***/

/* Função para leitura de temperatura e umidade */
void sendSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t)) {
    Serial.println("Falha ao ler do sensor DHT!");
    delay(100);
    return;
  }
  temperature.updateAndReportParam("Temperature", t);
  humidity.updateAndReportParam("Temperature", h);
}

/* Função para leitura do sensor de luminosidade LDR */
void ldrSensor() {
  float Luminosidade;
  int Valor_Ldr = 0;
  Valor_Ldr = analogRead(LDR_SENSOR);
  Luminosidade = map(Valor_Ldr, 0, 4095, 0, 100);
  ldr.updateAndReportParam("Temperature", Luminosidade);
  Serial.print(Luminosidade);
  Serial.println("%");
  delay(100);
}