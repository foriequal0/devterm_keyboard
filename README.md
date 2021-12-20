# DevTerm Keyboard Firmware

## How to install

1. Install Arduino STM32 hardware package on your Arduino IDE

   https://github.com/rogerclarkmelbourne/Arduino_STM32/wiki/Installation

2. Set the board configuration

    * Board: GENERIC STM32F103R series
    * Variant: STM32F103R8
    * Upload Method: STM32duino bootloader
    * CPU Speed: 72Mhz

3. Verify & Upload

## What is a good trackball controller?

* It should be responsive to your rolling speed. The faster you roll, the faster it moves.
* But it should be precise if you want without the separate mode.
* Also it shouldn't be janky when you move diagonally in any angle.
* While doing so, it shouldn't be slippery. It stops to move immediately when you stop to roll.

## TODO

* Power saving mode

  Currently it draws ~30mA. We can reduce them to less than a few milliamps with some low power mode.
