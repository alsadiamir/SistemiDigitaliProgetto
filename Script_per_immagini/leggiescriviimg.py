#!/usr/local/bin/python3
from PIL import Image
import numpy as np
import re

y = cv2.imread('D:/Immagine2.png');
np.savetxt('D:/img2.txt',y.flatten());
x = np.loadtxt('D:/myfile.txt',fmt='%d');
cv2.imwrite('D:/ImmOut.png',x);

#per mostrare immagine
img = Image.fromarray(img, 'L')
img.show()