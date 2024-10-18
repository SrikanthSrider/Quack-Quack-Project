//----------------------ESPNOW libraries and funcions---------------------//
#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xA8, 0x42, 0xE3, 0xBB, 0x45, 0x84};

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

esp_now_peer_info_t peerInfo;

//use the below print statement to check that the connection is up and running

/*void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}*/
//-------------------------------------------------------------------//

//-------------------------------Pins--------------------------------//
int button_onoff_pin = 25;
int button_light_pin = 26;
int button_bright_pin = 27;
int button_quack_pin = 14;

const int UD_1_pin = 33;
const int UD_2_pin = 32;
//-------------------------------------------------------------------//

//-------------------------Button and functions Setup------------------------------//
#include <ezButton.h>

//---------------------------------On-Off Button-----------------------//
ezButton button_onoff(button_onoff_pin);

int button_onoff_laststate;
int button_onoff_count;
bool LED_ON;

int button_onoff_count_getter() {
  int button_onoff_currentstate = button_onoff.getState();
  if (button_onoff_currentstate != button_onoff_laststate) {
    button_onoff_laststate = button_onoff_currentstate;
    if (button_onoff_currentstate == 1) {
      if (button_onoff.getCount() == 1) {
        //Serial.println(button_onoff.getCount());
        button_onoff_count = 1;
        LED_ON = true;
      } 
      else if (button_onoff.getCount() == 2) {
        //Serial.println(button_onoff.getCount());
        button_onoff_count = 2;
        LED_ON = false;
        button_onoff.resetCount();
      }
      else {
        //Serial.println(button_onoff.getCount());
        button_onoff.resetCount();
        //Serial.println(button_onoff.getCount());
      }
    }
  }
  return button_onoff_count;
}
//-------------------------------------------------------------------//

//-----------------------Lightchange Button-------------------------//
ezButton button_light(button_light_pin);

int button_light_laststate;
int button_light_count;

/*int button_light_count_getter() {
  if (LED_ON == true) {
    int button_light_currentstate = button_light.getState();
    if (button_light_currentstate != button_light_laststate) {
      button_light_laststate = button_light_currentstate;
      if (button_light_currentstate == 1) {
        //Serial.println("LED IS ON");
        if (button_light.getCount() < 7) {
          button_light_count = button_light.getCount();
          //Serial.println(button_light_count);
        }
        else if (button_light.getCount() == 7) {
          button_light_count = 7;
          //Serial.print(button_light_count); Serial.print(" ");
          button_light.resetCount();
        }
        else {
          //Serial.println(button_light_count);
          button_light.resetCount();
          button_light_count = button_light.getCount();
          //Serial.println(button_light_count);
        }
      }
    }
  }
  else if (LED_ON == false) {
    //Serial.print("LED IS OFF ");
    //Serial.println(button_light.getCount());
    button_light.resetCount();
    button_light_count = button_light.getCount();
  }
  return button_light_count;
}*/

int button_light_count_getter_switchonitsown() {
  if (LED_ON == true) {
    int button_light_currentstate = button_light.getState();
    if (button_light_currentstate != button_light_laststate) {
      button_light_laststate = button_light_currentstate;
      if (button_light_currentstate == 1) {
        //Serial.println("LED IS ON");
        if (button_light.getCount() == 1) {
          button_light_count = button_light.getCount();
          //Serial.println(button_light_count);
        }
        else if (button_light.getCount() == 2) {
          button_light_count = 2;
          //Serial.print(button_light_count); Serial.print(" ");
          button_light.resetCount();
        }
        else {
          //Serial.println(button_light_count);
          button_light.resetCount();
          button_light_count = button_light.getCount();
          //Serial.println(button_light_count);
        }
      }
    }
  }
  else if (LED_ON == false) {
    //Serial.print("LED IS OFF ");
    //Serial.println(button_light.getCount());
    button_light.resetCount();
    button_light_count = button_light.getCount();
  }
  return button_light_count;
}
//-----------------------------------------------------------------//

//---------------------Brightness Change Button---------------------//
ezButton button_bright(button_bright_pin);

int button_bright_laststate;
int button_bright_count;

int button_bright_count_getter() {
  if (LED_ON == true) {
    int button_bright_currentstate = button_bright.getState();
    if (button_bright_currentstate != button_bright_laststate) {
      button_bright_laststate = button_bright_currentstate;
      if (button_bright_currentstate == 1) {
        //Serial.println("LED IS ON");        
        if (button_bright.getCount() < 4) {
          button_bright_count = button_bright.getCount();
         //Serial.println(button_bright_count);
        }
        else if (button_bright.getCount() == 4) {
          button_bright_count = 4;
          //Serial.print(button_bright_count); Serial.print(" ");
          button_bright.resetCount();
        }
        else {
          //Serial.println(button_light_count);
          button_bright.resetCount();
          button_bright_count = button_bright.getCount();
          //Serial.println(button_bright_count);
        }
      }
    }
  }
  else if (LED_ON == false) {
    //Serial.print("LED IS OFF ");
    //Serial.println(button_bright.getCount());
    button_bright.resetCount();
    button_bright_count = button_bright.getCount();
  }
  return button_bright_count;
}
//-----------------------------------------------------------------//

//---------------------------------Quack Button-----------------------//
ezButton button_quack(button_quack_pin);

int button_quack_laststate;
int button_quack_count;

int button_quack_count_getter() {
  int button_quack_currentstate = button_quack.getState();
  if (button_quack_currentstate != button_quack_laststate) {
    button_quack_laststate = button_quack_currentstate;
    if (button_quack_currentstate == 1) {
      if (button_quack.getCount() == 1) {
        //Serial.println(button_onoff.getCount());
        button_quack_count = 1;
      } 
      else if (button_quack.getCount() == 2) {
        //Serial.println(button_onoff.getCount());
        button_quack_count = 2;
        button_quack.resetCount();
      }
      else {
        //Serial.println(button_onoff.getCount());
        button_quack.resetCount();
        //Serial.println(button_onoff.getCount());
      }
    }
  }
  return button_quack_count;
}
//-------------------------------------------------------------------//


//--------------------------------Button and functions Setup END---------------------------------------//

//-------------------joystick declarations and functions-----------//

int UD_1;
int UD_2;

int UD_1_mapped;
int UD_2_mapped;

void Get_UD_1_mapped(){
  if (UD_1 > 2100 || UD_1 < 1500){
    UD_1_mapped = map(UD_1, 0, 4095, -1000, 1000);
  }
  else {
    UD_1_mapped = 0;
  }
}

void Get_UD_2_mapped(){
  if (UD_2 > 2100 || UD_2 < 1500){
    UD_2_mapped = map(UD_2, 0, 4095, 1000, -1000);
  }
  else {
    UD_2_mapped = 0;
  }
}

//-------------------------------------------------------------------//




void setup() {

//-----------------------ESPNOW Setup---------------------//
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initialising ESP-NOW");
    return;
  }
  //esp_now_register_send_cb(OnDataSent);

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
//--------------------------------------------------------//

//--------------Joystick Function set-up------------------//
//--------------------------------------------------------//

//-----------------------Button Setup---------------------//
  button_onoff.setDebounceTime(50);
  button_light.setDebounceTime(50);
  button_bright.setDebounceTime(50);
  button_quack.setDebounceTime(50);
  
//--------------------------------------------------------//
}




void loop() {
//------------------------Button loops-------------------------------------//
  button_onoff.loop();
  button_light.loop();
  button_bright.loop();
  button_quack.loop();
//-----------------------------------------------------------------------//


//-----------------------Button Functions to call-------------------------//
  button_onoff_count_getter();
  button_light_count_getter_switchonitsown();
  button_bright_count_getter();
  button_quack_count_getter();
  //Serial.print(button_onoff_count); Serial.print(" ");
//-----------------------------------------------------------------------//

//----------------Joy stick Functions---------------------------------------//  
  UD_1 = analogRead(UD_1_pin);
  UD_2 = analogRead(UD_2_pin);
  
  Get_UD_1_mapped();
  Get_UD_2_mapped();

  
//----------------------------------------------------------------------//


//-------------------------ESPNOW loop----------------------------------//

//---------Assigning to structure-------//
  myData.button_onoff_count2 = button_onoff_count;
  myData.LED_ON2 = LED_ON;
  myData.button_light_count2 = button_light_count;
  myData.button_bright_count2 = button_bright_count;

  myData.UD_1_mapped2 = UD_1_mapped;
  myData.UD_2_mapped2 = UD_2_mapped;
  myData.button_quack_count2 = button_quack_count;
  //Serial.print(myData.button_onoff_count2); Serial.print(" "); 
  //Serial.println(myData.button_bright_count2);

   /*Serial.print(" UD_1: "); Serial.print(UD_1); 
   Serial.print(" UD_1_mapped: "); Serial.print(UD_1_mapped);
   Serial.print(" myData.UD_1_mapped2 = "); Serial.println(myData.UD_1_mapped2);
   Serial.print(" UD_2: "); Serial.print(UD_2); 
   Serial.print(" UD_2_mapped: "); Serial.print(UD_2_mapped);
   Serial.print(" myData.UD_2_mapped2 = "); Serial.println(myData.UD_2_mapped2);
   Serial.println("-------------------------------------------------------------------");*/
  
//-------------------------------------//

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  /*if(result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  //Serial.println("Getting Delayed.");*/
  delay(10);
//----------------------------------------------------------------------//

}
