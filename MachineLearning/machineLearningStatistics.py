import csv
import cv2
import numpy as np
import os
import math
import random
import itertools
import matplotlib.pyplot as plt

from sklearn.model_selection import train_test_split
from sklearn.model_selection import KFold
from sklearn.metrics import confusion_matrix
from sklearn.metrics import classification_report
from sklearn.metrics import accuracy_score

path_to_data = "HAPT-data-set-DU" 

classes = {} # define mapping of classes
with open(os.path.join(path_to_data, "activity_labels.txt")) as f:
    for line in f:
       (key, val) = line.split()
       classes[int(key)] = val

inv_classes = {v: k for k, v in classes.items()}

activityLabels = (list(classes.values()))

########### Load Data Set

# Training data - as currenrtly split
attribute_list = []
label_list = []

reader=csv.reader(open(os.path.join(path_to_data, "x.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in columns 0-561
        attribute_list.append(list(row[i] for i in (range(0,561))))

reader=csv.reader(open(os.path.join(path_to_data, "y.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in column 1
        label_list.append(row[0])

temp = list(zip(attribute_list, label_list))
random.shuffle(temp)
attribute_list, label_list = zip(*temp)

attributes=np.array(attribute_list).astype(np.float32)
labels=np.array(label_list).astype(np.float32)

print('Files read')

def KNNStatistics(K,attributes,labels):
	success = 0
	failure = 0
	counter = 0
	allSolutions = [] 
	allLabels = [] 
	print('KNN with K as '+str(K))
	kf = KFold(n_splits=10)
	for train,test in kf.split(attributes):
	    train_x, test_x, train_y, test_y = np.array(attributes[train]), np.array(attributes[test]), np.array(labels[train]).astype(np.uint8), np.array(labels[test]).astype(np.uint8)
	    solutions,right,wrong = KNN(K,train_x,train_y,test_x,test_y)
	    allSolutions = np.concatenate((allSolutions,solutions))
	    allLabels = np.concatenate((allLabels,test_y))
	    success += right
	    failure += wrong
	    counter += 1
	    print('Round '+str(counter)+' complete')

	print(classification_report(allLabels, allSolutions))
	print('Accuracy: '+str(accuracy_score(allLabels, allSolutions)))

	confusionMatrix = confusion_matrix(allLabels, allSolutions)

	plt.figure()
	plot_confusion_matrix(confusionMatrix, classes=classes.values(),title='Normalized confusion matrix')
	plt.show()

def SVMStatistics(v,attributes,labels):
	success = 0
	failure = 0
	counter = 0
	allSolutions = [] 
	allLabels = [] 
	print('SVM with C as '+str(v))
	kf = KFold(n_splits=10)
	for train,test in kf.split(attributes):
	    train_x, test_x, train_y, test_y = np.array(attributes[train]), np.array(attributes[test]), np.array(labels[train]).astype(np.uint8), np.array(labels[test]).astype(np.uint8)
	    solutions,right,wrong = SVM(v,train_x,train_y,test_x,test_y)
	    allSolutions = np.concatenate((allSolutions,solutions))
	    allLabels = np.concatenate((allLabels,test_y))
	    success += right
	    failure += wrong
	    counter += 1
	    print('Round '+str(counter)+' complete')

	print(classification_report(allLabels, allSolutions))
	print('Accuracy: '+str(accuracy_score(allLabels, allSolutions)))

	# confusionMatrix = confusion_matrix(allLabels, allSolutions)

	# plt.figure()
	# plot_confusion_matrix(confusionMatrix, classes=classes.values(),title='Normalized confusion matrix')
	# plt.show()

def plot_confusion_matrix(cm,classes,title='Confusion matrix',cmap=plt.cm.Blues):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    credit - http://scikit-learn.org/stable/auto_examples/model_selection/plot_confusion_matrix.html
    """
    cm = np.round((cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]),2)

    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=90)
    plt.yticks(tick_marks, classes)

    
    #print("Normalized confusion matrix")

    #print(cm)

    plt.colorbar()
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="black") #if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')

############ Perform Training -- k-NN
def KNN(K,train_x, train_y, test_x, test_y):
	'''
	Experiments: Kfold, value of K, confusion matrix, weighted knn
	'''
	# define kNN object
	knn = cv2.ml.KNearest_create()

	# set to use BRUTE_FORCE neighbour search as KNEAREST_KDTREE seems to  break
	# on this data set (may not for others - http://code.opencv.org/issues/2661)
	knn.setAlgorithmType(cv2.ml.KNEAREST_BRUTE_FORCE)

	# set default 3, can be changed at query time in predict() call
	#knn.setDefaultK(3) 


	# set up classification, turning off regression
	knn.setIsClassifier(True)

	# perform training of k-NN
	knn.train(train_x, cv2.ml.ROW_SAMPLE, train_y.astype(int))

	############ Perform Testing -- k-NN

	correct = 0
	wrong = 0
	solutions = []

	# for each testing example
	for i in range(0, len(test_x[:,0])) :

	    # perform k-NN prediction (i.e. classification)

	    # (to get around some kind of OpenCV python interface bug, vertically stack the
	    #  example with a second row of zeros of the same size and type which is ignored).
	    sample = np.vstack((test_x[i,:], np.zeros(len(test_x[i,:])).astype(np.float32)))

	    # now do the prediction returning the result, results (ignored) and also the responses
	    # + distances of each of the k nearest neighbours
	    # N.B. k at classification time must be < maxK from earlier training
	    ret, results, neigh_respones, distances = knn.findNearest(sample, k = K) #EXPERIMENT WITH THIS

	    if (results[0] == test_y[i]) : correct+=1
	    elif (results[0] != test_y[i]) : wrong+=1

	    solutions.append(ret)

	# output summmary statistics
	total = correct + wrong
	# print("KNN")
	# print("Performance Summary")
	# print("Total Correct : {}%".format(round((correct / float(total)) * 100, 2)))
	# print("Total Wrong : {}%".format(round((wrong / float(total)) * 100, 2)))

	#print(confusion_matrix(test_y, solutions))

	solutions = np.array(solutions).astype(np.float32)

	return solutions, round((correct / float(total)) * 100,2),round((wrong / float(total)) * 100,2)

############ Perform Training -- SVM
def SVM(v,train_x, train_y, test_x, test_y):

	# define SVM object
	svm = cv2.ml.SVM_create()

	# set kernel
	# choices : # SVM_LINEAR / SVM_RBF / SVM_POLY / SVM_SIGMOID / SVM_CHI2 / SVM_INTER
	svm.setKernel(cv2.ml.SVM_POLY)

	# set parameters (some specific to certain kernels)
	svm.setC(v) # penalty constant on margin optimization - doesn't seem to do much - only Linear kernel - EXPONENTIAL (default is 10)
	
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
	solutions = []

	# for each testing example
	for i in range(0, len(test_x[:,0])) :

	    # (to get around some kind of OpenCV python interface bug, vertically stack the
	    #  example with a second row of zeros of the same size and type which is ignored).
	    sample = np.vstack((test_x[i,:],np.zeros(len(test_x[i,:])).astype(np.float32)))

	    # perform SVM prediction (i.e. classification)
	    _, result = svm.predict(sample, cv2.ml.ROW_SAMPLE)

	    # and for undocumented reasons take the first element of the resulting array
	    # as the result

	    # record results as either being correct or wrong
	    if (result[0] == test_y[i]) : correct+=1
	    elif (result[0] != test_y[i]) : wrong+=1

	    solutions.append(float(result[0]))

	# output summmary statistics
	total = wrong + correct

	# print()
	# print("SVM")
	# print("Performance Summary")
	# print("Total Correct : {}%".format(round((correct / float(total)) * 100, 2)))
	# print("Total Wrong : {}%".format(round((wrong / float(total)) * 100, 2)))

	solutions = np.array(solutions).astype(np.float32)

	return solutions,round((correct / float(total)) * 100,2),round((wrong / float(total)) * 100,2)

# for k in range(1,31):
# 	KNNStatistics(k,attributes,labels)

#KNNStatistics(3,attributes,labels)
SVMStatistics(10,attributes,labels)