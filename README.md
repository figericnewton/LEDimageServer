# LEDimageServer
The LED image server provides an interface for controlling a screen of individually programmable LED's using an ESP8266 WiFi board.

Project structure:
```
/data                    ... Folder not included in this repository, but holds all the images and animations
  /anim                  ... Contains the animations to be displayed on the LED screen
  /img                   ... Contains image files to be displayed on the LED screen
  /font                  ... Crude fixed-width 15px wide by 36px high font I made for displaying on the screen
/server                  ... Folder containing the code which actually runs on the ESP8266 chip
  server.ino             ... This is the 'main' program, sets up the shared resources and calls other parts of the program as needed
  AnimationInterface.ino ... This defines the setup and updating of the NeoPixelBus objects while displaying an animation.
  ImageInterface.ino     ... This defines the setup and updating of the NeoPixelBus objects while displaying a single image.
  TextInterface.ino      ... This sets up and updates the NeoPixelBus objects to display text on the screen
  WebInterface.ino       ... This handles servering up the website and changing state
  credentials.h          ... defines my SSID and PPK for my network (not included in the repository)
/web                     ... Contains website for intuitive user interface
  home.html              ... home page for the server gives a preview of what's currently running
  select_animation.html  ... allows a user to switch between animations displayed
  select_image.html      ... allows a user to switch between images to display
  text_display.html      ... interface for setting up text to be displayed on the LED screen
  upload_image.html      ... simple interface for selecting a new bitmap file to load onto the device
  /res                   ... resources used by the website
    displayOff.png       ... preview image for the display being turned off
/tools                   ... This folder has tools not used during runtime
  convertVideo.py        ... uses VLC and PIL packages to convert a video file into individual frames stored in GRB binary format for fast display on the board
  deploy.py              ... simple helper script for moving files to SD card which stores data outside of program memory for the NodeMCU board
  StripCutGuide.ino      ... simple program for the controller that was used for testing and cutting LED strips to size
```

## Project Hardware
 - NodeMCU V2 (ESP8266 and CP2102 chips). This is my LED controller and server.
 - 18 150px WS2812B strips, cut in half and assembled as a 75px by 36px screen with data transmitted in a row-major alternating pattern
 - 4 channel logic level converter. Steps up the 3.3V outputs from ESP8266 to 5V on the three buses I use.
 - 5V 60A regulated powersupply for the pixels and board... I may need to get a bigger one soon
 - Micro SD slot
 - Lots of wire

## Wiring Schematic
 FIXME: create and add a schematic.

## Display Operation
### The display is operated at the high level as a state machine with 5 different modes:
 - DISP_OFF -- this is the mode for display being turned off
 - DISP_IMAGE -- this mode displays a static image or scrolls the image across the screen
 - DISP_ANIMATING -- in this mode frames are loaded at around 30fps to the LED screen creating a video display
 - DISP_TEXT -- for this mode text is either fixed on the screen or scrolling across
 - DISP_CAMERA -- not yet implemented, future intent is to stream video from a client through the webserver

### Website Interface
The current mode is set and updated over http through the server's website. Note for security my home network router is setup to give the server a static IP and firewalls block any inbound traffic to the LED server from outside my home subnet. Here is what the animation selection page looks like
![Animation selection page](https://i.imgur.com/VtC7KcV.png)

And here's the home page
![Home page](https://i.imgur.com/yvMcvck.png)

### Controlling the LED's
The display itself is broken up into three different buses -- top, mid, and bot -- each of which consists of 12 rows for a total of 900 pixels per sector. This was done so that each bus can be updated asynchronously giving a faster refresh rate for the 2700pixel screen. The NeoPixelBus library does the heavy lifting of updating GPIO3 (top) using DMA, GPIO2 (mid) using UART, and GPIO1 (bot) using UART.

## Future Work
There's still lots to be done with this project
- I want to play with streaming video from a camera to the screen and then maybe doing some video manipulation on top of that
- There are a lot of bugs right now in the code since I just went through a big refactor to put a nice web interface on top of things
- I need to finish waterproofing the hardware and mount the panel on the side of the house
- The possibilities seem quite endless for what I can do with this! I'm thinking long term it would be cool to set up triggers for the screen. Like for instance I could put a motion sensor out by the street to detect when car or person is nearing the house and then flash something on the screen as they come close (think scaring tricker treaters!)

## Credits
- This project makes heavy usage of the NeoPixelBus library produced by Makuna https://github.com/Makuna/NeoPixelBus
- Project also relies on style sheets available free to use from https://www.w3schools.com/w3css/default.asp
