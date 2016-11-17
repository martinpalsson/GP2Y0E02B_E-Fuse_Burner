# GP2Y0E02B_E-Fuse_Burner
Burn E-fuses to permanently change device address on Sharp GP2Y0E02B and GP2Y0E03 sensors.

    PLEASE KEEP IN MIND!
    Use whitout proper care may render the sensor USELESS.
    E-Fuses can be burned only ONCE. The worst thing that
    can happen is that your sensor can get the slave-ID 0x00,
    and you can only have one of those!
    Don't try to give your sensor the slave-ID 0xF0, the
    sensor won't work with that slave-ID.
    Don't apply more than 3.3V to any of the sensors pads.
    
    Version 1

    This program is for E-Fuse programming
    on Sharp GP2Y0E02B, GP2Y0E03
    sensors. 
    The code is written to be excecuted by
    an Arduino Due but can easily be ported
    to any device. Just remember that the sensor
    expects 3.3V.
    
    The program makes use of the I2C-scanner found at
    http://playground.arduino.cc/Main/I2cScanner
    though a bit modified to be able to find the address 0x00.
    
    According to the application notes
    (http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0e02_03_appl_e.pdf)
    you should be able to verify that everything went well
    in the burn. How ever, it didn't work out for
    me. You find my code for that commented out in
    the end of the EFuseSlaveID function.

    The program can be tested by simply not
    connecting the Vpp pin to the pad. The program
    will run but no fuse will be burned.
    The same way you can test your sensors address
    after burning e-fuses. Just don't try to burn
    e-fuses again on the same sensor!

    Workflow:
    1. Set desired address as SETADDR (preprocessor directive), Table 14 in application notes (don't use 0xF0!).
    2. Save the file.
    3. Program your board.
    4. Connect your sensor to the I2C bus.
    5. Open up the serial monitor in Arduino IDE.
    (Optional: by pushing the reset button, run the program once without Vpp connected. Expected address to be found is 0x04 (0x80 bitshifted by one right). If thats not the case the fuses of this sensor may already be burned).
    6. Connect the Vpp pad in some way. (Solder a jumper or secure a jumper with isolated calipers). Make sure it's fed nothing more than 3.3V
    7. Run the program ONCE by pushing the reset button on the arduino. 
    8. Disconnect the Vpp pin (the one connected in step 6).
    9. With the Vpp pin disconnected, run the program once again by pushing the reset button.
    10. Note the address found in the scan. The address should correspond to the address given in SETADDR bitshifted one to the right.
    11. Give me feedback. martin.palsson@outlook.com
    
    Martin Pålsson 2016
