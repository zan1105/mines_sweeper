# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\mines_sweeper_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mines_sweeper_autogen.dir\\ParseCache.txt"
  "mines_sweeper_autogen"
  )
endif()
