#!/usr/bin/python

import sys
from subprocess import call
from subprocess import Popen, PIPE
import os

from os import path
executable = "/DeepFlow_omp"
ppm_root = "/frame"
match_root = "/match"
if not path.exists("/tmp/deepflow2"):
    run = Popen(["hadoop", "fs", "-get", executable, "/tmp/deepflow2"])
    run.communicate()
    run = Popen(["chmod", "+x", "/tmp/deepflow2"])
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
    Popen(["hadoop", "fs", "-get", "-f", ppm1, "/tmp/1.ppm"]),
    Popen(["hadoop", "fs", "-get", "-f", ppm2, "/tmp/2.ppm"]),
    Popen(["hadoop", "fs", "-get", "-f", match_forward, "/tmp/forward.match"]),
    Popen(["hadoop", "fs", "-get", "-f", match_backward, "/tmp/backward.match"])
    ]
    for x in run:
        x.communicate()
    run = [
    Popen(["/tmp/deepflow2", "/tmp/1.ppm", "/tmp/2.ppm", "/tmp/forward.flo","-match","/tmp/forward.match"]),
    Popen(["/tmp/deepflow2", "/tmp/2.ppm", "/tmp/1.ppm", "/tmp/backward.flo", "-match", "/tmp/backward.match"])
    ]
    [x.communicate() for x in run]

    run = [
    Popen(["hadoop", "fs", "-put", "/tmp/forward.flo", "/flow/forward_{}_{}.flo".format(prev,next)]),
    Popen(["hadoop", "fs", "-put", "/tmp/backward.flo", "/flow/backward_{}_{}.flo".format(next,prev)])
    ]
    [x.communicate() for x in run]
    print('succ')