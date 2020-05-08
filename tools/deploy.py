
import os
import shutil
import time
dataDir = os.path.join('..', 'data')
webDir = os.path.join('..', 'web')
target = 'D:' #configure to match SD card drive

#remove the old files from SD card
for dir in os.listdir(dataDir):
  toDelete = os.path.join(target, dir)
  if os.path.exists(toDelete):
    shutil.rmtree(toDelete)

toDelete = os.path.join(target, 'web')
if os.path.exists(toDelete):
  shutil.rmtree(toDelete)

time.sleep(1)

#copy over the new files to the SD card
for dir in os.listdir(dataDir):
  if os.path.isdir(dir):
    shutil.copytree(os.path.join(dataDir, dir), os.path.join(target, dir))

shutil.copytree(webDir, os.path.join(target, 'web'))
