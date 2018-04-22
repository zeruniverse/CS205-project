Create AWS cluster, note use the lastest version emr-5.13.0, otherwise some commands don't work

1. Use input_generator.py to generate input pairs
To run input_generator.py:
python input_generator.py NUM_FRAMES

2. run scp.sh to copy files to aws, usage:
scp.sh SERVER_ADDRESS

Then switch to aws master node:

3. run file_submit.py to submit files to hdfs:
python file_submit.py NUM_FRAMES

4. Change hadoop configuration
cd $HADOOP_HOME/etc/hadoop/conf
Open mapped-site.xml
Then set these two things:
In mapred-site.xml:
mapreduce.map.memory.mb: 4096
mapreduce.reduce.memory.mb: 8192
Each Container will run JVMs for the Map and Reduce tasks. The JVM heap size should be set to lower than the Map and Reduce memory defined above, so that they are within the bounds of the Container memory allocated by YARN.
In mapred-site.xml:
mapreduce.map.java.opts: -Xmx3072m
mapreduce.reduce.java.opts: -Xmx6144m

(recommended configuration worked in my case: master m4.xlarge, two nodes m4.2xlarge)
 <property>
    <name>mapreduce.map.java.opts</name>
    <value>-Xmx12290m</value>
  </property>

  <property>
    <name>mapreduce.reduce.java.opts</name>
    <value>-Xmx1000m</value>
  </property>

  <property>
    <name>mapreduce.reduce.memory.mb</name>
    <value>1536</value>
  </property>

  <property>
    <name>mapreduce.map.memory.mb</name>
    <value>15360</value>
  </property>

  5. run hadoop streaming
  hadoop jar /usr/lib/hadoop/hadoop-streaming-2.8.3-amzn-0.jar -files /home/hadoop/mapper.py,/home/hadoop/reducer.py -mapper "python mapper.py" -reducer "python reducer.py" -input  /file_list.txt -output my_output

  6.Download the output files from hdfs to master node:
  python file_get.py
