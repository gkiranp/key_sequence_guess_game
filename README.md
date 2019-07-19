# key sequence guess game
A simple game that generates 3 sequence and allows user to guess it by inputs

# Problem statement
Given a system with the following:

 * LED 1 - which can be {red, green, orange}
 * LED 2 - which can be {red, green, orange}
 * LED 3 - which can be {red. green, orange}
 * Button A
 * Button B
 * Button C

_Design & Implement game that fulfills these requirements:_

  The system implements a game in which the user has to guess a sequence of three button presses.
  The sequence can contain any combination, e.g. BAC, CCB, AAA.
  The LEDs should always represent the result of the last 3 button presses.
   * LED 3 will always represent the most recent button event
   * LED 2 the one before that
   * LED 1 the one before that

  Red indicates that the button pressed was wrong for this position, and does not appear in a different position.
  Orange indicates that the button pressed was wrong for this position, but it does appear in a different position.
  Green indicates that the button pressed was correct for this position.

  Only "button down" events are generated, so there is no need to deal with held buttons.
  The button sequence is randomly generated on system start-up. 
  After a 3-green result, another random sequence is generated.
