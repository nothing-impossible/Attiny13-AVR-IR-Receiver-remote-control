# Attiny13-AVR-IR-Receiver-remote-control
this is Attiny13 remote control IR Receiver Code for NEC protocol use interrupting configuration to receiver remote control data
it tahe (Program Memory Usage : 618 bytes (60.4 %) , Data Memory Usage : 13 bytes ( 20.3 %)) from the Attiny13 

*it's importing to connect the output of the ir receiver to the interrupt pin INT0 in attiny13 which is PIN PB1 according to the datasheet

it have 3 principle functions :
1) the IR_Setup() which step the configuration setting like interrupt and timer.
2) the IR_Scan() function it use in the ISR interrupt to store the receiving data in the IR_data_out variable.
3) the IR_Code() function it use to compare your input (32 bit data) Remot control button code 
   and it return true if it is correct and false if it is wrong.
   
thank you.
