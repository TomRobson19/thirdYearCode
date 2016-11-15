#####################################################################

import cv2
import os
import numpy as np
import random
import math

directory_to_cycle = "road-images2016-DURHAM"

#####################################################################

# display all images in directory

for filename in list(sorted(os.listdir(directory_to_cycle))):

    # if it is a PNG file

    if '.png' in filename:
        print(os.path.join(directory_to_cycle, filename));

        # read it and display in a window

        img = cv2.imread(os.path.join(directory_to_cycle, filename), cv2.IMREAD_COLOR)

        #remove the top 60% of the image to focus on the roads
        height = img.shape[0]
        img = img[(int)(7*height/10):height,0:img.shape[1]]


        # remove all non-blue dominant pixels
        # for i in range(img.shape[0]):
        #     for j in range(img.shape[1]):
        #         if (img[i][j][1]>=img[i][j][0]) and (img[i][j][1]>=img[i][j][2]):
        #             img[i][j][0] = 0
        #             img[i][j][1] = 0
        #             img[i][j][2] = 0
        #         if (img[i][j][2]>img[i][j][0]):
        #             img[i][j][0] = 0
        #             img[i][j][1] = 0
        #             img[i][j][2] = 0

        #hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        #gray = np.zeros((hsv.shape[0],hsv.shape[1]), np.uint8)

        
        #for i in range(hsv.shape[0]):
         #   for j in range(hsv.shape[1]):
          #      hsv[i][j][2] = 255
                #gray[i][j] = (0.5*(hsv[i][j][0]))+(0.5*(hsv[i][j][1]))
                #gray[i][j] = ((hsv[i][j][2]))

        #bgr = cv2.cvtColor(hsv,cv2.COLOR_HSV2BGR)
        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

        filtered = cv2.bilateralFilter(gray,9,75,75)

        median = np.median(filtered)
        # apply Canny edge detection using the computed median
        lower = int(max(0, (1.0 - 0.33) * median))
        upper = int(min(255, (1.0 + 0.33) * median))

        canny = cv2.Canny(filtered, lower, upper)

        #contours = cv2.findContours(canny,cv2.RETR_CCOMP,cv2.CHAIN_APPROX_NONE)

        canny = cv2.morphologyEx(canny, cv2.MORPH_OPEN, (5,5))

        canny = cv2.morphologyEx(canny, cv2.MORPH_CLOSE, (5,5))

        data = cv2.findNonZero(canny)

        t = 2
        v = 100 
        trials = 10000
        points = []
        lineArray = []
        gradientBound = 0.1
        similarityBound = 2

        for counter in range(trials):
            r1 = random.randint(0,len(data)-1)
            r2 = random.randint(0,len(data)-1)

            points = [data[r1][0],data[r2][0]] #select points from non zeros
            
            if (points[0][0]-points[1][0] != 0):
                gradient = (points[0][1]-points[1][1])/float(points[0][0]-points[1][0])
            else:
                gradient = (points[0][1]-points[1][1])/float((points[0][0]-points[1][0])+1)

            c = points[0][1] - (gradient*points[0][0])

            length = math.sqrt(abs(points[0][0]-points[1][0])**2 + abs(points[0][1]-points[1][1])**2)

            if (abs(gradient) < 0.5): #remove horizontal lines
                counter -= 1
            elif (length < 50):
                counter -= 1
            else:
                lines = np.zeros((canny.shape[0],canny.shape[1]), np.uint8)
                #lines = cv2.line(lines,(points[0][0],points[0][1]),(points[1][0],points[1][1]),255,2)

                lines = cv2.line(lines,(int((0-c)/gradient),0),(int((canny.shape[0]-c)/gradient),canny.shape[0]),255,2)

                compare =  np.zeros((canny.shape[0],canny.shape[1]), np.uint8)
                compare = cv2.bitwise_and(canny,lines)

                pixels = cv2.findNonZero(compare)

                if (len(pixels) > v):
                    lineArray.append([(points[0][0],points[0][1]),(points[1][0],points[1][1]),gradient])
                    draw = True
                    for i in range (0,len(lineArray)-1):
                        #reject if gradient is too similar, the lines cross over or the points are too close together
                        if  (lineArray[i][2] - gradientBound <= gradient <= lineArray[i][2] + gradientBound) or \
                            (lineArray[i][0][0] < points[0][0] and lineArray[i][1][0] > points[1][0]) or \
                            (lineArray[i][0][0] > points[0][0] and lineArray[i][1][0] < points[1][0]) or \
                            (lineArray[i][0][0] - similarityBound <= points[0][0] <= lineArray[i][0][0] + similarityBound) or \
                            (lineArray[i][1][0] - similarityBound <= points[0][0] <= lineArray[i][1][0] + similarityBound) or \
                            (lineArray[i][0][0] - similarityBound <= points[1][0] <= lineArray[i][0][0] + similarityBound) or \
                            (lineArray[i][1][0] - similarityBound <= points[1][0] <= lineArray[i][1][0] + similarityBound):
                                draw = False
                                break
                    if draw == True:
                        #img = cv2.line(img,(points[0][0],points[0][1]),(points[1][0],points[1][1]),(0,0,255),3)
                        img = cv2.line(img,(int((0-c)/gradient),0),(int((canny.shape[0]-c)/gradient),canny.shape[0]),(0,0,255),3)


        canny = cv2.cvtColor(canny,cv2.COLOR_GRAY2BGR)
        out = np.concatenate((img,canny),axis=0)

        cv2.imshow('image',out)
        key = cv2.waitKey(1000000000) # wait for this long or until any key press
        if (key == ord('x')):
            break


# close all windows

cv2.destroyAllWindows()

#####################################################################