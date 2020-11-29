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
import shutil
import time
dataDir = os.path.join('..', 'data')
webDir = os.path.join('..', 'web')
target = 'D:' #configure to match SD card drive

replaceData = input("Copy/replace data directory too [y/n]?\n")

#remove the old files from SD card
if os.path.exists(os.path.join(target,'web')):
  shutil.rmtree(os.path.join(target,'web'))
if replaceData == 'y' and os.path.exists(os.path.join(target,'data')):
  shutil.rmtree(os.path.join(target,'data'))

time.sleep(1)

#copy over the new files to the SD card
if replaceData == 'y':
  shutil.copytree(dataDir, os.path.join(target, 'data'))

shutil.copytree(webDir, os.path.join(target, 'web'))
