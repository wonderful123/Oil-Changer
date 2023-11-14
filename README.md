# Oil Change Machine Software Architecture

# Files contained in the lib directory

## System Architecture Overview

## Class Overview

| Class Name           | Responsibility                                             | Design Patterns Used      |
|----------------------|------------------------------------------------------------|---------------------------|
| `SystemController`   | Coordinates overall machine operation and state management | TinyFSM (State Management)|
| `UserInterface`      | Manages physical user interface interactions               | Observer                  |
| `ButtonController`   | Handles input from physical buttons                        | Observer                  |
| `DisplayController`  | Manages the display units                                  | Observer                  |
| `WebServer`          | Provides remote web-based control and monitoring          | Observer, Service-Oriented|
| `SensorManager`      | Collects and processes sensor data                         | Observer                  |
| `MotorController`    | Controls motor operations                                  | TinyFSM, Observer         |
| `ConfigurationManager`| Manages configuration and calibration settings            | -                         |
| `Error`              | Handles error detection and management                     | -                         |
| `Logger`             | Manages logging of operational data                        | Observer                  |

## Communication and Data Flow
- **Event-Driven Communication**: Components use the Observer pattern to notify relevant classes about state changes.
- **State Management**: TinyFSM is used for managing states in `SystemController` and `MotorController`.
- **Data Encapsulation and Access**: Data is accessed and modified through well-defined interfaces and methods.

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
- **Class**: `ConfigurationManager`
- **Description**: Handles machine settings and calibration data, influencing the behavior of other components.

### Diagnostics and Logging
- **Class**: `Error`, `Logger`
- **Description**: Used for running diagnostics and collecting operational data for logging and analysis.

## Communication and Data Flow
- **Event-Driven Communication**: Components notify relevant classes about state changes.
- **Service-Oriented Approach**: `WebServerManager` acts as an interface to services like calibration, configuration, and diagnostics.
- **Data Encapsulation and Access**: Data is accessed through well-defined interfaces or methods.

