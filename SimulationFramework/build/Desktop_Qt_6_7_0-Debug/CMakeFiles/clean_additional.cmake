# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SimulationFramework_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SimulationFramework_autogen.dir/ParseCache.txt"
  "SimulationFramework_autogen"
  "tests/CMakeFiles/SimulationFramework-Tests_autogen.dir/AutogenUsed.txt"
  "tests/CMakeFiles/SimulationFramework-Tests_autogen.dir/ParseCache.txt"
  "tests/SimulationFramework-Tests_autogen"
  )
endif()
