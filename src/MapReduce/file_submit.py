from subprocess import Popen
import sys

for i in range(1, int(sys.argv[1])):
    put1 = Popen(["hadoop", "fs", "-put", "-f", "match/forward_{}_{}.match".format(i, i + 1), "/match/forward_{}_{}.match".format(i, i + 1)])
    put2 = Popen(["hadoop", "fs", "-put", "-f", "match/backward_{}_{}.match".format(i+1, i), "/match/backward_{}_{}.match".format(i+1, i)])
    put3 = Popen(["hadoop", "fs", "-put", "-f", "out/frame_%06d.ppm" % i,
                 "/frame/frame_%06d.ppm" % i])
    put1.communicate()
    put2.communicate()
    put3.communicate()

