# Ambientlighting
Ambient lighting for roof of bbq-house



# WriteUp

## The LEDs
its clear that we need to use multiple leds to cover the effect we have in Mind, so there are not many solutions, we can work with multiple rgb leds in combination with a microcontroller und use some kind of logicshifter to controll the leds or we use these individual controllable leds like a bus system they gets called NeoPixels or WS2811 or WS2812 or WS2812b. So this should be the soulution since we can add as many leds as we want and we dosnt need to change sth on pins or hardware.

WS2811: 12V not individual controllable its always three leds in a tuple.
WS2812B: 5V high voltage drop with lots of leds
WS2813: 5V and securtity line
WS2815: 12V and security line
SK6812: 5V but no real voltage-drop because 4th Chanel

Choise is 
WS2815 : 60leds/m 5m ip67 : 65€
SK6812 : 60leds/m 6m ip67 : 65€
WS2812b: 60leds/m 6m ip67 : 30€ so lets take this one 

