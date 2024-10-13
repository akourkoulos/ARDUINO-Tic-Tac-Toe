#include "Mouse.h" // library for joystick
#include <avr/sleep.h> // library for sleep function

#define Xmove 1 // oponents move
#define Omove 2 // your move


#define arduinoWon 1 // indicate that the arduino won the game 
#define youWon 2    // indicate that you won the game 
#define noWin 3     // indicate a tie
#define errorFlag 5 // indicate an error

#define arduinoStarts 1 // value indicates that arduino plays first
#define youStart 2 // value indicates that you plays first

int whoStarts = 2 ;

const int buttonPin = 2;  // the number of the pushbutton pin
const int joystickButton = 3 ;
const int ledPin[3][3] = {{4,5,6},{7,8,9},{10,11,12}}; // pins for Led output

// flag indicating if the game goes on or is finished 
int endOfGame = 0;
//tic tac toe grid 
int grid[3][3];

const int xAxis = A0;       // joystick X axis
const int yAxis = A1;       // joystick Y axis


//****** the setup function runs once when you press reset or power the board ****** 
//*                                                                                * 
//*                    Enable interrupts and initialize In/Out                     *
//*                                                                                *
//**********************************************************************************
void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  // initialize joystick X axis as input
  pinMode(xAxis, INPUT);
  // initialize joystick Y axis as input
  pinMode(yAxis, INPUT);
  // enable interupt when pushing the pushbutton 
  pinMode(buttonPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(buttonPin), InsertPosition, RISING);
  // enable interupt when pushing the pushbutton of joystick
  pinMode(joystickButton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(joystickButton), changePlayersTurn, RISING);
 

 
  //-----------set timer1 interrupt at 0.25Hz--------------
  TCCR1A = 0;// set entire TCCR1A register to 0 (increase every cycle)
  TCCR1B = 0;// same for TCCR1B prescaler ( manages how fast timer is counting)
  TCNT1  = 0;//initialize counter value to 0 (counts every time TCCR1A = TCCR1B)
  // set compare match register for 1hz increments
  OCR1A = 3906;// interrupt every 0.25s, 3906 = (16*10^6) / (4*1024) - 1 (must be <65536)
  // turn on CTC(Clear Timer Compare Match) mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler(one count every 1024 clock periods)
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
//-----------end set timer1 interrupt at 0.25Hz--------------

 // initialize the LED pins as an output:
  for(int i=0 ; i < 3 ; i++){
    for(int j=0 ; j < 3 ; j++){
      pinMode(ledPin[i][j], OUTPUT);
    }  
  }
  
  // initialize grid to empty
  for(int i=0 ; i < 3 ; i++){
    for(int j=0 ; j < 3 ; j++){
      grid[i][j] = 0;
    }  
  }

  endOfGame = 0 ;
}


//****** the loop function runs over and over again forever ****** 
//*                                                              * 
//*                     Do nothing but sleep                     *
//*                                                              *
//****************************************************************
void loop() {

  // There are five different sleep modes in order of power saving:
	// SLEEP_MODE_IDLE - the lowest power saving mode
	// SLEEP_MODE_ADC
	// SLEEP_MODE_PWR_SAVE
	// SLEEP_MODE_STANDBY
	// SLEEP_MODE_PWR_DOWN - the highest power saving mode
  set_sleep_mode(SLEEP_MODE_IDLE); // sleep but allow interrupts
	sleep_enable();

	// Now enter sleep mode.
	sleep_mode();
}



//****************************************************************
//*    Receive move from you based on joystick input and         *
//*    pushButtpn press, play the move of arduino and display    *
//*    the results on the 9 output leds                          * 
//****************************************************************
void InsertPosition(void) {
  if(endOfGame == 0){
    //read joystick x andy position when pushbutton is pressed
    int readXAxis = analogRead(xAxis); 
    int readYAxis = analogRead(yAxis);
    
    //joystich measurements
    /*(1023,0)    (1023,507)    (1023,1023)
                    ^
                    |
      (505,0) <-  (505,507)  -> (505,1023)      x  ^     y <-
                    |                              |
                    v  
      (0,0)       (0,507)       (0,1023)
    */

    if(abs(readXAxis-0) < abs(readXAxis-505) & abs(readXAxis-0) < abs(readXAxis-1023)){    //if joystick down position
      if(abs(readYAxis-0) < abs(readYAxis-507) & abs(readYAxis-0) < abs(readYAxis-1023)){  //if joystick down left
        if(grid[2][0] == 0)
          grid[2][0]= Omove;
        else 
          return;
      }
      else if(abs(readYAxis-507) < abs(readYAxis-0) & abs(readYAxis-507) < abs(readYAxis-1023)){  //if joystick down mid
        if(grid[2][1] == 0)
          grid[2][1]= Omove;
        else 
          return;
      }
      else { //if joystick down right
        if(grid[2][2] == 0)
          grid[2][2]= Omove;
        else 
          return;
      }
    }
    else if(abs(readXAxis-505) < abs(readXAxis-0) & abs(readXAxis-505) < abs(readXAxis-1023)){ //if joystick mid position
      if(abs(readYAxis-0) < abs(readYAxis-507) & abs(readYAxis-0) < abs(readYAxis-1023)){  //if joystick mid left
        if(grid[1][0] == 0)
          grid[1][0]= Omove;
        else 
          return;
      }
      else if(abs(readYAxis-507) < abs(readYAxis-0) & abs(readYAxis-507) < abs(readYAxis-1023)){  //if joystick mid mid
        if(grid[1][1] == 0)
          grid[1][1]= Omove;
        else 
          return;
      }
      else { //if joystick mid right
        if(grid[1][2] == 0)
          grid[1][2]= Omove;
        else 
          return;
      }
    }
    else{ //if joystick up position
      if(abs(readYAxis-0) < abs(readYAxis-507) & abs(readYAxis-0) < abs(readYAxis-1023)){  //if joystick up left
        if(grid[0][0] == 0)
          grid[0][0]= Omove;
        else 
          return;
      }
      else if(abs(readYAxis-507) < abs(readYAxis-0) & abs(readYAxis-507) < abs(readYAxis-1023)){  //if joystick up mid
        if(grid[0][1] == 0)
          grid[0][1]= Omove;
        else 
          return;
      }
      else { //if joystick up right
        if(grid[0][2] == 0)
          grid[0][2]= Omove;
        else 
          return;
      }
    }
    
    endOfGame = checkEndOfGame(); // Check if the player won after his move 

    if(endOfGame == 0){ // if game is font finished
      playXMove(); // call function for arduino to play a move
      endOfGame = checkEndOfGame(); // Check if arduino won after his move 
    }
    InsertMoveInLED(); //call function for inserting the moves in LED grid
  }
  else if(endOfGame == arduinoWon | endOfGame == youWon | endOfGame == noWin){ // if someone won refresh the game
    endOfGame = 0; // refresh the endOfGame flag
    // initialize grid to empty
    for(int i=0 ; i < 3 ; i++){
      for(int j=0 ; j < 3 ; j++){
        grid[i][j] = 0;
      }  
    }

    // if arduino plays next turn first
    if(whoStarts == arduinoStarts){
      playXMove(); // call function for arduino to play a move
      InsertMoveInLED(); //call function for inserting the moves in LED grid
    }
  }
}

//****************************************************************
//*                                                              * 
//*              Insert new moves in LED GRID                    *
//*                                                              *
//****************************************************************

void InsertMoveInLED(void) {
  for(int i=0 ; i < 3 ; i++){  // for all elements of tic tac toe grid 
    for(int j=0 ; j < 3 ; j++){
      if(grid[i][j] == Xmove) //dispable the x moves
        digitalWrite(ledPin[i][j], LOW); 
      else if(grid[i][j] == Omove)   // enable the o moves
        digitalWrite(ledPin[i][j], HIGH);
    }  
  }
}


//****************************************************************
//*                                                              * 
//*       ISR of TIMER blink LEDS for available positions        *
//*                                                              *
//****************************************************************
bool nextStateOfLeds = 0 ; // determinds if leds will be on or off on the next timer interrupt (mainly use to synchronise the LEds)

ISR(TIMER1_COMPA_vect){//timer1 interrupt triggered every 0.25 sec 
  for(int i=0 ; i < 3 ; i++){  // for all elements of tic tac toe grid 
    for(int j=0 ; j < 3 ; j++){
      if(grid[i][j] == 0) //if empty cell blink the LED
        digitalWrite(ledPin[i][j], nextStateOfLeds);  // change leds state
    }
  }
  nextStateOfLeds = !nextStateOfLeds;
}


//****************************************************************
//*                                                              * 
//*                   Check if Sombody won                       *
//*                                                              *
//****************************************************************


 int checkEndOfGame(void){
    if(grid[0][0] == grid[0][1] & grid[0][0] == grid[0][2] & grid[0][0]!=0){  // if the 1st row is complete by a player
      if(grid[0][0] == Xmove){  // if arduino completed the row
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][0] == Omove){ // if you completed the row
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[1][0] == grid[1][1] & grid[1][0] == grid[1][2] & grid[1][0]!=0){ // if the 2nd row is complete by a player
      if(grid[1][0] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[1][0] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[2][0] == grid[2][1] & grid[2][0] == grid[2][2] & grid[2][0]!=0){ // if the 3rd row is complete by a player
      if(grid[2][0] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[2][0] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[0][0] == grid[1][0] & grid[0][0] == grid[2][0] & grid[0][0]!=0){ // if the 1st column is complete by a player
      if(grid[0][0] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][0] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[0][1] == grid[1][1] & grid[0][1] == grid[2][1] & grid[0][1]!=0){ // if the 2nd column is complete by a player
      if(grid[0][1] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][1] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[0][2] == grid[1][2] & grid[0][2] == grid[2][2] & grid[0][2]!=0){ // if the 3rd column is complete by a player
     if(grid[0][2] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][2] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[0][0] == grid[1][1] & grid[0][0] == grid[2][2] & grid[0][0]!=0){ // if the 1st diagonal is complete by a player
      if(grid[0][0] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][0] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else if(grid[0][2] == grid[1][1] & grid[0][2] == grid[2][0] & grid[0][2]!=0){ // if the 2nd diagonal is complete by a player
      if(grid[0][2] == Xmove){
        Serial.println("ARDUINO Won");
        return arduinoWon;
      }
      else if(grid[0][2] == Omove){
        Serial.println("You Won");
        return youWon;
      }
    }
    else{ // if noone won 
      if(isMovesLeft(grid)){     // if there is an empty cell the game continues
      return 0 ; 
      }
      Serial.println("Its a Tie");
      return noWin;  // if all cells complete but no winner
    } 

    // Should never go there
    Serial.println("ERROR!!!!!");
    return errorFlag ;
 }


//****************************************************************
//*                                                              * 
//*          ISR when joystick pushbutton pushed for             *
//*                   changing the starting player               *
//*                                                              *
//****************************************************************
void changePlayersTurn(void){
  if(whoStarts == arduinoStarts){ // change the player who starts first next turn
    whoStarts = youStart;
  }
  else if(whoStarts == youStart){
    whoStarts = arduinoStarts;
    if(isGridEmpty(grid)){
      playXMove(); // call function for arduino to play a move
      InsertMoveInLED(); //call function for inserting the moves in LED grid
    }
  }
}


//************************************************************************************************************************************************************************************************
//****************************************************************
//*                                                              * 
//*       Algorithm for playing X moves as an Arduino            *
//*                                                              *
//****************************************************************
struct Move { 
    int row, col; 
};

 void playXMove(void){
    int tempBoard[3][3]; // create a temporary board
    // copy the real board to the temporary 
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            if (grid[i][j] == Xmove) { 
                tempBoard[i][j] = Xmove; 
            } 
            else if (grid[i][j] == Omove) { 
                tempBoard[i][j] = Omove; 
            } 
            else { 
                tempBoard[i][j] = 0; 
            } 
        } 
    } 
    struct Move bestMove = findBestMove(tempBoard); 
    grid[bestMove.row][bestMove.col] = Xmove;
 }




//****************************************************************
//*                                                              * 
//*        This will return the best possible move               *
//*                                                              *
//****************************************************************

struct Move findBestMove(int board[3][3]) 
{ 
    int bestVal = -1000; 
    struct Move bestMove; 
    bestMove.row = -1; 
    bestMove.col = -1; 
  
    // Traverse all cells, evaluate minimax function for 
    // all empty cells. And return the cell with optimal 
    // value. 
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) { 
            // Check if cell is empty 
            if (board[i][j] == 0) { 
                // Make the move 
                board[i][j] = Xmove; 
  
                // compute evaluation function for this 
                // move. 
                int moveVal = minimax(board, 0, false); 
  
                // Undo the move 
                board[i][j] = 0; 
  
                // If the value of the current move is 
                // more than the best value, then update 
                // best/ 
                if (moveVal > bestVal) { 
                    bestMove.row = i; 
                    bestMove.col = j; 
                    bestVal = moveVal; 
                } 
            } 
        } 
    } 
  
    return bestMove; 
} 
  

//****************************************************************
//*      This is the minimax function. It considers all          *
//*       the possible ways the game can go and returns          *
//*               the value of the board                         *
//****************************************************************

int minimax(int board[3][3], int depth, bool isMax) 
{ 
    int score = evaluate(board); 
  
    // If Maximizer has won the game return his/her 
    // evaluated score 
    if (score == 10) 
        return score; 
  
    // If Minimizer has won the game return his/her 
    // evaluated score 
    if (score == -10) 
        return score; 
  
    // If there are no more moves and no winner then 
    // it is a tie 
    if (isMovesLeft(board) == false) 
        return 0; 
  
    // If this maximizer's move 
    if (isMax) { 
        int best = -1000; 
  
        // Traverse all cells 
        for (int i = 0; i < 3; i++) { 
            for (int j = 0; j < 3; j++) { 
                // Check if cell is empty 
                if (board[i][j] == 0) { 
                    // Make the move 
                    board[i][j] = Xmove; 
                    int val = minimax(board, depth + 1, !isMax); 
                    if (val > best) { 
                        best = val; 
                    } 
  
                    // Undo the move 
                    board[i][j] = 0; 
                } 
            } 
        } 
        return best; 
    } 
  
    // If this minimizer's move 
    else { 
        int best = 1000; 
  
        // Traverse all cells 
        for (int i = 0; i < 3; i++) { 
            for (int j = 0; j < 3; j++) { 
                // Check if cell is empty 
                if (board[i][j] == 0) { 
                    // Make the move 
                    board[i][j] = Omove; 
  
                    // Call minimax recursively and choose 
                    int val = minimax(board, depth + 1, !isMax); 
                    if (val < best) { 
                        best = val; 
                    } 
                    // Undo the move 
                    board[i][j] = 0; 
                } 
            } 
        } 
        return best; 
    } 
} 


//****************************************************************
//*                                                              * 
//*              This is the evaluation function                 *
//*                                                              *
//****************************************************************

  
int evaluate(int b[3][3]) 
{ 
    // Checking for Rows for X or O victory. 
    for (int row = 0; row < 3; row++) { 
        if (b[row][0] == b[row][1] 
            && b[row][1] == b[row][2]) { 
            if (b[row][0] == Xmove) 
                return +10; 
            else if (b[row][0] == Omove) 
                return -10; 
        } 
    } 
  
    // Checking for Columns for X or O victory. 
    for (int col = 0; col < 3; col++) { 
        if (b[0][col] == b[1][col] 
            && b[1][col] == b[2][col]) { 
            if (b[0][col] == Xmove) 
                return +10; 
  
            else if (b[0][col] == Omove) 
                return -10; 
        } 
    } 
  
    // Checking for Diagonals for X or O victory. 
    if (b[0][0] == b[1][1] && b[1][1] == b[2][2]) { 
        if (b[0][0] == Xmove) 
            return +10; 
        else if (b[0][0] == Omove) 
            return -10; 
    } 
  
    if (b[0][2] == b[1][1] && b[1][1] == b[2][0]) { 
        if (b[0][2] == Xmove) 
            return +10; 
        else if (b[0][2] == Omove) 
            return -10; 
    } 
  
    // Else if none of them have won then return 0 
    return 0; 
} 


//****************************************************************
//*      This function returns true if there are moves           *
//*      remaining on the board. It returns false if             *
//*      there are no moves left to play.                        *
//****************************************************************

bool isMovesLeft(int board[3][3]) 
{ 
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            if (board[i][j] == 0) 
                return true; 
    return false; 
} 

//****************************************************************
//*      This function returns true if the board is Empty        *
//*                    It returns false if                       *
//*             there are inserted moves in board                *
//****************************************************************
bool isGridEmpty(int board[3][3]) 
{ 
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) 
            if (board[i][j] != 0) 
                return false; 
    return true; 
} 