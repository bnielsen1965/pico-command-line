# pico-command-line

The pico-command-line git submodule is a simple command line interface for the Raspberry Pi Pico with custom commands.

Use of the pico-command-line submodule within a Raspberry Pi Pico project 
enables the implementation of a command line interface through the stdio 
serial port. Custom commands are defined in the application for use in 
the command line interface.


## Usage

NOTE: The use of multicore or a multi-threaded OS is required for the pico-command-line submodule.
In the example the pico-sdk multicore-launch-core1() function is used to launch the commandProcessor on core1
while picoConsole() is executed on core0 in the main method. Operating systems such as FreeRTOS can be used 
to execute commandProcessor and picoConsole as tasks.

See the example directory for an example of how to use the pico-command-line submodule.

Use git to add pico-command-line as a submodule to your project.

> git submodule add https://github.com/bnielsen1965/pico-command-line.git

> git submodule update --init

Define an array of struct command_t objects in your application that 
define the custom commands and their associated methods.

```c
// include pico-command-line header file to define types and functions
#include "pico-command-line.h"

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
```


In your applications initialization call the initPicoStdio() method 
to initialize stdio for serial communication. And call the initCommandProcessor()
method to initialize the command processor with the array of commands and the size of the array.

```c
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
```


Lastly add the commandProcessor function to an available core or as a task if you are using FreeRTOS.
And call the picoConsole() function in your main application loop or another task if you are using FreeRTOS.

```c
// main application entry point
int main()
{
  init();
  multicore_launch_core1(commandProcessor);
  picoConsole();
}
```



