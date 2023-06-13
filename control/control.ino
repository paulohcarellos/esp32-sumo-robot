// Importação de bibliotecas
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definição dos pinos
#define sckOLEDPin 22 // Pino para tela
#define sdaOLEDPin 21 // Pino para tela
#define pinoLinhaDig1 15 // Pino digital do sensor de linha 1
#define pinoLinhaAnalog1 2 // Pino analógico do sensor de linha 1
#define pinoLinhaDig2 16 // Pino digital do sensor de linha 2
#define pinoLinhaAnalog2 4 // Pino analógico do sensor de linha 2
#define pinoEcho 1 // Pino de eco do sensor de distância
#define pinoTrig 3 // Pino de trigger do sensor de distância
#define pinoMotor1Frente 19 // Pino de acionamento do motor 1
#define pinoMotor1Tras 18 // Pino de acionamento do motor 1
#define pinoMotor2Frente 5 // Pino de acionamento do motor 2
#define pinoMotor2Tras 17 // Pino de acionamento do motor 2
#define ledBuiltIn 2

// Definições para o display OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// Variáveis globais comm valores de leitura de sensores
int valSensorLinhaAnalog1 = 0; // Variável para inserir valor analógico do sensor de linha
bool valSensorLinhaDig1 = 0; // Variável para inserir valor digital do sensor de linha
int valSensorLinhaAnalog2 = 0; // Variável para inserir valor analógico do sensor de linha
bool valSensorLinhaDig2 = 0; // Variável para inserir valor digital do sensor de linha
int distancia = 0; // Variável para a distância medida pelo sensor ultrassônico
int cicloMotor = 0; // Variável utilizada para controlar a velocidade do motor
bool frente = true;

// Variáveis para o PWM
const int freq = 5000; // Frequência em Hz do ciclo do sinal PWM
const int resolution = 8; // Resolução do sinal PWM (8bits varia de 0 a 255)
const int motor1PWMChanFrente = 0; // Canal PWM do motor 1 (frente)
const int motor1PWMChanTras = 1; // Canal PWM do motor 1 (tras)
const int motor2PWMChanFrente = 2; // Canal PWM do motor 2 (frente)
const int motor2PWMChanTras = 3; // Canal PWM do motor 2 (tras)

// Função que faz a leitura da distância com o sensor ultrassônico
int leDistUltras(){
  int tempDuracao = 0;
  int tempDistancia = 0;
  
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  tempDuracao = pulseIn(pinoEcho, HIGH);
  tempDistancia = tempDuracao * 0.034 / 2;
  
  return tempDistancia;
}

void setup(){  
  Serial.begin(9600);

  // Definindo os pinos
  pinMode(pinoLinhaDig1, INPUT);
  pinMode(pinoLinhaAnalog1, INPUT);
  pinMode(pinoLinhaDig2, INPUT);
  pinMode(pinoLinhaAnalog2, INPUT);
  pinMode(pinoEcho, INPUT);
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoMotor1Frente, OUTPUT);
  pinMode(pinoMotor1Tras, OUTPUT);
  pinMode(pinoMotor2Frente, OUTPUT);
  pinMode(pinoMotor2Tras, OUTPUT);
  pinMode(ledBuiltIn, OUTPUT);

  // Configurando os canais de PWM
  ledcSetup(motor1PWMChanFrente, freq, resolution);
  ledcSetup(motor1PWMChanTras, freq, resolution);
  ledcSetup(motor2PWMChanFrente, freq, resolution);
  ledcSetup(motor2PWMChanTras, freq, resolution);

  // Atrelando canais de PWM a pinos
  ledcAttachPin(pinoMotor1Frente, motor1PWMChanFrente);
  ledcAttachPin(pinoMotor1Tras, motor1PWMChanTras);
  ledcAttachPin(pinoMotor2Frente, motor2PWMChanFrente);
  ledcAttachPin(pinoMotor2Tras, motor2PWMChanTras);

  // Inicializa alguns valores para os pinos
  digitalWrite(pinoTrig, LOW);
  digitalWrite(pinoMotor1Frente, HIGH);
  digitalWrite(pinoMotor1Tras, LOW);
  digitalWrite(pinoMotor2Frente, HIGH);
  digitalWrite(pinoMotor2Tras, LOW);
  digitalWrite(ledBuiltIn, LOW);
  ledcWrite(motor1PWMChanFrente, 0);
  ledcWrite(motor1PWMChanTras, 0);
  ledcWrite(motor2PWMChanFrente, 0);
  ledcWrite(motor2PWMChanTras, 0);
  cicloMotor = 0;

  // Setup do display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
}  
   
void loop(){
  // Faz a limpeza do display e configura fonte
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 1);
  
  valSensorLinhaAnalog1 = analogRead(pinoLinhaAnalog1);
  valSensorLinhaDig1 = digitalRead(pinoLinhaDig1);
  valSensorLinhaAnalog2 = analogRead(pinoLinhaAnalog2);
  valSensorLinhaDig2 = digitalRead(pinoLinhaDig2);
  
  distancia = leDistUltras();

  // Output da leitura do sensor de linha 1
  if (valSensorLinhaDig1 == LOW){
        Serial.print("Linha1:\tLow:\t");
        display.print("Linha1:\tLow:\t");
  } else{
        Serial.print("Linha1:\tHigh:\t");
        display.print("Linha1:\tHigh:\t");
  }
  Serial.println(valSensorLinhaAnalog1);
  display.println(valSensorLinhaAnalog1);

  // Output da leitura do sensor de linha 2
  if (valSensorLinhaDig2 == LOW){
        Serial.print("Linha2:\tLow:\t");
        display.print("Linha2:\tLow:\t");
  } else{
        Serial.print("Linha2:\tHigh:\t");
        display.print("Linha2:\tHigh:\t");
  }
  Serial.println(valSensorLinhaAnalog2);
  display.println(valSensorLinhaAnalog2);

  if (valSensorLinhaDig1 == HIGH || valSensorLinhaDig2 == HIGH){
    digitalWrite(ledBuiltIn, HIGH);
  }
  else{
    digitalWrite(ledBuiltIn, LOW);
  }

  // Output da leitura do sensor de distância
  Serial.print("Distância:\t");
  Serial.println(distancia);

  display.print("Distancia:\t");
  display.println(distancia);

  // Testando acionamento dos motores
  cicloMotor = (cicloMotor == 10) ? 0 : cicloMotor + 1; // Incrementa o valor até 10 e depois passa a ser zero
  frente = (cicloMotor == 10) ? !frente : frente;
  int velocidade = 180 + cicloMotor * (255 - 180) / 10; // O valor mínimo para acionamento do motor é 180 para 8 bits de resolução
  if (frente == true){
    ledcWrite(motor1PWMChanFrente, velocidade);
    ledcWrite(motor2PWMChanFrente, velocidade);

    ledcWrite(motor1PWMChanTras, 0);
    ledcWrite(motor2PWMChanTras, 0);
  }
  else {
    ledcWrite(motor1PWMChanFrente, 0);
    ledcWrite(motor2PWMChanFrente, 0);

    ledcWrite(motor1PWMChanTras, velocidade);
    ledcWrite(motor2PWMChanTras, velocidade);
  }

  if(ledcRead(motor1PWMChanFrente) > 0){
    Serial.print("Motor 1:\tFrente\tVel:");
    Serial.println(cicloMotor);
    display.print("Motor 1:\tFrente\tVel:");
    display.println(cicloMotor);
  } else{
    Serial.print("Motor 1:\tTras\tVel:");
    Serial.println(cicloMotor);
    display.print("Motor 1:\tTras\tVel:");
    display.println(cicloMotor);
  }
  if(ledcRead(motor2PWMChanFrente) > 0){
    Serial.print("Motor 2:\tFrente\tVel:");
    Serial.println(cicloMotor);
    display.print("Motor 2:\tFrente\tVel:");
    display.println(cicloMotor);
  } else{
    Serial.print("Motor 2:\tTras\tVel:");
    Serial.println(cicloMotor);
    display.print("Motor 2:\tTras\tVel:");
    display.println(cicloMotor);
  }

  Serial.println(velocidade);
  display.println(velocidade);

  Serial.println("---------------------");
   
  display.display();
  delay(1000);
}
