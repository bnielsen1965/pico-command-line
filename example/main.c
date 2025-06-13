
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"

#include "pico-command-line.h"

#define LED_PIN 17

// help message
const char * HELP = 
  "Pi Pico Basic Command Prompt - A simple 80 character command\n"
  "line buffer used to control the Pi Pico\n"
  "Commands:\n"
  "help - Display this help message\n"
  "bootrom - Reset and boot into BOOTSEL ROM\n"
  "led on|off - Toggle the LED\n";



// declare method
void init ();
void commandUnknown (char *cmdLine);
void commandHelp (char *cmdLine);
void commandBootrom (char *cmdLine);
void commandLED (char *cmdLine);


// array of command methods
struct command_t commands[] = {
  {NULL, &commandUnknown},
  {"help", &commandHelp},
  {"bootrom", &commandBootrom},
  {"led", &commandLED}
};


// main application entry point
int main()
{
  init();
  multicore_launch_core1(commandProcessor);
  picoConsole();
}


// initialize application
void init () {
  // initialize LED pin and set direction to output
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
  gpio_put(LED_PIN, 0);
  // initialize standard input/output for pico console
  initPicoStdio();
  // initialize command processor with commands array and size of array
  initCommandProcessor(commands, sizeof(commands) / sizeof(commands[0]));
}


// command led
void commandLED (char *cmdLine) {
  // duplicate command line to buffer
  char buffer[COMMAND_LINE_LEN];
  strncpy(buffer, cmdLine, COMMAND_LINE_LEN);
  buffer[COMMAND_LINE_LEN - 1] = '\0';
  // split buffer with strtok
  char *token = strtok(buffer, " ");
  token = strtok(NULL, " ");
  // check token
  if (strcmp(token, "on") == 0) {
    printf("LED ON\n");
    gpio_put(LED_PIN, 1);
  } else if (strcmp(token, "off") == 0) {
    gpio_put(LED_PIN, 0);
  } else {
    printf("Invalid LED command. Use 'on' or 'off'.\n");
  }
}

// command bootrom
void commandBootrom (char *cmdLine) {
  printf("Booting into bootrom...\n");
  reset_usb_boot(0, 0);
}

// command help
void commandHelp (char *cmdLine) {
  printf("%s", HELP);
}

// handle unknown commands
void commandUnknown (char *cmdLine) {
  printf("Unknown command %s. Enter help command for help.\n", commandLine);
}
