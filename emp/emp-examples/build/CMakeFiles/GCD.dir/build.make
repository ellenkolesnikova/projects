# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.27.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.27.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/Ellen/Desktop/coding/emp/emp-examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/Ellen/Desktop/coding/emp/emp-examples/build

# Include any dependencies generated for this target.
include CMakeFiles/GCD.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/GCD.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/GCD.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/GCD.dir/flags.make

CMakeFiles/GCD.dir/src/gcd.cc.o: CMakeFiles/GCD.dir/flags.make
CMakeFiles/GCD.dir/src/gcd.cc.o: /Users/Ellen/Desktop/coding/emp/emp-examples/src/gcd.cc
CMakeFiles/GCD.dir/src/gcd.cc.o: CMakeFiles/GCD.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/Ellen/Desktop/coding/emp/emp-examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/GCD.dir/src/gcd.cc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/GCD.dir/src/gcd.cc.o -MF CMakeFiles/GCD.dir/src/gcd.cc.o.d -o CMakeFiles/GCD.dir/src/gcd.cc.o -c /Users/Ellen/Desktop/coding/emp/emp-examples/src/gcd.cc

CMakeFiles/GCD.dir/src/gcd.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GCD.dir/src/gcd.cc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/Ellen/Desktop/coding/emp/emp-examples/src/gcd.cc > CMakeFiles/GCD.dir/src/gcd.cc.i

CMakeFiles/GCD.dir/src/gcd.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GCD.dir/src/gcd.cc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/Ellen/Desktop/coding/emp/emp-examples/src/gcd.cc -o CMakeFiles/GCD.dir/src/gcd.cc.s

# Object files for target GCD
GCD_OBJECTS = \
"CMakeFiles/GCD.dir/src/gcd.cc.o"

# External object files for target GCD
GCD_EXTERNAL_OBJECTS =

bin/GCD: CMakeFiles/GCD.dir/src/gcd.cc.o
bin/GCD: CMakeFiles/GCD.dir/build.make
bin/GCD: /usr/local/lib/libemp-tool.dylib
bin/GCD: /usr/local/opt/openssl/lib/libssl.dylib
bin/GCD: /usr/local/opt/openssl/lib/libcrypto.dylib
bin/GCD: CMakeFiles/GCD.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/Ellen/Desktop/coding/emp/emp-examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/GCD"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GCD.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/GCD.dir/build: bin/GCD
.PHONY : CMakeFiles/GCD.dir/build

CMakeFiles/GCD.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/GCD.dir/cmake_clean.cmake
.PHONY : CMakeFiles/GCD.dir/clean

CMakeFiles/GCD.dir/depend:
	cd /Users/Ellen/Desktop/coding/emp/emp-examples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/Ellen/Desktop/coding/emp/emp-examples /Users/Ellen/Desktop/coding/emp/emp-examples /Users/Ellen/Desktop/coding/emp/emp-examples/build /Users/Ellen/Desktop/coding/emp/emp-examples/build /Users/Ellen/Desktop/coding/emp/emp-examples/build/CMakeFiles/GCD.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/GCD.dir/depend

