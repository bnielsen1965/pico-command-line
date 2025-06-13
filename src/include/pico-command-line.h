
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/mutex.h"

#ifndef PICO_COMMAND_LINE_H
#define PICO_COMMAND_LINE_H

void initPicoStdio ();

void picoConsole ();
void initConsole (char * lineBuffer, char * commandLine);
size_t appendChar(char * buffer, char c);
size_t deleteChar(char * buffer);
size_t clearBuffer (char * buffer);
void flushInput ();



#ifndef COMMAND_LINE_LEN
#define COMMAND_LINE_LEN 80
#endif

// access to externally defined variables
extern semaphore_t semCommand;
extern char commandLine[];


struct command_t {
    const char *name;
    void (*handler)(char *cmdLine);
};

// task to process commands
void commandProcessor ();
void initCommandProcessor (struct command_t *commands, int size);

#endif
