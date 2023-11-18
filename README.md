
# Oil Change Machine Software Architecture

## System Architecture Overview

### Introduction

The Oil Change Machine is designed with a modular and scalable architecture, focusing on robustness, maintainability, and efficient operation. The system integrates various hardware and software components, each responsible for specific functionalities, from user interaction to hardware control and data management.

### Core Components

-   **SystemController**: The central unit that orchestrates the overall operation of the machine. It coordinates the activities of all other components, making decisions based on sensor data, user inputs, and predefined logic.
-   **HardwareManager**: The `HardwareManager` serves as the centralized hub for managing all hardware-related interactions and initialization in the Oil Change Machine. Its primary role is to abstract the hardware layer from the higher-level system logic, providing a unified interface for hardware operations and ensuring a clean separation of concerns.
-   **UserInterface and Controllers**: Comprising `UserInterface`, `ButtonController`, and `DisplayController`, this module handles all direct interactions with the user, facilitating input and feedback.
-   **WebServer**: Offers remote control and monitoring capabilities, providing a web-based interface for machine operation and diagnostics.
-   **SensorManager**: Responsible for collecting and processing data from various sensors, crucial for the precise operation of the machine.
-   **MotorController**: Manages the operation of motors and other actuators, directly influencing the physical processes of the oil change.

### Design Patterns and Frameworks

-   **Observer Pattern**: Widely used across the system to ensure components can react dynamically to state changes in a loosely coupled manner. It's pivotal in the communication between `UserInterface`, `SensorManager`, `MotorController`, and `SystemController`.
-   **TinyFSM (Finite State Machine)**: Employed in `SystemController` and `MotorController` for managing state transitions, ensuring predictable and manageable behavior of these key components.

### Data Flow and Communication

-   The system employs event-driven communication, primarily facilitated by the Observer pattern, allowing for efficient and real-time interaction between components.
-   TinyFSM aids in handling complex state transitions and decision-making processes in a controlled manner.
-   Data encapsulation and access are enforced through well-defined interfaces, ensuring data integrity and system stability.

### Integration and Scalability

-   The architecture is designed to be easily expandable, allowing the addition of new hardware components or software modules with minimal impact on existing functionalities.
-   The modular design facilitates isolated development and testing of individual components, enhancing the system's reliability and ease of maintenance.

### Conclusion

The Oil Change Machine's software architecture reflects a commitment to robust, efficient, and user-friendly operation. The modular design, combined with carefully chosen design patterns and frameworks, ensures a system that is both powerful in its capabilities and flexible in its adaptability to future enhancements.

## Files contained in the lib directory

## Class Overview

| Class Name            | Responsibility                                             | Design Patterns Used       |
|-----------------------|------------------------------------------------------------|----------------------------|
| `SystemController`    | Coordinates overall machine operation and state management | TinyFSM (State Management) |
| `UserInterface`       | Manages physical user interface interactions               | Observer                   |
| `ButtonController`    | Handles input from physical buttons                        | Observer                   |
| `DisplayController`   | Manages the display units                                  | Observer                   |
| `WebServer`           | Provides remote web-based control and monitoring           | Observer, Service-Oriented |
| `SensorManager`       | Collects and processes sensor data                         | Observer                   |
| `MotorController`     | Controls motor operations                                  | TinyFSM, Observer          |
| `ConfigManager`       | Manages configuration and calibration settings             | -                          |
| `Error`               | Handles error detection and management                     | -                          |
| `Logger`              | Manages logging of operational data                        | Observer                   |
| `HardwareManager`     | Manages all hardware interactions                          | TinyFSM, Observer          |

### Central Control Unit

- **Class**: `SystemController`
- **Description**: Oversees the entire operation of the oil change machine, coordinating activities of other classes and managing the machine's state.

### User Interface Integration

- **Classes**: `UserInterface`, `ButtonController`, `DisplayController`, `WebServer`
- **Description**: Manages user interactions through physical buttons and displays, as well as remote web-based control.

### Sensor Data Handling

- **Class**: `SensorManager`
- **Description**: Collects and processes data from various sensors like flow meters, temperature sensors, and voltage sensors.

### Motor and Pump Control

- **Class**: `MotorController`
- **Description**: Controls motor operations based on commands from the `SystemController` and sensor data.

### Configuration and Calibration

- **Class**: `ConfigManager`
- **Description**: Handles machine settings and calibration data, influencing the behavior of other components.

### Diagnostics and Logging

- **Class**: `Error`, `Logger`
- **Description**: Used for running diagnostics and collecting operational data for logging and analysis.

## Communication and Data Flow

- **Event-Driven Communication**: Components use the Observer pattern to notify relevant classes about state changes.
- **State Management**: TinyFSM is used for managing states in `SystemController` and `MotorController`.
- **Data Encapsulation and Access**: Data is accessed and modified through well-defined interfaces and methods.

### Implementation Sequence

1.  **ConfigManager**
    
    -   **Rationale**: As it manages configuration and calibration settings, implementing `ConfigManager` first ensures that other components can be configured correctly during their initialization.
    -   **Dependencies**: None. It serves as a foundational component for others.
2.  **HardwareManager**
    
    -   **Rationale**: Centralizes control over all hardware interactions. Implementing it early ensures that hardware initialization and management are in place.
    -   **Dependencies**: Requires basic configurations from `ConfigManager`.
3.  **SensorManager and MotorController**
    
    -   **Rationale**: These components interact directly with physical sensors and motors, respectively. Their early implementation allows for testing of hardware interactions.
    -   **Dependencies**: Depends on `HardwareManager` for hardware control and `ConfigManager` for configuration settings.
4.  **SystemController**
    
    -   **Rationale**: As the central unit coordinating the operation, its implementation ties together the individual components.
    -   **Dependencies**: Requires `SensorManager`, `MotorController`, and `HardwareManager` to be in place for effective control and coordination.
5.  **User Interface Components (UserInterface, ButtonController, DisplayController)**
    
    -   **Rationale**: With the core system components in place, the focus shifts to user interaction elements.
    -   **Dependencies**: Depends on `SystemController` for processing user inputs and controlling the system.
6.  **WebServer**
    
    -   **Rationale**: Implements remote control and monitoring capabilities. It's implemented after the core system and user interface components are in place.
    -   **Dependencies**: Requires a functioning `SystemController` and user interface components for full integration.
