#include <Adafruit_NeoPixel.h>

//no idea if we need this but lets keep it here
#ifdef __AVR__
  #include <avr/power.h>
#endif

//lets define out pins 
#define PIN1       5
#define PIN2       4
#define NUMPIXELS 84

//instatiate neopiel library wit name pixels
Adafruit_NeoPixel pix1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRBW + NEO_KHZ800); 
Adafruit_NeoPixel pix2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRBW + NEO_KHZ800); 

Adafruit_NeoPixel static_color; 

//gloal rgb
char rgb1[] = {0,0,0};
char rgb2[] = {0,0,0};


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
  if(tM == 0) return static_color.Color(0,0,0,0);
  

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
  
  return static_color.Color(Ro,Go,Bo,Wo);
}

//read rgb
void set_rgb(String hexStr,char* rgb){
  char hex[7];
  hexStr.toCharArray(hex,7);
  unsigned long int rgb_long = strtol(hex,(char**)0, 16);

  rgb[0] = (rgb_long / 256 / 256) % 256;
  rgb[1] = (rgb_long / 256) % 256;
  rgb[2] = rgb_long% 256;
}

void prog_off(){
  do_update = false;
  pix1.clear();
  pix2.clear();
  pix1.show();
  pix2.show();
}

//clear current, set static color and chill
void prog_static(String hex1, String hex2){

  //stop updating if there is updating 
  do_update = false;

  set_rgb(hex1,rgb1);
  set_rgb(hex2,rgb2);

  //set the two color strips
  for (int i=0; i<NUMPIXELS; i++) {
    pix1.setPixelColor(i,rgb2rgbw(rgb1[0],rgb1[1],rgb1[2]));
    pix2.setPixelColor(i,rgb2rgbw(rgb2[0],rgb2[1],rgb2[2]));
  }
  pix1.show();
  pix2.show();
}

void setPix(int strip, int x_led, String col){
  set_rgb(col,rgb1);
  if (strip == 1) pix1.setPixelColor(x_led,rgb2rgbw(rgb1[0],rgb1[1],rgb1[2]));
  else            pix2.setPixelColor(x_led,rgb2rgbw(rgb1[0],rgb1[1],rgb1[2]));
}

void show1(){
  pix1.show();
  pix2.show();
}

//clear current, set static color and chill
void prog_sin(String hex1,String hex2){

  do_update = false;

  
  set_rgb(hex1,rgb1);
  set_rgb(hex2,rgb2);
  
  do_update = true;

  
}


void led_setup(){
  
  //TBD check if needed  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  
  //set Pinmodes
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);

  //initialize ledstripe
  pix1.begin();
  pix2.clear();

  //default led programm
  prog_static("FFFFFF","FF0000");

  Serial.println("LED Setup Done");
}


void led_update(){
  if ( do_update == false)
    return; 

  float x = 0.0;
  float val1 = 0.0;
  float val2 = 0.0;
  
  offset_x += x_speed;
  
  for (int i=0; i<NUMPIXELS; i++) {
    x = offset_x + i*x_pix;
    
    val1 = (sin(x)+1.0)*(y_max-y_min)/2 + y_min;
    val2 = (sin(x)+1.0)*(y_max-y_min)/2 + y_min;

    float fak1 = val1/y_max;
    float fak2 = val2/y_max;
  
    pix1.setPixelColor(i,rgb2rgbw(fak1*rgb1[0],fak1*rgb1[1],fak1*rgb1[2]));
    pix2.setPixelColor(i,rgb2rgbw(fak2*rgb2[0],fak2*rgb2[1],fak2*rgb2[2]));
  }
  
  pix1.show();
  pix2.show();
}
