import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df=pd.read_csv('threshold_policy λ=0.333333.csv',header=None)
x=df[0]
y=df[1]
fig=plt.bar(x,y)
plt.show()