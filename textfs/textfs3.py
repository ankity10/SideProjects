import time,sys,shelve
import os.path

class File_class(object):

	def __init__(self, file_name):
		self.name = str(file_name)
		self.content = str()
		self.inode = str(time.time())
		self.created_at = time.ctime()

	def copy(self,source,destination):
		src = open(source,"r")
		self.name = destination
		self.content = src.read()

	def echo(self):
		print self.content


class Storage():

	def __init__(self):
		self.store = shelve.open("textfs.sys",writeback=True)

	def exist(self, inode):
		if inode in self.store: 
			return True
		return False

	def add(self, file):
		if self.exist(file.inode):
			print("File already exists")
			return False
		else:
			self.store[file.inode] = file
			return True
			
	def delete(self, file):
		if self.exist(file.inode):
			del self.store[file.inode]
			return True
		else
			print("File does not exist")
			return False

class Filesystem():

	def __init__(self):
		self.commands = []
		self.storage = shelve.open("textfs.sys",writeback=True)
		print("File system started")
		print("Available commands : create, copy, echo, copy")


	def create(self, name):
		file = File(name)




#f.create("b.txt")
io = shelve.open("textfs.sys",writeback=True)
print("COMMANDS:create,delete,echo,copy")
print(" > ",end = "")
cmd = input(">").split()

while(cmd[0] != "exit"):

	if cmd[0] == "create":
		for i in range(1,len(cmd)):
			found=0
			for inode_no, obj in io.items():
				if obj.name==cmd[i]:
					found=1
					break
			if found:
				print("File "+cmd[i]+" already exists!")
			else:
				f=file_class()
				f.create(cmd[i])
				io[f.inode_no]=f
				print("File "+cmd[i]+" created!")

	elif cmd[0]=="copy":
		if len(cmd)==3:
			found=0
			for inode_no, obj in io.items():
				if obj.name==cmd[2]:
					found=1
					break
			if found==1:
				print ("File "+cmd[2]+" already exists!")
			else:
				if os.path.isfile(cmd[1]):	
					f=file_class()
					f.copy(cmd[1],cmd[2])
					io[f.inode_no]=f
					print (len(io.items()))
				else:
					print ("File "+cmd[1]+" doesn't exist!")

		else:
			print ("Arguments incorrect. ")
			print ("Correct command: copy source destination")

	elif cmd[0]=="echo":
		if len(cmd)==2:
			found=0
			for inode_no,obj in io.items():
				if obj.name==cmd[1] :
					obj.echo()
					found=1
			if found==0:
				print("File not found")
		else:
			print("Arguments incorrect. ")
			print("Correct command: echo filename")

	elif cmd[0]=="delete":
		if len(cmd)==2:	
			found=0
			for inode_no,obj in io.items():
				if obj.name==cmd[1]:
					del io[obj.inode_no]

		else:
			print("Arguments incorrect. ")
			print("Correct command: delete filename")

	elif cmd[0]=="ls":
		if len(cmd)==1:
			for inode_no,obj in io.items():
				print obj.name

		else:
			print("Arguments incorrect. ")
			print("Correct command: ls")


	else:
		print("Invalid Command")
	print(" > ")
	cmd=input().split()
	
