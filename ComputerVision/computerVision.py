#####################################################################

# Example : load and display a set of images from a directory
# basic illustrative python script

# For use with provided test / training datasets

# Author : Toby Breckon, toby.breckon@durham.ac.uk

# Copyright (c) 2015 / 2016 School of Engineering & Computing Science,
#                    Durham University, UK
# License : LGPL - http://www.gnu.org/licenses/lgpl.html

# version: 0.2 (bug fix on waitKey() usage - line 39)
# version: 0.3 (python 3 - line 33)

#####################################################################

import cv2
import os
import numpy as np

directory_to_cycle = "road-images2016-DURHAM"

#####################################################################

# display all images in directory

for filename in os.listdir(directory_to_cycle):

    # if it is a PNG file

    if '.png' in filename:
        print(os.path.join(directory_to_cycle, filename));

        # read it and display in a window

        img = cv2.imread(os.path.join(directory_to_cycle, filename), cv2.IMREAD_COLOR)

        height = img.shape[0]
        img = img[(int)(height/2):height,0:img.shape[1]]

        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) #this or hsv?

        canny = cv2.Canny(gray, 100, 200)

        data = cv2.findNonZero(canny)









        canny = cv2.cvtColor(canny,cv2.COLOR_GRAY2BGR)
        out = np.concatenate((img,canny),axis=1)

        cv2.imshow('image',out)
        key = cv2.waitKey(1000000000) # wait for this long or until any key press
        if (key == ord('x')):
            break


# close all windows

cv2.destroyAllWindows()

#####################################################################