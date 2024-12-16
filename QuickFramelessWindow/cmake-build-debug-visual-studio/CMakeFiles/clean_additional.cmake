# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\FramelessWindow_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\FramelessWindow_autogen.dir\\ParseCache.txt"
  "FramelessWindow_autogen"
  )
endif()
