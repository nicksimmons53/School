# -*- coding: utf-8 -*-
"""
Created on Thu Sep 13 21:40:53 2018

@author: JRuan
"""

import pandas as pd
import matplotlib.pyplot as plt
import scipy.stats as stats
import statistics as pystat
import numpy as np


data = pd.read_csv('brfss.csv', index_col=0)

# data is a numpy array and the columns are age, current weight (kg), 
# last year's weight (kg), height (cm), and gender (1: male; 2: female).
data = data.drop('wtkg2',axis=1).dropna(axis=0, how='any').values


# 4a answer
plt.figure(1)
curr_weight = data[:, 1]
year_weight = data[:, 2]
height = data[:, 3]
age = data[:, 0]
plt.xticks([0.5, 1.5, 2.5, 4], ['curr_weight', 'weight_a_year_ago', 'height'])

plt.plot(0.5, max(curr_weight), 'kv', 
         0.5, min(curr_weight), 'k^', 
         0.5, pystat.mean(curr_weight), 'b+',
         0.5, pystat.median(curr_weight), 'bx', 
         0.5, np.percentile(curr_weight, 75), 'gv', 
         0.5, np.percentile(curr_weight, 25), 'g^',
         0.5, pystat.mean(curr_weight) + np.std(curr_weight), 'r<',
         0.5, pystat.mean(curr_weight) - np.std(curr_weight), 'r>')

plt.plot(1.5, max(year_weight), 'kv', 
         1.5, min(year_weight), 'k^', 
         1.5, pystat.mean(year_weight), 'b+',
         1.5, pystat.median(year_weight), 'bx', 
         1.5, np.percentile(year_weight, 75), 'gv', 
         1.5, np.percentile(year_weight, 25), 'g^',
         1.5, pystat.mean(year_weight) + np.std(year_weight), 'r<',
         1.5, pystat.mean(year_weight) - np.std(year_weight), 'r>')

plt.plot(2.5, max(height), 'kv', 
         2.5, min(height), 'k^', 
         2.5, pystat.mean(height), 'b+',
         2.5, pystat.median(height), 'bx', 
         2.5, np.percentile(height, 75), 'gv', 
         2.5, np.percentile(height, 25), 'g^',
         2.5, pystat.mean(height) + np.std(height), 'r<',
         2.5, pystat.mean(height) - np.std(height), 'r>')

plt.xlim([0, 4])
plt.legend(["max", "min", "mean", "median", "per75", "perc25", "mean+std", "mean-std"], loc=1)

# 4b answer
plt.figure(2)
weight_change = np.subtract(curr_weight, year_weight)
plt.scatter(weight_change, curr_weight)
plt.xlabel('Weight Differece')
plt.ylabel('Current Weight (kg)')
plt.title('Weight Change Correlation')

plt.figure(3)
plt.scatter(weight_change, year_weight)
plt.xlabel('Weight Differece')
plt.ylabel('Weight from a Year Ago (kg)')
plt.title('Weight Change Correlation')

plt.figure(4)
plt.scatter(weight_change, age)
plt.xlabel('Weight Differece')
plt.ylabel('Age')
plt.title('Weight Change Correlation')

# 4c answer

plt.show( )
