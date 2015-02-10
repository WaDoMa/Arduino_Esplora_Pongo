//Pongo by Mongo ;-))))
#include <Esplora.h>
#include <TFT.h>            // Arduino LCD library
#include <SPI.h>


struct screen
{
  int resolution_h,resolution_v;
};

struct ball
{
  int x,y;
  int previous_x,previous_y;
  double delta_x,delta_y;
  int width,height;
  int col_r,col_g,col_b;
};

struct paddle
{
  int x,y;
  int previous_x,previous_y;
  int width,height;
  int col_r,col_g,col_b;
};

screen scr={};
ball ball1={};
paddle paddle1={};

void setup()
{
  EsploraTFT.begin();  // initialize the display
  EsploraTFT.background(0, 0, 0);  // set the background the black
  scr = {EsploraTFT.width(),EsploraTFT.height()};  //determine and store resolution of screen
  ball1 = {0,0,NULL,NULL,1.1,1.5,5,5,255,255,255};  //initialize ball position to upper left corner, ball trajectory
  paddle1 = {scr.resolution_h/2,scr.resolution_v/2,NULL,NULL,20,5,255,255,255};  //initialize paddle position to the middle of the screen
}

void loop()
{
  int ballSpeed = 0;
  paddle1.x = map(Esplora.readJoystickX(), 512, -512, 0, scr.resolution_h) - paddle1.width / 2;
  paddle1.y = map(Esplora.readJoystickY(), -512, 512, 0, scr.resolution_v) - paddle1.height / 2;
  EsploraTFT.fill(0, 0, 0);  // set the fill color to black and erase the previous position of the paddle if different from present
  if (paddle1.previous_x != paddle1.x || paddle1.previous_y != paddle1.y)
  {
    EsploraTFT.rect(paddle1.previous_x, paddle1.previous_y, paddle1.width, paddle1.height);
  }
  EsploraTFT.fill(paddle1.col_r, paddle1.col_b, paddle1.col_g);  // draw the paddle on screen, save the current position as the previous
  EsploraTFT.rect(paddle1.x, paddle1.y, paddle1.width, paddle1.height);
  paddle1.previous_x = paddle1.x;
  paddle1.previous_y = paddle1.y;
  
  ballSpeed = map(Esplora.readSlider(), 0, 1023, 0, 80) + 1;   // read the slider to determinde the speed of the ball
  if (millis() % ballSpeed < 2)
  {
    moveBall();
  }
}

// this function determines the ball's position on screen
void moveBall()
{
  if (ball1.x > scr.resolution_h || ball1.x < 0)  // if the ball goes offscreen, reverse the direction:
  {
    ball1.delta_x = -ball1.delta_x;
  }
  if (ball1.y > scr.resolution_v || ball1.y < 0)
  {
    ball1.delta_y = -ball1.delta_y;
  }

  // check if the ball and the paddle occupy the same space on screen
  if (inPaddle(ball1.x, ball1.y, paddle1.x, paddle1.y, paddle1.width, paddle1.height))
  {
    ball1.delta_y = -ball1.delta_y;
  }

  ball1.x += ball1.delta_x;  // update the ball's position
  ball1.y += ball1.delta_y;

  EsploraTFT.fill(0, 0, 0);// erase the ball's previous position
  if (ball1.previous_x != ball1.x || ball1.previous_y != ball1.y)
  {
    EsploraTFT.rect(ball1.previous_x, ball1.previous_y, ball1.width, ball1.height);
  }
  EsploraTFT.fill(255, 255, 255);  // draw the ball's current position
  EsploraTFT.rect(ball1.x, ball1.y,ball1.width, ball1.height);

  ball1.previous_x = ball1.x;  //store present to previous position
  ball1.previous_y = ball1.y;
  
  Serial.print(ball1.x);  //output ball position for debugging
  Serial.print("\t");
  Serial.print(ball1.y);
  Serial.print("\t");
  Serial.print(ball1.delta_x);
  Serial.print("\t");
  Serial.println(ball1.delta_y); 
  
}

// this function checks the position of the ball to see if it intersects with the paddle
boolean inPaddle(int x, int y, int rectX, int rectY, int rectWidth, int rectHeight)
{
 if ((x >= rectX && x <= (rectX + rectWidth)) && (y >= rectY && y <= (rectY + rectHeight)))
  {
    return true;
  }
  else
  {
    return false;
  }
}


