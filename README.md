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
* **Raspberry pi**
  - User Interface
    * Allows for flooding the bus with start and stop commands to the CAN Logger 3
    * allows for configuring and running self tests
  - **CAN Shield**
    * Checks to see if voltage and resistance on the CAN bus are correct
* **CAN Logger 3**
  - Flood CAN0-2
    * Floods the bus at 100% bus load to obersve if any packets get dropped
* **Test Subject**
  - Logs can data
    * Outputs can data into a log file that determines wether the test was passed
