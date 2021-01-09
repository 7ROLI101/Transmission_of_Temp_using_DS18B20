# Transmission_of_Temp_using_DS18B20
Using the DS18B20 temperature sensor and the ATMEGA328P microcontroller to transmit data with LiFi.  
I am only going to be developing the code that would allow me to use 1 module on the bus.  
  
As of right now, initDS18B20(), reading of information and writing of information is working properly.  
You would need to turn code optimization on to -O1. This is because the delays that I am using are reliant on the delay functions given within AVR (_delay_ms and _delay_us), and these function relies on the code optimization to be set to work properly. To fix this, I would try and use a timer to get rid of the dependency on code optimization, but I am only concerned with getting the temperature, nothing else.
