import numpy as np
import cv2

video_capture = cv2.VideoCapture(0)
video_capture.set(3, 160)
video_capture.set(4, 120)

 

# Setup Output Pins
while(True):
    ret, frame = video_capture.read()
    crop_img = frame[60:120, 0:160]
    # Convert to grayscale
    gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
    # Gaussian blur
    blur = cv2.GaussianBlur(gray,(5,5),0)
    # Color thresholding
    ret,thresh1 = cv2.threshold(blur,10,255,cv2.THRESH_BINARY_INV)
    # Erode and dilate to remove accidental line detections
    mask = cv2.erode(thresh1, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)
    # Find the contours of the frame
    contours,hierarchy = cv2.findContours(mask.copy(), 1, cv2.CHAIN_APPROX_NONE)
    # Find the biggest contour (if detected)
    
    if len(contours) > 0:

        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)

 

        cx = int(M['m10']/M['m00'])

        cy = int(M['m01']/M['m00'])

 

        cv2.line(crop_img,(cx,0),(cx,720),(255,0,0),1)

        cv2.line(crop_img,(0,cy),(1280,cy),(255,0,0),1)

 

        cv2.drawContours(crop_img, contours, -1, (0,255,0), 1)

 

        print(cx)

        print(cy)

 

        if cx >= 120:

            print("Turn Left!")

 

        if cx < 120 and cx > 50:

            print("On Track!")

 

        if cx <= 50:

            print("Turn Right")

 

    else:

        print("I don't see the line")

 



    #Display the resulting frame

    cv2.imshow('frame',crop_img)

    if cv2.waitKey(1) & 0xFF == ord('q'):

        break
