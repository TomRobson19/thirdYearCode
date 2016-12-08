import csv
import cv2
import numpy as np
import os
import math
import random

path_to_data = "HAPT-data-set-DU" 

classes = {} # define mapping of classes
with open(os.path.join(path_to_data, "activity_labels.txt")) as f:
    for line in f:
       (key, val) = line.split()
       classes[int(key)] = val

inv_classes = {v: k for k, v in classes.items()}

########### Load Data Set

# Training data - as currenrtly split
attribute_list = []
label_list = []

reader=csv.reader(open(os.path.join(path_to_data, "Train/x_train.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in columns 0-561
        attribute_list.append(list(row[i] for i in (range(0,561))))

reader=csv.reader(open(os.path.join(path_to_data, "Train/y_train.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in column 1
        label_list.append(row[0])

training_attributes=np.array(attribute_list).astype(np.float32)
training_labels=np.array(label_list).astype(np.float32)

# Testing data - as currently split

attribute_list = []
label_list = []

reader=csv.reader(open(os.path.join(path_to_data, "Test/x_test.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in columns 0-561
        attribute_list.append(list(row[i] for i in (range(0,561))))

reader=csv.reader(open(os.path.join(path_to_data, "Test/y_test.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in column 1
        label_list.append(row[0])

testing_attributes=np.array(attribute_list).astype(np.float32)
testing_labels=np.array(label_list).astype(np.float32)

print ('Files read')

############ Perform Training -- k-NN
def KNN(train_x, train_y, test_x, test_y):

	'''
	Experiments: Kfold, value of K, confusion matrix, weighted knn

	'''
	# define kNN object
	knn = cv2.ml.KNearest_create()

	# set to use BRUTE_FORCE neighbour search as KNEAREST_KDTREE seems to  break
	# on this data set (may not for others - http://code.opencv.org/issues/2661)
	knn.setAlgorithmType(cv2.ml.KNEAREST_BRUTE_FORCE)

	# set default 3, can be changed at query time in predict() call
	knn.setDefaultK(3) #EXPERIMENT WITH THIS


	# set up classification, turning off regression
	knn.setIsClassifier(True)

	# perform training of k-NN
	knn.train(train_x, cv2.ml.ROW_SAMPLE, train_y.astype(int))

	############ Perform Testing -- k-NN

	correct = 0
	wrong = 0

	# for each testing example
	for i in range(0, len(test_x[:,0])) :

	    # perform k-NN prediction (i.e. classification)

	    # (to get around some kind of OpenCV python interface bug, vertically stack the
	    #  example with a second row of zeros of the same size and type which is ignored).
	    sample = np.vstack((test_x[i,:], np.zeros(len(test_x[i,:])).astype(np.float32)))

	    # now do the prediction returning the result, results (ignored) and also the responses
	    # + distances of each of the k nearest neighbours
	    # N.B. k at classification time must be < maxK from earlier training
	    _, results, neigh_respones, distances = knn.findNearest(sample, k = 3)

	    if (results[0] == test_y[i]) : correct+=1
	    elif (results[0] != test_y[i]) : wrong+=1

	# output summmary statistics
	total = correct + wrong
	print("KNN")
	print("Performance Summary")
	print("Total Correct : {}%".format(round((correct / float(total)) * 100, 2)))
	print("Total Wrong : {}%".format(round((wrong / float(total)) * 100, 2)))

############ Perform Training -- SVM
def SVM(train_x, train_y, test_x, test_y):

	#implement own grid search 

	# define SVM object
	svm = cv2.ml.SVM_create()

	# set kernel
	# choices : # SVM_LINEAR / SVM_RBF / SVM_POLY / SVM_SIGMOID / SVM_CHI2 / SVM_INTER
	svm.setKernel(cv2.ml.SVM_LINEAR)

	# set parameters (some specific to certain kernels)
	svm.setC(10) # penalty constant on margin optimization - doesn't seem to do much
	svm.setType(cv2.ml.SVM_C_SVC) # multiple class (2 or more) classification
	svm.setGamma(0.5) # used for SVM_RBF kernel only, otherwise has no effect
	svm.setDegree(3)  # used for SVM_POLY kernel only, otherwise has no effect

	# set the relative weights importance of each class for use with penalty term
	svm.setClassWeights(np.ones(12))

	# # define and train svm object
	# use kernel specified above with kernel parameters set previously
	svm.train(train_x, cv2.ml.ROW_SAMPLE, train_y.astype(int))

	############ Perform Testing -- SVM

	correct = 0 # handwritten digit correctly identified
	wrong = 0   # handwritten digit wrongly identified

	# for each testing example
	for i in range(0, len(test_x[:,0])) :

	    # (to get around some kind of OpenCV python interface bug, vertically stack the
	    #  example with a second row of zeros of the same size and type which is ignored).
	    sample = np.vstack((test_x[i,:],
	                        np.zeros(len(test_x[i,:])).astype(np.float32)))

	    # perform SVM prediction (i.e. classification)
	    _, result = svm.predict(sample, cv2.ml.ROW_SAMPLE)

	    # and for undocumented reasons take the first element of the resulting array
	    # as the result

	    # record results as either being correct or wrong
	    if (result[0] == test_y[i]) : correct+=1
	    elif (result[0] != test_y[i]) : wrong+=1

	# output summmary statistics
	total = wrong + correct

	print()
	print("SVM")
	print("Performance Summary")
	print("Total Correct : {}%".format(round((correct / float(total)) * 100, 2)))
	print("Total Wrong : {}%".format(round((wrong / float(total)) * 100, 2)))

KNN(training_attributes, training_labels, testing_attributes, testing_labels)
SVM(training_attributes, training_labels, testing_attributes, testing_labels)