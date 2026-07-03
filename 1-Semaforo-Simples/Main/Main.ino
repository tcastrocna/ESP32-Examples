#include <Arduino.h>

// Define as portas dos LEDs no ESP32
// Semáforo 1 
const int ledVermelho1 = 12; // LED vermelho (Pare)
const int ledAmarelo1 = 14; // LED Amarelo (Atenção)
const int ledVerde1 = 27; // LED Verde (Siga)

// Semáforo 2 
const int ledVermelho2 = 26; //LED vermelho (Pare)
const int ledAmarelo2 = 25; // LED Amarelo (Atenção)
const int ledVerde2 = 33; // LED Verde (Siga)


// Função de configuração principal
void setup(){
  // Configura as portas como saídas
  //Semáforo1
  pinMode(ledVermelho1, OUTPUT);
  pinMode(ledAmarelo1, OUTPUT);
  pinMode(ledVerde1, OUTPUT);

  //Semáforo2
  pinMode(ledVermelho2, OUTPUT);
  pinMode(ledAmarelo2, OUTPUT);
  pinMode(ledVerde2, OUTPUT);
}

// Função do loop , onde a aplicação funciona
void loop(){
  // Liga led vermelho do semáforo 1
  digitalWrite(ledVermelho1, HIGH);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVerde1, LOW);

  delay(1000); // Espera 1 segundo

  //Liga led Verde semáforo 2
  digitalWrite(ledVermelho2, LOW);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVerde2, HIGH);

  delay(5000); // Espera 5 segundos

  // Liga led amarelo semáforo 2
  digitalWrite(ledVermelho2, LOW);
  digitalWrite(ledAmarelo2, HIGH);
  digitalWrite(ledVerde2, LOW);

  delay(2000); // Espera 2 segundos

  // Liga Led vermelho semáforo 2
  digitalWrite(ledVermelho2, HIGH);
  digitalWrite(ledAmarelo2, LOW);
  digitalWrite(ledVerde2, LOW);

  delay(1000); // Espera 1 segundo

  //Liga led Verde semáforo 1
  digitalWrite(ledVermelho1, LOW);
  digitalWrite(ledAmarelo1, LOW);
  digitalWrite(ledVerde1, HIGH);

  delay(5000); //Espera 5 segundos

  // Liga led amarelo semáforo 1
  digitalWrite(ledVermelho1, LOW);
  digitalWrite(ledAmarelo1, HIGH);
  digitalWrite(ledVerde1, LOW);

  delay(2000); // Espera 2 segundos
}