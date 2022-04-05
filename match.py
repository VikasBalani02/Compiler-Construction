import sys
import os

f = open('randomer.txt', 'r')
for line in f:
    print(line[:-2] + '_'+',')