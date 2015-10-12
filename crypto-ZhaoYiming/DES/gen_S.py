in_file = open("table_S.txt","r")
ou_file = open("new_tableS.txt","w")


arr = map(int, in_file.read().split())

for i in xrange(len(arr)):
	if i % 16 == 0:
		print >> ou_file
	print >> ou_file, int(str(bin(arr[i]))[5:1:-1], 2),
