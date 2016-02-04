cmake_minimum_required(VERSION 3.0)

# Sanity check
set(expected_nonempty_vars PROJECT_NAME GIT_REPOSITORY GIT_TAG SOURCE_DIR)
foreach(varname ${expected_nonempty_vars})
  if("${${varname}}" STREQUAL "")
    message(FATAL_ERROR "Variable ${varname} is set to an empty string")
  endif()
endforeach()

project(${PROJECT_NAME} LANGUAGES NONE)

include(CMakeParseArguments)
include(ExternalProject)

message(STATUS "  GIT_REPOSITORY: ${GIT_REPOSITORY}")
message(STATUS "  GIT_TAG: ${GIT_TAG}")
message(STATUS "  SOURCE_DIR: ${SOURCE_DIR}")

ExternalProject_Add(${PROJECT_NAME}DownloadSource
  GIT_REPOSITORY ${GIT_REPOSITORY}
  GIT_TAG ${GIT_TAG}
  SOURCE_DIR ${SOURCE_DIR}
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

