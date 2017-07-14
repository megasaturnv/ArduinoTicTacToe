#define BAUD_RATE 9600

int difficulty = 8;

//Byte array to convert keyboard keypad numbers to TicTacToe.ino's original number layout
//keypadConversion[0] = 255 because 0 is not part of the 3x3 keypad grid on a keyboard
//255 allows the program to show an error message to the user later in the code
const byte keypadConversion[] = {255, 6, 7, 8, 3, 4, 5, 0, 1, 2};
/*
  Old TicTacToe layout:    New layout (Matches keyboard keypad):
  0 | 1 | 2                7 | 8 | 9
  ---+---+---              ---+---+---
  3 | 4 | 5                4 | 5 | 6
  ---+---+---              ---+---+---
  6 | 7 | 8                1 | 2 | 3
*/

const byte buttonPins[] = {12, 11, 10, //List of pins connected to 9 physical buttons, Arranged into a 9x9 grid
                           4,  5,  6,  //e.g. top-right tic-tac-toe button is connected to pin 10
                           7,  8,  9
                          }; //These pins are pulled high with the Arduino's internal pull-up resistors

void setup() {
  Serial.begin(BAUD_RATE);
  //Set pinMode of all pins in buttonPins[] to INPUT_PULLUP
  //Loop for the amount of values in buttonPins
  //In this case, sizeof(byte) == 1 so it's not really needed.
  //This is better practice when needing to know the amount of items in an array of values which take up more than 1 byte each (e.g. an int or long array).
  for (byte i = 0; i < (sizeof(buttonPins) / sizeof(byte)); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  drawNumberedBoard();
}

bool digitalReadDebounce(int pin) { //Digital read a pin with software debounce
  if (digitalRead(pin) == LOW) {
    delay(50);
    bool debounceRunning = true;
    while (debounceRunning) {
      if (digitalRead(pin) == HIGH) {
        debounceRunning = false;
      }
    }
    delay(50);
    return true;
  }
  return false;
}

char displayChar(int c) {
  switch (c) {
    case -1:
      return 'X';
    case 0:
      return ' ';
    case 1:
      return 'O';
  }
}

void drawNumberedBoard() {
  Serial.println("Grid number layout:");
  Serial.println(" 7 | 8 | 9 ");
  Serial.println("---+---+---");
  Serial.println(" 4 | 5 | 6 ");
  Serial.println("---+---+---");
  Serial.println(" 1 | 2 | 3 ");
}

void draw(int board[9]) {
  Serial.print("\n "); Serial.print(displayChar(board[0])); Serial.print(" | "); Serial.print(displayChar(board[1])); Serial.print(" | "); Serial.print(displayChar(board[2])); Serial.println(" ");
  Serial.println("---+---+---");
  Serial.print(" "); Serial.print(displayChar(board[3])); Serial.print(" | "); Serial.print(displayChar(board[4])); Serial.print(" | "); Serial.print(displayChar(board[5])); Serial.println(" ");
  Serial.println("---+---+---");
  Serial.print(" "); Serial.print(displayChar(board[6])); Serial.print(" | "); Serial.print(displayChar(board[7])); Serial.print(" | "); Serial.print(displayChar(board[8])); Serial.println(" ");
}

int win(const int board[9]) {
  //list of possible winning positions
  unsigned wins[8][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}, {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, {0, 4, 8}, {2, 4, 6}};
  int winPos;
  for (winPos = 0; winPos < 8; ++winPos) {
    if (board[wins[winPos][0]] != 0 && board[wins[winPos][0]] == board[wins[winPos][1]] && board[wins[winPos][0]] == board[wins[winPos][2]])
      return board[wins[winPos][2]];
  }
  return 0;
}

int minimax(int board[9], int player, int depth) {
  //check the positions for players
  int winner = win(board);
  if (winner != 0) return winner * player;

  int move = -1;
  int score = -2;
  int i;
  for (i = 0; i < 9; ++i) {

    if (board[i] == 0) {
      board[i] = player;
      int thisScore = 0;
      if (depth < difficulty) {
        thisScore = -minimax(board, player * -1, depth + 1);
      }

      if (thisScore > score) {
        score = thisScore;
        move = i;
      }
      //choose the worst move for opponent
      board[i] = 0;
    }
  }
  if (move == -1) return 0;
  return score;
}

void computerMove(int board[9]) {
  unsigned long computerMoveStartTime = millis();
  Serial.println("Thinking..."); //Say when the computer is thinking (without it the program looks like it has crashed)
  int move = -1;
  int score = -2;
  int i;
  for (i = 0; i < 9; ++i) {
    if (board[i] == 0) {
      board[i] = 1;
      int tempScore = -minimax(board, -1, 0);
      board[i] = 0;
      if (tempScore > score) {
        score = tempScore;
        move = i;
      }
    }
  }
  //returns a score based on minimax tree at a given node.
  board[move] = 1;

  Serial.print("Arduino was thinking for "); Serial.print(millis() - computerMoveStartTime); Serial.println(" ms");
}

void playerMove(int board[9]) {
  byte move = 0;
  do {
    Serial.println("Input move ([1..9]): ");

    bool waitingForPlayerMove = true;
    while (waitingForPlayerMove) {
      if (Serial.available() > 0) {
        byte moveKeypadNum = Serial.parseInt();
        if (moveKeypadNum <= 9) { //if keypad number is less than or equal to 9, the move is valid. 'moveKeypadNum' cannot be negative ( < 0 ) because it is a 'byte' type not 'int'.
          //If the user types '0', keypadConversion[0] = 255 which is also an invalid move (So the error message will appear)
          move = keypadConversion[moveKeypadNum];
          waitingForPlayerMove = false;
        } else {
          move = 255; //Set 'move' to 255 which will be picked up later as an invalid move
        }
      }
      for (byte i = 0; i <= 8; i++) {
        if (digitalReadDebounce(buttonPins[i])) {
          move = i;
          waitingForPlayerMove = false;
        }
      }
    }

    if ((move > 8 || move < 0) || board[move] != 0) {
      Serial.println("Invalid move"); //Say if the player's move was invalid
    }

  } while ((move > 8 || move < 0) || board[move] != 0);
  board[move] = -1;

  //Reverse lookup in keypadConversion[] array
  Serial.print("You played X in position ");
  for (int i = 1; i <= 9; i++) { //For position [1] to [9] in keypadConversion[] array... (the first position, [0] is not used in keypadConversion[])
    if (move == keypadConversion[i]) { //If the selected move == the current value selected in the array
      Serial.println(i); //Print where we are in the array ( i )
      break; //There's no need to continue the for loop, so break out of it
    }
  }
}

void loop() {
  int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  Serial.println("====================================");
  Serial.println("Would you like to play 1st (type '1' or press left buttons) or 2nd(type '2' or press right buttons)?");

  byte player;
  bool waiting = true;
  while (waiting) {
    if (Serial.available() > 0) {
      player = Serial.parseInt();
      waiting = false;
    }
    //Loop for the amount of values in buttonPins
    if (digitalReadDebounce(buttonPins[0]) || digitalReadDebounce(buttonPins[3]) || digitalReadDebounce(buttonPins[6])) { //If player presses one of the left buttons
      player = 1;
      waiting = false;
    } else if (digitalReadDebounce(buttonPins[2]) || digitalReadDebounce(buttonPins[5]) || digitalReadDebounce(buttonPins[8])) { //If player presses one of the right buttons
      player = 2;
      waiting = false;
    }
  }
  if (player != 1 && player != 2) { //If user doesn't select a valid player type
    Serial.print("Invalid player number received. Picking at random... "); //Notify the player
    randomSeed(millis()); //Set random seed to millis()
    player = random(1, 3); //Set player type as '1' or '2' randomly. '1' = 'X' and '2' = 'O'.
    /*random(1, 3); is used because the max range of random is (3 - 1 = 2)
      To quote the Arduino docs: (https://www.arduino.cc/en/Reference/Random)

      Parameters
          min - lower bound of the random value, inclusive (optional)
          max - upper bound of the random value, exclusive

      Returns
          a random number between min and max-1 (long) */
  }
  //Say what the player is playing as
  Serial.print("You will play: ");
  Serial.print(player == 1 ? "1st" : "2nd"); //If player == 1, print "X". Else, print "O"
  Serial.println(" as 'X'");

  unsigned turn;
  for (turn = 0; turn < 9 && win(board) == 0; ++turn) {
    if ((turn + player) % 2 == 0)
      computerMove(board);
    else {
      draw(board);
      playerMove(board);
    }
  }
  switch (win(board)) {
    case 0:
      Serial.println("It's a draw.");
      break;
    case 1:
      draw(board);
      Serial.println("You lose. Arduino wins!");
      break;
    case -1:
      Serial.println("You win!");
      break;
  }

  Serial.println("Type anything or press any button to continue\n");
  waiting = true;
  while (waiting) {
    if (Serial.available() > 0) { //If user typed anything
      waiting = false;
      delay(10);
      while (Serial.available() > 0) { //Clear out the serial buffer
        char t = Serial.read();
      }
    }
    for (byte i = 0; i <= 8; i++) {
      if (digitalReadDebounce(buttonPins[i])) { //If user pressed a button
        waiting = false;
      }
    }
  }
  delay(500); //Extra delay to make sure user has not debounced the button a second time (after digitalReadDebounce())
}
