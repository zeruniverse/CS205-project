# Run MapReduce on AWS EMR

1. Create AWS cluster, use the lastest version **emr-5.13.0** (Core Hadoop), our script doesn't work on older emr versions. We use *m4.2xlarge* nodes for our experiments.

2. ssh into master node, install `git` using the following command

```bash
sudo yum install git
```

3. Clone our repository

```bash
git clone https://github.com/zeruniverse/CS205-project.git
```

4. Install `cmake` and other libraries to compile deepflow implementations. We wrote a script for this.

```bash
cd CS205-project/src/MapReduce
bash setup_master_cmake.sh
```

Then you can compile deepflow (`RBSOR_serial` and `RBSOR_OMP`) following [this](https://github.com/zeruniverse/CS205-project/blob/master/README.md#serial-and-openmp)

**As an alternative, you can download our pre-compiled binary** if you are using AWS EMR-5.13.0. By doing this, you **don't need to install cmake**.

```bash
# serial
wget -O deepflow2 'https://github.com/zeruniverse/CS205-project/releases/download/0.02/deepflow2_rbsor_serial'
# OMP
wget -O deepflow2 'https://github.com/zeruniverse/CS205-project/releases/download/0.02/deepflow2_rbsor_omp'
```

Put compiled or downloaded deepflow2 under path `CS205-project/src/MapReduce`

5. Use `input_generator.py` to generate input frame pairs as MapReduce input.

```bash
python input_generator.py END_FRAME
```

`END_FRAME` is the last frame to process. The last flow pair will be between `END_FRAME-1` and `END_FRAME`. The first frame is always 1.

6. copy your frames and matches to master node using `scp`, or `wget` our samples. Use `wget` for example:

```bash
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_match.zip
wget https://github.com/zeruniverse/CS205-project/releases/download/0.01/bunny_640x360_frame.zip
unzip bunny_640x360_match.zip
unzip bunny_640x360_frame.zip
```

7. run file_submit.py to submit files to HDFS

```bash
python file_submit.py END_FRAME
```

8. Change hadoop configuration

```bash
cd /etc/hadoop/conf
sudo vim mapred-site.xml
### Change the following settings:
# mapreduce.map.memory.mb: 15360
# mapreduce.reduce.memory.mb: 1536
# mapreduce.map.java.opts: -Xmx12290m
# mapreduce.reduce.java.opts: -Xmx1000m
# mapreduce.tasktracker.map.tasks.maximum: 1
### If you are not using m4.2xlarge, change accordingly. The rule of thumb is giving more memory to mapper.
```
9. run hadoop streaming

```bash
hadoop jar /usr/lib/hadoop/hadoop-streaming-2.8.3-amzn-0.jar -D mapred.reduce.tasks=0 -files mapper.py -mapper "python mapper.py" -input  /file_list.txt -output my_output
```

10. Download the output files from hdfs to master node:

```bash
hadoop fs -get /flow .
```
