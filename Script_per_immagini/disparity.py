#!/usr/local/bin/python3
from PIL import Image
import numpy as np
import re
import cv2

left = cv2.cvtColor(cv2.imread('D:/blurred.png'), cv2.COLOR_BGR2GRAY)
right = cv2.cvtColor(cv2.imread('D:/blurredres.png'), cv2.COLOR_BGR2GRAY)

stereo_matcher = cv2.StereoSGBM_create(numDisparities = 16, blockSize = 5)
disparity = stereo_matcher.compute(left,right)
disparity = (disparity).astype(np.uint8)
disparity = cv2.applyColorMap(disparity, cv2.COLORMAP_JET)

cv2.imwrite('D:/disparity.png',disparity)