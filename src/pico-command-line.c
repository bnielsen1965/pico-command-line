
#include "pico-command-line.h"


/////////////////////
// console methods //
/////////////////////

// console process/task to handle stdin
void picoConsole (void *args) {
  int16_t c; // must be 16 bit to get the error codes
  char lineBuffer[COMMAND_LINE_LEN + 1]; // define input buffer
  size_t cursor = 0;
  // initialize console
  initConsole (lineBuffer, commandLine);

  // console process/task loop
  for (;;) {
    c = getchar();
    // some characters will not be processed
    if (c <= 0x07) continue;
    if (c >= 0x09 && c <= 0x0c) continue;
    if (c >= 0x0e && c <= 0x1a) continue;
    if (c >= 0x1c && c <= 0x1f) continue;
    // handle escape sequence
    if (c == 0x1b) {
      flushInput();
      continue;
    }
    // handle enter
    if (c == 0x0d) {
      // copy input line buffer to command line buffer
      memcpy(commandLine, lineBuffer, COMMAND_LINE_LEN);
      // clear input line buffer and send newline to terminal
      cursor = clearBuffer(lineBuffer);
      printf("\n");
      // release command semaphore to enable processing of command line
      sem_release(&semCommand);
      continue;
    }
    // handle backspace / del
    if (c == 0x7f || c == 0x08) {
      if (cursor == 0) continue;
      cursor = deleteChar(lineBuffer);
      // use backspace, space, backspace to erase character on terminal
      printf("%c%c%c", 0x08, ' ', 0x08);
      continue;
    }
    // don't allow characters when at end of buffer
    if (cursor == COMMAND_LINE_LEN) continue;
    // handle character input
    cursor = appendChar(lineBuffer, c);
    printf("%c", c);
  }
}


// initialize pico stdio, called by the application to initialize stdio before starting console process.
void initPicoStdio () {
  stdio_init_all();
  stdio_flush();
  setbuf(stdout, NULL); // make sure character input is buffered
}


// initialize console, called by the console process on startup.
void initConsole (char * lineBuffer, char * commandLine) {
  // clear line buffers and set end of line nulls
  clearBuffer(commandLine);
  commandLine[COMMAND_LINE_LEN] = '\0';
  clearBuffer(lineBuffer);
  lineBuffer[COMMAND_LINE_LEN] = '\0';
  // flush all input
  flushInput();
  // // TODO sem_init should be in command processor init function.
  // sem_init(&semCommand, 0, 1);
}


// clear contents of the specified buffer
size_t clearBuffer (char * buffer) {
  memset(buffer, '\0', COMMAND_LINE_LEN);
  return strlen(buffer);
}

// append a character to the input buffer and return new length
size_t appendChar (char * buffer, char c) {
  if (strlen(buffer) == COMMAND_LINE_LEN) return COMMAND_LINE_LEN;
  buffer[strlen(buffer)] = c;
  return strlen(buffer);
}

// delete character from input buffer and return length
size_t deleteChar (char * buffer) {
  if (strlen(buffer) == 0) return 0;
  buffer[strlen(buffer) - 1] = '\0';
  return strlen(buffer);
}

// flush stdio input
void flushInput () {
  int16_t c; // must be 16 bit to capture error codes
  do {
    // read characters until nothing left to read
    c = getchar_timeout_us(500);
  } while (c != PICO_ERROR_TIMEOUT && c != PICO_ERROR_GENERIC);
}




///////////////////////////////
// command processor process //
///////////////////////////////

// commands list from main application
int processorCommandsSize;
struct command_t *processorCommands;

// semaphore to block command processing until console permits it and char array to hold command line input
semaphore_t semCommand;
char commandLine[COMMAND_LINE_LEN + 1];



// command processor process/task to process command line input
void commandProcessor () {
  // pointer to unknown command handler if found
  struct command_t *unknownCommand = NULL;
  for (;;) {
    // block until console permits command processing
    sem_acquire_blocking(&semCommand);
    if (strlen(commandLine) == 0) goto continue1;
    // find command handler
    for (int i = 0; i < processorCommandsSize; i++) {
      if (processorCommands[i].name == NULL) {
        if (!unknownCommand) unknownCommand = &processorCommands[i];
        continue;
      }
      // test if command matches and call handler
      int clLen = strlen(commandLine);
      int pcLen = strlen(processorCommands[i].name);
      if (0 == strncmp(commandLine, processorCommands[i].name, pcLen) && (pcLen == clLen || commandLine[pcLen] == ' ')) {
        processorCommands[i].handler(commandLine);
        goto continue1;
      }
    }

    // handle unknown command
    if (unknownCommand != NULL) unknownCommand->handler(commandLine);
    else printf("Unknown command %s.\n", commandLine);


    continue1:;
  }
}


// initialize the command processor with a list of commands
void initCommandProcessor (struct command_t *commands, int size) {
  processorCommandsSize = size;
  processorCommands = commands;
  sem_init(&semCommand, 0, 1);
}
