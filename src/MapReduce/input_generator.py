import sys

f = open("file_list.txt", "w")
for i in range(1, int(sys.argv[1])):
    f.write(str(i) + "," + str(i+1) + "\n")
f.close()