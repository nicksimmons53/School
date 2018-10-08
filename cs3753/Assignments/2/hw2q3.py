#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Sep 26 17:13:16 2018

@author: nick
"""

import random
from matplotlib import pyplot as plt
from scipy.stats import norm 
from scipy.stats import binom 
import scipy.stats as stats
import numpy as np

# 3a function to simulate 'value' amount of iterations
def coinTossSimulation(value):
    tossResults = {'heads':[ ], 'tails':[ ]}
    for num in range(value):
        tossDict = randomToss( )
        tossResults['heads'].append(tossDict['heads'])
        tossResults['tails'].append(tossDict['tails'])
    return tossResults

# 3a generate random tosses
def randomToss( ):
    outcomes = {'heads':0, 'tails':0}
    flips = list(outcomes.keys( ))
    for i in range(100):
        result = random.choice(flips)
        if (result in 'heads'):
            outcomes['heads'] += 1
        else:
            outcomes['tails'] += 1
    return outcomes

def binomCDF( ):
    cdfList = [ ]
    x = 0
    for i in range(10):
        cdfList.append(binom.cdf(x, 100, 0.5))
        x += 10
    return cdfList

if __name__ == '__main__':
    
    # 3a answer
    tosses = coinTossSimulation(10**5)
    hist, bin_edges = np.histogram(tosses['heads'], bins=100)
    cdf = np.cumsum(hist)

    plt.figure(1)
    plt.hist(tosses['heads'], 100)
    plt.title('3A - Coin Flip Simulation')
    plt.xlabel('Number of Heads')
    plt.ylabel('Number of Iterations')
        
    plt.figure(2)
    plt.plot(bin_edges[1:], cdf/cdf[-1])
    plt.title('CDF of Coin Tosses')
    plt.xlabel('Number of Heads')
    plt.ylabel('Cumalative Probability')
    
    # 3b answer
    plt.figure(3)
    plt.loglog(binomCDF( ))
    plt.title('Binomial CDF ')
    plt.xlabel('Number of Heads')
    plt.ylabel('Cumalative Probability')
    
    # 3c answer 
    plt.figure(4)
    plt.probplot(tosses)
    
    plt.show( )
    