#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Sep 25 08:58:52 2018

@author: nick
"""

import numpy as np

def q1(x): 
    z = x[:, :2]
    y = x[:, :2].dot([[3, 0], [0, 2]]);
    print(z)
    print(y)
    
if __name__ == '__main__':
    x = np.array([[1,  2,   3,  4],
                  [5,  6,   7,  8],
                  [9,  10, 11, 12],
                  [13, 14, 15, 16]]);
    q1(x)