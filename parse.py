import numpy as np
import argparse as ap
import os

pser = ap.ArgumentParser()
pser.add_argument("--a",type=float)
pser.add_argument("--b",type=float)
pser.add_argument("--n",type=int)
inp = pser.parse_args()

a=inp.a
b=inp.b
n=inp.n

while(n%4 != 0)
	n-=1;

print(a)

div = (b-a)/n
print(div)

numline = np.arange(a,b,div).tolist()
numline.append(b)

func = np.sin(numline)

print(func)

func = np.insert(func,0,div)
func = np.insert(func,0,n+1)

numarray = np.array(func,'float32')

file = open("py"+str(n),'wb')
numarray.tofile(file)
file.close()
