import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import pylab as plab
import scipy.stats as stats

df2 = pd.read_csv("hw3q2.csv")

def partA(df2):
    plt.figure(1)
    df2.boxplot( )
    plab.suptitle("Fig1. Boxplot for question Q2a")
    plt.show( )
    
def partB(df2):
    dfLog2 = df2.applymap(np.log2)
    
    plt.figure(2)
    dfLog2.boxplot( )
    plab.suptitle("Fig1. Boxplot for question Q2b")
    plt.show( )
    
def partC(df2):
    stats = df2.describe( )
    print(stats)
    
def partD(df2):
    plt.figure(3)
    df2.hist(normed=True, bins=20)
    plab.suptitle("Fig1. Histogram for question Q2d")
    plt.show( )
    
def partF(df2):
    a = df2['a']
    b = df2['b']
    c = df2['c']
    d = df2['d']
    
    # A
    plt.figure(4)
    
    plt.plot(1 - stats.norm.cdf(a.sort_values( )))
    plt.suptitle("Fig1. CCDF Plot for question Q2f")
    plt.xlabel("Data Column A")
    plt.ylabel("CCDF")
    plt.show( )
    
    # B
    plt.figure(5)
    stats.probplot(b, plot=plt)
    plt.suptitle("Fig2. Plot for question Q2f")
    plt.show( )
    
    # C
    plt.figure(6)
    stats.probplot(c, plot=plt)
    plt.suptitle("Fig3. Plot for question Q2f")
    plt.show( )
    
    # D
    plt.figure(7)
    plt.plot(1 - stats.norm.cdf(d.sort_values( )))
    plt.suptitle("Fig4. CCDF Plot for question Q2f")
    plt.xlabel("Data Column D")
    plt.ylabel("CCDF")
    plt.show( )

partA(df2)
partB(df2)
partC(df2)
partD(df2)
partF(df2)  

