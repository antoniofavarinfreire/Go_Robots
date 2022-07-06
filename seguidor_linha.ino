// Portas driver motor
#define PININ1 2
#define PININ2 4
#define PININ3 5
#define PININ4 7
#define PINENA 3
#define PINENB 6

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

#define RUNTIME 18500 // Valor para executar o percurso

//constantes do PID
double kp = 2
double ki = 5
double kd = 1

unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;

  
//--- inicialização das entradas utilizadas ---//

void setup() {

 pinMode(L, INPUT);
 pinMode(C, INPUT);
 pinMode(R, INPUT);
 pinMode(ENA, OUTPUT);
 pinMode(IN1, OUTPUT);
 pinMode(IN2, OUTPUT);
 pinMode(IN3, OUTPUT);
 pinMode(IN4, OUTPUT);
 pinMode(ENB, OUTPUT);

 digitalWrite(ENA, LOW);
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW);
 digitalWrite(IN3, LOW);
 digitalWrite(IN4, LOW);
 digitalWrite(ENB, LOW);

 //Serial.begin(9600);
 //Serial.println("Setup feito.");

}
   


void loop() {

  followLineMEF();
  PID();

}

void PID(){
  currentTime = millis();                //get current time
  elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
  
  error = Setpoint - inp;                                // determine error
  cumError += error * elapsedTime;                // compute integral
  rateError = (error - lastError)/elapsedTime;   // compute derivative

  double out = kp*error + ki*cumError + kd*rateError;  //PID output               

  lastError = error;                                //remember current error
  previousTime = currentTime;          //remember current time

  return out;       
}

// Função para controle do driver de motor
void motorControl(int speedLeft, int speedRight) {

    // Definições das portas digitais
    pinMode(PININ1, OUTPUT);
    pinMode(PININ2, OUTPUT);
    pinMode(PININ3, OUTPUT);
    pinMode(PININ4, OUTPUT);
    pinMode(PINENA, OUTPUT);
    pinMode(PINENB, OUTPUT);

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

    analogWrite (PINENA, speedLeft);
    analogWrite (PINENB, speedRight);
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

// Função de parada do robô
bool motorStop(long runtime, long currentTime) {
    if (millis() >= (runtime + currentTime)) {
        motorOption('0', 0, 0);
        int cont = 0;
        while (true) {
            rgbControl(255, 0, 0, 500);
            rgbControl(0, 0, 0, 500);
            cont++;
        }
        return false;
    }
    return true;
}

// Função para leitura dos sensores
void readSensors(void) {
    Serial.print(analogRead(A0));
    Serial.print(' ');
    Serial.print(analogRead(A1));
    Serial.print(' ');
    Serial.print(analogRead(A2));
    Serial.print(' ');
    Serial.print(analogRead(A3));
    Serial.print(' ');
    Serial.print(analogRead(A4));
    Serial.print(' ');
    Serial.println(analogRead(A5));
    Serial.print(' ');
}

// Função para controle do seguidor de linha em modo de maquina de estado finita
void followLineMEF(void) {
    bool flag = true;
    long currentTime = millis();

    while (flag) {
        flag = motorStop(RUNTIME, currentTime);

        // leitura do sensor (1 1 1 1 1 1)
        if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD) {
            motorOption('8', SPEED0, SPEED0);
            // leitura do sensor (0 1 1 1 1 0)
        } else if ( analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED0);
            // leitura do sensor (0 0 1 1 0 0)
        } else if ( analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED0);
            // leitura do sensor (0 1 1 1 0 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED1);
            // leitura do sensor (0 0 1 1 1 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD ) {
            motorOption('8', SPEED1, SPEED0);
        // leitura do sensor (0 0 1 0 0 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED2);
            // leitura do sensor (0 0 0 1 0 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD ) {
            motorOption('8', SPEED2, SPEED0);
            // leitura do sensor (0 1 1 0 0 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED3);
            // leitura do sensor (0 0 0 1 1 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED3, SPEED0);
            // leitura do sensor (1 1 1 0 0 0)
        } else if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) <= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED4);
            // leitura do sensor (0 0 0 1 1 1)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) <= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD) {
            motorOption('8', SPEED4, SPEED0);
            // leitura do sensor (0 1 0 0 0 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED5);
            // leitura do sensor (0 0 0 0 1 0)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED5, SPEED0);
            // leitura do sensor (1 1 0 0 0 0)
        } else if (analogRead(A0) <= TRESHOLD && analogRead(A1) <= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('8', SPEED0, SPEED6);
            // leitura do sensor (0 0 0 0 1 1)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) <= TRESHOLD && analogRead(A5) <= TRESHOLD) {
            motorOption('8', SPEED6, SPEED0);
            // leitura do sensor (1 0 0 0 0 0)
        } else if (analogRead(A0) <= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) >= TRESHOLD) {
            motorOption('6', SPEED7, SPEED7);
            // leitura do sensor (0 0 0 0 0 1)
        } else if (analogRead(A0) >= TRESHOLD && analogRead(A1) >= TRESHOLD && analogRead(A2) >= TRESHOLD && analogRead(A3) >= TRESHOLD && analogRead(A4) >= TRESHOLD && analogRead(A5) <= TRESHOLD) {
            motorOption('4', SPEED7, SPEED7);
        }
    }
    motorOption('0', 0, 0);
}