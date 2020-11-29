import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

df=pd.read_csv('your_policy λ=0.200000.csv',header=None)
x=df[0]
y=df[1]
fig=plt.bar(x,y)
plt.show()