# CANBusTester
Can Logging devices on the market rely on heavy operating systems such as Linux in order to run the software needed to log CAN data. These operating systems utilize interupts to perform many different tasks on the machine which leads to frames being lost on the CAN bus while at 100% network capacity. The goal of this device is to create a testing envirnment where the test subject is reading CAN data at 100% and the number of packets lost is recorded and displayed in the test results.



*Device Goal - Test the performance of can logging devices

*What does my canbus tester measure

## Requirements
- Measure performance of CAN Bus logging devices (Now add Subrequirements)
- Operates at 100% bus load indefinitely
- Test subject outputs a log file(needs log parameters as subrequirement) 
- Device requires SocketCAN to work
- CAN bus tester is able to test any CAN bus logging device
- This device will support multiple I/O connection types (list types as subreq.)
- Implements self tests to isolate the testing system and verify it works
- Number of CAN lines

## Test System Block Diagram
![alt text](CANBusTester.drawio.png)
-Look at activity diagrams/action flow diagram Ex: ATM
- Split into two block diagrams physical and functional

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
* **Test Case - Sending/Receiving on 2 Bus Lines**
  - To find out wether a single CAN Logger 3 could handle sending and receiving messages on two separate CAN lines at 100% bus load the device was set to send messages with a 630us interval. The figure below is a comparison between using only one CAN logger(left) and two CAN Loggers(right). It can be seen that the single CAN Logger dropped a significant amount of frames.

  <img src="figures/CANLogger3SingleTest.png" height="250" width="425"/> <img src="figures/CANLogger3SeparateTest.png" height="250" width="425"/> 
  
  - Truck Cape was used to verify network usage was at 100%

  <img src="figures/busloadMeasurement.png" height="500" width="425"/>
  

