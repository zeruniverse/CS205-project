# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/hongxiangqiu/Desktop/clion-2018.1/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/hongxiangqiu/Desktop/clion-2018.1/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hongxiangqiu/Desktop/DeepFlow_omp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DeepFlow.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DeepFlow.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DeepFlow.dir/flags.make

CMakeFiles/DeepFlow.dir/deepflow2.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/deepflow2.c.o: ../deepflow2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DeepFlow.dir/deepflow2.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/deepflow2.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/deepflow2.c

CMakeFiles/DeepFlow.dir/deepflow2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/deepflow2.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/deepflow2.c > CMakeFiles/DeepFlow.dir/deepflow2.c.i

CMakeFiles/DeepFlow.dir/deepflow2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/deepflow2.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/deepflow2.c -o CMakeFiles/DeepFlow.dir/deepflow2.c.s

CMakeFiles/DeepFlow.dir/deepflow2.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/deepflow2.c.o.requires

CMakeFiles/DeepFlow.dir/deepflow2.c.o.provides: CMakeFiles/DeepFlow.dir/deepflow2.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/deepflow2.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/deepflow2.c.o.provides

CMakeFiles/DeepFlow.dir/deepflow2.c.o.provides.build: CMakeFiles/DeepFlow.dir/deepflow2.c.o


CMakeFiles/DeepFlow.dir/src/image.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/src/image.c.o: ../src/image.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/DeepFlow.dir/src/image.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/src/image.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/src/image.c

CMakeFiles/DeepFlow.dir/src/image.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/src/image.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/src/image.c > CMakeFiles/DeepFlow.dir/src/image.c.i

CMakeFiles/DeepFlow.dir/src/image.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/src/image.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/src/image.c -o CMakeFiles/DeepFlow.dir/src/image.c.s

CMakeFiles/DeepFlow.dir/src/image.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/src/image.c.o.requires

CMakeFiles/DeepFlow.dir/src/image.c.o.provides: CMakeFiles/DeepFlow.dir/src/image.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/src/image.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/src/image.c.o.provides

CMakeFiles/DeepFlow.dir/src/image.c.o.provides.build: CMakeFiles/DeepFlow.dir/src/image.c.o


CMakeFiles/DeepFlow.dir/src/io.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/src/io.c.o: ../src/io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/DeepFlow.dir/src/io.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/src/io.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/src/io.c

CMakeFiles/DeepFlow.dir/src/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/src/io.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/src/io.c > CMakeFiles/DeepFlow.dir/src/io.c.i

CMakeFiles/DeepFlow.dir/src/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/src/io.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/src/io.c -o CMakeFiles/DeepFlow.dir/src/io.c.s

CMakeFiles/DeepFlow.dir/src/io.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/src/io.c.o.requires

CMakeFiles/DeepFlow.dir/src/io.c.o.provides: CMakeFiles/DeepFlow.dir/src/io.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/src/io.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/src/io.c.o.provides

CMakeFiles/DeepFlow.dir/src/io.c.o.provides.build: CMakeFiles/DeepFlow.dir/src/io.c.o


CMakeFiles/DeepFlow.dir/src/opticalflow.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/src/opticalflow.c.o: ../src/opticalflow.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/DeepFlow.dir/src/opticalflow.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/src/opticalflow.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow.c

CMakeFiles/DeepFlow.dir/src/opticalflow.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/src/opticalflow.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow.c > CMakeFiles/DeepFlow.dir/src/opticalflow.c.i

CMakeFiles/DeepFlow.dir/src/opticalflow.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/src/opticalflow.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow.c -o CMakeFiles/DeepFlow.dir/src/opticalflow.c.s

CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.requires

CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.provides: CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.provides

CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.provides.build: CMakeFiles/DeepFlow.dir/src/opticalflow.c.o


CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o: ../src/opticalflow_aux.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow_aux.c

CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow_aux.c > CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.i

CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/src/opticalflow_aux.c -o CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.s

CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.requires

CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.provides: CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.provides

CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.provides.build: CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o


CMakeFiles/DeepFlow.dir/src/solver.c.o: CMakeFiles/DeepFlow.dir/flags.make
CMakeFiles/DeepFlow.dir/src/solver.c.o: ../src/solver.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/DeepFlow.dir/src/solver.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DeepFlow.dir/src/solver.c.o   -c /home/hongxiangqiu/Desktop/DeepFlow_omp/src/solver.c

CMakeFiles/DeepFlow.dir/src/solver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DeepFlow.dir/src/solver.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hongxiangqiu/Desktop/DeepFlow_omp/src/solver.c > CMakeFiles/DeepFlow.dir/src/solver.c.i

CMakeFiles/DeepFlow.dir/src/solver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DeepFlow.dir/src/solver.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hongxiangqiu/Desktop/DeepFlow_omp/src/solver.c -o CMakeFiles/DeepFlow.dir/src/solver.c.s

CMakeFiles/DeepFlow.dir/src/solver.c.o.requires:

.PHONY : CMakeFiles/DeepFlow.dir/src/solver.c.o.requires

CMakeFiles/DeepFlow.dir/src/solver.c.o.provides: CMakeFiles/DeepFlow.dir/src/solver.c.o.requires
	$(MAKE) -f CMakeFiles/DeepFlow.dir/build.make CMakeFiles/DeepFlow.dir/src/solver.c.o.provides.build
.PHONY : CMakeFiles/DeepFlow.dir/src/solver.c.o.provides

CMakeFiles/DeepFlow.dir/src/solver.c.o.provides.build: CMakeFiles/DeepFlow.dir/src/solver.c.o


# Object files for target DeepFlow
DeepFlow_OBJECTS = \
"CMakeFiles/DeepFlow.dir/deepflow2.c.o" \
"CMakeFiles/DeepFlow.dir/src/image.c.o" \
"CMakeFiles/DeepFlow.dir/src/io.c.o" \
"CMakeFiles/DeepFlow.dir/src/opticalflow.c.o" \
"CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o" \
"CMakeFiles/DeepFlow.dir/src/solver.c.o"

# External object files for target DeepFlow
DeepFlow_EXTERNAL_OBJECTS =

DeepFlow: CMakeFiles/DeepFlow.dir/deepflow2.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/src/image.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/src/io.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/src/opticalflow.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/src/solver.c.o
DeepFlow: CMakeFiles/DeepFlow.dir/build.make
DeepFlow: /usr/lib/x86_64-linux-gnu/libjpeg.so
DeepFlow: /usr/lib/x86_64-linux-gnu/libpng.so
DeepFlow: CMakeFiles/DeepFlow.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Linking C executable DeepFlow"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DeepFlow.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DeepFlow.dir/build: DeepFlow

.PHONY : CMakeFiles/DeepFlow.dir/build

CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/deepflow2.c.o.requires
CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/src/image.c.o.requires
CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/src/io.c.o.requires
CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/src/opticalflow.c.o.requires
CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/src/opticalflow_aux.c.o.requires
CMakeFiles/DeepFlow.dir/requires: CMakeFiles/DeepFlow.dir/src/solver.c.o.requires

.PHONY : CMakeFiles/DeepFlow.dir/requires

CMakeFiles/DeepFlow.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DeepFlow.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DeepFlow.dir/clean

CMakeFiles/DeepFlow.dir/depend:
	cd /home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hongxiangqiu/Desktop/DeepFlow_omp /home/hongxiangqiu/Desktop/DeepFlow_omp /home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug /home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug /home/hongxiangqiu/Desktop/DeepFlow_omp/cmake-build-debug/CMakeFiles/DeepFlow.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DeepFlow.dir/depend

