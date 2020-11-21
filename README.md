# LEDimageServer
The LED image server provides an interface for controlling a screen of individually programmable LED's using an ESP8266 WiFi board.

Project structure:
```
/data                    ... Folder not included in this repository, but holds all the images and animations
  /anim                  ... Contains the animations to be displayed on the LED screen
  /img                   ... Contains image files to be displayed on the LED screen
/server                  ... Folder containing the code which actually runs on the ESP8266 microcontroller
  animation.cpp          ... Operating mode where the user selects an animation off the webpage and the animation is then displayed to the pixels in a loop
  credentials.h          ... defines my SSID and PPK for my network (not included in the repository)
  debugging.cpp          ... Defines some useful features for debugging the server. Note that debugging is a bit tricky due to overlap of Serial port (Uart) usage
  debugging.h            ... (see above)
  displayOff.cpp         ... Operating mode where the display is turned off
  font.h                 ... Unused, defines a bitmap font I may use in the future
  globals.h              ... Defines shared libraries and other various assets needed by all features of the server
  image_display.cpp      ... Unused, will be very similar to animation.cpp when implemented, just displays a single picture instead of a video
  operating_mode.cpp     ... This is the interface layer to various operating modes
  operating_mode.h       ... (see above)
  pong.cpp               ... Operating mode where users connect to the server via a webpage and play a game of pong on the pixel display
  server.ino             ... This is the 'main' program, primarily contains outer loop code for updating the actual NeoPixelBus objects and calls other parts of the program
  text_display.cpp       ... Unused, may be used in the future along with font to allow the user to send messages to the screen
  video_stream.cpp       ... Operating mode where a user can connect and either receive a live stream of what's being displayed, or they can send a stream of video to be displayed on the screen
  website.cpp            ... Provides the backbone for the webserver which handles client connections
  website.h              ... (see above)
/web                     ... Contains website for intuitive user interface with LEDs
  home.html              ... home page for the server gives a preview of what's currently running
  pong.html              ... user interface for playing a game of pong on the display
  select_animation.html  ... allows a user to select an animation for display on the server
  select_image.html      ... Unused, in the future will be used to allow a user to select an image to display on the LEDs
  text_display.html      ... Unused, in the future will allow for sending a text message to the screen
  upload_image.html      ... Unused, in the future may be used to allow for an image to be uploaded to the server
  video_stream.html      ... Provides a page for streaming video to/from the server -- currently only works when run locally due to browser restrictions requiring TLS for webcam access
  /res                   ... resources used by the website
    displayOff.png       ... preview image for the display being turned off
/tools                   ... This folder has tools not used during runtime
  convertVideo.py        ... uses VLC and PIL packages to convert a video file into individual frames stored in GRB binary format for fast display on the board
  deploy.py              ... Helper script for moving files to SD card which stores the website and large data items
  StripCutGuide.ino      ... simple program for the controller that was used for testing and cutting LED strips to size
```

## Project Hardware
![Bread Board](https://i.imgur.com/fI3c1ja.png)
 - NodeMCU V2 (ESP8266 and CP2102 chips). This is my LED controller and server.
 - 18 150px WS2812B strips, cut in half and assembled as a 75px by 36px screen with data transmitted in a row-major alternating pattern
 - 4 channel logic level converter. Steps up the 3.3V outputs from ESP8266 to 5V on the three buses I use.
 - 5V 60A regulated powersupply for the pixels and board... I may need to get a bigger one soon
 - Micro SD slot
 - Lots of wire

## Wiring Schematic
 FIXME: create and add a schematic.
![LED wiring](https://i.imgur.com/WgRxYZS.png)
![Board wiring](https://i.imgur.com/xizbI3t.png)

## Display Operation
### The display is operated as a state machine where each state corresponds to an OperatingMode object:
OperatingMode objects provide an abstract interface for a section of the website and a way to use the LED display. This interface consists of the following:
 - a setup function called initially for creating any unique parts of the website, listeners, etc.
 - a function callback for updating the pixel display data when its the selected operating mode

Currently the following OperatingModes are implemented
 - OffOperatingMode -- as the name implies, this is the default mode and triggered whenever the user selects the off action on the website
 - AnimationOperatingMode -- in this mode a File containing a video saved off in raw GRB format is read and displayed on the screen
 - PongOperatingMode -- in this mode, users connect to the webpage and play a game of pong (live control is handled through websockets)
 - VideoOperatingMode -- in this mode a user can either stream data from the server to get a real-time preview of the display, or a user can stream webcam data to the server for live display (currently only works when run locally due to browser TLS requirement)

### Website Interface
The current mode is set and updated over http through the server's website. Note for security my home network router is setup to give the server a static IP and firewalls block any inbound traffic to the LED server from outside my home subnet. Here is what the animation selection page looks like
![Animation selection page](https://i.imgur.com/VtC7KcV.png)

And here's the home page
![Home page](https://i.imgur.com/yvMcvck.png)

### Controlling the LED's
The display itself is broken up into three different buses -- top, mid, and bot -- each of which consists of 12 rows for a total of 900 pixels per sector. This was done so that each bus can be updated asynchronously giving a faster refresh rate for the 2700pixel screen. The NeoPixelBus library does the heavy lifting of updating GPIO3 (top) using DMA, GPIO2 (mid) using UART, and GPIO1 (bot) using UART.

## Future Work
There's still lots to be done with this project
- more games would be cool, I'm thinking tetris and snake would be pretty easy/fun to code up
- I should probably go back and re-implement some of the "less interesting" features I previously had but haven't gone updated to work with the rest of the new code like text display and static images
- I should replace the images in this readme with current ones, the display from a hardware perspective has been improved quite a bit since last May haha
- I should also consider making a video demonstrating the project usage and throw it up on youtube or something

## Credits
- This project makes heavy usage of the NeoPixelBus library produced by Makuna https://github.com/Makuna/NeoPixelBus
- And also the ESPAsyncWebServer library created by me-no-dev Hristo Gochkov https://github.com/me-no-dev/ESPAsyncWebServer
- Project also relies on W3 style sheets available free to use from https://www.w3schools.com/w3css/default.asp
