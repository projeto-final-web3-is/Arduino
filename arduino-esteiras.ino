#include <EEPROM.h>

// pino para controlar a saída dos motores de passo da esteira principal
#define stepPin 2
//pino para controle de direção dos motores da esteira principal
#define dirPin 5

// pino para controlar a saída dos motores de passo da esteira secundária
#define stepPinY 3 
//pino para controle de direção dos motores da esteira secundária
#define dirPinY 6 

// pino para ativar e desativar os motores de passo
#define enablePin 8

// pino dos sensores infravermelhos da esteira principal
#define pinoSensor 9

#define ledPin 13 



// triangulo - 0 - verde
// circulo - 1 - vermelho
// quadrado - 2 - azul


// variável
int leituraSensor = 0;
int contador = 0;
bool motorParado = false;
char resposta = '3';

String itemAnterior = "vazio";

void setup() {
  EEPROM.put(0, 0);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(pinoSensor, INPUT);
  digitalWrite(enablePin, LOW);

  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  while (true) {
    if (Serial.available() > 0) {
      char receivedChar = Serial.read(); 
      if (receivedChar == '0') {
        Serial.println("recebido 0");
        digitalWrite(dirPinY, LOW);
        for (int x = 0; x < 620; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        Serial.println("T:1");
        itemAnterior = "triangulo";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '1') {
         digitalWrite(dirPinY, HIGH);
        for (int x = 0; x < 1240; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); 
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); 
        }
        
        itemAnterior = "circulo";
        Serial.println("C:1");
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '2') {
         digitalWrite(dirPinY, HIGH);
        for (int x = 0; x < 1240; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        
        itemAnterior = "triangulo";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '3') {
         digitalWrite(dirPinY, LOW);
        for (int x = 0; x < 1240; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        Serial.println("T:1");
        itemAnterior = "triangulo";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '4') {
         digitalWrite(dirPinY, HIGH);
        for (int x = 0; x < 620; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        Serial.println("C:1");
        itemAnterior = "circulo";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '5') {
         digitalWrite(dirPinY, HIGH);
        for (int x = 0; x < 620; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        Serial.println("S:1");
        itemAnterior = "quadrado";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == '6') {
         digitalWrite(dirPinY, LOW);
        for (int x = 0; x < 620; x++) {
          digitalWrite(stepPinY, HIGH);
          delayMicroseconds(500); // Tempo de pulso alto
          digitalWrite(stepPinY, LOW);
          delayMicroseconds(500); // Tempo de pulso baixo
        }
        Serial.println("S:1");
        itemAnterior = "quadrado";
  
        delay(1000);
        motorParado = false;
      } else if (receivedChar == "9") {
        Serial.println("S:0");
        motorParado = false;
      }
    } else if (motorParado != true){
        digitalWrite(dirPin, LOW);
        leituraSensor = digitalRead(pinoSensor);

        if (leituraSensor == LOW) {
          Serial.println("A:I");
          int value = readNumberInEEpromMemory();

          if (value == 0) {
            pararMotor();
            saveNumberInEEpromMemory(1);
          } else {
            saveNumberInEEpromMemory(0);
          }

          Serial.println(value);
          pararMotor();
          motorParado = true;
        } else {
          digitalWrite(stepPin, HIGH);
          delayMicroseconds(1000);
          digitalWrite(stepPin, LOW);
          delayMicroseconds(1000);
        }
    }
  }
}

void pararMotor() {
  Serial.println("U:0");


    digitalWrite(enablePin, HIGH); // Desliga o motor temporariamente
    delay(1000);
    
    contador++;
    
    
    for (int i = 0; i < 1420; i++) {
      digitalWrite(enablePin, LOW); // Liga o motor novamente
      digitalWrite(dirPin, LOW);
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }

    digitalWrite(enablePin, HIGH); // Desliga o motor permanentemente
}

void moveMotorAnteHorario() {
  digitalWrite(dirPin, HIGH);

  while (true) {
    leituraSensor = digitalRead(pinoSensor);

    if (leituraSensor == LOW) {
      pararMotor();
      motorParado = true;
      break;
    } else {
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(1000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(1000);
    }
  }
}

int memoryAddress = 0;

void saveNumberInEEpromMemory(int number) {
  EEPROM.put(memoryAddress, number);
}

int readNumberInEEpromMemory() {
  int value;
  EEPROM.get(memoryAddress, value);
  return value;
}