# pico-command-line

The pico-command-line git submodule is a simple command line interface for the Raspberry Pi Pico with custom commands.

Use of the pico-command-line submodule within a Raspberry Pi Pico project 
enables the implementation of a command line interface through the stdio 
serial port. Custom commands are defined in the application for use in 
the command line interface.


## Usage

See the example directory for an example of how to use the pico-command-line submodule.

Use git to add pico-command-line as a submodule to your project.

git submodule add https://github.com/bnielsen1965/pico-command-line.git
git submodule update --init

Define an array of struct command_t objects in your application that 
define the custom commands and their associated methods.

In your applications initialization call the initPicoStdio() method 
to initialize stdio for serial communication. And call the initCommandProcessor()
method to initialize the command processor with the array of commands and the size of the array.

Lastly add the commandProcessor function to an available core or as a task if you are using FreeRTOS.
And call the picoConsole() function in your main application loop or another task if you are using FreeRTOS.





