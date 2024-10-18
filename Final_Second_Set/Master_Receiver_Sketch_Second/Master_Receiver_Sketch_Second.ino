//----------------------------------ESPNOW libraries and functions-------------------------------------------------------------//
#include <esp_now.h>
#include <WiFi.h>


typedef struct struct_message {
  int button_onoff_count2;
  bool LED_ON2;
  int button_light_count2;
  int button_bright_count2;

  int UD_1_mapped2;
  int UD_2_mapped2;

  int button_quack_count2;

} struct_message;

struct_message myData;

int button_onoff_count;
bool LED_ON;
int button_onoff_lastcount;

int button_light_count;
int button_light_lastcount;

int button_bright_count;
int button_bright_lastcount;

int UD_1_mapped;
int UD_2_mapped;

int button_quack_count;
int button_quack_lastcount;
//----------------------------------------------------------------------------------------------------------------------------//

//-------------------------------Pins--------------------------------//
int Servo1_pin = 33;
int Servo2_pin = 4;

int I2S_BCLK_PIN = 27;
int I2S_LRC_PIN = 14;
int I2S_DOUT_PIN = 26; //DOUT refers to DIN pin on amplifier, needs DAC pin
//-------------------------------------------------------------------//


//-----------------------------------------LED Libraries and Functions-------------------------------------------------------//
#include <FastLED.h>

#define NUM_LEDS 30
#define LED_DATA_PIN 32
#define COLOUR_ORDER RGB
#define CHIPSET NEOPIXEL
int BRIGHTNESS = 60;
int DELAY = 20;
#define VOLTS 3
#define MAX_AMPS 700 //in milliamps

CRGB leds[NUM_LEDS];

CRGB colours[] = {CRGB::OrangeRed, CRGB::Yellow, CRGB::Green, CRGB::Blue, CRGB::Indigo, CRGB::Violet, CRGB::Red};
int BrightnessLevels[] = {100, 200, 100, 60};
int DelaySettings[] = {12, 6, 12, 20};


//-----------------------------Colour Setting Functions----------------------------//
void Set_Colour(CRGB colour) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = colour;
  }
}

void Fade_up() {
    for (int i = 0; i < BRIGHTNESS; i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(DELAY);
  }
}
void Fade_down() {
    for (int i = BRIGHTNESS; i>=0; i--) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(DELAY);
  }
}
//--------------------------------------------------------------------------------//


//----------------------------BrightnessChange Function-------------------------------//
void BrightnessChange() {
  if (LED_ON == true) {
    
    if (button_bright_count >= 1 && button_bright_count <= 4) {
      
      int LAST_BRIGHTNESS = BRIGHTNESS;
      BRIGHTNESS = BrightnessLevels[button_bright_count - 1];
      DELAY = DelaySettings[button_bright_count - 1];
      
      if (BRIGHTNESS > LAST_BRIGHTNESS) {
        for (int i = LAST_BRIGHTNESS; i < BRIGHTNESS; i++) {
          FastLED.setBrightness(i);
          FastLED.show();
          if (BRIGHTNESS - LAST_BRIGHTNESS > 40) {
            delay(12);
          }
          else {
            delay(30);
          }
        }
      }
      else {
        for (int i = LAST_BRIGHTNESS; i >= BRIGHTNESS; i--) {
          FastLED.setBrightness(i);
          FastLED.show();
          if (LAST_BRIGHTNESS - BRIGHTNESS > 40) {
            delay(12);
          }
          else {
            delay(30);
          }
        }
      }
    }
  }
}

//--------------------------------------------------------------------------------//

//----------------------------On Off Function------------------------------------//
void LED_OnOff() {
  //Serial.println("Function is called");
  if (button_onoff_count == 1) {
    Set_Colour(CRGB::Red);
    FastLED.show();
  }
  else if (button_onoff_count == 2) {
    FastLED.clear();
    FastLED.show();
  }
}
//-------------------------------------------------------------------------------//

//---------------------------------------------LED Libraries and Functions END------------------------------------------------------//


//-------------------------Servo Libraries and functions----------------------------------------------------------------------//
#include <ESP32Servo.h>

Servo Servo1;
Servo Servo2;

int Servo1_speed;
int Servo2_speed;

void Assign_Servo1_spd() {
  if (UD_1_mapped != 0) {
    Servo1_speed = map(UD_1_mapped, -1000, 1000, 40, 140);
  }
  else if (UD_1_mapped == 0) {
    Servo1_speed = 95;
  }
  Servo1.write(Servo1_speed);
}

void Assign_Servo2_spd() {
  if (UD_2_mapped != 0) {
    Servo2_speed = map(UD_2_mapped, -1000, 1000, 140, 40);
  }
  else if (UD_2_mapped == 0) {
    Servo2_speed = 95;
  }
  Servo2.write(Servo2_speed);
}
//--------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------Quack Libraries and Function-------------------------------------------------------------------------//
#include <SPIFFS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>

// Define pin assignments for I2S audio output and the button
int I2S_BCLK = I2S_BCLK_PIN;
int I2S_LRC = I2S_LRC_PIN;
int I2S_DOUT = I2S_DOUT_PIN; 

// Initialize objects for audio playback
AudioGeneratorWAV audioGenerator;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *output;

//------------------------Function------------------------------//
void Quack_Quack() {
  if (button_quack_count == 1 || button_quack_count == 2) {
    file = new AudioFileSourceSPIFFS("/Double Quack.wav");

    // Check if the file was opened successfully
    if (!file->isOpen()) {
      Serial.println("Failed to open WAV file");
      return;
    }

    // Start WAV playback
    audioGenerator.begin(file, output);
    while (audioGenerator.isRunning()) {
      audioGenerator.loop();
    }

    // Close the file
    file->close();
    Serial.println("file is playing");
    }
  }
//---------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------------------------------//

//-----------------------------------------LED Libraries and Functions Special-------------------------------------------------------//

//----------------------------ColourSwitch Function-------------------------------//
/*void SwitchColour() {
  // Check if the count is within the valid range
  if (LED_ON == true) {
    if (button_light_count >= 1 && button_light_count <= 7) {
      // Fade down
      Fade_down();
      // Set color based on count
      Set_Colour(colours[button_light_count - 1]);  // Array index is 0-based
      // Fade up
      Fade_up();
    }
  }
}*/

int last_colour;

void SwitchColouronitsown() {
  // Check if the count is within the valid range
  if (LED_ON == true) {
    if (button_light_count == 1) {
      while (true) {
        for (int i = last_colour; i < 7; i++) {

          //-----------------------------------Servo Loop-----------------------//  
          Assign_Servo1_spd();
          Assign_Servo2_spd();
          //--------------------------------------------------------------------//
      
          int button_bright_currentcount = button_bright_count;
          if (button_bright_currentcount != button_bright_lastcount) {
            button_bright_lastcount = button_bright_currentcount;
            //Serial.print(button_bright_currentcount); Serial.print(" ");
            //Serial.println(button_bright_lastcount);
            BrightnessChange();
            //Serial.println(button_bright_count);
          }
          else if (button_bright_currentcount == 0) {
              BRIGHTNESS = BrightnessLevels[2];
              DELAY = DelaySettings[2];
              FastLED.setBrightness(BRIGHTNESS);
          }
          int button_quack_currentcount = button_quack_count;
          if (button_quack_currentcount != button_quack_lastcount) {
            button_quack_lastcount = button_quack_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            Quack_Quack();
          }
          int button_onoff_currentcount = button_onoff_count;
          if (button_onoff_currentcount != button_onoff_lastcount) {
            button_onoff_lastcount = button_onoff_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            LED_OnOff();
          }
          
          // Fade down
          Fade_down();
          // Set color based on count

          button_bright_currentcount = button_bright_count;
          if (button_bright_currentcount != button_bright_lastcount) {
            button_bright_lastcount = button_bright_currentcount;
            //Serial.print(button_bright_currentcount); Serial.print(" ");
            //Serial.println(button_bright_lastcount);
            BrightnessChange();
            //Serial.println(button_bright_count);
          }
          else if (button_bright_currentcount == 0) {
              BRIGHTNESS = BrightnessLevels[2];
              DELAY = DelaySettings[2];
              FastLED.setBrightness(BRIGHTNESS);
          }
          button_quack_currentcount = button_quack_count;
          if (button_quack_currentcount != button_quack_lastcount) {
            button_quack_lastcount = button_quack_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            Quack_Quack();
          }
          button_onoff_currentcount = button_onoff_count;
          if (button_onoff_currentcount != button_onoff_lastcount) {
            button_onoff_lastcount = button_onoff_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            LED_OnOff();
          }
          
          Set_Colour(colours[i]);  // Array index is 0-based

          button_bright_currentcount = button_bright_count;
          if (button_bright_currentcount != button_bright_lastcount) {
            button_bright_lastcount = button_bright_currentcount;
            //Serial.print(button_bright_currentcount); Serial.print(" ");
            //Serial.println(button_bright_lastcount);
            BrightnessChange();
            //Serial.println(button_bright_count);
          }
          else if (button_bright_currentcount == 0) {
              BRIGHTNESS = BrightnessLevels[2];
              DELAY = DelaySettings[2];
              FastLED.setBrightness(BRIGHTNESS);
          }
          button_quack_currentcount = button_quack_count;
          if (button_quack_currentcount != button_quack_lastcount) {
            button_quack_lastcount = button_quack_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            Quack_Quack();
          }
          button_onoff_currentcount = button_onoff_count;
          if (button_onoff_currentcount != button_onoff_lastcount) {
            button_onoff_lastcount = button_onoff_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            LED_OnOff();
          }

          // Fade up
          Fade_up();
          if (i < 6) {
          last_colour = i;          
          } 
          else {
            last_colour = 0;
          }

          //-----------------------------------Servo Loop-----------------------//  
          Assign_Servo1_spd();
          Assign_Servo2_spd();
          //--------------------------------------------------------------------//
      
          button_bright_currentcount = button_bright_count;
          if (button_bright_currentcount != button_bright_lastcount) {
            button_bright_lastcount = button_bright_currentcount;
            //Serial.print(button_bright_currentcount); Serial.print(" ");
            //Serial.println(button_bright_lastcount);
            BrightnessChange();
            //Serial.println(button_bright_count);
          }
          else if (button_bright_currentcount == 0) {
              BRIGHTNESS = BrightnessLevels[2];
              DELAY = DelaySettings[2];
              FastLED.setBrightness(BRIGHTNESS);
          }
          button_quack_currentcount = button_quack_count;
          if (button_quack_currentcount != button_quack_lastcount) {
            button_quack_lastcount = button_quack_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            Quack_Quack();
          }
          button_onoff_currentcount = button_onoff_count;
          if (button_onoff_currentcount != button_onoff_lastcount) {
            button_onoff_lastcount = button_onoff_currentcount;
            //Serial.print(button_onoff_currentcount); Serial.print(" "); 
            //Serial.println(button_onoff_lastcount);
            LED_OnOff();
          }
          
          if (button_light_count == 2 || LED_ON == false) {
          break;
          }
        }  
        if (button_light_count == 2 || LED_ON == false) {
          break;
        }
      }
    }
  }
  else if (LED_ON == false) {
    last_colour = 0;

  }
}
//--------------------------------------------------------------------------------//

//---------------------------------------------LED Libraries and Functions Special END------------------------------------------------------//



//--------------------------------Final function that assigns to everything when data is received--------------------------//
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //Serial.print("Bytes received: ");
  //Serial.println(len);

  //Serial.print("Switch State: ");
  //Serial.println(myData.button_onoff_count2);
  button_onoff_count = myData.button_onoff_count2;
  //Serial.println(button_onoff_count);

  //Serial.print("LED State: ");
  //Serial.println(myData.LED_ON2);
  LED_ON = myData.LED_ON2;
  //Serial.println(LED_ON);

  //Serial.print("Light State: ");
  //Serial.println(myData.button_light_count2);
  button_light_count = myData.button_light_count2;
  //Serial.println(button_light_count);

  //Serial.print("Brightness State: ");
  //Serial.println(myData.button_bright_count2);
  button_bright_count = myData.button_bright_count2;
  //Serial.println(button_bright_count);

  //Serial.print("myData.UD_1_mapped2: "); Serial.print(myData.UD_1_mapped2);
  UD_1_mapped = myData.UD_1_mapped2;
  //Serial.print(" UD_1_mapped "); Serial.println(UD_1_mapped);

  //Serial.print("myData.UD_2_mapped2: "); Serial.print(myData.UD_2_mapped2);
  UD_2_mapped = myData.UD_2_mapped2;
  //Serial.print(" UD_2_mapped "); Serial.println(UD_2_mapped);

  button_quack_count = myData.button_quack_count2;  

}
//--------------------------------------------------------------------------------------------------------------------------//





void setup() {

//--------------------------------ESPNOW Setup--------------------------------//
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP_NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
//-----------------------------------------------------------------------------//

//-----------------------------Motor Setup-------------------------------------//
  Servo1.attach(Servo1_pin);
  Servo2.attach(Servo2_pin);

  //Servo1.setPeriodHertz(200);
  //Servo2.setPeriodHertz(200);

//-----------------------------------------------------------------------------//

//----------------------------------Quack Setup--------------------------------//
  if (!SPIFFS.begin()) {
      Serial.println("Failed to mount SPIFFS filesystem");
      return;
    }

    // Initialize I2S audio output
    output = new AudioOutputI2S();
    output->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    output->SetGain(3);


    Serial.println("Setup complete");
//-----------------------------------------------------------------------------//

//------------------------------------FastLED Setup----------------------------//
  FastLED.addLeds<CHIPSET, LED_DATA_PIN>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_AMPS); //takes precedence over brightness
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  //-----------------------------------------------------------------------------//
}

void loop() {
  // put your main code here, to run repeatedly:
//-----------------------------------Servo Loop-----------------------//  
  Assign_Servo1_spd();
  Assign_Servo2_spd();
//--------------------------------------------------------------------//
  int button_bright_currentcount = button_bright_count;
  if (button_bright_currentcount != button_bright_lastcount) {
    button_bright_lastcount = button_bright_currentcount;
    //Serial.print(button_bright_currentcount); Serial.print(" ");
    //Serial.println(button_bright_lastcount);
    BrightnessChange();
    //Serial.println(button_bright_count);
  }
  else if (button_bright_currentcount == 0) {
      BRIGHTNESS = BrightnessLevels[2];
      DELAY = DelaySettings[2];
      FastLED.setBrightness(BRIGHTNESS);
  }

  int button_quack_currentcount = button_quack_count;
  if (button_quack_currentcount != button_quack_lastcount) {
    button_quack_lastcount = button_quack_currentcount;
    //Serial.print(button_onoff_currentcount); Serial.print(" "); 
    //Serial.println(button_onoff_lastcount);
    Quack_Quack();
  }
  int button_onoff_currentcount = button_onoff_count;
  if (button_onoff_currentcount != button_onoff_lastcount) {
    button_onoff_lastcount = button_onoff_currentcount;
    //Serial.print(button_onoff_currentcount); Serial.print(" "); 
    //Serial.println(button_onoff_lastcount);
    LED_OnOff();
  }
  int button_light_currentcount = button_light_count;
  if (button_light_currentcount != button_light_lastcount) {
    button_light_lastcount = button_light_currentcount;
    //Serial.print(button_light_currentcount); Serial.print(" "); 
    //Serial.println(button_light_lastcount);
    SwitchColouronitsown();
    //Serial.println(button_light_count);
  }      
}
