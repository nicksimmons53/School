# -*- coding: utf-8 -*-
"""
Created on Tue Aug 29 08:01:48 2017
HW1 Code Skeleton
@author: Jianhua Ruan
"""
import math
import matplotlib.pyplot as plt
from functools import reduce

### 2a Complete the following two functions to implement merge sort

def merge(a, b):
    """Given two non-decreasingly sorted list of numbers, 
       return a single merged array in non-decreasing order
    """
    
    # Concatenate the two lists
    mergedList = a + b
    
    mergedList = mergeSort(mergedList)
    
    return mergedList

def mergeSort(inputArray):
    """
    Given a list of numbers in order order, 
    return a new list sorted in non-decreasing order, 
    and leave the original list unchanged.
    """
    
    sortedList = sorted(inputArray)

    return sortedList

def maximum(listOfNums):
    i = 0
    for num in listOfNums:
        if num > i:
            i = num
    return i

def minimum(listOfNums):
    i = None
    for num in listOfNums:
        if not i:
            i = num
        elif num < i:
           i = num
    return i

def mean(listOfNums):
    summ = 0
    count = 0
    for num in listOfNums:
        summ += num
        count += 1
    mean = summ / count
    return mean

def stddev(listOfNums):
    stdevList = []
    summ = 0
    count = 0
    for num in listOfNums:
        result = (num - mean(listOfNums))
        result = result ** 2
        stdevList.append(result)
        
    for num2 in stdevList:
        summ += num2
        count += 1
    
    stdevMean = summ / count
    stdev = math.sqrt(stdevMean)
    return stdev

def median(listOfNums):
    count = 0
    for num in listOfNums:
        count += 1
        
    med = math.floor(count / 2)  
    return listOfNums[med]

def percentile75(listOfNums):
    count = 0
    for num in listOfNums:
        count += 1
    
    return listOfNums[math.ceil(0.75 * count)]

def percentile25(listOfNums):
    count = 0
    for num in listOfNums:
        count += 1
    
    return listOfNums[math.ceil(0.25 * count)]

def summaryStatistics(listOfNums):
    """Givne a list of numbers in random order, return the summary statistics 
    that includes the max, min, mean, population standard deviation, median,
    75 percentile, and 25 percentile.
    """    

### 2b Complete this part. 
# You can decide to return the following statistics either in a sequence 
# type (i.e., list, tuple), or a key-value pair (i.e., dictionary, preferred.)
    
    maxVal = maximum(listOfNums)
    minVal = minimum(listOfNums)
    meanVal = mean(listOfNums)
    stdev = stddev(listOfNums)
    medianVal = median(listOfNums)
    perc75 = percentile75(listOfNums)
    perc25 = percentile25(listOfNums)

    return {'max': maxVal, 
            'min': minVal, 
            'mean': meanVal, 
            'std': stdev,
            'median': medianVal,
            'perc75': perc75,
            'perc25': perc25}
    

def scaleToInt(listOfNums):     
    """Given a list of real numbers in any range, scale them to be integers
    between 0 and 15 (inclusive). For each number x in the list, the new number
    is computed with the formula round(15 * (x-min) / (max-min))
    """    
    
    newList = []
    maxVal = max(listOfNums)
    minVal = min(listOfNums)
    for num in listOfNums:
        scaledNum = round(15 * (num - minVal) / (maxVal - minVal))
        newList.append(scaledNum)

### complete this part for 2c   
    return newList
        
def myHistWithRescale(listOfNums):
    """Givne a list of real numbers in any range, first scale the numbers to
    inters between 0 and 15 (inclusive), then return the number of occurrences
    of each integer in a list
    """
    scaledData = scaleToInt(listOfNums)
    
### 2c complete the following part to count the number of occurrences of 
# each digit in scaledData

    counts = [0]*16 # all counts initialize to 0
    flagged = [ ]    
    for num in scaledData:       
        if num in flagged:
            continue
        
        counts[num] = scaledData.count(scaledData[num])
        flagged.append(num)
        
    return counts


## This if statement makes sure that the following code will be executed only 
## if you are running it as a script (rather than loading it as a module).
if __name__ == '__main__':
   
    import random
    
    # Testing merge sort
    
    a = [random.randint(0, 20) for _ in range(10)]
    
    b = mergeSort(a)
    
    print('random array is: ', a)
    print('sorted array is: ', b)
    print('mergeSort is', 'correct' if b == sorted(a) else 'incorrect')
    
    # Generate three sets of random data
    
    listA = [random.gauss(5, 3) for _ in range(1000)]
    listB = [10*random.random() for _ in range(1000)]
    listC = [math.exp(random.gauss(1, 0.5)) for _ in range(1000)]
    
    
    # testing summaryStatistics
    ssA = summaryStatistics(listA)    
    ssB = summaryStatistics(listB)    
    ssC = summaryStatistics(listC)    
        
    
    print("Summary statistics for data set A: \n", ssA)
    print("Summary statistics for data set B: \n", ssB)
    print("Summary statistics for data set C: \n", ssC)
            


    # 3a complete the following code to plot the summary statistics of 
    # the above data using Fig 1 in HW1 as a template

	# close all figures to make new figures
    plt.close('all')
	
    plt.figure(1)
    
    # insert details of plotting'
    data = ["DataA", "DataB", "DataC"]
    value_range = ['-2.5', '0', '2.5', '5', '7.5', '10', '12.5', '15', '17.5']
    
    plt.ylabel('Value')
    plt.xticks([0.5,1.5, 2.5, 4], data)
    
    plt.plot(0.5, ssA["max"], 'kv', 
             0.5, ssA["min"], 'k^', 
             0.5, ssA["mean"], 'b+',
             0.5, ssA["median"], 'bx', 
             0.5, ssA["perc75"], 'gv', 
             0.5, ssA["perc25"], 'g^',
             0.5, ssA["mean"] + ssA["std"], 'r<',
             0.5, ssA["mean"] - ssA["std"], 'r>')
    
    plt.plot(1.5, ssB["max"], 'kv', 
             1.5, ssB["min"], 'k^', 
             1.5, ssB["mean"], 'b+',
             1.5, ssB["median"], 'bx', 
             1.5, ssB["perc75"], 'gv', 
             1.5, ssB["perc25"], 'g^',
             1.5, ssB["mean"] + ssB["std"], 'r<',
             1.5, ssB["mean"] - ssB["std"], 'r>')
    
    plt.plot(2.5, ssC["max"], 'kv', 
             2.5, ssC["min"], 'k^', 
             2.5, ssC["mean"], 'b+',
             2.5, ssC["median"], 'bx', 
             2.5, ssC["perc75"], 'gv', 
             2.5, ssC["perc25"], 'g^',
             2.5, ssC["mean"] + ssC["std"], 'r<',
             2.5, ssC["mean"] - ssC["std"], 'r>')
    
    plt.xlim([0, 4])
    
    plt.legend(["max", "min", "mean", "median", "per75", "perc25", 
                "mean+std", "mean-std"], loc=1)
        
    plt.show()
    
    

    # testing 2c, count the number of occurrences of each 
    # integer in resaled data, 
    countA = myHistWithRescale(listA)
    countB = myHistWithRescale(listB)
    countC = myHistWithRescale(listC)

    
    print (countA)
    print (countB)
    print (countC)
    

    # 3b. complete the following code to plot the counts using Fig 2 in HW1 
    # as a template

    plt.figure(2)
    
    plt.xlabel('Value')
    plt.ylabel('Frequency')
    
    values = list(range(0, 15, 2))
    
    plt.plot(countA, 'ro:')
    plt.plot(countB, 'g')
    plt.plot(countC, 'bx-.')
    
    plt.legend(["Rescaled DataA", "Rescaled DataB", "Rescaled DataC"], loc=1)

    # insert details of plotting
    
    plt.show()