import os
from collections import defaultdict

os.system("find -not -empty -type f -printf '%s\t%i\n'>a.txt")
fil=open("a.txt","r");
n=0
a=defaultdict(list)
for line in fil:
	size=int
	inode=int
	size,inode=line.split()	
	a[size].append(inode)	
print a.items()


