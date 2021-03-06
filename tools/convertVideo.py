# LEDimageServer - webserver and controller application for a screen of LEDs
# Copyright (C) 2020 Eric Newton

# LEDimageServer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# LEDimageServer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with LEDimageServer.  If not, see <https://www.gnu.org/licenses/>.

import os
import re
import sys
import shutil
import time
import subprocess
import PIL.Image
import json
from tkinter import *
import tkinter, tkinter.filedialog

#gamma correction table
gamTable = [
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,
    3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,
    6,   7,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  11,  11,  11,
    12,  12,  13,  13,  14,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,
    19,  20,  20,  21,  22,  22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,
    29,  30,  30,  31,  32,  33,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,
    41,  42,  43,  43,  44,  45,  46,  47,  48,  49,  50,  50,  51,  52,  53,  54,
    55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  71,
    72,  73,  74,  75,  76,  77,  78,  80,  81,  82,  83,  84,  86,  87,  88,  89,
    91,  92,  93,  94,  96,  97,  98,  100, 101, 102, 104, 105, 106, 108, 109, 110,
    112, 113, 115, 116, 118, 119, 121, 122, 123, 125, 126, 128, 130, 131, 133, 134,
    136, 137, 139, 140, 142, 144, 145, 147, 149, 150, 152, 154, 155, 157, 159, 160,
    162, 164, 166, 167, 169, 171, 173, 175, 176, 178, 180, 182, 184, 186, 187, 189,
    191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
    223, 225, 227, 229, 231, 233, 235, 238, 240, 242, 244, 246, 248, 251, 253, 255
]

srcFile = tkinter.filedialog.askopenfilename(title='Select video you want to convert') # shows dialog box and return the path
srcFile = srcFile.replace('/', os.sep)
vidName = input("Enter the name to give video:\n")
if not re.match("^[a-zA-Z_]*$", vidName):
  print ("Only letters or underscores allowed.")
  sys.exit()
if len(vidName) > 8:
  print ("Name must be less than 8 characters.")

dstDir = os.path.join(os.path.dirname(sys.path[0]), 'data', 'anim', vidName)
if os.path.exists(dstDir):
  shutil.rmtree(dstDir)

time.sleep(1)
os.mkdir(dstDir)
time.sleep(1)

vlcLoc = r"C:\Program Files (x86)\VideoLAN\VLC"
os.chdir(vlcLoc)
cmd = 'vlc.exe "' + srcFile + '" --no-avcodec-corrupted --video-filter=scene --scene-path="' + dstDir + '" --scene-format="bmp" --scene-width="72" --scene-height="36" --scene-prefix="" --scene-ratio=1 vlc://quit'
os.system(cmd)
print (cmd)
time.sleep(1)

gifImgs = []
frameCount = 0
with open(os.path.join(dstDir, 'data.grb'), 'wb') as grbOutFile:
  for filename in os.listdir(dstDir):
    if filename == 'data.grb':
      continue

    try:
      im = PIL.Image.open(os.path.join(dstDir, filename))
    except:
      os.remove(os.path.join(dstDir, filename))
      continue
    if len(gifImgs) < 15:
      gifImgs.append(im)
    
    frameCount += 1
    print ('processing frame ' + str(frameCount))
    byte_dat = im.tobytes("raw","RGB");
    for r, g, b in zip(*[iter(byte_dat)]*3):
      grbOutFile.write(gamTable[g].to_bytes(1,'little'))
      grbOutFile.write(gamTable[r].to_bytes(1,'little'))
      grbOutFile.write(gamTable[b].to_bytes(1,'little'))
    os.remove(os.path.join(dstDir, filename))

gifImgs[0].save(os.path.join(dstDir, 'pvw.gif'), save_all=True, append_images=gifImgs[1:], duration=40, loop=0)

metadata = {}
metadata['frames'] = frameCount
metadata['prevName'] = 'pvw.gif'

with open(os.path.join(dstDir, 'meta.json'), 'w+', newline='') as metafile:
  json.dump(metadata, metafile, indent=2)
