#!/usr/bin/python

import sys
from subprocess import call
from subprocess import Popen, PIPE
import os

from os import path
executable = "/deepflow2"
ppm_root = "/frame"
match_root = "/match"

if not path.exists("/tmp"):
    run = Popen(["mkdir", "/tmp"])
    run.communicate()
run = Popen(["hadoop", "fs", "-get", "-f", executable, "/tmp/deepflow2"])
run.communicate()
run = Popen(["chmod", "777", "/tmp/deepflow2"])
run.communicate()

for line in sys.stdin.readlines():
    prev, next = line.split(",")
    prev = int(prev)
    next = int(next)
    ppm1 = ppm_root+"/"+"frame_%06d.ppm" % prev
    ppm2 = ppm_root+"/"+"frame_%06d.ppm" % next
    match_forward = match_root+"/"+"forward_{}_{}.match".format(prev,next)
    match_backward = match_root + "/"+"backward_{}_{}.match".format(next,prev)
    run =[
    Popen(["hadoop", "fs", "-get", "-f", ppm1, "/tmp/frame_%06d.ppm" % prev]),
    Popen(["hadoop", "fs", "-get", "-f", ppm2, "/tmp/frame_%06d.ppm" % next]),
    Popen(["hadoop", "fs", "-get", "-f", match_forward, "/tmp/forward_{}_{}.match".format(prev,next)]),
    Popen(["hadoop", "fs", "-get", "-f", match_backward, "/tmp/backward_{}_{}.match".format(next,prev)])
    ]
    for x in run:
        x.communicate()
    run = [
    Popen(["/tmp/deepflow2", "/tmp/frame_%06d.ppm" % prev, "/tmp/frame_%06d.ppm" % next,
           "/tmp/forward_{}_{}.flo".format(prev,next),"-match","/tmp/forward_{}_{}.match".format(prev,next)]),
    Popen(["/tmp/deepflow2", "/tmp/frame_%06d.ppm" % next, "/tmp/frame_%06d.ppm" % prev,
           "/tmp/backward_{}_{}.flo".format(next,prev), "-match", "/tmp/backward_{}_{}.match".format(next,prev)])
    ]
    for x in run:
        x.communicate()
    run = [
    Popen(["hadoop", "fs", "-put", "/tmp/forward_{}_{}.flo".format(prev,next), "/flow/forward_{}_{}.flo".format(prev,next)]),
    Popen(["hadoop", "fs", "-put", "/tmp/backward_{}_{}.flo".format(next,prev), "/flow/backward_{}_{}.flo".format(next,prev)])
    ]
    for x in run:
        x.communicate()
    run = [
        Popen(["rm", "-f", "/tmp/forward_{}_{}.flo".format(prev,next)]),
        Popen(["rm", "-f", "/tmp/backward_{}_{}.flo".format(next,prev)]),
        Popen(["rm", "-f", "/tmp/forward_{}_{}.match".format(prev,next)]),
        Popen(["rm", "-f", "/tmp/backward_{}_{}.match".format(next,prev)]),
        Popen(["rm", "-f", "/tmp/frame_%06d.ppm" % prev]),
        Popen(["rm", "-f", "/tmp/frame_%06d.ppm" % next]),
    ]
    for x in run:
        x.communicate()
    print("pair" + str(prev) + "succ")
