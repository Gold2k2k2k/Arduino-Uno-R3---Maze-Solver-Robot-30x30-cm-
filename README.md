# 30x30 cm Maze Solver Robot - Arduino Uno R3

An autonomous maze-solving robot project designed for a 30x30 cm grid maze. The robot uses an Arduino Uno R3 microcontroller and three HY-SRF05 ultrasonic sensors to navigate and avoid walls.

## Hardware Components
- **Microcontroller:** Arduino Uno R3
- **Motor Driver:** L298N
- **Distance Sensors:** 3x HY-SRF05 Ultrasonic Sensors (Left, Right, Front)
- **Maze Grid Size:** 30x30 cm per cell

## Pinout Configuration

### HY-SRF05 Ultrasonic Sensors
| Sensor | Trig Pin | Echo Pin |
|:---:|:---:|:---:|
| **Front Sensor** | 10 | 11 |
| **Left Sensor** | 12 | 13 |
| **Right Sensor** | 8 | 9 |

### L298N Motor Driver
| Motor Driver | Arduino Uno R3 Pin | Function |
|:---:|:---:|:---:|
| **en1 (Motor A PWM)** | 3 | Speed control (Right) |
| **in1** | 2 | Direction control |
| **in2** | 4 | Direction control |
| **en2 (Motor B PWM)** | 6 | Speed control (Left) |
| **in3** | 5 | Direction control |
| **in4** | 7 | Direction control |

## Navigation Logic
The robot is programmed with logical conditions to navigate and follow walls in a 30x30cm maze:
- **Move Forward:** When the left distance is between 8cm and 15cm and the front is clear (>= 15cm).
- **Balance Right:** When the robot gets too close to the left wall (between 2cm and 8cm).
- **Turn Right:** When the front distance >= 5cm, and the left distance is between 4cm and 30cm.
- **Turn Left:** When there is an open space on the left (left distance >= 35cm).
- **Stuck Prevention (Fix front force error):** If the robot gets too close to the wall and the sensor values stop changing (stuck), it triggers an automatic backup sequence (`back()`) to escape the trap.
- The robot includes movement helper functions such as: `forward()`, `maxspeed()`, `balancedright()`, `right()`, `left()`, `back()`, `reverse()`, and `stop()`.

## How to Use
1. Clone or download this repository.
2. Open the `Maze_Solver_Uno_30x30.ino` file using the **Arduino IDE**.
3. Connect your Arduino Uno R3 to your computer via USB.
4. Select the correct Board (`Arduino Uno`) and COM Port in the IDE.
5. Click **Upload** to flash the code to the robot.
6. Power on the robot and place it inside the maze.
