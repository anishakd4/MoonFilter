import cv2
import sys
import numpy as np

#Read input image
image = cv2.imread("../assets/anish.jpg")

#create a clone of input image to work on
output = image.copy()

#convert to LAB color space
output = cv2.cvtColor(output, cv2.COLOR_BGR2LAB)

#split into channels
L, A, B = cv2.split(output)

#Interpolation values
originalValues = np.array([0, 15, 30, 50, 70, 90, 120, 160, 180, 210, 255 ])
values =         np.array([0, 0, 5, 15, 60, 110, 150, 190, 210, 230, 255  ])

#create lookup table
allValues = np.arange(0, 256)

#Creating the lookuptable
lookuptable = np.interp(allValues, originalValues, values)

#apply mapping for L channels
L = cv2.LUT(L, lookuptable)

#convert to uint8
L = np.uint8(L)

#merge back the channels
output = cv2.merge([L, A, B])

#convert back to BGR color space
output = cv2.cvtColor(output, cv2.COLOR_LAB2BGR)

#desaturate the image
#convert to HSV color space
output = cv2.cvtColor(output, cv2.COLOR_BGR2HSV)

#split into channels
H, S, V = cv2.split(output)

#Multiply S channel by saturation scale value
S = S * 0.01

#convert to uint8
S = np.uint8(S)

#limit the values between 0 and 256
S = np.clip(S, 0, 255)

#merge back the channels
output = cv2.merge([H, S, V])

#convert back to BGR color space
output = cv2.cvtColor(output, cv2.COLOR_HSV2BGR)

#create windows to display images
cv2.namedWindow("image", cv2.WINDOW_AUTOSIZE)
cv2.namedWindow("moon", cv2.WINDOW_AUTOSIZE)

#display images
cv2.imshow("image", image)
cv2.imshow("moon", output)

#press esc to exit the program
cv2.waitKey(0)

#destroy all the opened windows
cv2.destroyAllWindows()