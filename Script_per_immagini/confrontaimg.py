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

x = np.asarray(img_original)
y = np.asarray(img_conv)
x.astype(np.uint8)
y.astype(np.uint8)

out = np.full((480,640), 0)
out.astype(np.uint8)

for i in range(0,480):
    for j in range(0,640):
        if(int(x[i][j]) != int(y[i][j])):
            out[i][j] = 255
        
#data = np.array(out).reshape((480,640))

print(out)

# Convert to image and save
img = Image.fromarray(out)

img.show()
        