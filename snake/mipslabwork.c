/* mipslabwork.c
 This file written 2015 by F Lundevall
 This file should be changed by YOU! So add something here:
 Latest update 2015-08-28 by F Lundevall
 For copyright and licensing, see file COPYING */

#include <stdint.h>     /* Declarations of uint_32 and the like */
#include <pic32mx.h>    /* Declarations of system-specific addresses etc */
#include "mipslab.h"    /* Declatations for these labs */
#include <stdio.h>


volatile int* trisE;
volatile int* portE;

int mytime = 0x0000;
int timeoutcount = 0;
int prime = 1234567;
int direction = 1;

char textstring[] = "text, more text, and even more text!";

/* Interrupt Service Routine */

void user_isr (void) {
    
    
  IFS(0) = IFS(0) & ~256; //Reset the interrupt signal, T2IF
  if(++timeoutcount == 10){
    timeoutcount = 0;
    time2string( textstring, mytime );
    display_string( 3, textstring );
    display_update();
    tick( &mytime );
  }   
}

/* Lab-specific initialization goes here */
void labinit( void ) {
  trisE = (volatile int*) 0xbf886100;
  *trisE = 0x00;
  portE = (volatile int*) 0xbf886110;
  *portE = 0;
  TRISD = 0xFE0;
   //Timer2
  T2CON = 0x70; //1:256 
  TMR2 = 0;
  PR2 = 0x7A12; // 0x7A12 = 31 250 , 8 000 000/256 = 31 250
  T2CONSET = 0x8000; //Starts the timer?? Why t2conset instead of t2con |= 0x8000, because it is read?
  //Enable interrupts from timer2
    IPC(2) = 0x1F;
    IEC(0) = 0x100;
    // Enable interrupts globally
    enable_interrupt();
    
    return;
}

/* This function is called repetitively from the main program */
void labwork( void ) {
    prime = nextprime(prime);
    display_string(0, itoaconv(prime));
    display_update();
}

//Kod att använda tills när labb 3 funkar
struct obj //just the position of an object
{
  int x, y;
};

struct snake { //TODO - typer ska flyttas till header? https://stackoverflow.com/questions/26439354/error-incomplete-type-is-not-allowed
  int width;//All the parts of a snakes body is the same length and width so 
  int height;
  struct obj body[];
};
struct apple {
  struct obj body;
  int height;
  int width;
};


void changeDirection(){
  int x = getbtns();
  if(x & 8 && x & 4){ //8 = left button, 4 = right button
  } else {
    if(x & 8){ //Direction, 1 = väst, öst = 2, 3 = norr, 4 = syd
      switch (direction)
      {
      case 1:
        /* code */
        direction = 4;
        break;
      case 2:
        direction = 3;
        break;
      case 3:
        direction = 1;
        break;
      case 4: 
        direction = 2;
        break;
      default:
        break;
      }
    } else if(x & 4){
       switch (direction)
      {
      case 1:
        /* code */
        direction = 3;
        break;
      case 2:
        direction = 4;
        break;
      case 3:
        direction = 2;
        break;
      case 4: 
        direction = 1;
        break;
      default:
        break;
      }
    }
  }
}

int collision_dying(struct snake *snakeDying, struct snake *snakeHit){ //return 1 = collision
  struct obj head = (snakeDying -> body)[0];
  int widthHead = snakeDying -> width;
  int widthHit = snakeHit -> width;
  int heightHead = snakeDying -> height;
  int heightHit = snakeHit -> height;
  for(int i = 0; i < (sizeof(snakeHit -> body)/sizeof(struct obj)); i++){
    struct obj bodyPart = (snakeHit -> body)[i];
  if(collision(head.x, head.y, widthHead, heightHead, bodyPart.x, bodyPart.y, widthHit, heightHit)){
    return 1;
  }
  } //End of for loop
  return 0; //no collison
}
int collision_food(struct snake *snakeEating, struct apple *appleObj){
  struct obj head = (snakeEating -> body)[0];
  return collision(head.x, head.y, snakeEating -> width, snakeEating -> height, appleObj -> body.x, appleObj -> body.y, appleObj -> width, appleObj -> height);
}
int collision(int obj1x, int obj1y, int obj1Width, int obj1Height, int obj2x, int obj2y, int obj2width, int obj2height){//TODO - check that it works, probably doesn't
  if(!(obj2x > obj1x + obj1Width || obj2y > obj1y + obj1Height)){ //Check if collision with this part
      if(obj2x >= obj1x && obj2x <= obj1x + obj1Width && obj2y >= obj1y && obj2y <= obj1y + obj1Height ){ //If startpoint is inside head
        return 1; 
      } else{
        int con1 = obj2x <= obj1x &&  obj2x + obj2width >= obj1x; //If startpoint on obj1 is between startpoint and p2 on the x-axis
        int con2 = obj2y + obj2height >= obj1y && obj2y + obj2height <= obj1y + obj1Height ;//If between startpoint and the p3 on the y-axis
        //startpoint -- p2
        //p3         -- p4
      if(con1 && con2){ //checks if point diagonal to startpoint is inside head
        return 1;
      }
      if(obj2x + obj2width <= obj1x + obj1Width && obj2y + obj2height <= obj1y + obj1Height){
        return 1;
      }
      if(con1 && obj2y >= obj1y){ 
        return 1;
      }
      
      if(con2 && obj2x >= obj1x){
        return 1;
      }//All points checked
      }
    }
}

void moveBody(struct snake *snake){//Changes the positions of the snake.
  for(int i = (sizeof(snake -> body)/sizeof(struct obj))-1; i > 0; i--){ //A for loop copying the 
    (snake -> body)[i].x = (snake -> body)[i-1].x;
    (snake -> body)[i].y = (snake -> body)[i-1].y;
    
  }
}

void move(struct snake *snake){  //Direction, 1 = west, east = 2, 3 = north, 4 = south
  moveBody(snake);
  switch (direction) //OBS, inte helt säker på -> eller .
  {
  case 1:
    (snake -> body[0].x)--;
    break;
  case 2: 
    (snake -> body[0].x)++;
    break;
  case 3:
    (snake -> body[0].y)--;
    break;
  case 4:
    (snake -> body[0].y)++;
    break;
  default:
    break;
  }
}

void clearGameBoard(uint8_t *data, int size){
    for(int i = 0; i < size; i++){
      data[i] = 0;
    }
}

void setPixel(int x, int y, int black, uint8_t *board){
  int xaxis = x;
  int yaxis = (y/8)*8; //obs: (y/8) becaus it's int so 7/8 = 0
  board[x+yaxis] &= ~(1 << (y%8));
}

void setBlock(struct obj ob, int width, int height, uint8_t *board){
  for(int y = ob.y; y < ob.y + height; y++){
    for(int x = ob.x; x < ob.x + width; x++){
      setPixel(x,y,1, board);
    }
  }
}

void changeGameState(struct snake *snake1, struct snake *snake2, struct apple *ap, uint8_t *board, int boardSize){
  clearGameBoard(board, boardSize);

  for(int i = 0; i < sizeof(snake1 -> body)/sizeof(struct obj); i++){
    setBlock((snake1 -> body)[i], snake1 -> width, snake1 -> height, board);
  }
   for(int i = 0; i < sizeof(snake2 -> body)/sizeof(struct obj); i++){
    setBlock((snake2 -> body)[i], snake2 -> width, snake2 -> height, board);
  }
  setBlock(ap -> body, ap -> width, ap -> height, board);
}




