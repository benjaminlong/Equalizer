# generated by Buildyard, do not edit.

include(System)
list(APPEND FIND_PACKAGES_DEFINES ${SYSTEM})

find_package(vmmlib 1.5.1 REQUIRED)
if(vmmlib_FOUND)
  set(vmmlib_name vmmlib)
endif()
if(VMMLIB_FOUND)
  set(vmmlib_name VMMLIB)
endif()
if(vmmlib_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_VMMLIB)
  link_directories(${${vmmlib_name}_LIBRARY_DIRS})
  if(NOT "${${vmmlib_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${vmmlib_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(Lunchbox 1.7.5 REQUIRED)
if(Lunchbox_FOUND)
  set(Lunchbox_name Lunchbox)
endif()
if(LUNCHBOX_FOUND)
  set(Lunchbox_name LUNCHBOX)
endif()
if(Lunchbox_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_LUNCHBOX)
  link_directories(${${Lunchbox_name}_LIBRARY_DIRS})
  if(NOT "${${Lunchbox_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${Lunchbox_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(Collage 0.7.0 REQUIRED)
if(Collage_FOUND)
  set(Collage_name Collage)
endif()
if(COLLAGE_FOUND)
  set(Collage_name COLLAGE)
endif()
if(Collage_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_COLLAGE)
  link_directories(${${Collage_name}_LIBRARY_DIRS})
  if(NOT "${${Collage_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${Collage_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(OpenGL  REQUIRED)
if(OpenGL_FOUND)
  set(OpenGL_name OpenGL)
endif()
if(OPENGL_FOUND)
  set(OpenGL_name OPENGL)
endif()
if(OpenGL_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_OPENGL)
  link_directories(${${OpenGL_name}_LIBRARY_DIRS})
  if(NOT "${${OpenGL_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${OpenGL_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(Boost 1.41.0 REQUIRED program_options)
if(Boost_FOUND)
  set(Boost_name Boost)
endif()
if(BOOST_FOUND)
  set(Boost_name BOOST)
endif()
if(Boost_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_BOOST)
  link_directories(${${Boost_name}_LIBRARY_DIRS})
  if(NOT "SYSTEM ${${Boost_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(SYSTEM ${${Boost_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(X11 )
if(X11_FOUND)
  set(X11_name X11)
endif()
if(X11_FOUND)
  set(X11_name X11)
endif()
if(X11_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_X11)
  link_directories(${${X11_name}_LIBRARY_DIRS})
  if(NOT "${${X11_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${X11_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(hwsd 1.0.0)
if(hwsd_FOUND)
  set(hwsd_name hwsd)
endif()
if(HWSD_FOUND)
  set(hwsd_name HWSD)
endif()
if(hwsd_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_HWSD)
  link_directories(${${hwsd_name}_LIBRARY_DIRS})
  if(NOT "${${hwsd_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${hwsd_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(GLStats 0.1.0)
if(GLStats_FOUND)
  set(GLStats_name GLStats)
endif()
if(GLSTATS_FOUND)
  set(GLStats_name GLSTATS)
endif()
if(GLStats_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_GLSTATS)
  link_directories(${${GLStats_name}_LIBRARY_DIRS})
  if(NOT "${${GLStats_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${GLStats_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(hwloc 1.3)
if(hwloc_FOUND)
  set(hwloc_name hwloc)
endif()
if(HWLOC_FOUND)
  set(hwloc_name HWLOC)
endif()
if(hwloc_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_HWLOC)
  link_directories(${${hwloc_name}_LIBRARY_DIRS})
  if(NOT "${${hwloc_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${hwloc_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(OpenSceneGraph 3.0 COMPONENTS osgDB osgUtil)
if(OpenSceneGraph_FOUND)
  set(OpenSceneGraph_name OpenSceneGraph)
endif()
if(OPENSCENEGRAPH_FOUND)
  set(OpenSceneGraph_name OPENSCENEGRAPH)
endif()
if(OpenSceneGraph_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_OPENSCENEGRAPH)
  link_directories(${${OpenSceneGraph_name}_LIBRARY_DIRS})
  if(NOT "BEFORE SYSTEM ${${OpenSceneGraph_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(BEFORE SYSTEM ${${OpenSceneGraph_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(SAGE 4.0)
if(SAGE_FOUND)
  set(SAGE_name SAGE)
endif()
if(SAGE_FOUND)
  set(SAGE_name SAGE)
endif()
if(SAGE_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_SAGE)
  link_directories(${${SAGE_name}_LIBRARY_DIRS})
  if(NOT "${${SAGE_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${SAGE_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(OpenCV 2.0)
if(OpenCV_FOUND)
  set(OpenCV_name OpenCV)
endif()
if(OPENCV_FOUND)
  set(OpenCV_name OPENCV)
endif()
if(OpenCV_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_OPENCV)
  link_directories(${${OpenCV_name}_LIBRARY_DIRS})
  if(NOT "SYSTEM ${${OpenCV_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(SYSTEM ${${OpenCV_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(PythonInterp )
if(PythonInterp_FOUND)
  set(PythonInterp_name PythonInterp)
endif()
if(PYTHONINTERP_FOUND)
  set(PythonInterp_name PYTHONINTERP)
endif()
if(PythonInterp_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_PYTHONINTERP)
  link_directories(${${PythonInterp_name}_LIBRARY_DIRS})
  if(NOT "${${PythonInterp_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${PythonInterp_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(DisplayCluster 0.2.0)
if(DisplayCluster_FOUND)
  set(DisplayCluster_name DisplayCluster)
endif()
if(DISPLAYCLUSTER_FOUND)
  set(DisplayCluster_name DISPLAYCLUSTER)
endif()
if(DisplayCluster_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_DISPLAYCLUSTER)
  link_directories(${${DisplayCluster_name}_LIBRARY_DIRS})
  if(NOT "${${DisplayCluster_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${DisplayCluster_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(MAGELLAN )
if(MAGELLAN_FOUND)
  set(MAGELLAN_name MAGELLAN)
endif()
if(MAGELLAN_FOUND)
  set(MAGELLAN_name MAGELLAN)
endif()
if(MAGELLAN_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_MAGELLAN)
  link_directories(${${MAGELLAN_name}_LIBRARY_DIRS})
  if(NOT "${${MAGELLAN_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${MAGELLAN_name}_INCLUDE_DIRS})
  endif()
endif()

find_package(GLEW_MX )
if(GLEW_MX_FOUND)
  set(GLEW_MX_name GLEW_MX)
endif()
if(GLEW_MX_FOUND)
  set(GLEW_MX_name GLEW_MX)
endif()
if(GLEW_MX_name)
  list(APPEND FIND_PACKAGES_DEFINES EQUALIZER_USE_GLEW_MX)
  link_directories(${${GLEW_MX_name}_LIBRARY_DIRS})
  if(NOT "${${GLEW_MX_name}_INCLUDE_DIRS}" MATCHES "-NOTFOUND")
    include_directories(${${GLEW_MX_name}_INCLUDE_DIRS})
  endif()
endif()


if(EXISTS ${CMAKE_SOURCE_DIR}/CMake/FindPackagesPost.cmake)
  include(${CMAKE_SOURCE_DIR}/CMake/FindPackagesPost.cmake)
endif()

set(EQUALIZER_BUILD_DEBS bison;flex;freeglut3-dev;libavahi-compat-libdnssd-dev;libavcodec-dev;libavformat-dev;libavutil-dev;libboost-date-time-dev;libboost-program-options-dev;libboost-regex-dev;libboost-serialization-dev;libboost-system-dev;libgl1-mesa-dev;libglewmx1.6-dev;libhwloc-dev;libibverbs-dev;libjpeg-turbo8-dev;libopencv-dev;libopenmpi-dev;libopenscenegraph-dev;libqt4-dev;librdmacm-dev;libspnav-dev;libswscale-dev;libturbojpeg;libudt-dev;libx11-dev;libxmu-dev)

set(EQUALIZER_DEPENDS vmmlib;Lunchbox;Collage;OpenGL;Boost;X11;hwsd;GLStats;hwloc;OpenSceneGraph;SAGE;OpenCV;PythonInterp;DisplayCluster;MAGELLAN;GLEW_MX)

# Write defines.h and options.cmake
if(NOT PROJECT_INCLUDE_NAME)
  set(PROJECT_INCLUDE_NAME ${CMAKE_PROJECT_NAME})
endif()
if(NOT OPTIONS_CMAKE)
  set(OPTIONS_CMAKE ${CMAKE_BINARY_DIR}/options.cmake)
endif()
set(DEFINES_FILE "${CMAKE_BINARY_DIR}/include/${PROJECT_INCLUDE_NAME}/defines${SYSTEM}.h")
set(DEFINES_FILE_IN ${DEFINES_FILE}.in)
file(WRITE ${DEFINES_FILE_IN}
  "// generated by CMake/FindPackages.cmake, do not edit.\n\n"
  "#ifndef ${CMAKE_PROJECT_NAME}_DEFINES_${SYSTEM}_H\n"
  "#define ${CMAKE_PROJECT_NAME}_DEFINES_${SYSTEM}_H\n\n")
file(WRITE ${OPTIONS_CMAKE} "# Optional modules enabled during build\n")
foreach(DEF ${FIND_PACKAGES_DEFINES})
  add_definitions(-D${DEF})
  file(APPEND ${DEFINES_FILE_IN}
  "#ifndef ${DEF}\n"
  "#  define ${DEF}\n"
  "#endif\n")
if(NOT DEF STREQUAL SYSTEM)
  file(APPEND ${OPTIONS_CMAKE} "set(${DEF} ON)\n")
endif()
endforeach()
file(APPEND ${DEFINES_FILE_IN}
  "\n#endif\n")

include(UpdateFile)
update_file(${DEFINES_FILE_IN} ${DEFINES_FILE})
