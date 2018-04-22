from subprocess import Popen
import sys

ppm_root = "/frame"
match_root = "/match"
flow_root = "/flow"
executable = "deepflow2-static"
input_file = "file_list.txt"
put = Popen(["hadoop", "fs", "-mkdir" , ppm_root])
put.communicate()
put = Popen(["hadoop", "fs", "-mkdir" , match_root])
put.communicate()
put = Popen(["hadoop", "fs", "-mkdir" , flow_root])
put.communicate()
put = Popen(["hadoop", "fs", "-put", "-f", executable, "/"+ executable])
put.communicate()
put = Popen(["hadoop", "fs", "-put", "-f", input_file, "/"+ input_file])
put.communicate()
for i in range(1, int(sys.argv[1])):
    put1 = Popen(["hadoop", "fs", "-put", "-f", "match/forward_{}_{}.match".format(i, i + 1), match_root+"/forward_{}_{}.match".format(i, i + 1)])
    put2 = Popen(["hadoop", "fs", "-put", "-f", "match/backward_{}_{}.match".format(i+1, i), match_root+"/backward_{}_{}.match".format(i+1, i)])
    put3 = Popen(["hadoop", "fs", "-put", "-f", "out/frame_%06d.ppm" % i,
                 ppm_root+"/frame_%06d.ppm" % i])
    put1.communicate()
    put2.communicate()
    put3.communicate()
    print("files {} successfully submitted".format(i))

