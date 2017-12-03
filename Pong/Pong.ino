
//We always have to include the library
#include "LedControl.h"


byte p0[5] = {B00111100, B00100100, B00100100, B00100100, B00111100};
byte p1[5] = {B00001000, B00011000, B00001000, B00001000, B00011100};
byte p2[5] = {B00011000, B00100100, B00001000, B00010000, B00111100};

/* now display them one by one with a small delay */
int mainDelay;
int speedCounter;
int ballSpeed;
int ballCounter;
const int buttonLeft2 = 7;
const int buttonRight2 = 6;
const int buttonRight1 = 8;
const int buttonLeft1 = 9;
LedControl lc = LedControl(12, 10, 11, 1);
const byte pos[6] = {B11100000, B01110000, B00111000, B00011100, B00001110, B00000111};
int speed = 1;
int posl = 3;
int posr = 3;
int game = 2;



//Klasse für Spielball
class Ball {
  public:
    signed int xspeed;
    signed int yspeed;
    int xpos;
    int ypos;
    virtual void posWerte(int xposi, int yposi) {
      xpos = xposi;
      ypos = yposi;
    }
    virtual void speedWerte(int xspeedi, int yspeedi) {
      xspeed = xspeedi;
      yspeed = yspeedi;
      return 3;
    }
    virtual void xspeedrev() {
      xspeed = -1 * xspeed;
    }
    virtual void yspeedrev() {
      yspeed = -1 * yspeed;
    }
};


//Deklarieren des Balls
Ball *b;



//Diese Methode zeigt die beiden Schläger links und rechts an
void writeBat(int poslinks, int posrechts) {

  //Schläger 1
  lc.setRow(0, 0, pos[poslinks]);

  //Schläger 2
  lc.setRow(0, 7, pos[posrechts]);

  delay(mainDelay);
}



//Spielsieg-Methode
void Sieg(String gewinner) {
  if (gewinner == "rechts") {
    for (int row = 0; row < 8; row++) {
      for (int col = 0; col < 8; col++) {

        lc.setLed(0, row, col, true);
        delay(20);

      }
    }
  }
  else {
    for (int row = 8; row >= 0; row--) {
      for (int col = 0; col < 8; col++) {

        lc.setLed(0, row, col, true);
        delay(20);

      }
    }
  }
  delay(600);
  setup();
}






//Vorbereitung
void setup() {
  mainDelay = 80;
  speedCounter = 5;
  ballSpeed = 1;
  pinMode(buttonRight1, INPUT);
  pinMode(buttonLeft1, INPUT);
  pinMode(buttonRight2, INPUT);
  pinMode(buttonLeft2, INPUT);
  b = new Ball();
  b->posWerte(3, 4);
  b->speedWerte(1, 0);
  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 2);
  /* and clear the display */
  lc.clearDisplay(0);
  posr = 3;
  posl = 3;
  ballCounter = 0;
/*
  delay (100);
  if (digitalRead(buttonLeft1) == HIGH || digitalRead(buttonRight1) == HIGH) {
    if (digitalRead(buttonLeft2) == HIGH || digitalRead(buttonRight2) == HIGH) {
      lc.setRow(0, 1, p2[0]);
      lc.setRow(0, 2, p2[1]);
      lc.setRow(0, 3, p2[2]);
      lc.setRow(0, 4, p2[3]);
      lc.setRow(0, 5, p2[4]);
      delay(3000);
      game = 2;
    }
    else {
      lc.setRow(0, 1, p1[0]);
      lc.setRow(0, 2, p1[1]);
      lc.setRow(0, 3, p1[2]);
      lc.setRow(0, 4, p1[3]);
      lc.setRow(0, 5, p1[4]);
      delay(3000);
      game = 1;
    }

  }
  else {
    lc.setRow(0, 1, p0[0]);
    lc.setRow(0, 2, p0[1]);
    lc.setRow(0, 3, p0[2]);
    lc.setRow(0, 4, p0[3]);
    lc.setRow(0, 5, p0[4]);
    delay(3000);
    game = 0;
    ballSpeed = 1;
    b->speedWerte(1, 1);
    mainDelay = 30;
  }*/
  
  lc.clearDisplay(0);

}



//Hauptprogramm
void player2() {
  if (digitalRead(buttonLeft1) == HIGH && posl < 5) {
    posl++;
  }

  if (digitalRead(buttonRight1) == HIGH && posl > 0) {
    posl--;
  }

  if (digitalRead(buttonLeft2) == HIGH && posr < 5) {
    posr++;
  }

  if (digitalRead(buttonRight2) == HIGH && posr > 0) {
    posr--;
  }

  hauptteil();
}








//Unbesiegbarer Bot
void player1() {
  if (digitalRead(buttonLeft1) == HIGH && posl < 5) {
    posl++;
  }

  if (digitalRead(buttonRight1) == HIGH && posl > 0) {
    posl--;
  }

  if (b->ypos + b->yspeed > 1 && b->ypos + b->yspeed < 6) {
    posr = b->ypos + b->yspeed - 1 + random(-1, 2);
  }
  if (b->ypos + b->yspeed == 1) {
    posr = b->ypos + b->yspeed - 1 + random(0, 2);
  }

  if (b->ypos + b->yspeed < 1) {
    posr = 0;
  }
  if (b->ypos + b->yspeed == 6) {
    posr = b->ypos + b->yspeed - 1 + random(-1, 1);
  }

  if (b->ypos + b->yspeed > 6) {
    posr = 5;
  }

  hauptteil();
}


//zweiBots
void player0() {
  if (b->ypos + b->yspeed > 1 && b->ypos + b->yspeed < 6) {
    posl = b->ypos + b->yspeed - 1 + random(-1, 2);

    posr = b->ypos + b->yspeed - 1 + random(-1, 2);
  }
  if (b->ypos + b->yspeed == 1) {
    posl = b->ypos + b->yspeed - 1 + random(0, 2);

    posr = b->ypos + b->yspeed - 1 + random(0, 2);
  }

  if (b->ypos + b->yspeed < 1) {
    posl = 0;

    posr = 0;
  }
  if (b->ypos + b->yspeed == 6) {
    posl = b->ypos + b->yspeed - 1 + random(-1, 1);

    posr = b->ypos + b->yspeed - 1 + random(-1, 1);
  }

  if (b->ypos + b->yspeed > 6) {
    posl = 5;

    posr = 5;
  }
  hauptteil();
}

void hauptteil() {
  //Schlägerzeichnen
  writeBat(posl, posr);

  //Ball Bewegung
  ballCounter++;
  if (ballCounter > ballSpeed)  {

    ballCounter = 0;

    //Schlägerkollision

    int altx = b->xpos;
    int alty = b->ypos;


    if (b->ypos + b->yspeed > 7 || b->ypos + b->yspeed < 0) {
      b->yspeedrev();
    }
    if (b->xpos == 6) {
      int rvar = b->ypos + b->yspeed - posr;
      switch (rvar) {
        case 0:  b->xspeedrev(); if (b->yspeed > -1) {
            if (b->ypos > 1) {
              b->yspeed--;
            } else {
              b->yspeed = 1;
            }
          };  b->xpos = 6; break;
        case 1:  b->xspeedrev();                                                                         b->xpos = 6; break;
        case 2:  b->xspeedrev(); if (b->yspeed < 1)  {
            if (b->ypos < 7) {
              b->yspeed++;
            } else {
              b->yspeed = -1;
            }
          }; b->xpos = 6; break;
        default: Sieg("links"); break;
      }
      speedCounter++;
    }

    if (b->xpos == 1) {
      int lvar = b->ypos + b->yspeed - posl;
      switch (lvar) {
        case 0:  b->xspeedrev(); if (b->yspeed > -1) {
            if (b->ypos > 1) {
              b->yspeed--;
            } else {
              b->yspeed = 1;
            }
          }; b->xpos = 1; break;
        case 1:  b->xspeedrev();                                     b->xpos = 1; break;
        case 2:  b->xspeedrev(); if (b->yspeed < 1)  {
            if (b->ypos < 7) {
              b->yspeed++;
            } else {
              b->yspeed = -1;
            }
          }; b->xpos = 1; break;
        default: Sieg("rechts"); break;
      }
      speedCounter++;
    }
    if (speedCounter > 4) {
      speedCounter = 0;
      if (ballSpeed > 0) {
        ballSpeed--;
      }
    }
    b->ypos = (b->ypos + b->yspeed);
    b->xpos = (b->xpos + b->xspeed);
    lc.setLed(0, b->xpos, b->ypos, true);


    lc.setLed(0, altx, alty, false);
  }
}



void loop() {
  player0();/*
  switch (game) {
    case 0: player0(); break;
    case 1: player1(); break;
    case 2: player2(); break;
  }*/
}


