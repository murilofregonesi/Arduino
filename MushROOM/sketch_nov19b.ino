/*
 * ESTUFA DE COGUMELOS 
 * Controle de temperatura e umidade
 */

#include <DHT.h>           // Biblioteca DHT 11
#include <LiquidCrystal.h> // Biblioteca LCD

#define DHTPIN A1     // Pino DHT 11
#define DHTTYPE DHT11 // Versão DHT

DHT dht(DHTPIN, DHTTYPE);              // Inicializa DHT 11
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Inicializa LCD

int lcd_contrast = 10; // Constraste do LCD (PWM)

int potPin = A7;          // Porta do potenciômetro
int potValue = 0;         // Valor do potenciômetro 0-1023
int potEstado[2] = {0,0}; // Vetor de estados do pot. [k-1 k]

int relePin = 6;         // Porta do relé
bool releStatus = false; // Status do relé
float umiLimite = 0.0;   // Limite inferior de humidade [%]
float umiTolera = 5.0;   // Tolerância na umidade [%]

unsigned long timerIni = 0; // Início da contagem [ms]
unsigned long timerAtu = 0; // Contagem atual [ms]
int timerDur = 3000;        // Duração do timer [ms]
bool timerLoop = false;     // Estado do timer

byte grau[8] ={ B00001100,
                B00010010,
                B00010010,
                B00001100,
                B00000000,
                B00000000,
                B00000000,
                B00000000,}; // Array símbolo grau

byte seta[8] = { 0b00000,
                 0b00100,
                 0b01110,
                 0b10101,
                 0b00100,
                 0b00100,
                 0b00100,
                 0b00000}; // Array símbolo seta

void setup() 
{
  Serial.begin(9600);
  dht.begin();

  lcd.begin(16,2);         // Inicializa LCD
  lcd.clear();             // Limpa o LCD
  lcd.createChar(0, grau); // Cria char grau
  lcd.createChar(1, seta); // Cria char seta

  pinMode(lcd_contrast, OUTPUT);  // Testar melhor contraste
  analogWrite(lcd_contrast, 100); // analogWrite from 0 to 255
  
  pinMode(relePin, OUTPUT);       // Porta relé como saída
  digitalWrite(relePin, HIGH);    // Hum. desativado
}

void loop() 
{
  float dht_H = dht.readHumidity();    // Leitura H sensor DHT11
  float dht_T = dht.readTemperature(); // Leitura T sensor DHT11

  imprimirLCD(dht_H, dht_T); // Imprimir leituras no LCD

  verificaPot(potValue); // Verificar potenciômetro
  
  while (timerLoop) // Loop de mudança no pot.
  {
    lcd.setCursor(0,0);
    lcd.print("Alterar limite");
    lcd.print(" ");
  
    lcd.setCursor(0,1);
    lcd.print("Umid : ");
    lcd.print(" ");
    lcd.setCursor(7,1);
    lcd.print(umiLimite,1);
    lcd.setCursor(12,1);
    lcd.print("%");

    timerAtu = millis(); // Verifica tempo atual
    if ((timerAtu - timerIni) >= timerDur)
    {
      timerLoop = false;         // Sai do loop
      lcd.clear();               // Limpa o LCD
      imprimirLCD(dht_H, dht_T); // Imprimir leituras no LCD
    }
    
    verificaPot(potValue); // Verifica nova mudança
    delay(75);
  }

  // Verificando umidade para atuação do relé
  if (dht_H < (umiLimite - umiTolera) && !releStatus)
  {
    releStatus = true;
    digitalWrite(relePin, LOW);
  }
  if (dht_H > (umiLimite + umiTolera) && releStatus)
  {
    releStatus = false;
    digitalWrite(relePin, HIGH);
  }
  if (releStatus)
  {
    lcd.setCursor(14,1);
    lcd.write((byte)1); // Seta para cima
  }
  
  delay(1800); // Delay para novo loop
}

void imprimirLCD(float umid, float temp)
{
  lcd.setCursor(0,0);
  lcd.print("Temp : ");
  lcd.print(" ");
  lcd.setCursor(7,0);
  lcd.print(temp,1);
  lcd.setCursor(12,0);
  lcd.write((byte)0); // Grau
   
  lcd.setCursor(0,1);
  lcd.print("Umid : ");
  lcd.print(" ");
  lcd.setCursor(7,1);
  lcd.print(umid,1);
  lcd.setCursor(12,1);
  lcd.print("%");
}

void verificaPot(int valorPot)
{
  potValue = analogRead(potPin); // Valor do potenciômetro 0-1023
  
  if (valorPot <= 47) // 100  - Entre 0 e 47
  {
    potEstado[1] = 11; // Atualiza estado k do pot.
    umiLimite = 100.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 140) // 95  - Entre 47 e 140
  {
    potEstado[1] = 10; // Atualiza estado k do pot.
    umiLimite = 95.0;  // Atualiza limite de umidade
  }
  else if (valorPot <= 233) // 90  - Entre 140 e 233
  {
    potEstado[1] = 9; // Atualiza estado k do pot.
    umiLimite = 90.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 326) // 85  - Entre 233 e 326
  {
    potEstado[1] = 8; // Atualiza estado k do pot.
    umiLimite = 85.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 419) // 80  - Entre 326 e 419
  {
    potEstado[1] = 7; // Atualiza estado k do pot.
    umiLimite = 80.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 512) // 75  - Entre 419 e 512
  {
    potEstado[1] = 6; // Atualiza estado k do pot.
    umiLimite = 75.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 605) // 70  - Entre 512 e 605
  {
    potEstado[1] = 5; // Atualiza estado k do pot.
    umiLimite = 70.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 698) // 65  - Entre 605 e 698
  {
    potEstado[1] = 4; // Atualiza estado k do pot.
    umiLimite = 65.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 791) // 60  - Entre 698 e 791
  {
    potEstado[1] = 3; // Atualiza estado k do pot.
    umiLimite = 60.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 884) // 55  - Entre 791 e 884
  {
    potEstado[1] = 2; // Atualiza estado k do pot.
    umiLimite = 55.0; // Atualiza limite de umidade
  }
  else if (valorPot <= 977) // 50  - Entre 884 e 977
  {
    potEstado[1] = 1; // Atualiza estado k do pot.
    umiLimite = 50.0; // Atualiza limite de umidade
  }
  else // OFF - Entre 977 e 1023
  {
    potEstado[1] = 0; // Atualiza estado k do pot.
    umiLimite = 0.0;  // Atualiza limite de umidade
  }

  if (potEstado[1] != potEstado[0]) // Procura alteração
  {
    timerIni = millis();         // Reinicializa timer  
    timerLoop = true;            // Força loop do timer
    potEstado[0] = potEstado[1]; // Atualiza estado
    lcd.clear();                 // Limpa o LCD
  }
}

