# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/2.8.12.1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/2.8.12.1/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/local/Cellar/cmake/2.8.7/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sweaver/Programming/hyphy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sweaver/Programming/hyphy

# Include any dependencies generated for this target.
include contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/depend.make

# Include the progress variables for this target.
include contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/flags.make

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/flags.make
contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o: contrib/gtest-1.7.0/src/gtest_main.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/sweaver/Programming/hyphy/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o"
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gtest_main.dir/src/gtest_main.cc.o -c /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0/src/gtest_main.cc

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gtest_main.dir/src/gtest_main.cc.i"
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0/src/gtest_main.cc > CMakeFiles/gtest_main.dir/src/gtest_main.cc.i

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gtest_main.dir/src/gtest_main.cc.s"
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0/src/gtest_main.cc -o CMakeFiles/gtest_main.dir/src/gtest_main.cc.s

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires:
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires
	$(MAKE) -f contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/build.make contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides.build
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.provides.build: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o

# Object files for target gtest_main
gtest_main_OBJECTS = \
"CMakeFiles/gtest_main.dir/src/gtest_main.cc.o"

# External object files for target gtest_main
gtest_main_EXTERNAL_OBJECTS =

contrib/gtest-1.7.0/libgtest_main.a: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o
contrib/gtest-1.7.0/libgtest_main.a: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/build.make
contrib/gtest-1.7.0/libgtest_main.a: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libgtest_main.a"
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && $(CMAKE_COMMAND) -P CMakeFiles/gtest_main.dir/cmake_clean_target.cmake
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gtest_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/build: contrib/gtest-1.7.0/libgtest_main.a
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/build

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/requires: contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/src/gtest_main.cc.o.requires
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/requires

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/clean:
	cd /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 && $(CMAKE_COMMAND) -P CMakeFiles/gtest_main.dir/cmake_clean.cmake
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/clean

contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/depend:
	cd /Users/sweaver/Programming/hyphy && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sweaver/Programming/hyphy /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 /Users/sweaver/Programming/hyphy /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0 /Users/sweaver/Programming/hyphy/contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : contrib/gtest-1.7.0/CMakeFiles/gtest_main.dir/depend

