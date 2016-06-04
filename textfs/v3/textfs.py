import os.path
import random

filedata_size_MB = 1
chunk_size_Bytes = 1000
filedata_num_chunks = filedata_size_MB * 1000


inode_table_num_chunks = 100
superblock_num_chunks = 101

superblock_start = 0

inode_table_start = superblock_num_chunks * chunk_size_Bytes

filedata_start = inode_table_start + (inode_table_num_chunks * chunk_size_Bytes)

fs = "filesystem.txt"

	
if not os.path.isfile(fs):
	print("initialize the filesystem using 'python setup.py' ")
	exit()

fin = open(fs, "r")

split_delimiter = ""
for i in range(0,50):
	split_delimiter = split_delimiter + "0"

#==========================global func==================
def file_exist(filename):
	# fin.close()
	fin =open(fs,"r")
	fin.seek(inode_table_start, 0)


	for i in range(1, 101):
		name = fin.read(100)
		name = name.split(split_delimiter)[0]

		if name == filename:
			return True

		fin.seek(inode_table_start + (i*chunk_size_Bytes),0)
	return False

# print(file_exist("asda"))

def create(filename):
	# fin.close()
	fin = open(fs,"r")
	fin.seek(superblock_start, 0)
	position = 0 #inode position in inode table

	while position < 100:
		fin.seek(position,0)
		status =int(fin.read(1))
		if status == 0:
			fin.seek(superblock_start, 0)

			str1 = fin.read(position)
			str2 = "1"
			
			fin.seek(position+1, 0)
			rem_bytes = (1000 -(position +1)) + (100*1000) + (position*1000)
			str3 = fin.read(rem_bytes)

			str4 = filename
			for i in range(0,1000-len(filename)): # 0 padding for full inode block of empty files
				str4 = str4 + "0"

			fin.seek((101 + position +1)*1000, 0)
			str5 = fin.read()
			str_final = str1 + str2 +str3+str4 +str5
			fout = open(fs, "w")
			fout.write(str_final)
			fout.close()
			print(position)
			return True

		position = position + 1

	print("Cannot create more files. Inode table full, max number of inode are 100")
	return False

# print(create("abc.txt"))

def make_in_6(no):
	st = str(no)
	temp=""
	for i in range(len(st),6):
		temp+="0"
	st = temp+st
	return st


def copy(src, dest):
	print("in copy")
	# fin.close()
	fin = open(fs,"r")

	fin_src = open(src, "r")
	src_buffer = fin_src.read()
	fin_src.close()

	fin.seek(1000,0)

	max_comp = filedata_size_MB * 1000 
	available = 0
	available_list = []

	for i in range(0, max_comp):
		ch = int(fin.read(1))
		if ch == 0:
			available_list.append(i)
			available = available + 1

	available = available * 994

	src_size = len(src_buffer)

	if  src_size > available:
		print("Cannot copy file, destination file too large")
		return False
	else:
		random.shuffle(available_list)
		req = (src_size+993)//994
		src_data_list = []
		for i in range(0,req-1):
			s = ""
			for j in range(i*994,i*994+994):
				s = s + str(src_buffer[j])
			s = s + make_in_6(available_list[i+1])
			src_data_list.append((available_list[i],s))


		#last chunk	
		s=""
		for i in range((req-1)*994, len(src_buffer)):
			s = s + src_buffer[i]

		ln=len(s)

		for i in range(ln,994):
			s = s + "0"

		s = s + make_in_6(-1)

		src_data_list.append((available_list[req-1], s))

		start = available_list[0]
		end = available_list[req-1]
		end_bytes = len(src_buffer) - (req-1)*994

		start = str(start)
		end = str(end)
		end_bytes = str(end_bytes)

		start = start + ";"
		end = end+";"
		end_bytes = end_bytes + ";"
		for i in range(len(start),100):
			start = start+"0"

		for i in range(len(end),100):
			end = end+"0"

		for i in range(len(end_bytes),100):
			end_bytes = end_bytes+"0"


		start_node=int()

		fin.seek(0,0)
		s = fin.read(100)

		fin.seek(inode_table_start,0)

		for index, i in enumerate(s):
			if i == "1":
				fin.seek(101*1000 + index*1000)
				file_block = fin.read(100)
				name = file_block.split(split_delimiter)[0]
				if name == dest:
					start_node = 101*1000 + index*1000

		fin.seek(0,0)
		

		str1 = fin.read(start_node)


		str2 = ""

		name = dest;
		for i in range(len(name), 100):
			name = name + "0"

		str2 = name + start + end + end_bytes + "0"*600

		fin.seek(start_node+1000,0)
		str3 = fin.read( (201*1000) - (start_node+1000) ) 

		src_data_list.sort(key=lambda tup: tup[0])

		str4 = ""

		fin.seek(filedata_start)

		first = src_data_list[0][0]

		str4 = fin.read( (first * 1000) )

		for i in range(0, len(src_data_list)-1):
			str4 = str4 + src_data_list[i][1]
			
			line = src_data_list[i][0]
			line+=1
			fin.seek(filedata_start + line*1000,0)
			# fin.seek((filedata_start) + (first * 1000) + ((i+1) * 1000), 0)
			
			first = src_data_list[i][0]+1
			second = src_data_list[i+1][0]
			diff = second-first
			str4 = str4 + fin.read(diff*1000)

		str4 = str4 + src_data_list[len(src_data_list)-1][1]
		last = src_data_list[len(src_data_list)-1][0]
		
		fin.seek((filedata_start+(last+1)*1000),0)
		# fin.seek(1000,1)
		str4 = str4 + fin.read()

		final_str = str1 + str2 + str3 + str4

		fout = open(fs,"w")
		fout.write(final_str)
		fout.close()
		print("Copied successfully!")
		return True




def ls():
	fin = open(fs,"r")
	fin.seek(0,0)
	s = fin.read(100)

	file_list = []

	for index,i in enumerate(s):
		if i == "1":
			fin.seek((101*1000 + index*1000))
			file_block = fin.read(100)
			name = file_block.split(split_delimiter)[0]
			
			file_list.append(name)

			print(name)




def delete(filename):
	fin = open(fs, "r")
	fin.seek(0, 0)
	s = fin.read(100)
	file_start=int()
	file_end=int()
	found=0
	file_index=int()
	for index,i in enumerate(s):
		if i == "1":
			fin.seek((101*1000) + (index*1000))
			file_block = fin.read(100)
			name = file_block.split(split_delimiter)[0]

			if name == filename:
				file_index = index
				
				file_start = int(fin.read(100).split(";")[0])
				file_end = int(fin.read(100).split(";")[0])
				
				fin.seek(1000,0)
				sb_block_content=fin.read(100*1000)

				fin.seek((101*1000+100*1000)+(file_start*1000),0)
				
				curr_chunk=file_start
				count = 0
				while curr_chunk != "0000-1":
					sb_block_content = sb_block_content.replace( sb_block_content[int(curr_chunk)], "0")
					fin.seek((101*1000+100*1000)+(file_start*1000) + (count*1000)+994, 0)
					curr_chunk=fin.read(6)
					count = count+1
				s = s.replace(s[file_index], "0")
				# s[file_index] = "0"
				str1 = s
				str2 = sb_block_content
				fin.seek((101*1000)+(100*1000))
				str3 = fin.read()

				str_final = str1 + str2 +str3

				fout = open(fs, "w")
				fout.write(str_final)
				fout.close()
				print("File Deleted successfully!")
				return True
				break
				
	return False



# ls()



print("COMMANDS: create, delete, echo, copy, ls")
cmd=input(" > ").strip().split(" ")


while(cmd[0] != "exit"):
	# print (cmd[0])

	if cmd[0] == "create"  :
		if len(cmd) != 2:
			print("Invalid arguments")
		else:
			filename = cmd[1]
			if file_exist(filename):
				print("File "+filename+" already exists")
			else:
				create(filename)


	elif cmd[0] == "copy":
		if len(cmd) != 3:
			print("Invalid arguments")
		else:
			src = cmd[1]
			dest = cmd[2]
			if not os.path.isfile(src):
				print("Source file "+src+" does'nt exists")
			else:
				if not file_exist(dest):
					print("Can't copy, destination file "+dest+" does'nt exist, create using 'create "+dest+"'")
				else:
					print(copy(src,dest))

	elif cmd[0] == "ls":
		if len(cmd) != 1:
			print("Invalid arguments")
		else:
			ls()
		

	elif cmd[0] == "delete":
		if len(cmd) != 2:
			print ("Invalid arguments")
		else:
			delete(cmd[1])






	cmd=input(" > ").strip().split(" ")












