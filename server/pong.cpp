/*****************************************************************************
* LEDimageServer - webserver and controller application for a screen of LEDs *
* Copyright (C) 2020 Eric Newton                                             *
*                                                                            *
* LEDimageServer is free software: you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by       *
* the Free Software Foundation, either version 3 of the License, or          *
* (at your option) any later version.                                        *
*                                                                            *
* LEDimageServer is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of             *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
* GNU General Public License for more details.                               *
*                                                                            *
* You should have received a copy of the GNU General Public License          *
* along with LEDimageServer.  If not, see <https://www.gnu.org/licenses/>.   *
*****************************************************************************/

#ifndef PONG_CPP
#define PONG_CPP

#include "globals.h"

#define BALL_TAIL_SIZE 10
#define PADDLE_HEIGHT 8
#define MAX_BALL_SPEED 3

void pong__setup(AsyncWebServer* server);
void pong__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len);
void pong__updateFrame();
void pong__beginGame();
void pong__drawBoard();

//FIXME: these probably shouldn't be defined in this CPP file and instead should be shared?
NeoGamma<NeoGammaTableMethod> colorGamma;
const RgbColor BallColor(0, 0, 120); //blue
const RgbColor PaddleColor(0, 120, 0); //green
const RgbColor OffColor(0);

OperatingMode PongOperatingMode = {
  .setup = pong__setup,
  { .prevPath = "/web/res/displayOff.png" }, //FIXME: need to generate a preview image
  .updateFrame = pong__updateFrame,
};

AsyncWebSocket pong__ws("/pong");
struct GameStateInfo {
  int demand[2]; //demand for the two players
  int PY[2], PVY[2]; //position and velocity of the two players
  int BVX, BVY; //ball velocity components
  int BX[BALL_TAIL_SIZE], BY[BALL_TAIL_SIZE]; //position history for ball
  int gameOver;
} gameStateInfo;

void pong__drawBoard() {
  const uint16_t ballPattern = 0x6FF6;
  //pattern for ball is as follows, x, y give position of upper left corner in the 4x4 square
  //0110
  //1111
  //1111
  //0110
  
  //first draw the two paddles
  int i, hist, x, y;
  for (i = 0; i < 2; i++) {
    x = i*(PANEL_WIDTH - 1);
    for (y = 0; y < PANEL_HEIGHT; y++) {
      if ( y >= gameStateInfo.PY[i] && y < gameStateInfo.PY[i] + PADDLE_HEIGHT ) {
        neoPixFrameBuffer.SetPixelColor(x, y, PaddleColor);
      } else {
        neoPixFrameBuffer.SetPixelColor(x, y, OffColor);
      }
    }
  }
  //then draw the ball/history
  RgbColor newColor;
  for (hist = 0; hist < BALL_TAIL_SIZE; hist++) {
    newColor = colorGamma.Correct(RgbColor::LinearBlend(
        OffColor,
        BallColor,
        hist*1.0/(BALL_TAIL_SIZE - 1)));
    for (x = 0; x < 4; x++) {
      for (y = 0; y < 4; y++) {
        if ( ((ballPattern) & (1 << (4*x+y))) != 0 ) { //The bit in our 16 bit pattern is true
          neoPixFrameBuffer.SetPixelColor(gameStateInfo.BX[hist] + x, gameStateInfo.BY[hist] + y, newColor);
        }
      }
    }
  }
}
void pong__updateGameStateInfo() {
  for (int i = 0; i < 2; i++) {
    gameStateInfo.PVY[i] = gameStateInfo.demand[i] - gameStateInfo.PY[i];
    gameStateInfo.PY[i] = gameStateInfo.demand[i];
    if (gameStateInfo.PY[i] <= 0) { //up against wall
      gameStateInfo.PY[i] = 0;
    } else if (gameStateInfo.PY[i] >= PANEL_HEIGHT - PADDLE_HEIGHT ) { //up against wall
      gameStateInfo.PY[i] = PANEL_HEIGHT - PADDLE_HEIGHT;
    }
  }
  
  for (int i = 0; i < BALL_TAIL_SIZE - 1; i++) {
    gameStateInfo.BX[i] = gameStateInfo.BX[i+1];
    gameStateInfo.BY[i] = gameStateInfo.BY[i+1];
  }
  gameStateInfo.BX[BALL_TAIL_SIZE - 1] += gameStateInfo.BVX;
  gameStateInfo.BY[BALL_TAIL_SIZE - 1] += gameStateInfo.BVY;

  //check/correct for top and bottom wall collisions
  if (gameStateInfo.BY[BALL_TAIL_SIZE - 1] <= 0) {
    gameStateInfo.BVY *= -1;
    gameStateInfo.BY[BALL_TAIL_SIZE - 1] -= gameStateInfo.BY[BALL_TAIL_SIZE - 1] - 0;
  } else if (gameStateInfo.BY[BALL_TAIL_SIZE - 1] >= PANEL_HEIGHT - 4) {
    gameStateInfo.BVY *= -1;
    gameStateInfo.BY[BALL_TAIL_SIZE - 1] -= gameStateInfo.BY[BALL_TAIL_SIZE - 1] - (PANEL_HEIGHT - 4);
  }

  int yDiff;
  //check/correct for left and right paddle or game over
  for (int i = 0; i < 2; i++) {
    if (gameStateInfo.BX[BALL_TAIL_SIZE - 1] == i*(PANEL_WIDTH - 4)) { //on the side, do we collide with paddle?
      if ( gameStateInfo.BY[BALL_TAIL_SIZE - 1] >= gameStateInfo.PY[i] - 3 && gameStateInfo.BY[BALL_TAIL_SIZE - 1] <= gameStateInfo.PY[i] + PADDLE_HEIGHT - 1) {
        gameStateInfo.BVX *= -1; //flip the direction
        gameStateInfo.BVY += gameStateInfo.PVY[i]; //add the paddle's velocity
        yDiff = gameStateInfo.BY[BALL_TAIL_SIZE - 1] - gameStateInfo.PY[i] - (PADDLE_HEIGHT/2 - 2);
        if (yDiff < -1) { //add velocity based on paddle position relative to ball
          gameStateInfo.BVY += yDiff + 1;
        } else if (yDiff > 1) {
          gameStateInfo.BVY += yDiff - 1;
        }
        gameStateInfo.BX[BALL_TAIL_SIZE - 1] += 2*gameStateInfo.BVX; //visually the collision happened last round
      } else {
        gameStateInfo.gameOver = 1;
      }
    }
  }
  if (gameStateInfo.BVY > MAX_BALL_SPEED) {
    gameStateInfo.BVY = MAX_BALL_SPEED;
  } else if (gameStateInfo.BVY < -MAX_BALL_SPEED) {
    gameStateInfo.BVY = -MAX_BALL_SPEED;
  }
}
void pong__updateFrame() {
  //WRITE_OUT("pong__updateFrame\n");
  if (gameStateInfo.gameOver > 0) {
    gameStateInfo.gameOver++;
    if (gameStateInfo.gameOver > 30*4) { //start a new game
      pong__beginGame();
      neoPixFrameBuffer.ClearTo(OffColor);
    }
  } else {
    pong__updateGameStateInfo();
    pong__drawBoard();
  }
  pong__ws.cleanupClients();
}
void pong__setup(AsyncWebServer* server) {
  pong__ws.onEvent(pong__wsEvent);
  server->addHandler(&pong__ws);
  server->serveStatic("/pong.html", SDFS, "/web/pong.html");
}
void pong__beginGame() {
  gameStateInfo.gameOver = 0;
  for (int pnum = 0; pnum < 2; pnum++) {
    gameStateInfo.demand[pnum] = PANEL_HEIGHT/2 - PADDLE_HEIGHT/2; //start out in the center
    gameStateInfo.PY[pnum] = gameStateInfo.demand[pnum];
    gameStateInfo.PVY[pnum] = 0;
  }
  gameStateInfo.BVX = random(0, 2)*2 - 1;
  gameStateInfo.BVY = 0;
  for (int i = 0; i < BALL_TAIL_SIZE; i++) {
    gameStateInfo.BX[i] = PANEL_WIDTH/2 - 2;
    gameStateInfo.BY[i] = PANEL_HEIGHT/2 - 2;
  }
  WRITE_OUT("Starting new game of pong!\n");
}

void pong__wsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  if (type == WS_EVT_CONNECT) {
    WRITE_OUT("ws[%s][%u] connect\n", server->url(), client->id());
    if (CurrentOperatingMode != &PongOperatingMode) {
      pong__beginGame();
      CurrentOperatingMode = &PongOperatingMode;
    }
  } else if(type == WS_EVT_DISCONNECT) {
    WRITE_OUT("ws[%s][%u] disconnect\n", server->url(), client->id());
  } else if (type == WS_EVT_DATA) {
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    StaticJsonDocument<256> msgJSON;
    if (info->final && info->index == 0 && info->len == len) {
      //the whole message is in a single frame and we got all of its data
      WRITE_OUT("ws[%s][%u] received:\n", server->url(), client->id());
      WRITE_OUT("%.*s\n", info->len, (char *)data);
      if (deserializeJson(msgJSON, data)) {
        WRITE_OUT("Failed to deserialize client message!\n");
        return;
      }
      gameStateInfo.demand[msgJSON["player"].as<int>()] = msgJSON["demand"].as<int>();
    }
  } else {
    WRITE_OUT("ws[%s][%u] unhandled event type (ignored)\n", server->url(), client->id());
  }
}
#endif
