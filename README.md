
# Whackamole
### Course project EE337

A fun game inspired from the [Whack-a-mole](https://en.wikipedia.org/wiki/Whac-A-Mole) arcade game.  
 
The player will type onto the keyboard connected to the custom 8051 micro-controller developed by IIT Bombay  

## Sailent features of the game
* The duration of the game is 10 seconds.
* Display the current score as well as the highest score
* Generated the psedo randomness of the mole using the concept of [Linear feedback shift register](https://en.wikipedia.org/wiki/Linear-feedback_shift_register)

## Files
* [main.c](https://github.com/nirmalshah123/Whackamole/blob/main/main.c) - Main program.
* [lcd.h](https://github.com/nirmalshah123/Whackamole/blob/main/lcd.h) - Driver code for lcd
* [serial.c](https://github.com/nirmalshah123/Whackamole/blob/main/serial.c) - Driver code for communicating with the USB to UART IC.
