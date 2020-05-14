
import os
import shutil
import time
dataDir = os.path.join('..', 'data')
webDir = os.path.join('..', 'web')
target = 'D:' #configure to match SD card drive

replaceData = raw_input("Copy/replace data directory too [y/n]?\n")

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
