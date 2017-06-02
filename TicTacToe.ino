int difficulty = 8;

void setup() {
  Serial.begin(9600);
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
  Serial.println("Grid number layout");
  Serial.println(" 0 | 1 | 2 ");
  Serial.println("---+---+---");
  Serial.println(" 3 | 4 | 5 ");
  Serial.println("---+---+---");
  Serial.println(" 6 | 7 | 8 ");
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
}

void playerMove(int board[9]) {
  byte move = 0;
  do {
    Serial.print("Input move ([0..8]): ");

    while (Serial.available() == 0) {
      delay(100);
    }
    move = Serial.parseInt();

    if ((move > 8 || move < 0) || board[move] != 0) {
      Serial.println("Invalid move");
    } else {
      Serial.println(String(move) + ". Thinking...");
    }

  } while ((move > 8 || move < 0) || board[move] != 0);
  board[move] = -1;
}

void loop() {
  int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  Serial.println("====================================");
  drawNumberedBoard();
  Serial.println("Would you like to play X(1) or O(2)?");

  while (Serial.available() == 0) {
    delay(100);
  }

  byte player = Serial.parseInt();
  if (player != 1 && player != 2) {
    randomSeed(millis());
    player = random(1, 2);
    Serial.println("Invalid player number received. You will play as number " + String(player));
  }

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
}
