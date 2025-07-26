project(example)

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp src/terminal.cpp)

# Add header files
target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include/
)

# Link the static library from lib
# using its alias raw_term::lib.
# This will cause the include directories
# for that target to be added to this project.
target_link_libraries(${PROJECT_NAME} raw_term::lib)
