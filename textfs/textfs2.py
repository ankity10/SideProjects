import time,os

def create(file_name):
	with open("superblock.txt","a+") as sb_obj:
		with open("textfs.txt","a+") as textfs_obj:
			pos=str(textfs_obj.tell())
			sb_obj.write(file_name+";"+pos+";"+pos+";"+str(time.time())+"\n")


def copy(source,destination):
	with open("superblock.txt","a+") as sb_obj:
		with open("textfs.txt","a") as textfs_obj:
			with open(source,"r") as des:
				pos=str(textfs_obj.tell())
				inode_no=str(time.time())
				textfs_obj.write(inode_no+";"+destination+";"+des.read())
				#textfs_obj.write(des.read())
				pos2=str(textfs_obj.tell())
				sb_obj.write(destination+";"+pos+";"+pos2+";"+str(time.time())+"\n")


def echo(file_name):
	with open("superblock.txt","r") as sb_obj:
		with open("textfs.txt","r") as textfs_obj:
			for sb_info in sb_obj:
				sb_file_info=sb_info.split(";")
				if(sb_file_info[0]==file_name):
					f_start=int(sb_file_info[1])
					f_end=int(sb_file_info[2])
			position=textfs_obj.seek(f_start,0)
			f_info=textfs_obj.read(f_end-f_start).split(";")
			print f_info[2]


def ls():
	with open("superblock.txt","r") as sb_obj:
		for sb_info in sb_obj:
			sb_file_info=sb_info.split(";")
			print sb_file_info[0]


def delete(file_name):
	sb_upper=""
	sb_lower=""
	textfs_upper=""
	textfs_lower=""
	with open("superblock.txt","r") as sb_obj:
		with open("textfs.txt","r") as textfs_obj:
			found=0
			f_start=int
			f_end=int
			for sb_info in sb_obj:
				sb_file_info=sb_info.split(";")
				if sb_file_info[0]==file_name:
					f_start=int(sb_file_info[1])
					f_end=int(sb_file_info[2])
					print "File found"
					found=1
				else:
					if not found:
						sb_upper=sb_upper+str(sb_info)
					else:
						sb_file_info[1]=int(sb_file_info[1])-(f_end-f_start)
						sb_file_info[2]=int(sb_file_info[2])-(f_end-f_start)
						temp=str(sb_file_info[0])+";"+str(sb_file_info[1])+";"+str(sb_file_info[2])+";"+str(sb_file_info[3])
						sb_lower=sb_lower+str(temp)
			textfs_upper=textfs_obj.read(f_start)
			textfs_obj.seek(f_end,0)
			textfs_lower=textfs_obj.read()
	with open("superblock.txt","w") as sb_obj:
		with open("textfs.txt","w") as textfs_obj:
			sb_obj.write(sb_upper+sb_lower)
			textfs_obj.write(textfs_upper+textfs_lower)


print "COMMANDS:create,delete,echo,copy,exit,ls"
cmd=raw_input(">").split()

while(cmd[0]!="exit"):
	if cmd[0]=="create":
		create(cmd[1])
	elif cmd[0]=="copy":
		copy(cmd[1],cmd[2])
	elif cmd[0]=="echo":
		echo(cmd[1])
	elif cmd[0]=="ls":
		ls()
	elif cmd[0]=="delete":
		delete(cmd[1])
	cmd=raw_input(">").split()

