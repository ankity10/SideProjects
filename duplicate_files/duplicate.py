import os
import hashlib
from collections import defaultdict

def hashfile(file_path):
    sha1 = hashlib.sha1()
    f = open(file_path, 'rb')
    try:
        sha1.update(f.read())
    finally:
        f.close()
    return sha1.hexdigest()

os.system("find -not -empty -type f -printf '%s\t%p\n'>a.txt")
fil=open("a.txt","r");
n=0
a=defaultdict(list)
for line in fil:
	size=int
	path=str
	size,path=line.split()	
	a[size].append(path)

sha1_list = defaultdict(list)

for size, path in a.items():
	for file_path in path:
		sha1_list[hashfile(file_path)].append(file_path)

print sha1_list.items()
