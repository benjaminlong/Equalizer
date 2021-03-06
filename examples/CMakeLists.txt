# Copyright (c) 2010 Daniel Pfeifer <daniel@pfeifer-mail.de>
#               2010-2013 Stefan Eilemann <eile@eyescale.ch>
#                    2011 Maxim Makhinya <maxmah@gmail.com>

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

if(Equalizer_BINARY_DIR) # in-repository build
  update_file(CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt)

  set(EQUALIZER_LIBRARIES Equalizer)
  set(EQUALIZER_ADMIN_LIBRARY EqualizerAdmin)
  set(EQUALIZER_SERVER_LIBRARY EqualizerServer)
  set(EQUALIZER_SEQUEL_LIBRARY Sequel)
else() # installed examples
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../CMake)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
  find_package(Equalizer @VERSION@ REQUIRED)
  include_directories(${EQUALIZER_INCLUDE_DIRS})
  set(EQ_DEFINITIONS -DEQ_BUILD_DIR="${CMAKE_BINARY_DIR}/" -DEQ_SOURCE_DIR="${CMAKE_SOURCE_DIR}/")
  add_definitions(${EQ_DEFINITIONS})

  include(configure)
  include(CMakeParseArguments)
  set(EQ_AGL_USED @EQ_AGL_USED@)
  set(EQ_GLX_USED @EQ_GLX_USED@)

  if(APPLE)
    include_directories(SYSTEM /usr/X11R6/include)
    set(EQ_EXAMPLES_OSX_ARCHITECTURES "@EQ_EXAMPLES_OSX_ARCHITECTURES@")
  endif(APPLE)
  if(EQ_AGL_USED)
    find_library(CARBON_LIBRARY Carbon)
  endif()
endif()

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES ${EQ_EXAMPLES_OSX_ARCHITECTURES})
  message(STATUS "Building examples for ${CMAKE_OSX_ARCHITECTURES}")

  # filter out -arch settings from environment (MacPorts...)
  set(ALL_OSX_ARCHITECTURES ppc ppc64 i386 x86_64)
  foreach(ARCH ${ALL_OSX_ARCHITECTURES})
    list(FIND CMAKE_OSX_ARCHITECTURES ${ARCH} INDEX)
    if(INDEX EQUAL -1) # Arch not enabled
      string(REGEX REPLACE "-arch ${ARCH}" "" CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS}")
      string(REGEX REPLACE "-arch ${ARCH}" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
      string(REGEX REPLACE "-arch ${ARCH}" "" CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS}")
    endif()
  endforeach()
endif()

include(EqStringifyShaders)

function(EQ_ADD_EXAMPLE NAME)
  cmake_parse_arguments(THIS "" "" "SOURCES;HEADERS;SHADERS;LINK_LIBRARIES"
    ${ARGN})

  eq_stringify_shaders(SHADERS ${THIS_SHADERS})

  set(THIS_TARGET ${NAME})
  set(THIS_INPUT)
  source_group(\\ FILES ${THIS_SOURCES} ${THIS_HEADERS} ${THIS_SHADERS})
  add_executable(${THIS_TARGET} MACOSX_BUNDLE ${THIS_SOURCES} ${THIS_HEADERS}
                 ${SHADERS})
  target_link_libraries(${THIS_TARGET} ${EQUALIZER_LIBRARIES}
    ${THIS_LINK_LIBRARIES} ${OPENGL_gl_LIBRARY})

  set_target_properties(${THIS_TARGET} PROPERTIES OUTPUT_NAME ${NAME}
                        FOLDER "Examples")
  if(APPLE AND AGL) # set preference to 32 bit if AGL is available
    update_file(${CMAKE_SOURCE_DIR}/CMake/MacOSXBundleInfo.plist.in
      ${CMAKE_CURRENT_BINARY_DIR}/MacOSXBundleInfo.plist)
    set_target_properties(${THIS_TARGET} PROPERTIES
      MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/MacOSXBundleInfo.plist)
  endif()
  add_dependencies(${THIS_TARGET} ${EQUALIZER_SERVER_LIBRARY}) # dlopen'ed

  install(TARGETS ${THIS_TARGET} DESTINATION bin COMPONENT apps)
  install(FILES CMakeLists.txt ${THIS_SOURCES} ${THIS_HEADERS} ${THIS_SHADERS}
    DESTINATION share/Equalizer/examples/${NAME} COMPONENT examples)
endfunction(EQ_ADD_EXAMPLE NAME)

include_directories(include)
add_definitions(-DEQ_SYSTEM_INCLUDES) # get GL headers
if(GLEW_MX_FOUND)
  include_directories(BEFORE SYSTEM ${GLEW_MX_INCLUDE_DIRS})
endif()

install(DIRECTORY configs DESTINATION share/Equalizer COMPONENT data
  FILES_MATCHING PATTERN "*.eqc"  PATTERN "issue*" EXCLUDE)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeLists.txt
        DESTINATION share/Equalizer/examples COMPONENT examples)
install(DIRECTORY include DESTINATION share/Equalizer/examples
        COMPONENT examples)
install(FILES ${CMAKE_SOURCE_DIR}/CMake/EqStringifyShaders.cmake
              ${CMAKE_SOURCE_DIR}/CMake/FindGLEW_MX.cmake
              ${CMAKE_SOURCE_DIR}/CMake/FindPackages.cmake
              ${CMAKE_SOURCE_DIR}/CMake/configure.cmake
              DESTINATION share/Equalizer/examples/CMake COMPONENT examples)

if(CUDA_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/eqNBody")
  add_subdirectory(eqNBody)
endif()
if(OSG_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/osgScaleViewer")
  add_subdirectory(osgScaleViewer)
endif()

add_subdirectory(eVolve)
add_subdirectory(eqAsync)
add_subdirectory(eqHello)
add_subdirectory(eqPixelBench)
add_subdirectory(eqPly)
add_subdirectory(seqPly)
