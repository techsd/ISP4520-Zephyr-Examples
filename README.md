# ISP4520-Zephyr-Examples
Zephyr implementation of LoRa and LoRaWan examples using ISP4520 

## Overview

This project show how to use Zephyr implmentation of LoRA/LoRaWAN on a ISP4520 module.
The examples are configured for the EU variant of the ISP4520 module.

## Getting started

Before getting started, make sure you have a proper nRF Connect SDK development environment.
Follow the official
[Getting started guide](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/nrf/getting_started.html).

Then initialize the workspace:

```shell
west init -m https://github.com/insightsip/ISP4520-Zephyr-Examples.git --mr main my-workspace
cd my-workspace
west update
```

For the class_a example, to be able to connect to your network modify in the class_a/main.c file the following keys:
- LORAWAN_DEV_EUI
- LORAWAN_JOIN_EUI
- LORAWAN_APP_KEY

Build the application, for the class_a:

```shell
cd class_a
west build -b nrf52dk_nrf52832
```

Flash the device:

```shell
west flash
```

## Additional Settings for ISP4520

To set up the environment for the project 'ISP4520-Zephyr-Examples', follow these instructions:

1. Ensure you have a proper nRF Connect SDK development environment. Follow the official Getting started guide.

2. Initialize the workspace:
   ```shell
   west init -m https://github.com/insightsip/ISP4520-Zephyr-Examples.git --mr main my-workspace
   cd my-workspace
   west update
   ```

3. For the `class_a` example, modify the keys in `class_a/src/main.c` to connect to your network:
   * `LORAWAN_DEV_EUI`
   * `LORAWAN_JOIN_EUI`
   * `LORAWAN_APP_KEY`

4. Build the application for the `class_a` example:
   ```shell
   cd class_a
   west build -b nrf52dk_nrf52832
   ```

5. Flash the device:
   ```shell
   west flash
   ```

For updating and extending the functionality, refer to the following files:

* `UPDATE.md`: Contains a detailed description of changes, a plan for resolution, and a list of operations.
* `EXAMPLE.md`: Provides practical examples of using components and scenarios.
* `DIAGRAM.md`: Includes visual diagrams of architecture, sequence, and deployment.
* `ARCHITECTURE.md`: Provides a deep analysis of the project structure, description of files, components, dependencies, and configurations.

## Verify Project Settings for Zephyr Development Environment

Ensure the following settings are configured for the Zephyr development environment:

* Single Mode BLE 5 Ready
* NFC-A Tag for OOB pairing
* Fully integrated LoRa & BLE matching and Antennas
* Integrated LoRa and BLE 32 MHz & 32.768 kHz Clocks
* LoRa section based on Semtech SX126x series transceiver
* BLE section based on Nordic Semi nRF52
* Externally Controlled or using embedded 32-bit ARM Cortex M4 CPU
* 512 kB Flash and 64 kB SRAM
* Analog and Digital peripherals
* SPI interface
* Supply Voltage 1.8V to 3.6V
