# Fork of ArduinoTicTacToe
https://github.com/klauscam/ArduinoTicTacToe

## Added additional features

Ran code through Arduino IDE auto formatter. Added: drawing grid number layout (void drawNumberedBoard() ), invalid player number detection, "thinking..." message if the player enters a valid number, an error message if the player enters an invalid number. Changed "move >= 9" to "move > 8" and changed variable type of 'move' and 'player' from int to byte.

Reminder to self: may also change ((move > 8 || move < 0) || board[move] != 0) to (move > 8 || move < 0 || board[move] != 0) since the additional brackets are unnecessary.
