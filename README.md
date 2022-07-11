# CANBusTester
System used to stress test CAN logging devices at full bus load.

## Requirements
- Operates at 100% bus load
- Works with all test subject platforms
- Doesn't rely on a operating system
- Implements self tests to isolate the testing system and verify it works

## Test System Block Diagram
![alt text](CANBusTester.drawio.png)

## Implementation
* Raspberry pi
  - User Interface
    * allows for flooding the bus with start and stop commands
