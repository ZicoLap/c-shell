# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build

# Include any dependencies generated for this target.
include CMakeFiles/vmm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/vmm.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/vmm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vmm.dir/flags.make

CMakeFiles/vmm.dir/src/vmm.c.o: CMakeFiles/vmm.dir/flags.make
CMakeFiles/vmm.dir/src/vmm.c.o: ../src/vmm.c
CMakeFiles/vmm.dir/src/vmm.c.o: CMakeFiles/vmm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/vmm.dir/src/vmm.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vmm.dir/src/vmm.c.o -MF CMakeFiles/vmm.dir/src/vmm.c.o.d -o CMakeFiles/vmm.dir/src/vmm.c.o -c /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/vmm.c

CMakeFiles/vmm.dir/src/vmm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vmm.dir/src/vmm.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/vmm.c > CMakeFiles/vmm.dir/src/vmm.c.i

CMakeFiles/vmm.dir/src/vmm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vmm.dir/src/vmm.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/vmm.c -o CMakeFiles/vmm.dir/src/vmm.c.s

CMakeFiles/vmm.dir/src/main.c.o: CMakeFiles/vmm.dir/flags.make
CMakeFiles/vmm.dir/src/main.c.o: ../src/main.c
CMakeFiles/vmm.dir/src/main.c.o: CMakeFiles/vmm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/vmm.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vmm.dir/src/main.c.o -MF CMakeFiles/vmm.dir/src/main.c.o.d -o CMakeFiles/vmm.dir/src/main.c.o -c /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/main.c

CMakeFiles/vmm.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vmm.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/main.c > CMakeFiles/vmm.dir/src/main.c.i

CMakeFiles/vmm.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vmm.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/main.c -o CMakeFiles/vmm.dir/src/main.c.s

CMakeFiles/vmm.dir/src/output_utility.c.o: CMakeFiles/vmm.dir/flags.make
CMakeFiles/vmm.dir/src/output_utility.c.o: ../src/output_utility.c
CMakeFiles/vmm.dir/src/output_utility.c.o: CMakeFiles/vmm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/vmm.dir/src/output_utility.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/vmm.dir/src/output_utility.c.o -MF CMakeFiles/vmm.dir/src/output_utility.c.o.d -o CMakeFiles/vmm.dir/src/output_utility.c.o -c /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/output_utility.c

CMakeFiles/vmm.dir/src/output_utility.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vmm.dir/src/output_utility.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/output_utility.c > CMakeFiles/vmm.dir/src/output_utility.c.i

CMakeFiles/vmm.dir/src/output_utility.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vmm.dir/src/output_utility.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/src/output_utility.c -o CMakeFiles/vmm.dir/src/output_utility.c.s

# Object files for target vmm
vmm_OBJECTS = \
"CMakeFiles/vmm.dir/src/vmm.c.o" \
"CMakeFiles/vmm.dir/src/main.c.o" \
"CMakeFiles/vmm.dir/src/output_utility.c.o"

# External object files for target vmm
vmm_EXTERNAL_OBJECTS =

vmm: CMakeFiles/vmm.dir/src/vmm.c.o
vmm: CMakeFiles/vmm.dir/src/main.c.o
vmm: CMakeFiles/vmm.dir/src/output_utility.c.o
vmm: CMakeFiles/vmm.dir/build.make
vmm: CMakeFiles/vmm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable vmm"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vmm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vmm.dir/build: vmm
.PHONY : CMakeFiles/vmm.dir/build

CMakeFiles/vmm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vmm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vmm.dir/clean

CMakeFiles/vmm.dir/depend:
	cd /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build /home/zico/Desktop/BS/betriebssysteme_public/hausuebung/hu2/memory/build/CMakeFiles/vmm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vmm.dir/depend

