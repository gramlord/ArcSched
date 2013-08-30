/*

  This is a simple led flasher. It flashes the led, passed in as an argument, on for
  50ms then off for 1950ms. It is a slight variation to flashLed13 in that the led pin
  to use it passed in as an argument. See Ex01_BlinkLed13.ino and Ex02_FlashLed.ino
  for more details.
  
  Uses for this include, but are not limited to, passing in pins, pointers to
  struct's or (and this is the killer usage) a class instances "this".
  
  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
  
  $Id$

*/

#include <ArcSched.h>

ArcSched sched;

void flashLed(SchedFunctionDetail *fd, void *arg) {
  int pin = (int) arg;
  
  digitalWrite(pin, ! digitalRead(pin));
  sched.setInterval(fd, digitalRead(pin) == HIGH ? 50 : 1950);
}

void setupFlashLed13() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  
  // Pass in (pin) 13 as the argument to be passed to the flashLed() funciton.
  sched.registerFunction(flashLed, (void *) 13, "flashLed", SchedPriorityLow, 50);
}

void setup() {
  sched.setup();
  setupFlashLed13();
}

void loop() {
  sched.loop();
}
