#!/usr/local/bin/python3
from PIL import Image
import numpy as np
import re
import cv2

# Read in entire file
with open('D:/myfile.txt') as f:
   s = f.read()

# Find anything that looks like numbers
l=re.findall(r'\d+',s)

# Convert to numpy array and reshape
data = np.array(l,dtype=np.uint8).reshape((480,640))


print(data)



# Convert to image and save
img = Image.fromarray(data, 'L')

img.save('D:/blurredres.png')