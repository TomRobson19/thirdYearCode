'''
Code to perform grid search for SVM, based on code from http://scikit-learn.org/0.15/auto_examples/grid_search_digits.html
'''
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
from sklearn import datasets
from sklearn.grid_search import GridSearchCV
from sklearn.svm import SVC

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

X_train, X_test, y_train, y_test = train_test_split(attributes, labels, test_size=0.1, random_state=0)

# Set the parameters by cross-validation
# tuned_parameters = [{'kernel': ['rbf'], 'gamma': [1e-1,1e-2,1e-3,1e-4,1e-5],'C': [1,10,100,1000,10000]},
#                     {'kernel': ['linear'], 'C': [1,10,100,1000,10000]},
#                     {'kernel': ['poly'], 'C': [1,10,100,1000,10000], 'gamma': [1e-1,1e-2,1e-3,1e-4,1e-5], 'degree': [1,2,3,4,5,6,7,8,9,10]}]

# tuned_parameters = [{'kernel': ['sigmoid'], 'gamma': [1e-1,1e-2,1e-3,1e-4,1e-5],'C': [1,10,100,1000,10000]},
#                     {'kernel': ['inter'], 'C': [1,10,100,1000,10000]}, #####NOT IN SKLEARN
#                     {'kernel': ['chi2'], 'C': [1,10,100,1000,10000], 'gamma': [1e-1,1e-2,1e-3,1e-4,1e-5]}] #####NOT IN SKLEARN

tuned_parameters = [{'kernel': ['linear'], 'C': [1,10,100,1000,10000]}]

scores = ['accuracy']

for score in scores:
    print("# Tuning hyper-parameters for %s" % score)
    print()

    clf = GridSearchCV(SVC(C=1), tuned_parameters, cv=5, scoring=score)
    clf.fit(X_train, y_train)

    print("Best parameters set found on development set:")
    print()
    print(clf.best_estimator_)
    print()
    print("Grid scores on development set:")
    print()
    for params, mean_score, scores in clf.grid_scores_:
        print("%0.3f (+/-%0.03f) for %r"
              % (mean_score, scores.std() / 2, params))
    print()

    print("Detailed classification report:")
    print()
    print("The model is trained on the full development set.")
    print("The scores are computed on the full evaluation set.")
    print()
    y_true, y_pred = y_test, clf.predict(X_test)
    print(classification_report(y_true, y_pred))
    print()