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
CMAKE_SOURCE_DIR = /home/user/cpp/chatServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/cpp/chatServer/build

# Include any dependencies generated for this target.
include src/server/CMakeFiles/chatserver.dir/depend.make

# Include the progress variables for this target.
include src/server/CMakeFiles/chatserver.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/CMakeFiles/chatserver.dir/flags.make

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o: src/server/CMakeFiles/chatserver.dir/flags.make
src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o: ../src/server/chatserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/cpp/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatserver.dir/chatserver.cpp.o -c /home/user/cpp/chatServer/src/server/chatserver.cpp

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatserver.dir/chatserver.cpp.i"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/cpp/chatServer/src/server/chatserver.cpp > CMakeFiles/chatserver.dir/chatserver.cpp.i

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatserver.dir/chatserver.cpp.s"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/cpp/chatServer/src/server/chatserver.cpp -o CMakeFiles/chatserver.dir/chatserver.cpp.s

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.requires:

.PHONY : src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.requires

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.provides: src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.requires
	$(MAKE) -f src/server/CMakeFiles/chatserver.dir/build.make src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.provides.build
.PHONY : src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.provides

src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.provides.build: src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o


src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o: src/server/CMakeFiles/chatserver.dir/flags.make
src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o: ../src/server/chatservice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/cpp/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatserver.dir/chatservice.cpp.o -c /home/user/cpp/chatServer/src/server/chatservice.cpp

src/server/CMakeFiles/chatserver.dir/chatservice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatserver.dir/chatservice.cpp.i"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/cpp/chatServer/src/server/chatservice.cpp > CMakeFiles/chatserver.dir/chatservice.cpp.i

src/server/CMakeFiles/chatserver.dir/chatservice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatserver.dir/chatservice.cpp.s"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/cpp/chatServer/src/server/chatservice.cpp -o CMakeFiles/chatserver.dir/chatservice.cpp.s

src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.requires:

.PHONY : src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.requires

src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.provides: src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.requires
	$(MAKE) -f src/server/CMakeFiles/chatserver.dir/build.make src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.provides.build
.PHONY : src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.provides

src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.provides.build: src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o


src/server/CMakeFiles/chatserver.dir/main.cpp.o: src/server/CMakeFiles/chatserver.dir/flags.make
src/server/CMakeFiles/chatserver.dir/main.cpp.o: ../src/server/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/cpp/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/server/CMakeFiles/chatserver.dir/main.cpp.o"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chatserver.dir/main.cpp.o -c /home/user/cpp/chatServer/src/server/main.cpp

src/server/CMakeFiles/chatserver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chatserver.dir/main.cpp.i"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/cpp/chatServer/src/server/main.cpp > CMakeFiles/chatserver.dir/main.cpp.i

src/server/CMakeFiles/chatserver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chatserver.dir/main.cpp.s"
	cd /home/user/cpp/chatServer/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/cpp/chatServer/src/server/main.cpp -o CMakeFiles/chatserver.dir/main.cpp.s

src/server/CMakeFiles/chatserver.dir/main.cpp.o.requires:

.PHONY : src/server/CMakeFiles/chatserver.dir/main.cpp.o.requires

src/server/CMakeFiles/chatserver.dir/main.cpp.o.provides: src/server/CMakeFiles/chatserver.dir/main.cpp.o.requires
	$(MAKE) -f src/server/CMakeFiles/chatserver.dir/build.make src/server/CMakeFiles/chatserver.dir/main.cpp.o.provides.build
.PHONY : src/server/CMakeFiles/chatserver.dir/main.cpp.o.provides

src/server/CMakeFiles/chatserver.dir/main.cpp.o.provides.build: src/server/CMakeFiles/chatserver.dir/main.cpp.o


# Object files for target chatserver
chatserver_OBJECTS = \
"CMakeFiles/chatserver.dir/chatserver.cpp.o" \
"CMakeFiles/chatserver.dir/chatservice.cpp.o" \
"CMakeFiles/chatserver.dir/main.cpp.o"

# External object files for target chatserver
chatserver_EXTERNAL_OBJECTS =

../bin/chatserver: src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o
../bin/chatserver: src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o
../bin/chatserver: src/server/CMakeFiles/chatserver.dir/main.cpp.o
../bin/chatserver: src/server/CMakeFiles/chatserver.dir/build.make
../bin/chatserver: src/server/CMakeFiles/chatserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/cpp/chatServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../../bin/chatserver"
	cd /home/user/cpp/chatServer/build/src/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chatserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/CMakeFiles/chatserver.dir/build: ../bin/chatserver

.PHONY : src/server/CMakeFiles/chatserver.dir/build

src/server/CMakeFiles/chatserver.dir/requires: src/server/CMakeFiles/chatserver.dir/chatserver.cpp.o.requires
src/server/CMakeFiles/chatserver.dir/requires: src/server/CMakeFiles/chatserver.dir/chatservice.cpp.o.requires
src/server/CMakeFiles/chatserver.dir/requires: src/server/CMakeFiles/chatserver.dir/main.cpp.o.requires

.PHONY : src/server/CMakeFiles/chatserver.dir/requires

src/server/CMakeFiles/chatserver.dir/clean:
	cd /home/user/cpp/chatServer/build/src/server && $(CMAKE_COMMAND) -P CMakeFiles/chatserver.dir/cmake_clean.cmake
.PHONY : src/server/CMakeFiles/chatserver.dir/clean

src/server/CMakeFiles/chatserver.dir/depend:
	cd /home/user/cpp/chatServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/cpp/chatServer /home/user/cpp/chatServer/src/server /home/user/cpp/chatServer/build /home/user/cpp/chatServer/build/src/server /home/user/cpp/chatServer/build/src/server/CMakeFiles/chatserver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/server/CMakeFiles/chatserver.dir/depend
