# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitclone-lastrun.txt" AND EXISTS "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitinfo.txt" AND
  "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/opt/homebrew/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/SFML/SFML.git" "sfml-src"
    WORKING_DIRECTORY "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/SFML/SFML.git'")
endif()

execute_process(
  COMMAND "/opt/homebrew/bin/git"
          checkout "2.6.x" --
  WORKING_DIRECTORY "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '2.6.x'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/opt/homebrew/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitinfo.txt" "/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/sebastiandion/CLionProjects/FinalProject/cmake-build-debug/_deps/sfml-subbuild/sfml-populate-prefix/src/sfml-populate-stamp/sfml-populate-gitclone-lastrun.txt'")
endif()
