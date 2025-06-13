# example pico-command-line implementation

This is an example implementation of the `pico-command-line` library for the Raspberry Pi Pico. It demonstrates how to use the library to create a simple command-line interface on the Pico.

## Usage

To use this example, you will need to have the Raspberry Pi Pico SDK installed.

Edit the setup.sh script to set the `PICO_SDK_PATH` and `PICO_BOARD` environment variables. Then run *source setup.sh* to apply the environment variables in the current shell.

Create a build directory and navigate into it: mkdir build && cd build

Run CMake to configure the project: cmake ..

Build the project: make

Connect the Raspberry Pi Pico to your computer via USB while holding the BootSel button to place the Pico into the mass storage mode.

Copy the generated binary file (example.uf2) from the build directory to the mounted Pico drive.

i.e. cp example.uf2 /run/media/$USER/RPI-RP2/

The Pico will automatically reset and load the new firmware. Once it has booted, you should connect to the USB or UART interface with a terminal program (e.g., minicom, screen, or PuTTY).

i.e. screen /dev/ttyUSB0 115200

You should now be able to run the example commands in the terminal.

i.e. led on

