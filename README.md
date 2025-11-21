# PokeyBots Robotics Code

This code was written for the PokeyBots Robots made by [ABC Robokits](https://abcrobokits.com).  

They utilize  XAIO ESP32C3 boards. These are the basic programs for setup and control of the robots.  The program `get_mac_address` prints the mac address for the controller board and for the robot board  to be used in the other two programs.  The `controller_code` program is the program which will be uploaded to the joystick controller.  The `robot_code` program is the program which will be uploaded to the robot board in the chassis.

## Software Preparation

  1.  Download and install the latest verions of [Arduino IDE](https://www.arduino.cc/en/software/) according to your operating system.
  2.  Launch the Arduino application.
  3.  Add `ESP32` board package to your arduino IDE:
       - Navigate to **File > Preferences** and fill in **Additional Boards Manager URLs** with the url below.
         
         ```
         https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
         ```
       - Navigate to **Tools > Board > Boards Manager** and type the keyword *esp32* in the search box, select the latest version of **esp32**, and install it.
  4. Select your board by navigating to **Tools > Board > ESP32 Arduino** and select *XIAO_ESP32C3*.  Note:  You may have to scroll a lot to find it!
  5. Select your port by navigating to **Tools > Port** and select the serial port name of the connected XIAO ESP32C3.  For Windows, this is likely to be **COM3** or highter (**COM1** and **COM2** are reserved for hardware serial ports).
  6. Download the **ESP32Servo** library.
     - Click the **Library Manager** from the left-side menu.
     - Search *ESP32Servo* in the search bar.
     - Install the latest version.
         
        
