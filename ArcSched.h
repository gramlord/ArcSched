/*
  ArcSched - a single threaded, millisecond-based function scheduler

  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.

  $Id$
*/

#ifndef _ArcSched_h
#define _ArcSched_h

#include <Arduino.h>
#include <sys/types.h>

// The struct which is used to communicate between the scheduler and the
// called functions. When a function is called this is used to send
// information to the function from the scheduler and for the function to send
// changes back to the scheduler.
struct SchedFunctionDetail {
  // details that can go function to scheduler and scheduler to function
  uint8_t priority;
  bool priorityFlag;
  uint32_t interval;
  bool intervalFlag;
  
  // details that go function to scheduler
  bool removeFlag;
  
  // details that go scheduler to function
  uint32_t overshoot;
};

// The struct used internally by the scheduler to manage functions
struct SchedDetail {
  void (*function)(SchedFunctionDetail *, void *);
  void *argument;
  uint8_t priority;
  uint32_t lastRunTime;
  uint32_t interval;
  uint32_t runCount;
  uint32_t overshootTotal;
  char *name;
  SchedDetail *next;
};

// The scheduler priorities. Lower number means higher priority
#define SchedPriorityHigh 0
#define SchedPriorityMedium 1
#define SchedPriorityLow 2

class ArcSched {
  public:
    void setPriority(SchedFunctionDetail *fd, uint8_t priority);
    void setInterval(SchedFunctionDetail *fd, uint32_t interval);
    void deregister(SchedFunctionDetail *fd);
    uint32_t seconds(uint32_t s);
    uint32_t minutes(uint32_t s);
    uint32_t hours(uint32_t s);
    uint32_t days(uint32_t s);
    void registerFunction(void (*function)(SchedFunctionDetail *, void *), void *, char *, uint8_t, uint32_t);
    bool deregisterFunction(char *);  // not to be confused with deregister. this is for user's to use.
    void setup();
    void loop();

  private:
    SchedDetail *list;
};

#endif
