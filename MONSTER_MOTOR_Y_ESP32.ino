/*****************************************************
 *  02 Dic 2022
 *  Efrain Eddy Eduardo Tola
 *  controlar motores DC con driver moster motor sield de 2 canales
 *  Tensión máxima: 16 V
 *  Corriente máxima: 30 Amperios
 *  Práctica Corriente continua: 14 A
 * ***************************************************/
 
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define led      2  // led interno del esp32
 
// pines del esp32
#define enabi     32
#define enabd     33

#define pwmi      23   //~
#define mi1       18
#define mi2       5
#define md1       21
#define md2       19
#define pwmd      22   //~
// pines para el manejo de cortadora
#define cort1       26
#define cort2       27
#define pwmcort     14   //~
#define enabcort    25

// setting PWM properties
const int freq = 5000;    // freciencia 5khz
const int pwmiChannel = 0;
const int pwmdChannel = 0;
const int pwmcortChannel = 1;
const int resolution = 8;  // resolucion de 8bits 0-255

const int pasadas = 2;

void setup(){
  
  // configure LED PWM functionalitites
  ledcSetup(pwmiChannel, freq, resolution);  
  ledcSetup(pwmdChannel, freq, resolution);  
  
  ledcSetup(pwmcortChannel, freq, resolution);  
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pwmi, pwmiChannel);
  ledcAttachPin(pwmd, pwmdChannel);
  
  ledcAttachPin(pwmcort, pwmcortChannel);
  
    pinMode(mi1,OUTPUT);
    pinMode(mi2,OUTPUT);
    pinMode(md1, OUTPUT);
    pinMode(md2, OUTPUT);
    pinMode(led, OUTPUT);

    
    pinMode(enabi, OUTPUT);
    pinMode(enabd, OUTPUT);
    
    pinMode(cort1, OUTPUT);
    pinMode(cort2, OUTPUT);
    pinMode(enabcort,OUTPUT);
  
  Serial.begin(115200);
  SerialBT.begin("Podadora");                                               //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}
 
void loop(){
  
  if (Serial.available()) {
      SerialBT.write(Serial.read());
    }

  
  if (SerialBT.available())
  {
    char dato = ((byte) SerialBT.read());
      
    Serial.println(dato);
    if(dato == '1')motores(255, 255),digitalWrite(led, 1),delay(100);

    else if(dato == '2')motores(-255, -255),digitalWrite(led, 1),delay(100);

    else if(dato == '3')motores(-255, 255),digitalWrite(led, 1),delay(100);

    else if(dato == '4')motores(255, -255),digitalWrite(led, 1),delay(100);

    else if(dato == '5')cortadora(255),digitalWrite(enabcort,1);
    else if(dato == '6')cortadora(0),digitalWrite(enabcort,0);     //digitalWrite(led, 0)
    
    else if(dato == '7'){
      for (int i = 0; i < pasadas; i++){
        motores(200, 200),digitalWrite(led, 1),delay(1500);
        motores(-120, 120),digitalWrite(led, 1),delay(600);
        motores(200, 200),digitalWrite(led, 1),delay(300);
        motores(-120, 120),digitalWrite(led, 1),delay(270);
        motores(200, 200),digitalWrite(led, 1),delay(1800);
        motores(120, -120),digitalWrite(led, 1),delay(600);
        motores(200, 200),digitalWrite(led, 1),delay(300);
        motores(120, -120),digitalWrite(led, 1),delay(260);
        //motores(200, 200),digitalWrite(led, 1),delay(1500);
      }
    }
    else {
      motores(0, 0),digitalWrite(led, 0);
      digitalWrite(enabi,0);
      digitalWrite(enabd,0);
    }
  }   
}
 
void motores(int motor_izq, int motor_der)
{
    digitalWrite(enabi,1);
    digitalWrite(enabd,1);
  if ( motor_izq >= 0 )  
  {
    digitalWrite(mi1, HIGH);
    digitalWrite(mi2, LOW); 
    ledcWrite(pwmiChannel, motor_izq);
  }
  if ( motor_izq < 0 )
  {
    digitalWrite(mi1, LOW); 
    digitalWrite(mi2, HIGH);
    motor_izq = motor_izq*(-1); 
    ledcWrite(pwmiChannel, motor_izq);
  }
 
  if ( motor_der >= 0 ) //motor derecho
  {
    digitalWrite(md1, HIGH);
    digitalWrite(md2, LOW);
    ledcWrite(pwmdChannel, motor_der);
  }
  if ( motor_der < 0 )
  {
    digitalWrite(md1, LOW);
    digitalWrite(md2, HIGH);
    motor_der = motor_der*(-1);
    ledcWrite(pwmdChannel, motor_der);
  }
}

void cortadora(int valor){
  digitalWrite(cort1, HIGH);
  digitalWrite(cort2, LOW);
  ledcWrite(pwmcortChannel, valor);
}

////////////////////////////////////////////
