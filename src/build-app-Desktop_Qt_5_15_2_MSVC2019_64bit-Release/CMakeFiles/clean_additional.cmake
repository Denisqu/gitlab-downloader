# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\gitlab-downloader_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\gitlab-downloader_autogen.dir\\ParseCache.txt"
  "gitlab-downloader_autogen"
  )
endif()
