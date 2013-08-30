/*
  ArcSched - a single threaded, millisecond-based function scheduler

  Copyright 2013 Graeme Elsworthy, Arcsine Pty Ltd, graeme@arcsine.com.au

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.

  $Id$
*/

#include "ArcSched.h"

// Helper routines for updating a SchedFunctionDetail.
void ArcSched::setPriority(SchedFunctionDetail *fd, uint8_t priority) {
  fd->priority = priority;
  fd->priorityFlag = true;
}

void ArcSched::setInterval(SchedFunctionDetail *fd, uint32_t interval) {
  fd->interval = interval;
  fd->intervalFlag = true;
}

void ArcSched::deregister(SchedFunctionDetail *fd) {
  fd->removeFlag = true;
}

// Converters from seconds, minutes etc to milliseconds.
uint32_t ArcSched::seconds(uint32_t s) {
  return 1000L * s;
}

uint32_t ArcSched::minutes(uint32_t m) {
  return 60000L * m;
}

uint32_t ArcSched::hours(uint32_t h) {
  return 3600000L * h;
}

uint32_t ArcSched::days(uint32_t d) {
  return 86400000L * d;
}

// Register a function for use with the scheduler.
void ArcSched::registerFunction(void (*function)(SchedFunctionDetail *, void *), void *argument, char *name, uint8_t priority, uint32_t interval) {
  SchedDetail *p;
  uint8_t i;
  static char *noMem = "ArcSched: Insuficient memory.";

  p =(SchedDetail *) malloc(sizeof(SchedDetail));
  if(p == (SchedDetail *) 0) {
    Serial.println(noMem);
    return;
  }
  p->name = (char *) malloc(strlen(name) + 1);
  if(p->name == (char *) 0) {
    Serial.println(noMem);
    return;
  }
  
  p->function = function;
  p->argument = argument;
  p->priority = priority;
  p->interval = interval;
  strcpy(p->name, name);
  p->lastRunTime = millis();

  // add it to the list of scheduled jobs
  p->next = list;
  list = p;
}

// Setup the scheduler.
void ArcSched::setup() {
  list = (SchedDetail *) 0;
}

// The main scheduler processor.
void ArcSched::loop() {
  SchedDetail *p;
  SchedDetail **pp;
  SchedFunctionDetail fd;
  uint32_t prevInterval;

  for(uint8_t priority = SchedPriorityHigh; priority <= SchedPriorityLow; priority++) {
    for(p = list; p != (SchedDetail *) 0; p = p->next) {
      if((p->function != (void (*)(SchedFunctionDetail *, void *)) 0) && (p->priority == priority)) {
        if((millis() - p->lastRunTime) >= p->interval) {
          // Fill out the function detail struct.
          fd.priority = p->priority;
          fd.priorityFlag = false;
          fd.interval = p->interval;
          fd.intervalFlag = false;
          fd.removeFlag = false;

          // Call the function.
          (*p->function)(&fd, p->argument);

          // Deregister the entry if requested.
          if(fd.removeFlag) {
            p->function = (void (*)(SchedFunctionDetail *, void *)) 0;
          } else {
            // Update priority and interval if requested.
            if(fd.priorityFlag) p->priority = fd.priority;
            prevInterval = p->interval;
            if(fd.intervalFlag) p->interval = fd.interval;
            p->lastRunTime += prevInterval;
          }
        }
      }
    }
  }
  
  // Clean up the list by removing deregistered entries.
  for(pp = &list; *pp != (SchedDetail *) 0; ) {
    if((*pp)->function == (void (*)(SchedFunctionDetail *, void *)) 0) {
      p = *pp;
      *pp = p->next;
      free(p->name);
      free(p);
    } else {
      pp = &((*pp)->next);
    }
  }
}

// Deregister a function, given by it's name.
bool ArcSched::deregisterFunction(char *functionName) {
  SchedDetail *p;
  
  for(p = list; p != (SchedDetail *) 0; p = p->next) {
    if((p->function != (void (*)(SchedFunctionDetail *, void *)) 0) && (strcmp(p->name, functionName) == 0)) {
      // For now just clear the function pointer. it will be cleaned up at the next invocation of loop().
      p->function = (void (*)(SchedFunctionDetail *, void *)) 0;
      return true;
    }
  }

  return false;
}
