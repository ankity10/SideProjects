#!/usr/bin/python3
import os
import hashlib
from collections import defaultdict
import time
from threading import Thread

def hashfile(file_path):
    sha1 = hashlib.sha1()
    f = open(file_path, 'rb')
    try:
        sha1.update(f.read())
    finally:
        f.close()
    return sha1.hexdigest()

def main():
	threads = []
	for size,file_path in size_list.items():
		if(len(file_path) != 1):
			t = Thread(target=test, args=(size,file_path))
			t.start()
			threads.append(t)
	for i in threads:
		i.join()

def test(size_list, file_paths):
	for file_path in file_paths:
		sha1_list[hashfile(file_path)].append(file_path)
	
# Main function Start
os.system("find -not -empty -type f -printf '%s\t%p\n'>a.txt")
fil=open("a.txt","r")

size_list=defaultdict(list)
for line in fil:
	size=int
	path=str
	size,path=line.split()	
	size_list[size].append(path)	

sha1_list = defaultdict(list)

main()
#Creating a Main thread
# main_thread=Thread(target=main, args=(size_list,))
# main_thread.start()
# # time.sleep(2)
# # Wait for other subthreads
# main_thread.join()

for sha_val, file_paths in sha1_list.items():
	if(len(file_paths) > 1):
		print "Sha1 value: "+sha_val+" duplicate files: "
		for file_path in file_paths:
			print file_path,
