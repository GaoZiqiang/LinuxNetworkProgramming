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
CMAKE_SOURCE_DIR = /home/gaoziqiang/CLionProjects/myreactor/myreactor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gaoziqiang/CLionProjects/myreactor/myreactor/build

# Include any dependencies generated for this target.
include CMakeFiles/myreactorserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myreactorserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myreactorserver.dir/flags.make

CMakeFiles/myreactorserver.dir/main.cpp.o: CMakeFiles/myreactorserver.dir/flags.make
CMakeFiles/myreactorserver.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaoziqiang/CLionProjects/myreactor/myreactor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myreactorserver.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myreactorserver.dir/main.cpp.o -c /home/gaoziqiang/CLionProjects/myreactor/myreactor/main.cpp

CMakeFiles/myreactorserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myreactorserver.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaoziqiang/CLionProjects/myreactor/myreactor/main.cpp > CMakeFiles/myreactorserver.dir/main.cpp.i

CMakeFiles/myreactorserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myreactorserver.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaoziqiang/CLionProjects/myreactor/myreactor/main.cpp -o CMakeFiles/myreactorserver.dir/main.cpp.s

CMakeFiles/myreactorserver.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/myreactorserver.dir/main.cpp.o.requires

CMakeFiles/myreactorserver.dir/main.cpp.o.provides: CMakeFiles/myreactorserver.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/myreactorserver.dir/build.make CMakeFiles/myreactorserver.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/myreactorserver.dir/main.cpp.o.provides

CMakeFiles/myreactorserver.dir/main.cpp.o.provides.build: CMakeFiles/myreactorserver.dir/main.cpp.o


CMakeFiles/myreactorserver.dir/myreactor.cpp.o: CMakeFiles/myreactorserver.dir/flags.make
CMakeFiles/myreactorserver.dir/myreactor.cpp.o: ../myreactor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gaoziqiang/CLionProjects/myreactor/myreactor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/myreactorserver.dir/myreactor.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myreactorserver.dir/myreactor.cpp.o -c /home/gaoziqiang/CLionProjects/myreactor/myreactor/myreactor.cpp

CMakeFiles/myreactorserver.dir/myreactor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myreactorserver.dir/myreactor.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gaoziqiang/CLionProjects/myreactor/myreactor/myreactor.cpp > CMakeFiles/myreactorserver.dir/myreactor.cpp.i

CMakeFiles/myreactorserver.dir/myreactor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myreactorserver.dir/myreactor.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gaoziqiang/CLionProjects/myreactor/myreactor/myreactor.cpp -o CMakeFiles/myreactorserver.dir/myreactor.cpp.s

CMakeFiles/myreactorserver.dir/myreactor.cpp.o.requires:

.PHONY : CMakeFiles/myreactorserver.dir/myreactor.cpp.o.requires

CMakeFiles/myreactorserver.dir/myreactor.cpp.o.provides: CMakeFiles/myreactorserver.dir/myreactor.cpp.o.requires
	$(MAKE) -f CMakeFiles/myreactorserver.dir/build.make CMakeFiles/myreactorserver.dir/myreactor.cpp.o.provides.build
.PHONY : CMakeFiles/myreactorserver.dir/myreactor.cpp.o.provides

CMakeFiles/myreactorserver.dir/myreactor.cpp.o.provides.build: CMakeFiles/myreactorserver.dir/myreactor.cpp.o


# Object files for target myreactorserver
myreactorserver_OBJECTS = \
"CMakeFiles/myreactorserver.dir/main.cpp.o" \
"CMakeFiles/myreactorserver.dir/myreactor.cpp.o"

# External object files for target myreactorserver
myreactorserver_EXTERNAL_OBJECTS =

myreactorserver: CMakeFiles/myreactorserver.dir/main.cpp.o
myreactorserver: CMakeFiles/myreactorserver.dir/myreactor.cpp.o
myreactorserver: CMakeFiles/myreactorserver.dir/build.make
myreactorserver: CMakeFiles/myreactorserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gaoziqiang/CLionProjects/myreactor/myreactor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable myreactorserver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myreactorserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myreactorserver.dir/build: myreactorserver

.PHONY : CMakeFiles/myreactorserver.dir/build

CMakeFiles/myreactorserver.dir/requires: CMakeFiles/myreactorserver.dir/main.cpp.o.requires
CMakeFiles/myreactorserver.dir/requires: CMakeFiles/myreactorserver.dir/myreactor.cpp.o.requires

.PHONY : CMakeFiles/myreactorserver.dir/requires

CMakeFiles/myreactorserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myreactorserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myreactorserver.dir/clean

CMakeFiles/myreactorserver.dir/depend:
	cd /home/gaoziqiang/CLionProjects/myreactor/myreactor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gaoziqiang/CLionProjects/myreactor/myreactor /home/gaoziqiang/CLionProjects/myreactor/myreactor /home/gaoziqiang/CLionProjects/myreactor/myreactor/build /home/gaoziqiang/CLionProjects/myreactor/myreactor/build /home/gaoziqiang/CLionProjects/myreactor/myreactor/build/CMakeFiles/myreactorserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myreactorserver.dir/depend

