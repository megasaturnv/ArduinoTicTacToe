# Fork of ArduinoTicTacToe
https://github.com/klauscam/ArduinoTicTacToe

# Changes and additional features
## Version 1
* Ran code through the Arduino IDE auto formatter.
* Added drawing grid number layout ( void drawNumberedBoard() )
* Added invalid player type detection (O / X). Program will randomly choose X / O if player enters an invalid selection
* Added "thinking..." message if the player enters a valid number
* Added an error message if the player enters an invalid grid number
* Changed "move >= 9" to "move > 8" and changed variable type of 'move' and 'player' from int to byte.

Reminder to self: may also change ((move > 8 || move < 0) || board[move] != 0) to (move > 8 || move < 0 || board[move] != 0) since the additional brackets are unnecessary.

## Version 2
* Added code to say if you are O or X
* Remapped tic-tac-toe grid to match a keyboard's keypad layout:
```
 0 | 1 | 2        7 | 8 | 9
---+---+---      ---+---+---
 3 | 4 | 5  -->   4 | 5 | 6
---+---+---      ---+---+---
 6 | 7 | 8        1 | 2 | 3
```
* Updated drawNumberedBoard() to match grid number changes
* Added physical button support on pins 4-12
* Added more data validation for moves received via the serial port
* Fixed selecting random player type ( O / X ) if the player enters an invalid number. In the previous version the Arduino would always pick X
* Fixed bug with 'Thinking...' message
* Renamed TicTacToe.ino to ArduinoTicTacToe.ino

## To do
* Add 'Would you like to play again?' message
* Say how long the Arduino was thinking for in milliseconds
