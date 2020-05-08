
import os
import shutil
import time
dataDir = os.path.join('..', 'data')
webDir = os.path.join('..', 'web')
target = 'D:' #configure to match SD card drive

#remove the old files from SD card
for toDelete in [os.path.join(target, 'web'), os.path.join(target, 'data')]:
  if os.path.exists(toDelete):
    shutil.rmtree(toDelete)

time.sleep(1)

#copy over the new files to the SD card
shutil.copytree(dataDir, os.path.join(target, 'data'))
shutil.copytree(webDir, os.path.join(target, 'web'))
