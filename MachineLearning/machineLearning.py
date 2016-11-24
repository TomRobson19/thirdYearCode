import csv
import cv2
import numpy as np
import os
import math
import random

classes = {} # define mapping of classes
inv_classes = {v: k for k, v in classes.items()}

########### Load Data Set

path_to_data = "HAPT-data-set-DU" 

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

print(attributes)
print(len(attributes))
print(labels)
print(len(labels))

random.seed(1)
random.shuffle(attributes)
random.seed(1)
random.shuffle(labels)


# write first N% of the entries to first file

# N = 30.0

# writerA = csv.writer(open("outputA.data", "wt", encoding='ascii'), delimiter=',')
# writerA.writerows(entry_list[0:int(math.floor(len(entry_list)* (N/100.0)))])

# write the remaining (100-N)% of the entries of the second file
# writerB = csv.writer(open("outputB.data", "wt", encoding='ascii'), delimiter=',')
# writerB.writerows(entry_list[int(math.floor(len(entry_list)* (N/100.0))):len(entry_list)])
