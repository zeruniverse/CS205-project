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
