#!/usr/bin/python3
import os
from collections import defaultdict
import time
from threading import Thread
def main(a):
	for size,file_path in a.items():
		if(len(file_path) != 1):
			t = Thread(target=test, args=(size,file_path))
			t.start()

def test(a, b):
	
# Main function Start
os.system("find / -not -empty -type f -printf '%s\t%p\n'>a.txt")
fil=open("a.txt","r");
n=0
a=defaultdict(list)
for line in fil:
	size=int
	path=str
	size,path=line.split()	
	a[size].append(path)	

#Creating a Main thread
main_thread=Thread(target=main, args=(a,))
main_thread.start()

# Wait for other subthreads
main_thread.join()
		


