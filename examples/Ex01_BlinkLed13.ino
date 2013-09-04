/*
  This is a simple led 13 blinker. It blinks led 13 on and off 1 second at a time.
  
  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

#include <ArcSched.h>

ArcSched sched;

// This function will be called by the scheduler every second, and it simply
// toggles led13.
void blinkLed13(SchedFunctionDetail *fd, void *arg) {
  digitalWrite(13, ! digitalRead(13));
}

// This function sets led13 to the correct state and registers the blinkLed13() function
// with the scheduler, to be called every second. The string "blinkLed13" is a name label
// so you can refer to this registration in other scheduler functions (not used here).
// The (void *) 0 is passed in as "arg" in blinkLed13(), not used in this demo.
void setupBlinkLed13() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  (void) sched.registerFunction(blinkLed13, (void *) 0, "blinkLed13", SchedPriorityLow, sched.seconds(1));
}

// In the setup() function you setup the scheduler first and then the blinker.
void setup() {
  sched.setup();
  setupBlinkLed13();
}

// In loop() you must always call sched.loop() - this is where all the functions are
// called from.
void loop() {
  sched.loop();
}
