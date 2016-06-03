import os,sys

f = open("a.txt","w")
chunk_size_Bytes = 1000

filedata_size_MB = 1

filedata_num_chunks = filedata_size_MB * 1000

inode_table_num_chunks = 100
superblock_num_chunks = 100

filesystem_size_Bytes = (filedata_num_chunks+inode_table_num_chunks+superblock_num_chunks)*chunk_size_Bytes

for i in range(0,filesystem_size_Bytes):
	f.write("0")
f.close()

print("Complete..")
print("Filesystem of size "+str(filesystem_size_Bytes/1000000)+" MB initialized")