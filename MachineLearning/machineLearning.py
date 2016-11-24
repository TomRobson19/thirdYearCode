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

reader=csv.reader(open(os.path.join(path_to_data, "x.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in columns 0-561
        attribute_list.append(list(row[i] for i in (range(0,561))))

reader=csv.reader(open(os.path.join(path_to_data, "y.txt"),"rt", encoding='ascii'),delimiter=' ')
for row in reader:
        # attributes in column 1
        label_list.append(row[0])

attributes=np.array(attribute_list).astype(np.float32)
labels=np.array(label_list).astype(np.float32)

random.seed(1)
random.shuffle(attributes)
random.seed(1)
random.shuffle(labels)

# select first N% of the entries
N = 30.0

train_x = attributes[0:int(math.floor(len(attributes)* (N/100.0)))]
train_y = labels[0:int(math.floor(len(labels)* (N/100.0)))]

test_x = attributes[int(math.floor(len(attributes)* (N/100.0))):len(attributes)]
test_y = labels[int(math.floor(len(labels)* (N/100.0))):len(labels)]