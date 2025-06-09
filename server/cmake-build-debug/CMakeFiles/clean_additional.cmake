# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/omniserver_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/omniserver_autogen.dir/ParseCache.txt"
  "omniserver_autogen"
  )
endif()
