# Quack-Quack-Project
Codes for controlling lighting, sound and movement for any robot creations. In this particular case it is a robot duck which moved on water.

## Picture
<img src="https://github.com/user-attachments/assets/edc6f4a2-f1a6-4262-91e0-2ef387fea038" width="300" height="300"/>

### Movement

Duck had 2 big watermill like pedals on either side. Allowed duck to move back and forth, turn right left and rotate 360 degrees in either direction

### Sound

Duck had one speaker in order to make Quack Quack sounds. Sounds can be changed depending on file.

### Lights

7 different lights
Mode 1: Changing lights through each colour
Mode 2: Sticking to one colour of light
3 different levels of intensity available for each mode.

### Notes on actual code:
Master_Receiver_Sketch_Second is for the Receiver ESP32 and Master_Sender_Sketch_Second is for the Sender ESP32

In Master_Receiver_Sketch_Second, a placeholder called your_audio_file_name.wav exists. Replace that with your actual audio file stored in data.

Both sets of code have numerous Serial.print functions. Utilisize them whenever you run into errors. Optimal setting for serial print for ESP32 is 115200.
