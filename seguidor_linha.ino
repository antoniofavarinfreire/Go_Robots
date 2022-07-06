// Portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINEN5 3
#define PINEN6 6

// Portas sensor QTR
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3
#define SENSOR5 A4
#define SENSOR6 A5

// Valores de ajustes para o seguidor de linha MIF
#define TRESHOLD 750 // Valor de referencia para cor da linha branca
#define SPEED0 240 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 0 0)
#define SPEED1 200 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 1 1 1 0)

#define SPEED2 180 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 0 0)
#define SPEED3 150 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 0)
#define SPEED4 100 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 1 1 1)

#define SPEED5 0 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 0)
#define SPEED6 0 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 1 1)
#define SPEED7 120 // Valor de 0 a 255 para velocidade com a seguinte leitura do sensor (0 0 0 0 0 1)

#define RUNTIME 15000 // Valor para executar o percurso


double sensed_output, control_signal;
double setpoint;
double Kp = 0.038; 
double Ki = 0;
double Kd = 2.76; 
int T; 
unsigned long last_time;
double totalError, lastError;
int max_control;
int min_control;

  
//--- inicialização das entradas utilizadas ---//

void setup() {
 //Serial.begin(9600);
 //Serial.println("Setup feito.");
  PID();
}
   


void loop() {

  //followLineMEF();
  PID();
  //motorOption('6',255,255);
}
float error;
void PID(){
  
  long currentTime = millis();
  bool flag = true;
  while(flag){
    flag = motorStop(RUNTIME, currentTime);

    float position = followLineMEF();
    
    error = position - 3500;

    totalError += error;

    float motorSpeed = (Kp*error) + (Ki*totalError) + (Kd*(error-lastError));

    lastError = error;

    const float maxSpeedA = 255;
    const float maxSpeedB = 255;
    const float baseSpeedA = 180;
    const float baseSpeedB = 180;

    float speedA = (baseSpeedA + motorSpeed);
    float speedB = (baseSpeedB - motorSpeed);

    if(speedA > maxSpeedA){
      speedA = maxSpeedA;
    }

    if(speedB > maxSpeedB){
      speedB = maxSpeedB;
    }

    if(speedA < -baseSpeedA){
      speedA = -baseSpeedA;
    }
    
    if(speedB < -baseSpeedB){
      speedB = -baseSpeedB;
    }

    motorControl(speedA, speedB);
}
}

// Função para controle do driver de motor
void motorControl(int speedLeft, int speedRight) {

    // Definições das portas digitais
    pinMode(PININ1, OUTPUT);
    pinMode(PININ2, OUTPUT);
    pinMode(PININ3, OUTPUT);
    pinMode(PININ4, OUTPUT);
    pinMode(PINEN5, OUTPUT);
    pinMode(PINEN6, OUTPUT);

    // Ajustes motor da esquerda
    if (speedLeft < 0) {
        speedLeft = -speedLeft;
        digitalWrite (PININ3, HIGH);
        digitalWrite (PININ4, LOW);
    } else {
        digitalWrite (PININ3, LOW);
        digitalWrite (PININ4, HIGH);
    }

    // Ajustes motor da direita
    if (speedRight < 0) {
        speedRight = -speedRight;
        digitalWrite (PININ1, LOW);
        digitalWrite (PININ2, HIGH);
    } else {
        digitalWrite (PININ1, HIGH);
        digitalWrite (PININ2, LOW);
    }

    analogWrite (PINEN5, speedLeft);
    analogWrite (PINEN6, speedRight);
}

// Função para controle de motor com pre definições
void motorOption(char option, int speedLeft, int speedRight) {
    switch (option) {
        case '6': // Esquerda
            motorControl(-speedLeft, speedRight);
            break;
        case '4': // Direita
            motorControl(speedLeft, -speedRight);
            break;
        case '2': // Trás
            motorControl(-speedLeft, -speedRight);
            break;
        case '8': // Frente
            motorControl(speedLeft, speedRight);
            break;
        case '0': // Parar
            motorControl(0, 0);
            break;
    }
}

// Função de paada do robô
bool motorStop(long runtime, long currentTime) {
    if (millis() >= (runtime + currentTime)) {
        motorOption('0', 0, 0);
        int cont = 0;
        while (true) {
            cont++;
        }
        return false;
    }
    return true;
}

// Função para leitura dos sensores
void readSensors(bool readSerial, int *sensors) {
  // Função para leitura dos sensores
  sensors[0] = analogRead(SENSOR1);
  sensors[1] = analogRead(SENSOR2);
  sensors[2] = analogRead(SENSOR3);
  sensors[3] = analogRead(SENSOR4);
  sensors[4] = analogRead(SENSOR5);
  sensors[5] = analogRead(SENSOR6);
  if (readSerial) {
    Serial.print(sensors[0]);
    Serial.print(' ');
    Serial.print(sensors[1]);
    Serial.print(' ');
    Serial.print(sensors[2]);
    Serial.print(' ');
    Serial.print(sensors[3]);
    Serial.print(' ');
    Serial.print(sensors[4]);
    Serial.print(' ');
    Serial.println(sensors[5]);
  }
}

// Função para controle do seguidor de linha em modo de maquina de estado finita
float followLineMEF(void) {
  // Função para controle do seguidor de linha em modo de maquina de estado finita
  bool flag = true;
  long currentTime = millis();

  while (flag) {
    // Flag para verificar a parada
    flag = motorStop(RUNTIME, currentTime);

    // Leitura sensores
    int s[6];
    readSensors(false, s);

    // leitura do sensor (1 1 1 1 1 1) = 0
    
    if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
      motorOption('8', SPEED0, SPEED0);
      error = 0;

      // leitura do sensor (0 1 1 1 1 0) = 0
    } else if ( s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED0);
      error = 0;

      // leitura do sensor (0 0 1 1 0 0) = 0
    } else if ( s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED0);
      error = 0;
      // leitura do sensor (0 0 1 0 0 0) = 0.5
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED2);
      error = 0.5;
      
      // leitura do sensor (0 0 0 1 0 0) = -0.5
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD ) {
      motorOption('8', SPEED2, SPEED0);
      error = -0.5;

      // leitura do sensor (0 1 1 1 0 0) = 1
    } else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED1);
      error = 1;
      
      // leitura do sensor (0 0 1 1 1 0) = -1
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] <= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD ) {
      motorOption('8', SPEED1, SPEED0);
      error = -1;


      // leitura do sensor (0 1 1 0 0 0) = 1.5
    } else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED3);
      error = 1.5;
      
      // leitura do sensor (0 0 0 1 1 0) = -1.5
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED3, SPEED0);
      error = -1.5;

      // leitura do sensor (0 1 0 0 0 0) = 2
    } else if (s[0] >= TRESHOLD && s[1] <= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED5);
      error = 2;
      
      // leitura do sensor (0 0 0 0 1 0) = -2
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] <= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED5, SPEED0);
      error = -2;
      
      // leitura do sensor (1 1 1 0 0 0) = 2.5
    } else if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] <= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED4);
      error = 2.5;
      
      // leitura do sensor (0 0 0 1 1 1) = -2.8
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] <= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
      motorOption('8', SPEED4, SPEED0);
      error = -2.5;

      // leitura do sensor (1 1 0 0 0 0) = 3
    } else if (s[0] <= TRESHOLD && s[1] <= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('8', SPEED0, SPEED6);
      error = 3;
      
      // leitura do sensor (0 0 0 0 1 1) = -3.7
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] <= TRESHOLD && s[5] <= TRESHOLD) {
      motorOption('8', SPEED6, SPEED0);
      error = -3.4;

      // leitura do sensor (1 0 0 0 0 0) = 3.5
    } else if (s[0] <= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] >= TRESHOLD) {
      motorOption('6', SPEED7, SPEED7);
      error = 3.5;
      
      // leitura do sensor (0 0 0 0 0 1) = -4.2
    } else if (s[0] >= TRESHOLD && s[1] >= TRESHOLD && s[2] >= TRESHOLD && s[3] >= TRESHOLD && s[4] >= TRESHOLD && s[5] <= TRESHOLD) {
      motorOption('4', SPEED7, SPEED7);
      error = -3.9;
    }
  }
  return error;
}r
