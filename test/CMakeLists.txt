
# Sets the current target name
set(TARGET_NAME "testcase")

# Add source files in current directory and subdirectories
file(GLOB_RECURSE sources CONFIGURE_DEPENDS *.c *.cpp *.h)

# Append to the top-level build target
target_sources(Cbox-test PUBLIC ${sources})
