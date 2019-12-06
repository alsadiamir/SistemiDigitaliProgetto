#!/usr/local/bin/python3
from PIL import Image
from skimage.measure import compare_ssim as ssim
import matplotlib.pyplot as plt
import numpy as np 
import cv2

img_original=cv2.imread("D:/blurred.png")
img_conv=cv2.imread("D:/blurredres.png")

# convert the images to grayscale
img_original= cv2.cvtColor(img_original, cv2.COLOR_BGR2GRAY)
img_conv=cv2.cvtColor(img_conv, cv2.COLOR_BGR2GRAY)

x = np.asarray(img_original).flatten()
y = np.asarray(img_conv).flatten()

out = np.full((307200), 0)
out.astype(np.uint8)

for i in range(0,out.size):
    #print(x[i])
    #print(y[i])
    
    if(x[i] == y[i]):
        out[i] = 0
    else: 
        out[i] = 255
        
data = np.array(out).reshape((480,640))

print(data)



# Convert to image and save
img = Image.fromarray(data, 'L')

img.show()
        