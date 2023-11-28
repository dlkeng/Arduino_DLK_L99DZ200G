# Arduino DLK_L99DZ200G
Arduino library for L99DZ200G Automotive Front Door Device Controller from STMicro.

This Arduino library has been developed for the 
[L99DZ200G](https://www.st.com/en/automotive-analog-and-power/l99dz200g.html)
**_"Automotive Front Door device"_** using Arduino 1.8.13 and Teensyduino 1.56 using
the STMicro [AEK-MOT-MR200G1](https://www.st.com/en/evaluation-tools/aek-mot-mr200g1.html)
**_"Vehicle mirror controller board based on L99DZ200G multioutput driver"_** and
[AEK-MOT-TK200G1](https://www.st.com/en/evaluation-tools/aek-mot-tk200g1.html)
**_"Power liftgate controller board based on L99DZ200G multioutput driver"_**.

The AEK-MOT-MR200G1 {[picture](docs/AEK-MOT-MR200G1.png)} and AEK-MOT-TK200G1
{[picture](docs/AEK-MOT-TK200G1.png)} boards each have a controlling MCU on the boards,
but this MCU was disabled by connecting the reset pin (Pin 9) of the JTAG connector to
ground (Pin 2) and removing an inline resistor between the L99DZ200G NRESET output and
the MCU's PORST input.

Once the onboard MCU was disabled, an external Arduino-based MCU board (an Arduino Nano,
or Arduino Nano Every or Teensy 3.1) was connected to various test points on the AEK-MOT-MR200G1
and AEK-MOT-TK200G1 boards to provide various connectivity to the L99DZ200G device.

Additionally, in order for the external Arduino-based MCU board to manage the appropriate
wake-up from the sleep modes of the L99DZ200G device, a MOSFET transistor was added to the
boards to parallel the board's WAKE pushbutton.

See [MR200G1](docs/MR200G1_.S01.pdf) and [TK200G1](docs/TK200G1_.S01.pdf) for the schematics
of both of the boards showing the modifications and connection test points.

See [MR200G1_TP](docs/AEK-MOT-MR200G1_TP.png) and [TK200G1_TP](docs/AEK-MOT-TK200G1_TP.png)
for the images of both of the boards showing the connection test points.

See [MR200/TK200](docs/MR200_TK200-MCU.pdf) showing the connections used for tesing
this Arduino library with these boards.


