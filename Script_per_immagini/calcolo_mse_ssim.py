from skimage.measure import compare_ssim as ssim
import matplotlib.pyplot as plt
import numpy as np 
import cv2

img_original=cv2.imread("D:/blurred.png")
img_conv=cv2.imread("D:/blurredres.png")

# convert the images to grayscale
img_original= cv2.cvtColor(img_original, cv2.COLOR_BGR2GRAY)
img_conv=cv2.cvtColor(img_conv, cv2.COLOR_BGR2GRAY)

#funzione che calcola la differenza di qualita tra due immagini
#tramite l'algoritmo Mean Sqaured Error

def M_s_e (imageA,imageB): 
  	# the 'Mean Squared Error' between the two images is the
	# sum of the squared difference between the two images;
	# NOTE: the two images must have the same dimension
	err = np.sum((imageA.astype("float") - imageB.astype("float")) ** 2)
	err /= float(imageA.shape[0] * imageA.shape[1])
	
	# return the MSE, the lower the error, the more "similar"
	# the two images are
	return err
 
def compare_images(imageA, imageB, title):
	# compute the mean squared error and structural similarity
	# index for the images
	m = M_s_e(imageA, imageB)
	s = ssim(imageA, imageB)
 
	# setup the figure
	fig = plt.figure(title)
	plt.suptitle("MSE: %.2f, SSIM: %.2f," % (m, s))
 
	# show first image
	ax = fig.add_subplot(1, 2, 1)
	plt.imshow(imageA, cmap = plt.cm.gray)
	plt.axis("off")
 
	# show the second image
	ax = fig.add_subplot(1, 2, 2)
	plt.imshow(imageB, cmap = plt.cm.gray)
	plt.axis("off")
 
	# show the images
	plt.show() 
	
	
	
# initialize the figure
fig = plt.figure("Images")
images = ("Original", img_original), ("Convoluted", img_conv)
 
# loop over the images
for (i, (name, image)) in enumerate(images):
	# show the image
	ax = fig.add_subplot(1, 3, i + 1)
	ax.set_title(name)
	plt.imshow(image, cmap = plt.cm.gray)
	plt.axis("off")
 
 
# compare the images
compare_images(img_original,img_original,"Original vs Original")
compare_images(img_original, img_conv, "Original vs. Convoluted")	
