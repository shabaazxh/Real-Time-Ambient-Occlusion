# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.20.5/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.20.5/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shabaaz/Desktop/Vulkan

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shabaaz/Desktop/Vulkan/build

# Include any dependencies generated for this target.
include libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/progress.make

# Include the compile flags for this target's objects.
include libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/flags.make

libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o: libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/flags.make
libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o: ../libs/tinyobjloader/tiny_obj_loader.cc
libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o: libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shabaaz/Desktop/Vulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o -MF CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o.d -o CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o -c /Users/shabaaz/Desktop/Vulkan/libs/tinyobjloader/tiny_obj_loader.cc

libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.i"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shabaaz/Desktop/Vulkan/libs/tinyobjloader/tiny_obj_loader.cc > CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.i

libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.s"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && /usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shabaaz/Desktop/Vulkan/libs/tinyobjloader/tiny_obj_loader.cc -o CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.s

# Object files for target tinyobjloader
tinyobjloader_OBJECTS = \
"CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o"

# External object files for target tinyobjloader
tinyobjloader_EXTERNAL_OBJECTS =

libs/tinyobjloader/libtinyobjloader.a: libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/tiny_obj_loader.cc.o
libs/tinyobjloader/libtinyobjloader.a: libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/build.make
libs/tinyobjloader/libtinyobjloader.a: libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/shabaaz/Desktop/Vulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtinyobjloader.a"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && $(CMAKE_COMMAND) -P CMakeFiles/tinyobjloader.dir/cmake_clean_target.cmake
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinyobjloader.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/build: libs/tinyobjloader/libtinyobjloader.a
.PHONY : libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/build

libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/clean:
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader && $(CMAKE_COMMAND) -P CMakeFiles/tinyobjloader.dir/cmake_clean.cmake
.PHONY : libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/clean

libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/depend:
	cd /Users/shabaaz/Desktop/Vulkan/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shabaaz/Desktop/Vulkan /Users/shabaaz/Desktop/Vulkan/libs/tinyobjloader /Users/shabaaz/Desktop/Vulkan/build /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader /Users/shabaaz/Desktop/Vulkan/build/libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/tinyobjloader/CMakeFiles/tinyobjloader.dir/depend
