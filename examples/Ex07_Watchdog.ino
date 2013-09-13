/*
  This is a simple led 13 blinker. It blinks led 13 on and off 1 second at a time.
  
  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

/*
  This example demonstrates the use of the ArcSched requeueFunction() method.
  
  The requeueFunction() method is an essential component of creating a watchdog timer.
  That is, you expect an event to occur on a regular basis, and the watchdog
  timer will trigger if you don't receive the event after some pre-determinted time.
  
  One way to implement this watchdog functionality is to create an ArcSched job that
  will run a function in <timeout>, and every time you detect the event you reschedule,
  or requeue, the job so its <timeout> starts again. 
  
  To use this example you need a tactile button attached to your Arduino, controlled
  by ArcButton. I'll assume you're already familiar (at least a bit) with ArcButton.
  
  Your job in this example is to press the tactile button at least every 10 seconds.
  If you don't then the watchdog timer will be called (after 15 seconds) and you'll receive
  a reminder to hit the button.
*/

#include <ArcSched.h>
#include <ArcButton.h>

ArcSched sched;
ArcButton button;
bool reminderFlag;

void setup() {
  Serial.begin(115220);
  Serial.println("");
  Serial.println("ArcSched requeueFunciton() example");
  
  sched.setup();
  button.setup(6, &sched);  // pin 6 is where my button is attached
  
  reminderFlag = true;
  
  // Start the watchdog timer. Set to go off in 1.5 times the expected interval.
  sched.registerFunction(reminder, (void *) 0, "reminder", SchedPriorityLow, sched.seconds(15));
  
  // Every time the button comes up call "phew"
  button.setButtonUpProcessor(phew, (void *) 0);
}

// If this is called then we have not received a button press (release really) within
// our expected timeout interval.
void reminder(SchedFunctionDetail *fd, void *unusedArg) {
  if(reminderFlag == true) {
    Serial.println("Don't forget to hit the button...");
    reminderFlag = false;
  }
}

// Reset the watchdog timer.
void phew(void *unusedArg) {
  if(reminderFlag == false) {
    Serial.println("Thanks");
    reminderFlag = true;
  }
  
  // This is the heart of the example. It asks ArcSched to reschedule the "reminder" job
  // to trigger in 15 seconds from now, essentially pushing the trigger forward in time.
  sched.requeueFunction("reminder");
}

void loop() {
  button.loop();
  sched.loop();
}
