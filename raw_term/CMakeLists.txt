project(raw_term)

add_library(${PROJECT_NAME} src/raw_term.cpp src/logging.cpp)
add_library(raw_term::lib ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
  PUBLIC ${PROJECT_SOURCE_DIR}/include
)
