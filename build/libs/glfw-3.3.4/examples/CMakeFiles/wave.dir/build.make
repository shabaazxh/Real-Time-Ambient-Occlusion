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
include libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/compiler_depend.make

# Include the progress variables for this target.
include libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/progress.make

# Include the compile flags for this target's objects.
include libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/flags.make

libs/glfw-3.3.4/examples/wave.app/Contents/Resources/glfw.icns: ../libs/glfw-3.3.4/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content libs/glfw-3.3.4/examples/wave.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/examples/glfw.icns libs/glfw-3.3.4/examples/wave.app/Contents/Resources/glfw.icns

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/flags.make
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o: ../libs/glfw-3.3.4/examples/wave.c
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shabaaz/Desktop/Vulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o -MF CMakeFiles/wave.dir/wave.c.o.d -o CMakeFiles/wave.dir/wave.c.o -c /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/examples/wave.c

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wave.dir/wave.c.i"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/examples/wave.c > CMakeFiles/wave.dir/wave.c.i

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wave.dir/wave.c.s"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/examples/wave.c -o CMakeFiles/wave.dir/wave.c.s

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/flags.make
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o: ../libs/glfw-3.3.4/deps/glad_gl.c
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/shabaaz/Desktop/Vulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o -MF CMakeFiles/wave.dir/__/deps/glad_gl.c.o.d -o CMakeFiles/wave.dir/__/deps/glad_gl.c.o -c /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/deps/glad_gl.c

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wave.dir/__/deps/glad_gl.c.i"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/deps/glad_gl.c > CMakeFiles/wave.dir/__/deps/glad_gl.c.i

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wave.dir/__/deps/glad_gl.c.s"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/deps/glad_gl.c -o CMakeFiles/wave.dir/__/deps/glad_gl.c.s

# Object files for target wave
wave_OBJECTS = \
"CMakeFiles/wave.dir/wave.c.o" \
"CMakeFiles/wave.dir/__/deps/glad_gl.c.o"

# External object files for target wave
wave_EXTERNAL_OBJECTS =

libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/wave.c.o
libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/__/deps/glad_gl.c.o
libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/build.make
libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave: libs/glfw-3.3.4/src/libglfw3.a
libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave: libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/shabaaz/Desktop/Vulkan/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable wave.app/Contents/MacOS/wave"
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wave.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/build: libs/glfw-3.3.4/examples/wave.app/Contents/MacOS/wave
libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/build: libs/glfw-3.3.4/examples/wave.app/Contents/Resources/glfw.icns
.PHONY : libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/build

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/clean:
	cd /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples && $(CMAKE_COMMAND) -P CMakeFiles/wave.dir/cmake_clean.cmake
.PHONY : libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/clean

libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/depend:
	cd /Users/shabaaz/Desktop/Vulkan/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shabaaz/Desktop/Vulkan /Users/shabaaz/Desktop/Vulkan/libs/glfw-3.3.4/examples /Users/shabaaz/Desktop/Vulkan/build /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples /Users/shabaaz/Desktop/Vulkan/build/libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libs/glfw-3.3.4/examples/CMakeFiles/wave.dir/depend
