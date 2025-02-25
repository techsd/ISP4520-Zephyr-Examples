# Visual Diagrams of Architecture, Sequence, and Deployment

## Architecture Diagram

![Architecture Diagram](images/architecture_diagram.png)

The architecture diagram provides an overview of the system components and their interactions. It includes the following components:
- LoRaWAN Module (ISP4520)
- Zephyr RTOS
- Application Layer
- Hardware Abstraction Layer (HAL)
- Peripheral Devices

## Sequence Diagram

![Sequence Diagram](images/sequence_diagram.png)

The sequence diagram illustrates the flow of messages between the components during the execution of a specific scenario. It includes the following steps:
1. Initialization of the LoRaWAN stack.
2. Configuration of the device with necessary keys.
3. Joining the LoRaWAN network using OTAA.
4. Sending a message.
5. Receiving and logging downlink messages.

## Deployment Diagram

![Deployment Diagram](images/deployment_diagram.png)

The deployment diagram shows the physical deployment of the system components. It includes the following elements:
- Development Environment (PC with nRF Connect SDK)
- Target Device (ISP4520 module)
- LoRaWAN Gateway
- LoRaWAN Network Server
- Application Server
