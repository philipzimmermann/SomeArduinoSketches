
//We always have to include the library
#include "LedControl.h"
//For saving the High Score ;)
#include <EEPROM.h>

//Analog Joystick Pins
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

int pause = 100;
const int size1 = 8 * 8;
boolean isFood [size1];
boolean occupied [size1];
int posx[size1];
int posy[size1];
int snake_length = 0;
int dir = 0;
int last_dir = 0;

int highscore;

boolean food = false;

//datapin, clkPin, csPin, numDevices
LedControl lc = LedControl(12, 10, 11, 1);


void showScore(int score){
  int row=0;
  int col=0;
  for (int row = 0; row < 8&&row*8+col<score; row++) {
    for (int col = 0; col < 8&&row*8+col<score; col++) {
      delay(200);
      lc.setLed(0, row, col, true);
    }
  }
  delay(2000);
}
//Spielsieg-Methode
void gameover() {
  int score=snake_length+1;
  if(score>highscore){
    highscore=score;
    EEPROM.write(0,highscore);
  }
  lc.clearDisplay(0);
  showScore(score);
  lc.clearDisplay(0);
}

void draw() {
  for (int i = 0; i < 8; i++) {
    for (int k = 0; k < 8; k++) {
      if (occupied[i * 8 + k] || isFood[i * 8 + k]) {
        lc.setLed(0, i, k, true);
      }
      else {
        lc.setLed(0, i, k, false);
      }
    }
  }
}

void makeSnake() {
  posx[0] = 3;
  posx[1] = 3;
  posy[0] = 3;
  posy[1] = 2;
  occupied[posy[0] * 8 + posx[0]] = true;
  occupied[posy[1] * 8 + posx[1]] = true;
  snake_length += 1;
}

int checkPos(int pos) {
  if (pos > 7) {
    pos = 0;
  }
  if (pos < 0) {
    pos = 7;
  }
  return pos;
}

void reset_field() {
  for (int i = 0; i < size1; i++) {
    isFood [i] = false;
    occupied [i] = false;
    posx[i] = 0;
    posy[i] = 0;
    food=false;
    
  }
}

//Vorbereitung
void setup() {
  randomSeed(analogRead(2));
  highscore = EEPROM.read(0);
  Serial.println("Highscore: " + highscore);
  //AnalogStick vorbereitung
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
  Serial.print(posx[10]);

  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 2);
  /* and clear the display */
  lc.clearDisplay(0);
  showScore(highscore);
  lc.clearDisplay(0);
  reset_field();
  snake_length = 0;
  makeSnake();
  draw();
  delay(1000);
}





int readStick() {
  if (analogRead(X_pin) > 512 + 200 && last_dir != 2) {
    return 0;
  }
  if (analogRead(Y_pin) > 512 + 200 && last_dir != 1) {
    return 3;
  }
  if (analogRead(X_pin) < 512 - 200 && last_dir != 0) {
    return 2;
  }
  if (analogRead(Y_pin) < 512 - 200 && last_dir != 3) {
    return 1;
  }
  return dir;
}


int wait;
int food_counter = 0;

boolean foodCount(int val) {
  if (food_counter < val) {
    food_counter++;
    return false;
  }
  food_counter = 0;
  return true;
}



void spawnFood() {
  while (1) {
    int placex = random(0, 8);
    int placey = random(0, 8);
    if (occupied[placey * 8 + placex] == false && isFood[placey * 8 + placex] == false) {
      isFood[placey * 8 + placex] = true;
      food = true;
      break;
    }
  }
}

void loop() {
  Serial.println(snake_length);
  if (foodCount(random(5, 15)) && food == false) {
    spawnFood();
  }

  //Schwanzende
  occupied[posy[snake_length] * 8 + posx[snake_length]] = false;
  posy[snake_length] = posy[snake_length - 1];
  posx[snake_length] = posx[snake_length - 1];
  occupied[posy[snake_length] * 8 + posx[snake_length]] = true;

  //Schwanzteile
  for (int i = snake_length; i > 0; i--) {
    posy[i] = posy[i - 1];
    posx[i] = posx[i - 1];
    occupied[posy[i] * 8 + posx[i]] = true;
  }

  dir = readStick();
  
  //Kopf
  move(0);

  printMatrix();
  draw();
  delay(20);

  draw();
  wait = 0;
  while (wait < pause) {
    dir = readStick();
    delay(20);
    wait += 20;
  }
}

void printMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int k = 0; k < 8; k++) {

      Serial.print(occupied[i * 8 + k]);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("__________________________");
}

//Bewegt schlangenteil number aber eigentlich immer Kopf und isst essen
void move(int number) {
  switch (dir) {
    case 0: posy[number]++;
      posy[number] = checkPos(posy[number]);
      break;
    case 1: posx[number]++;
      posx[number] = checkPos(posx[number]);
      break;
    case 2: posy[number]--;
      posy[number] = checkPos(posy[number]);
      break;
    case 3: posx[number]--;
      posx[number] = checkPos(posx[number]);
      break;
    default: break;
  }
  if (occupied[posy[number] * 8 + posx[number]] == true) {
    gameover();
    setup();
    return;
  }
  if (isFood[posy[number] * 8 + posx[number]] == true) {
    isFood[posy[number] * 8 + posx[number]] = false;
    food = false;
    snake_length++;
  }
  occupied[posy[number] * 8 + posx[number]] = true;
  last_dir = dir;
}

