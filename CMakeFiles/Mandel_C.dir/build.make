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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jacobstahl/Desktop/code/Mandel_C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jacobstahl/Desktop/code/Mandel_C

# Include any dependencies generated for this target.
include CMakeFiles/Mandel_C.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Mandel_C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Mandel_C.dir/flags.make

CMakeFiles/Mandel_C.dir/main.cpp.o: CMakeFiles/Mandel_C.dir/flags.make
CMakeFiles/Mandel_C.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jacobstahl/Desktop/code/Mandel_C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Mandel_C.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Mandel_C.dir/main.cpp.o -c /home/jacobstahl/Desktop/code/Mandel_C/main.cpp

CMakeFiles/Mandel_C.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Mandel_C.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jacobstahl/Desktop/code/Mandel_C/main.cpp > CMakeFiles/Mandel_C.dir/main.cpp.i

CMakeFiles/Mandel_C.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Mandel_C.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jacobstahl/Desktop/code/Mandel_C/main.cpp -o CMakeFiles/Mandel_C.dir/main.cpp.s

CMakeFiles/Mandel_C.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Mandel_C.dir/main.cpp.o.requires

CMakeFiles/Mandel_C.dir/main.cpp.o.provides: CMakeFiles/Mandel_C.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Mandel_C.dir/build.make CMakeFiles/Mandel_C.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Mandel_C.dir/main.cpp.o.provides

CMakeFiles/Mandel_C.dir/main.cpp.o.provides.build: CMakeFiles/Mandel_C.dir/main.cpp.o


# Object files for target Mandel_C
Mandel_C_OBJECTS = \
"CMakeFiles/Mandel_C.dir/main.cpp.o"

# External object files for target Mandel_C
Mandel_C_EXTERNAL_OBJECTS =

Mandel_C: CMakeFiles/Mandel_C.dir/main.cpp.o
Mandel_C: CMakeFiles/Mandel_C.dir/build.make
Mandel_C: CMakeFiles/Mandel_C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jacobstahl/Desktop/code/Mandel_C/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Mandel_C"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Mandel_C.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Mandel_C.dir/build: Mandel_C

.PHONY : CMakeFiles/Mandel_C.dir/build

CMakeFiles/Mandel_C.dir/requires: CMakeFiles/Mandel_C.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Mandel_C.dir/requires

CMakeFiles/Mandel_C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Mandel_C.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Mandel_C.dir/clean

CMakeFiles/Mandel_C.dir/depend:
	cd /home/jacobstahl/Desktop/code/Mandel_C && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jacobstahl/Desktop/code/Mandel_C /home/jacobstahl/Desktop/code/Mandel_C /home/jacobstahl/Desktop/code/Mandel_C /home/jacobstahl/Desktop/code/Mandel_C /home/jacobstahl/Desktop/code/Mandel_C/CMakeFiles/Mandel_C.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Mandel_C.dir/depend

