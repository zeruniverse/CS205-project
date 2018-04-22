import sys

f = open("file_list.txt", "w")
#num = int(sys.argv[1])
num = 10
for i in range(1, num):
    f.write(str(i) + "," + str(i+1) + "\n")
f.close()