# Hello World Example
#
# Welcome to the OpenMV IDE! Click on the green run arrow button below to run the script!
# ACTUAL GITHUB
import sensor, image, time, math
import pyb
from pyb import UART, LED

#thresholds = [(63, 100, -2, 19, 32, 127), (45, 100, -6, 127, -128, -29)]
thresholds = [(65, 84, -8, 7, 27, 127), (45, 100, -6, 127, -128, -29)]
orangethreshold = [(0, 100, 26, 127, 35, 127)]

attackisyellow = True
isdebug = True

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False, (-4.76043, -6.297, -3.76291))
sensor.set_brightness(1)
sensor.set_contrast(0)
sensor.set_saturation(2)
sensor.set_windowing((50, 0, 240, 240)) # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.
uart = UART(3, 115200, timeout_char = 10)

def biggestBlob(bBlob):
    maxBlob = bBlob[0]
    for blob in bBlob:
        if blob.area() > maxBlob.area():
            maxBlob = blob
    return maxBlob

#def golfballblob(bBlob):
    #golfballBlob = False
    #for blob in bBlob:
        #if blob.area() < 200:
            #if golfballBlob == False:
                #golfballBlob = blob
            #elif blob.area() > golfballBlob.area():
                #golfballBlob = blob
    #return golfballBlob

def sortBlob(blobs):
    if len(blobs) != 0:
        largestBlob = biggestBlob(blobs)
        if isdebug:
            img.draw_rectangle(largestBlob.rect())
        return round(largestBlob.cx()), round(largestBlob.cy())
    else:
        return 250, 0

#def goaldist(blobs, centre):
    #if len(blobs) != 0:
        #largestBlob = biggestBlob(blobs)
        #corners = largestBlob.rect().corners()
        #if corners[0][0] < centre[0] and corners[1][0] > centre[0]:
            #if corners[0][1] > centre[1]:
                #return int(corners[0][1] - centre[1])
            #else:
                #return int(centre[1] - corners[2][1])
        #elif corners[0][1] < centre[1] and corners[3][1] > centre[1]:
            #if corners[0][0] > centre[0]:
                #return int(corners[0][0] - centre[0])
            #else:
                #return int(centre[0] - corners[1][0])
        #else:
            #closestdist = 1000
            #for i in corners:
                #if int((i[0]**2 + i[1]**2)**0.5) < closestdist:
                    #closestdist = int((i[0]**2 + i[1]**2)**0.5)
            #return closestdist

def goalcorners(blobs, centre):
    if len(blobs) != 0:
        largestBlob = biggestBlob(blobs)
        if isdebug:
            img.draw_rectangle(largestBlob.rect())
        corners = largestBlob.rect()
        corners = [(corners[0], corners[1]), (corners[2]+corners[0], corners[1]), (corners[2]+corners[0], corners[3]+corners[1]), (corners[0], corners[3]+corners[1])]
        #print(corners)
        #print(centre)
        if corners[0][0] < centre[0] and corners[1][0] > centre[0]:
            #print('In the middle.')
            if corners[0][1] > centre[1]:
                return [120, corners[0][1]]
            elif corners[2][1] > centre[1]:
                return [120, 120]
            else:
                return [120, corners[2][1]]
        elif corners[0][1] < centre[1] and corners[3][1] > centre[1]:
            #print('On the left/right.')
            if corners[0][0] > centre[0]:
                return [corners[0][0], 120]
            else:
                return [corners[1][0], 120]
        else:
            #print('On the diagonals.')
            closestdist = 1000
            closestcorner = -1
            for i in corners:
                if int(math.sqrt(((i[0]-120)**2 + (i[1]-120)**2))) < closestdist:
                    closestdist = int(math.sqrt(((i[0]-120)**2 + (i[1]-120)**2)))
                    closestcorner = corners.index(i)
            return list(corners[closestcorner])
    return [250, 0]

while(True):
    #while(1):
    #    print(sensor.get_rgb_gain_db())
    clock.tick()  # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.
    goalBlobs = img.find_blobs(thresholds, x_stride=5, y_stride=5, area_threshold=50, pixel_threshold=50, merge=False, margin=23)

    data = [254,250,0,250,0,250,0,250,0,250,0]

    yellowBlobs =[]
    blueBlobs = []
    orangeBlobs = []
    orangeBlobs = img.find_blobs(orangethreshold, x_stride=1, y_stride=1, area_threshold=0, pixel_threshold=0, merge=False, margin=23)

    for blob in goalBlobs:
        if blob.code() == 1:
            yellowBlobs.append(blob)
        elif blob.code() == 2:
            blueBlobs.append(blob)
    #print(orangeBlobs)
    if attackisyellow:
        data[1], data[2] = sortBlob(yellowBlobs)
        data[3], data[4] = sortBlob(blueBlobs)
        data[5], data[6] = sortBlob(orangeBlobs)
        acorner = goalcorners(yellowBlobs, (int(img.width()/2), int(img.height()/2)))
        dcorner = goalcorners(blueBlobs, (int(img.width()/2), int(img.height()/2)))
        data[7], data[8] = int(acorner[0]), int(acorner[1])
        data[9], data[10] = int(dcorner[0]), int(dcorner[1])
        #print(data[1], data[2])
        #print(math.sqrt((data[5]-120)**2+(data[6]-120)**2))#, math.sqrt((data[1]-60)**2+(data[2]-60)**2))
        #data[7] = 0
        #for i in blueBlobs:
            #if i[4] > data[7]: data[7] = i[4]
    else:
        data[1], data[2] = sortBlob(blueBlobs)
        data[3], data[4] = sortBlob(yellowBlobs)
        data[5], data[6] = sortBlob(orangeBlobs)
        acorner = goalcorners(blueBlobs, (int(img.width()/2), int(img.height()/2)))
        dcorner = goalcorners(yellowBlobs, (int(img.width()/2), int(img.height()/2)))
        data[7], data[8] = acorner[0], acorner[1]
        data[9], data[10] = dcorner[0], dcorner[1]
        #data[7] = 0
        #for i in yellowBlobs:
            #if i[4] > data[7]: data[7] = i[4]
            #elif i[4] > 150: data[7] += i[4]s

    #data[7] = int(data[7]/10)
    #print(data[7])

    for i in data:
        uart.writechar(i)


    if isdebug:
        #print(data)
        img.draw_cross(int(img.width()/2), int(img.height()/2))
