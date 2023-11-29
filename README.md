# Arduino DLK_L99DZ200G
Arduino library for L99DZ200G Automotive Front Door Device Controller from STMicro.

This Arduino library has been developed for the 
[L99DZ200G](https://www.st.com/en/automotive-analog-and-power/l99dz200g.html)
**_"Automotive Front Door device"_** using Arduino 1.8.13 and Teensyduino 1.56 using
the STMicro Evaluation Boards:
- [AEK-MOT-MR200G1](https://www.st.com/en/evaluation-tools/aek-mot-mr200g1.html)
**_"Vehicle mirror controller board based on L99DZ200G multioutput driver"_** and
- [AEK-MOT-TK200G1](https://www.st.com/en/evaluation-tools/aek-mot-tk200g1.html)
**_"Power liftgate controller board based on L99DZ200G multioutput driver"_**.

**[Vehicle mirror controller](docs/AEK-MOT-MR200G1_Block.PNG)**

**[Power liftgate controller](docs/AEK-MOT-TK200G1_Diagram.png)**

The AEK-MOT-MR200G1 ([picture](docs/AEK-MOT-MR200G1.png)) and AEK-MOT-TK200G1
([picture](docs/AEK-MOT-TK200G1.png)) boards each have a controlling MCU on the boards,
but this MCU was disabled by connecting the reset pin (Pin 9) of the JTAG connector to
ground (Pin 2) of the JTAG connector and removing an inline resistor between the
L99DZ200G NRESET output and the MCU's PORST input.

Once the onboard MCU was disabled, an external Arduino-based MCU board (an Arduino Nano,
or Arduino Nano Every or Teensy 3.1) was connected to various convenient test points on the
AEK-MOT-MR200G1 and AEK-MOT-TK200G1 boards to provide the connectivity to control the
L99DZ200G device.

Additionally, in order for the external Arduino-based MCU board to manage the appropriate
wake-up from the sleep modes of the L99DZ200G device, a MOSFET transistor was added to the
boards to parallel the board's WAKE pushbutton.

See [MR200G1](docs/MR200G1_.S01.pdf) and [TK200G1](docs/TK200G1_.S01.pdf) for the schematics
of both of the boards showing the modifications and connection test points.

See [MR200G1_TP](docs/AEK-MOT-MR200G1_TP.png) and [TK200G1_TP](docs/AEK-MOT-TK200G1_TP.png)
for the images of both of the boards showing the connection test points.

See [MR200/TK200](docs/MR200_TK200-MCU%20Wiring.pdf) showing the connections used for tesing
this Arduino library with these boards.

Doxygen-based HTML files for the library are in: **[API Docs](docs/html/class_d_l_k___l99_d_z200_g.html)**

Included Examples:
 - **[DLK_L99DZ200G_Library](examples/DLK_L99DZ200G_Library):**
 
    Exercises the DLK_L99DZ200G Arduino library API for the L99DZ200G device on the AEK-MOT-MR200G1
    Vehicle mirror controller board and the AEK-MOT-TK200G1 Power liftgate controller board,
    interactively using my Arduino **[CommandLine](https://github.com/dlkeng/Arduino_CommandLine)** library.

 - **[MR200G1_Demo](examples/MR200G1_Demo):**
 
    Demos operation of the AEK-MOT-MR200G1 Vehicle mirror controller board with the
    DLK_L99DZ200G Arduino library for the L99DZ200G device, including CAN control
    using an external MCP2515 CAN Controller Module.

 - **[TK200G1_Demo](examples/TK200G1_Demo):**
 
    Demos operation of the AEK-MOT-TK200G1 Power liftgate controller board with the
    DLK_L99DZ200G Arduino library for the L99DZ200G device, including CAN control
    using an external MCP2515 CAN Controller Module.
 
