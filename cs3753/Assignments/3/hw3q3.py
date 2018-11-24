import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats
from sklearn import linear_model
from sklearn.metrics import mean_squared_error as mse
from sklearn.metrics import r2_score as r2

hdfstore = pd.HDFStore('hw3q3.h5')
x_df = hdfstore['x']
y_df = hdfstore['y']

lr = linear_model.LinearRegression( )
lr.fit(x_df, y_df)
pred = lr.predict(x_df)

def partA(y_df, pred):
    rsquared = r2(y_df, pred)
    meanse = mse(y_df, pred)
    
    plt.figure(1)
    data = ['R2', 'MSE']
    xs = range(len(data))
    ys = [rsquared, meanse]
    plt.bar(xs, ys, 0.3)
    plt.xticks(xs, data)
    plt.suptitle("Fig1. Boxplot for question Q3a")
    plt.show( )

    return rsquared, meanse

def partB(x_df, y_df, rsquared, meanse):
    n = 1000
    data_size = len(y_df)
    coef_bs = np.zeros((n, x_df.shape[1]))
    for i in range(n):
        sample = np.random.choice(np.arange(data_size), size=data_size, replace=True)
        newY = y_df[sample]
        newX = x_df.iloc[sample, :]
        lr.fit(newX, newY)
        coef_bs[i, :] = lr.coef_
    coef_bs = pd.DataFrame(coef_bs)
    
    t = rsquared / coef_bs.std(axis=0)
    pvalue = 2 * stats.t.cdf(-np.abs(t), data_size - y_df[1] - 1)
    
    plt.figure(3)
    xs = range(len(pvalue))
    plt.bar(xs, pvalue)
    plt.suptitle("Fig2. Boxplot for question Q3b")
    
def partC(x_df, y_df):
    l = [-6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6]
    for i in l:
        las = linear_model.Lasso(alpha=2**i)
        las.fit(x_df, y_df)
        pred = las.predict(x_df)
        pd.Series(las.coef_).plot(kind='bar')
    
rsquared, meanse = partA(y_df, pred)
partB(x_df, y_df, rsquared, meanse)
partC(x_df, y_df)