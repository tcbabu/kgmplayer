# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/babu/BUILD/kgmplayer-4.3.0/x265/source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/babu/BUILD/kgmplayer-4.3.0/x265/build

# Utility rule file for clean-generated.

# Include the progress variables for this target.
include CMakeFiles/clean-generated.dir/progress.make

CMakeFiles/clean-generated:
	/usr/bin/cmake -P /home/babu/BUILD/kgmplayer-4.3.0/x265/source/cmake/clean-generated.cmake

clean-generated: CMakeFiles/clean-generated
clean-generated: CMakeFiles/clean-generated.dir/build.make

.PHONY : clean-generated

# Rule to build all files generated by this target.
CMakeFiles/clean-generated.dir/build: clean-generated

.PHONY : CMakeFiles/clean-generated.dir/build

CMakeFiles/clean-generated.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clean-generated.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clean-generated.dir/clean

CMakeFiles/clean-generated.dir/depend:
	cd /home/babu/BUILD/kgmplayer-4.3.0/x265/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/babu/BUILD/kgmplayer-4.3.0/x265/source /home/babu/BUILD/kgmplayer-4.3.0/x265/source /home/babu/BUILD/kgmplayer-4.3.0/x265/build /home/babu/BUILD/kgmplayer-4.3.0/x265/build /home/babu/BUILD/kgmplayer-4.3.0/x265/build/CMakeFiles/clean-generated.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clean-generated.dir/depend
