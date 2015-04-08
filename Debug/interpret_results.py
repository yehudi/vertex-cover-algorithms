#!/usr/bin/python

import re
import matplotlib.pyplot as plt
import numpy as np

f = open("results180.csv","r")
lines = [i.split(",") for i in f.readlines() if len(i.split(",")) > 1]
f.close()
ns = [int(i[0]) for i in lines if float(i[0]) > 1]
ncols = [i for i in range(len(lines)) if float(lines[i][0]) > 1] 
algos = [i for i in lines[0] if re.search("VC", i)]
alidx = [i for i in range(len(lines[0])) if re.search("VC", lines[0][i])]


print(ns)
print(ncols)
print(algos)
print(alidx)

for p in range(1,7):
    #Time per N peralgo
    for k in range(len(algos)):
        a = algos[k]
        x = ns
        y = [float(lines[i+p][alidx[k]+1])+1 for i in ncols]
        plt.plot(x,y,"-",label = a)
    plt.legend()
    plt.title("time per N per algo p="+str(lines[p][0]))
    plt.show()

    minsol = [min([float(lines[i+p][alidx[k]]) for k in range(len(algos))]) for i in ncols]

    for k in range(len(algos)):
        a = algos[k]
        x = ns
        y = [float(lines[ncols[i]+p][alidx[k]])/minsol[i] for i in range(len(ncols))]
        plt.plot(x,y,"-",label = a)
    plt.legend()
    plt.title("ratio per N per algo p="+str(lines[p][0]))
    plt.show()
