# Arduino TicTacToe Game

This project implements a TicTacToe game using an Arduino Uno R3 microcontroller. 
The game allows a player to compete against the Arduino, which uses an algorithm to determine its moves. 
The game board is visually represented by 9 LEDs, and the player interacts with the game using a joystick sensor and a pushbutton. 
The game can be reset, and the starting player can be changed using controls integrated into the system.

## Table of Contents
- [Features](#features)
- [Components](#components)
- [How It Works](#how-it-works)
- [Installation & Setup](#installation--setup)
- [How to Play](#how-to-play)
- [Project Circuit](#project-circuit)

## Features

- **Interactive Gameplay**: Play TicTacToe against the Arduino. 
- **LED Game Board**: The game board is represented using 9 LEDs.
- **Joystick Control**: Move the cursor and select positions using a joystick.
- **Automatic Arduino Moves**: Arduino uses an algorithm to determine its optimal moves.
- **Dynamic Start Control**: Easily switch who plays first (user or Arduino) using the joystick's integrated pushbutton.
- **Game Reset**: The game resets on pressing the main pushbutton after each match.
- **Blinking LEDs**: Empty board positions are indicated by blinking LEDs, updating every half second.
- **Game Result Feedback**: Arduino communicates the game's outcome via a serial interface.

## Components

- **Arduino Uno R3**
- **9 LEDs** (to represent the game board)
- **Joystick Module** (with integrated pushbutton)
- **Pushbutton** (for move selection and game reset)
- **Resistors** (for LED and button connections)
- **Breadboard & Connecting Wires**

## How It Works

1. **User Input**: The player uses the joystick to select a position on the 3x3 grid. Moving the joystick changes the cursor, and pressing the pushbutton selects the position.
2. **LED Representation**: 
   - **Player's Move**: When the player selects a position, the corresponding LED turns ON to indicate "O".
   - **Arduino's Move**: The Arduino selects a position based on an optimal algorithm and the corresponding LED turns OFF to indicate "X".
   - **Blinking LEDs**: Available positions blink every 0.5 seconds, making it easy to identify where you can play.
3. **Game Result**: After each turn, if there is a win or a tie, the Arduino pauses the game and displays the result through the serial interface.
4. **Reset & New Game**: Pressing the main pushbutton resets the game board, and a new match starts.
5. **Switching Starting Player**: By pressing the joystick’s integrated button, the player who starts the next game is switched (from user to Arduino and vice versa).

## Installation & Setup

1. **Connect the Components**:
   - Connect 9 LEDs to the digital pins of the Arduino.
   - Connect the joystick module to the analog pins.
   - Connect the pushbutton to a digital pin.
   - Ensure appropriate connections for resistors to prevent damage to components.
2. **Arduino IDE**:
   - Open the Arduino IDE and connect your Arduino Uno R3.
   - Load the `tictactoe.ino` file from this repository into the IDE.
   - Adjust the pin configurations in the code if necessary.
3. **Upload the Code**:
   - Verify the code and upload it to the Arduino.
   - Open the Serial Monitor to see game messages and results.

## How to Play

1. **Starting the Game**:
   - On power-up, the game begins with the player’s turn (unless changed by the joystick pushbutton).
   - Available moves are represented by blinking LEDs.
2. **Making a Move**:
   - Use the joystick to navigate to your desired position.
   - Press the main pushbutton to place your move ("O") on the selected position.
3. **Arduino’s Turn**:
   - After the player's move, the Arduino determines its optimal move and updates the board by disabling the corresponding LED.
4. **Winning the Game**:
   - The game continues until either the player or Arduino wins, or if there is a tie.
   - The result is displayed via the Serial Monitor.
5. **Reset & Next Game**:
   - Press the main pushbutton to reset the game and start a new round.
   - To change who plays first, press the joystick’s integrated pushbutton before starting the next game.

## Project Circuit

![alt text](https://github.com/akourkoulos/ARDUINO-Tic-Tac-Toe)
