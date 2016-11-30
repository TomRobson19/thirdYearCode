#####################################################################

import cv2
import os
import numpy as np
import random
import math

directory_to_cycle = "road-images2016-DURHAM"

f = open('data.txt', 'w')
printCounter = 0
#####################################################################

# display all images in directory

for filename in list(sorted(os.listdir(directory_to_cycle))):

    # if it is a PNG file

    if '.png' in filename:
        # read it and display in a window

        original= cv2.imread(os.path.join(directory_to_cycle, filename), cv2.IMREAD_COLOR)

        #remove the top 70% of the image to focus on the roads
        height = original.shape[0]
        img = original[(int)(7*height/10):height,0:original.shape[1]]

        gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)

        filtered = cv2.bilateralFilter(gray,9,75,75)

        median = np.median(filtered)
        # apply Canny edge detection using the computed median
        lower = int(max(0, (1.0 - 0.33) * median))
        upper = int(min(255, (1.0 + 0.33) * median))

        canny = cv2.Canny(filtered, lower, upper)

        opening = cv2.morphologyEx(canny, cv2.MORPH_OPEN, (5,5))

        data = cv2.findNonZero(canny)

        t = 2
        v = 100 
        trials = 1000
        points = []
        lineArray = []
        gradientBound = 0.1
        similarityBound = 2
        linesDetected = 0
        keepChecking = True

        while keepChecking:
            for counter in range(trials):
                r1 = random.randint(0,len(data)-1)
                r2 = random.randint(0,len(data)-1)

                points = [data[r1][0],data[r2][0]] #select points from non zeros
                
                if (points[0][0]-points[1][0] != 0):
                    gradient = (points[0][1]-points[1][1])/float(points[0][0]-points[1][0])
                else:
                    gradient = (points[0][1]-points[1][1])/float((points[0][0]-points[1][0])+1)

                c = points[0][1] - (gradient*points[0][0]) #calculate y-intercept

                if (abs(gradient) < 0.5): #remove horizontal lines
                    counter -= 1
                else:
                    lines = np.zeros((canny.shape[0],canny.shape[1]), np.uint8)

                    #draw lines for the whole cut off image
                    lines = cv2.line(lines,(int((0-c)/gradient),0),(int((canny.shape[0]-c)/gradient),canny.shape[0]),255,2)

                    compare =  np.zeros((canny.shape[0],canny.shape[1]), np.uint8)
                    compare = cv2.bitwise_and(canny,lines)

                    pixels = cv2.findNonZero(compare)

                    if (len(pixels) > v):
                        lineArray.append([(int((0-c)/gradient),0),(int((canny.shape[0]-c)/gradient),canny.shape[0]),gradient])
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
                            img = cv2.line(img,(int((0-c)/gradient),0),(int((canny.shape[0]-c)/gradient),canny.shape[0]),(0,0,255),3)
                            linesDetected += 1
            if linesDetected == 0 and v > 20:
                v = v-10
            else:
                keepChecking = False

        #cv2.imwrite('img'+str(printCounter)+'.png',original)

        print(filename,': detected '+str(linesDetected)+' edges/lines')
        printCounter += 1

        #cv2.imshow('image',out)
        cv2.imshow('original',original)
        key = cv2.waitKey(1) # wait for this long or until any key press
        if (key == ord('x')):
            break


# close all windows
f.close()
cv2.destroyAllWindows()

#####################################################################