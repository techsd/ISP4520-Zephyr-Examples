# SETUP.md

## Instructions for Setting Up the Environment for ISP4520-Zephyr-Examples

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
