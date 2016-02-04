##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

##############################################################################
# Do not edit. To add a module see TubeTKModules.cmake.
##############################################################################
cmake_minimum_required( VERSION 3.0.0 )
project( ITKModules )

# Including module 'ExternalProjectsConfig' set the
# following variables:
#
#   TubeTK_ITK_MODULES
#   <module>_URL
#   <module>_HASH_OR_TAG
#
include( ${TubeTK_SOURCE_DIR}/CMake/Superbuild/ExternalProjectsConfig.cmake )

# External Module is supported only for build tree of ITK >= 4.9
set( ITK_WITH_EXTERNAL_MODULE_SUPPORT 1 )
if(NOT ITK_VERSION VERSION_GREATER "4.8.1")
  #message(STATUS "ITKModules: Building ITK Modules externaly requires ITK 4.9 or later")
  set( ITK_WITH_EXTERNAL_MODULE_SUPPORT 0 )
endif()
if(NOT EXISTS ${ITK_CMAKE_DIR}/ITKModuleMacros.cmake)
  #message(STATUS "ITK Modules cannot be built against an ITK install tree")
  set( ITK_WITH_EXTERNAL_MODULE_SUPPORT 0 )
endif()

if(NOT ITK_WITH_EXTERNAL_MODULE_SUPPORT)
  message(STATUS "ITKModules: 'ITK External Module' not supported.")
endif()


foreach( itk-module ${TubeTK_ITK_MODULES} )

  set( ${itk-module}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${itk-module} )

  if( NOT EXISTS "${${itk-module}_URL}")

    message(STATUS "Downloading ${itk-module}")

    # Create build directory
    set(_download_project_dir ${CMAKE_CURRENT_BINARY_DIR}/${itk-module}-download-project)
    file(MAKE_DIRECTORY ${_download_project_dir})

    # Configure
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        -G ${CMAKE_GENERATOR}
        -DPROJECT_NAME=${itk-module}
        -DGIT_REPOSITORY=${${itk-module}_URL}
        -DGIT_TAG=${${itk-module}_HASH_OR_TAG}
        -DSOURCE_DIR=${${itk-module}_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/DownloadSource
      WORKING_DIRECTORY ${_download_project_dir}
      OUTPUT_QUIET
      RESULT_VARIABLE _result )
    if(NOT _result EQUAL 0)
      message(FATAL_ERROR "Failed to configure download project: ${itk-module}")
    endif()

    # Build
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        --build ${_download_project_dir}
        --config Release
      WORKING_DIRECTORY ${_download_project_dir}
      OUTPUT_QUIET
      RESULT_VARIABLE _result )
    if(NOT _result EQUAL 0)
      message(FATAL_ERROR "Failed to build download project: ${itk-module}")
    endif()

  endif()

  if( ITK_WITH_EXTERNAL_MODULE_SUPPORT )

    message(STATUS "Adding '${itk-module}' directory")

    # Adding the ITK module source directory will create
    # a target only if there is a "src" directory.
    add_subdirectory(${${itk-module}_SOURCE_DIR})

  endif()

  # Here we add a target only if needed. This could happen
  # in two scenarios:
  #
  # (1) The ITK module directory has been added above but it
  #     does NOT have a src folder.
  #
  # (2) ITK_DIR used to build TubeTK doesn't support External Module.
  #

  set( _module_type PUBLIC )
  if(NOT TARGET ${itk-module})

    if( EXISTS ${${itk-module}_SOURCE_DIR}/src )

      find_package( ITK REQUIRED )
      include( ${ITK_USE_FILE} )

      # Note: Using GLOB_RECURSE allows to create the
      # to lookup source files,

      file( GLOB_RECURSE _module_H_Files
        ${${itk-module}_SOURCE_DIR}/include/*.h )

      file( GLOB_RECURSE _module_HXX_Files
	${${itk-module}_SOURCE_DIR}/include/*.hxx )

      file( GLOB_RECURSE _module_CXX_Files
        ${${itk-module}_SOURCE_DIR}/src/*.cxx )

      add_library( ${itk-module} STATIC
        ${_module_H_Files}
        ${_module_HXX_Files}
        ${_module_CXX_Files} )
      target_link_libraries( ${itk-module} ${ITK_LIBRARIES} )

      # Generate the export macro header for symbol visibility/Windows DLL declspec
      include(GenerateExportHeader)
      set(_export_header_file "${${itk-module}_BINARY_DIR}/include/${itk-module}Export.h")
      generate_export_header(${itk-module}
	EXPORT_FILE_NAME ${_export_header_file}
	EXPORT_MACRO_NAME ${itk-module}_EXPORT
	#TEMPLATE_EXPORT_MACRO_NAME ${itk-module}_TEMPLATE_EXPORT
	NO_EXPORT_MACRO_NAME ${itk-module}_HIDDEN
	STATIC_DEFINE ITK_STATIC )

    else()

      add_library( ${itk-module} INTERFACE )

      set( _module_type INTERFACE )

    endif()

    TubeTKMacroInstallPlugins( ${itk-module} )

  endif()

  message("${itk-module}_BINARY_DIR:${${itk-module}_BINARY_DIR}")

  target_include_directories( ${itk-module} ${_module_type}
    ${${itk-module}_SOURCE_DIR}/include
    ${${itk-module}_BINARY_DIR}/include )

endforeach()

