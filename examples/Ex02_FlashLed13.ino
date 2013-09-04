/*
  This is a simple led 13 flasher. It flashes led 13 on for 50ms then off for 1950ms.
  It is a slight variation to blinkLed13 in that the interval is varied (either 50ms
  or 1950ms) with eaach call to flashLed13(). See Ex01_BlinkLed13.ino more details.
  
  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

#include <ArcSched.h>

ArcSched sched;

void flashLed13(SchedFunctionDetail *fd, void *arg) {
  digitalWrite(13, ! digitalRead(13));
  
  // This changes the call interval. In this case to either 50ms or 1950ms depending
  // on the value of led 13.
  sched.setInterval(fd, digitalRead(13) == HIGH ? 50 : 1950);
}

void setupFlashLed13() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  (void) sched.registerFunction(flashLed13, (void *) 0, "flashLed13", SchedPriorityLow, 50);
}

void setup() {
  sched.setup();
  setupFlashLed13();
}

void loop() {
  sched.loop();
}
