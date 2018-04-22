from subprocess import Popen

put = Popen(["hadoop", "fs", "-get", "/flow", "."])
put.communicate()

