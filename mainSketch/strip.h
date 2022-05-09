#include <Adafruit_NeoPixel.h>

//no idea if we need this but lets keep it here
#ifdef __AVR__
  #include <avr/power.h>
#endif

//lets define out pins 
#define PIN       5
#define NUMPIXELS 84

//instatiate neopiel library wit name pixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRBW + NEO_KHZ800); 

//my Varaibles 
int delay_val = 70;     //pause times
float x_speed = 0.003;  //speed
float x_pix = 0.01;     //this unit witch between pixels on x axis, we will have pixel_num * x_pix

float offset_x = 0.0;   //x value plus its the offset

float y_min = 10.0;     //min
float y_max = 255.0;    //max

float global_r = 0;     //global red
float global_g = 0;     //global green
float global_b = 0;     //global blue

bool do_update = false;

uint32_t rgb2rgbw(int r,int g,int b){
  //Get the maximum between R, G, and B
  float tM = max(r, max(g, b));

  //If the maximum value is 0, immediately return pure black.
  if(tM == 0) return pixels.Color(0,0,0,0);
  

  //This section serves to figure out what the color with 100% hue is
  float multiplier = 255.0f / tM;
  float hR = r * multiplier;
  float hG = g * multiplier;
  float hB = b * multiplier;  

  //This calculates the Whiteness (not strictly speaking Luminance) of the color
  float M = max(hR, max(hG, hB));
  float m = min(hR, min(hG, hB));
  float Luminance = ((M + m) / 2.0f - 127.5f) * (255.0f/127.5f) / multiplier;

  //Calculate the output values
  int Wo = Luminance;
  int Bo = b - Luminance;
  int Ro = r - Luminance;
  int Go = g - Luminance;

  //Trim them so that they are all between 0 and 255
  if (Wo < 0) Wo = 0;
  if (Bo < 0) Bo = 0;
  if (Ro < 0) Ro = 0;
  if (Go < 0) Go = 0;
  if (Wo > 255) Wo = 255;
  if (Bo > 255) Bo = 255;
  if (Ro > 255) Ro = 255;
  if (Go > 255) Go = 255;
  
  return pixels.Color(Ro,Go,Bo,Wo);
}

void prog_off(){
  do_update = false;
  pixels.clear();
}

//clear current, set static color and chill
void prog_static(int r,int g, int b){
  do_update = false;
  pixels.clear();
  for (int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i,rgb2rgbw(r,g,b));
  }
  pixels.show();
}

//clear current, set static color and chill
void prog_sin(int r,int g, int b){
  do_update = true;
  global_r = r;
  global_g = g;
  global_b = b;
  pixels.clear();
}


void led_setup(){
  
  //TBD check if needed  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  //set Pinmodes
  pinMode(PIN, OUTPUT);

  //initialize ledstripe
  pixels.begin();
  pixels.clear();

  //default led programm
  prog_static(255,255,255);

  Serial.println("LED Setup Done");
}


void led_update(){
  if ( do_update == false)
    return; 

    
  float y = 0.0;
  float x = 0.0;
  float val = 0.0;
  
  offset_x += x_speed;
  
  for (int i=0; i<NUMPIXELS; i++) {
    x = offset_x + i*x_pix;
    y = sin(x);
    val = (y+1.0)*(y_max-y_min)/2 + y_min;

    float fak = val/y_max;
  
    pixels.setPixelColor(i,rgb2rgbw(fak*global_r,fak*global_g,fak*global_b));
  }
  
  pixels.show();
}
