import time,sys,shelve


class file_class(object):

	def __init__(self):
		self.name=str
		self.content=str

	def create(self,file_name):
		self.inode_no=str(time.time())
		self.name=file_name
		self.content=""

	def copy(self,source,destination):
		src=open(source,"r")
		self.name=destination
		self.inode_no=str(time.time())
		self.content=src.read()

	def echo(self):
		print self.content



#f.create("b.txt")
io=shelve.open("textfs.sys",writeback=True)
print "COMMANDS:create,delete,echo,copy"
cmd=raw_input(">").split()

while(cmd[0]!="exit"):

	if cmd[0]=="create":
		found=0
		for inode_no, obj in io.items():
			if obj.name==cmd[1]:
				found=1
				break
		if found:
			print "File "+cmd[1]+" already exists"
		else:
			f=file_class()
			f.create(cmd[1])
			io[f.inode_no]=f

	elif cmd[0]=="copy":
		if len(cmd)==3:
			found=0
			for inode_no, obj in io.items():
				if obj.name==cmd[2]:
					found=1
					break
			if found==1:
				print "File "+cmd[2]+" already exists!"
			else:		
				f=file_class()
				f.copy(cmd[1],cmd[2])
				io[f.inode_no]=f
				print len(io.items())

	elif cmd[0]=="echo":
		found=0
		for inode_no,obj in io.items():
			if obj.name==cmd[1] :
				obj.echo()
				found=1
		if found==0:
			print "File not found"

	elif cmd[0]=="delete":
		found=0
		for inode_no,obj in io.items():
			if obj.name==cmd[1]:
				del io[obj.inode_no]

	elif cmd[0]=="ls":
		for inode_no,obj in io.items():
			print obj.name


	else:
		print "Invalid Command"

	cmd=raw_input(">").split()
	
