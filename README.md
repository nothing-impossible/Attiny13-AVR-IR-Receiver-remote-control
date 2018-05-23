# Attiny13-AVR-IR-Receiver-remote-control

This is Attiny13 remote control IR Receiver Code for NEC protocol, it uses interrupting configuration to receive remote control data.

 # Resources

### Program Memory Usage:
`618 bytes (60.4%)`
### Data Memory Usage: 
`13 bytes (20.3%)`

Using attiny13 is a cheap way to have IR wireless remote control to use it in your project.

*it's importing to connect the output of the IR receiver to the interrupt pin INT0 in attiny13 which is PIN PB1 according to the datasheet*

It has 3 principle functions:

1- The `IR_Setup()` which setup the configuration setting like interrupt and timer.

2- The `IR_Scan()` function it use in the ISR interrupt to store the received data in the IR_data_out variable.

3- The `IR_Code()` function it use to compare your input (32 bit data) remote control button code 

And it returns true if it is correct and false if it is wrong.
   
Enjoy
by [Dz Inventors](https://www.facebook.com/DZ.Inventors)
