# ECE 520/L – Lab 2: AXI GPIO

**Author:** Charbel Abouafech

## Overview

This lab uses three AXI GPIO peripherals on the Zybo Z7 board to read the switches and control the LEDs and RGB LED using software. The design was created in Vivado, exported as an XSA file, and used to create a Vitis platform and application project.

## Design Summary

The block design contains three AXI GPIO peripherals. AXI GPIO #1 drives LED0 to LED3, AXI GPIO #2 drives the three RGB LED channels, and AXI GPIO #3 reads SW0 to SW3. The application uses the device IDs from xparameters.h and the GPIO functions from xgpio.c to initialize and control each peripheral.

The LED GPIO and RGB LED GPIO are configured as outputs. The switch GPIO is configured as an input. The RGB LED uses a 3-bit value: bit 0 controls red, bit 1 controls green, and bit 2 controls blue. Writing 0x01 turns on red, 0x02 turns on green, 0x04 turns on blue, and 0x07 turns on all three colors to produce white.

The application continuously reads the four switches and updates the LEDs and RGB LED. When only one switch is enabled, the matching LED turns on and the corresponding RGB color is selected. With SW3 enabled by itself, all three RGB channels turn on to produce white.

When SW0 and SW1 are on and SW2 and SW3 are off, the four LEDs display a 4-bit binary counter. The counter counts from 0x0 to 0xF and then wraps back to 0x0.

When SW2 and SW3 are on and SW0 and SW1 are off, the LEDs display a ring counter. The sequence starts with LED0 on, shifts the active LED to the left after each update, and wraps back to LED0 after LED3. A software delay is included so that both counter sequences are visible on the board. The RGB LED is off during both counter modes.

Any other switch combination, including no switches enabled, turns off all four LEDs and the RGB LED.

## Verification and Results

The application was checked by testing the individual switch cases, the binary counter, the ring counter, and invalid switch combinations. The individual switch cases select the expected LED and RGB color. The binary counter cycles through all 4-bit values, while the ring counter moves one active LED at a time and wraps around. The RGB LED remains off during both counter modes and for invalid switch combinations.

## Known Issues or Limitations

No issues. The design works as intended.

## References

Course lecture materials for ECE 520/L.
