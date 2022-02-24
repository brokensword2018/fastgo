cmake_minimum_required (VERSION 3.11)

# 设置三方库
set(BUILD_SHARED_LIBS OFF)
set(CURL_ENABLE_SSL OFF)
add_subdirectory(third_party/jsoncpp)
INCLUDE_DIRECTORIES(third_party/jsoncpp/include)
add_subdirectory(third_party/curl)
INCLUDE_DIRECTORIES(third_party/curl/include)
link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  #GIT_REPOSITORY https://github.com/brokensword2018/FTXUI.git
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v0.11
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")

project(fastgo
  LANGUAGES CXX
  VERSION 1.0.0
)
aux_source_directory(./src SRCS)
add_executable(fastgo ${SRCS})
target_include_directories(fastgo PRIVATE src)

target_link_libraries(fastgo
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
  libjsoncpp.a
  libcurl.a
  -lz
)

install(TARGETS fastgo RUNTIME DESTINATION "bin")
