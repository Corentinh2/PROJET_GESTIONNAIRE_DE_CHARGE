# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appPROJET_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appPROJET_autogen.dir/ParseCache.txt"
  "appPROJET_autogen"
  "tst_communicationesp/CMakeFiles/TestCommunicationEsp_autogen.dir/AutogenUsed.txt"
  "tst_communicationesp/CMakeFiles/TestCommunicationEsp_autogen.dir/ParseCache.txt"
  "tst_communicationesp/TestCommunicationEsp_autogen"
  )
endif()
