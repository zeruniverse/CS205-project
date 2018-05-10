# Different Implementations of Deepflow

Folders here correspond to different (parallelization) implementations of deepflow.

Compiling:

### Serial and OMP

To make executables, use following commands:

```bash
cd X
mkdir build
cd build
cmake ..
make
```

`X` here is the implementation you want to try (for example, `serial`).

### MPI and MPI+OMP

```bash
cd X
bash compile_mpi.sh
```

### OpenACC

```bash
cd X
bash compile_acc.sh
```

### MapReduce

See `README` inside `MapReduce`.

See the main README for more details.

**We made a list below for the code <b>we wrote</b> (i.e. not from original DeepFlow).**
    <ul>
        <li>serial Jacobi: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/Jacobi_serial/src/solver.c">solver</a></li>
        <li>serial RBSOR: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_serial/src/solver.c">solver</a></li>
        <li>OpenMP Jacobi: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/Jacobi_OMP/src/solver_omp.c">solver</a></li>
        <li>OpenMP RBSOR: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_OMP/src/solver.c">solver</a></li>
        <li>OpenACC Jacobi: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/Jacobi_ACC/src_acc/solver_acc.c">solver</a></li>
        <li>OpenACC RBSOR: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_ACC/src_acc/solver_acc.c">solver</a></li>
        <li>MPI RBSOR: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_MPI/src/solver.c">solver</a></li>
        <li>MPI+OpenMP RBSOR: <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_MPI_OMP/src/solver.c">solver</a></li>
        <li>MapReduce+OpenMP: <a href="https://github.com/zeruniverse/CS205-project/tree/master/src/MapReduce">MapReduce code</a></li>
        <li>Tool to make slow motion video based on DeepFlow: <a href="https://github.com/zeruniverse/CS205-project/tree/master/tools/flo2svflow">flo2svflow</a></li>
        <li>Bash script to generate DeepMatch and DeepFlow for videos: <a href="https://github.com/zeruniverse/CS205-project/blob/master/tools/deepmatching-video.sh">deepmatching-video.sh</a>, <a href="https://github.com/zeruniverse/CS205-project/blob/master/src/RBSOR_serial/video_flow.sh">video_flow.sh</a></li>
    </ul>
