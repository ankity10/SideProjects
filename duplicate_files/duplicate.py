#!/usr/local/bin/python3
import hashlib
from collections import defaultdict
import time
import threading
import subprocess
import multiprocessing


log_file = open("duplicate_log.txt",'w')

def hashfile(file_path):
    sha1 = hashlib.sha1()
    f = open(file_path, 'rb')
    try:
        sha1.update(f.read())
    finally:
        f.close()
    return sha1.hexdigest()

def dispatch_threads():
	# threads container to hold threads
	threads = []
	thread_num = 0
	cpu_count = multiprocessing.cpu_count()
	# creating one thread for each size having more than one file
	for size, file_path in size_list.items():
		if(len(file_path) != 1):
			while threading.active_count() == cpu_count:
				pass
			thread_num = thread_num + 1
			thread_obj = threading.Thread(target=calc_sha_1, args=(size, file_path))
			thread_obj.start()
			threads.append(thread_obj)
			
			print(str(thread_num)+" thread started")

			# if thread_num%cpu_count == 0:
			# 	print("Waiting for previous 4 threads to complete the task")

			# 	for i in range(1,5):
			# 		threads[thread_num-i].join()

					# thread_obj.join()

	# joining threads to main thread
	# for thread in threads:
	# 	thread.join()
	log_file.write("Number of threads created are : " + str(thread_num)+"\n\n")

# 
def calc_sha_1(size_list, file_paths):
	for file_path in file_paths:
		sha1_list[hashfile(file_path)].append(file_path)


# Main function Start
# os.system("find -not -empty -type f -printf '%s;%p\n'>a.txt")
# info_file = open("a.txt","r")

process_info = subprocess.run(["find", "-not", "-empty", "-type", "f", "-printf","%s;%p&"], stdout = subprocess.PIPE)
binary_buffer = process_info.stdout
utf_buffer = binary_buffer.decode("utf-8")
# print(utf_buffer)


# size vs filenames dictionary
size_list = defaultdict(list)

for pair in utf_buffer.split("&"):
	if(pair):
		l=pair.strip().split(";")
		if len(l)==2:
			size=l[0]
			path=l[1]	
			size_list[int(size)].append(str(path))	

# sha-1 vs filenames dictionary
sha1_list = defaultdict(list)
# dispatch threads
dispatch_threads()

log_file.write("\n\n========================== Duplicate files ========================== \n\n")
# Printing the results

time.sleep(multiprocessing.cpu_count())
for sha_val, file_paths in sha1_list.items():
	if(len(file_paths) > 1):
		print("Sha1 value: "+sha_val+" duplicate files: ",end='\n\n')
		log_file.write("Sha1 value: "+sha_val+" duplicate files: \n\n")

		for file_path in file_paths:
			print(file_path)
			log_file.write(file_path+"\n")
		
		print("\n\n\n")
		log_file.write("\n")

print()
print("Program completed successfully!!!!")
log_file.write("\n\nProgram completed successfully!!!!\n\n")