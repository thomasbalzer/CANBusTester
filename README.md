# CANBusTester
System used to stress test CAN logging devices at full bus load.

## Requirements
- Operates at 100% bus load
- Test Subject outputs a log file to determine if the test was successful
- Doesn't rely on an operating system
- Implements self tests to isolate the testing system and verify it works

## Test System Block Diagram
![alt text](figures/CANBusTester.drawio.png)

## Implementation
* **Raspberry pi**
  - User Interface
    * Allows for flooding the bus with start and stop commands to the CAN Logger 3
    * Configuring and running self tests
  - CAN Shield
    * Checks to see if voltage and resistance on the CAN bus are correct
* **CAN Logger 3**
  - Flood CAN0-2
    * Floods the bus at 100% bus load to obersve if any packets get dropped
    * Push buttons can be used to start and stop test in addition to UI
* **Test Subject**
  - Logs can data
    * Outputs CAN data into a log file that determines wether the test was passed

## Test Plan
* **CAN Logger 3**
  - To find out wether a single CAN Logger 3(CL3) could handle sending and receiving messages on two separate CAN lines the device was set to send messages with a 630us interval or 100% bus load. The figure below is an example of a CL3 both sending and receiving on Can0 and Can1.

  ![singleCL3](figures/CANLogger3SingleTest.png)
  
  
  - The same test was performed with two CL3s where one is sending and the other is receiving.
  ![singleCL3](figures/CANLogger3SeparateTest.png)
