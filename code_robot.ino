#include <VirtualWire.h>

// Define du capteur de couleurs
#define S0 2
#define S1 3
#define S2 5
#define S3 4
#define sensorOut 6

//Define de la roue1
#define dir1 7 
#define pwm1 9 
#define brake1 8

//Define de la roue2
#define dir2 43
#define pwm2 44 
#define brake2 42 

//Define du capteur ultrason
#define trig1 22
#define echo1 23

int redFrequency = 0;
int greenFrequency = 0;
int blueFrequency = 0;

//Define des capteurs ultrason
//capteur 1
long lecture_echo;
long cm;



void setup_ultrason() {
  //capteur 1
  pinMode(trig1, OUTPUT);
  digitalWrite(trig1, LOW);
  pinMode(echo1, INPUT);
}

void setup_couleur() {
  // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
   // Begins serial communication 
  Serial.begin(9600);
}
void setup_emetteur_recepteur(){
  //pour changer les pins
 // par defaut recepteur: pin 11 et emetteur 12
  //vw_set_tx_pin(n° IO);
  //vw_set_rx_pin(n° IO);
  vw_setup(2000);
  vw_rx_start(); // On peut maintenant recevoir des messages
}

void reception_message() {
  byte message[VW_MAX_MESSAGE_LEN];
  byte taille_message = VW_MAX_MESSAGE_LEN;
  // N.B. La constante VW_MAX_MESSAGE_LEN est fournie par la lib VirtualWire

  /* 
   La variable "taille_message" doit impérativement être remise à 
   la taille du buffer avant de pouvoir recevoir un message. 
   Le plus simple est d'utiliser une variable locale pour ne pas
   avoir à réassigner la valeur à chaque début de loop().
   */

  // On attend de recevoir un message
  vw_wait_rx_max(200);

  if (vw_get_message(message, &taille_message)) {
    // On copie le message, qu'il soit corrompu ou non

    Serial.println((char*) message); // Affiche le message
  }
}

void setup_moteur(){
   pinMode(dir1,OUTPUT);
   pinMode(pwm1,OUTPUT);
   pinMode(brake1,OUTPUT);

   pinMode(dir2,OUTPUT);
   pinMode(pwm2,OUTPUT);
   pinMode(brake2,OUTPUT);
}

//envoi d'un int en radio
void envoi_message(int valeur) {
  vw_send((byte *) &valeur, sizeof(valeur));
  Serial.println("message envoyé");
  vw_wait_tx(); // On attend la fin de l'envoi
}

void lecture_couleur() {
  // Setting RED (R) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  
  // Reading the output frequency
  redFrequency = pulseIn(sensorOut, LOW);
  
   // Printing the RED (R) value
  Serial.print("R = ");
  Serial.print(redFrequency);
  delay(100);
  
  // Setting GREEN (G) filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  greenFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the GREEN (G) value  
  Serial.print(" G = ");
  Serial.print(greenFrequency);
  delay(100);
 
  // Setting BLUE (B) filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  
  // Reading the output frequency
  blueFrequency = pulseIn(sensorOut, LOW);
  
  // Printing the BLUE (B) value 
  Serial.print(" B = ");
  Serial.println(blueFrequency);
  if(blueFrequency<800&&redFrequency<800&&greenFrequency<800)
  {
    Serial.println("C'est blanc");
  }
  else if(blueFrequency>1400&&redFrequency>1400&&greenFrequency>1200)
  {
    Serial.println("C'est noir");
  }
  else if(blueFrequency<redFrequency && blueFrequency<greenFrequency)
  {
    Serial.println("C'est bleu");
  }
  else if(redFrequency<blueFrequency&& redFrequency<greenFrequency)
  {
    Serial.println("C'est rouge");
  }
  else if(greenFrequency<blueFrequency&&greenFrequency<redFrequency)
  {
    Serial.println("C'est vert");
  }
  delay(100);
}

int lecture_ultrason(int lequel){
  switch (lequel){
    case 1:
    digitalWrite(trig1, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig1, LOW);
    lecture_echo = pulseIn(echo1,HIGH);
    cm = lecture_echo /58;
    Serial.print("Distance en cm capteur 1: ");
    Serial.println(cm);
    if(cm<3){
    Serial.println("Danger");
    return 1;
    }
    else{
      return 0;
    }
    break;
  }
}

void avancer(){
  digitalWrite(dir1,HIGH);
  digitalWrite(pwm1,HIGH);
  digitalWrite(brake1,LOW);

  digitalWrite(dir2,HIGH);
  digitalWrite(pwm2,HIGH);
  digitalWrite(brake2,LOW);
}

void setup(){
  setup_ultrason();
  setup_couleur();
  setup_emetteur_recepteur();
  setup_moteur();
}

void loop(){
  avancer();
  lecture_couleur();
  lecture_ultrason(1);
  /*
  if (lecture_ultrason(1))
  {
    while (lecture_ultrason(1))
    {
      
    }
  }*/
}
