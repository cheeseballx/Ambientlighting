# Ambientlighting
Ambient lighting for roof of bbq-house



# WriteUp
first, we need to buy components, so choose leds, an power supply and a computation unit.

Next since we have choosen an ESP8266 we install ArduinoIDE and load the comunity module for NodeMCU Boards.
then we just try the node with simple example code. 

The first real code needs to deal with wifi, so we need to check this next


## The LEDs
its clear that we need to use multiple leds to cover the effect we have in Mind, so there are not many solutions, we can work with multiple rgb leds in combination with a microcontroller und use some kind of logicshifter to controll the leds or we use these individual controllable leds like a bus system they gets called NeoPixels or WS2811 or WS2812 or WS2812b. So this should be the soulution since we can add as many leds as we want and we dosnt need to change sth on pins or hardware.

WS2811: 12V not individual controllable its always three leds in a tuple.
WS2812B: 5V high voltage drop with lots of leds
WS2813: 5V and securtity line
WS2815: 12V and security line
SK6812: 5V but no real voltage-drop because 4th Chanel for white

lets stick to the 5V, thats a lot easier, so the sk6812 is 5v and does not have a hughe voltage-drop because it has its own chanel for white light. Its somehow also one of the most expensive ones. its more than 60 euro for the 60led/m. But i found cheep version on ali-express.

## The main Unit
we need wifi, thats for sure and it need to be able to controll theses led stripes. Its easy choise to buy a NodeMCU because Raspberry is to expensive for this simple solution, nodemcu is more or less an arduino with wifi and costs around 3€. arduino would be a good choise, if it shouldnt be connected to the internet. 

i bought from amazon a NodeMCU Amica V2 ESP8266MOD 12-F. Simply got a package with 5 Units in there

## The Power Supply
 
