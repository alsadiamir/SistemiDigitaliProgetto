#!/usr/local/bin/python3
from PIL import Image
import numpy as np
import re
import cv2

img=cv2.imread('D:/gray.png')
kernel = np.array([[0.0625,0.125,0.0625],[0.125,0.250,0.125],[0.0625,0.125,0.0625]], dtype=np.float32)
dst=cv2.filter2D(img,-1,kernel)

cv2.imwrite('D:/blurredconv.png',dst)