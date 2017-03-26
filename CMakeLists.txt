
# CMakeLists.txt, generated gencmake.pl, on 2014/12/14 13:45:37
cmake_minimum_required( VERSION 2.8.8 )

# CMakeScripts or use the ones that come by default with CMake.
# set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules ${CMAKE_MODULE_PATH})

project( dirent )

# The version number.
set( dirent_MAJOR 1 )
set( dirent_MINOR 0 )
set( dirent_POINT 0 )


# Allow developer to select is Dynamic or static library built
set( LIB_TYPE STATIC )  # set default static
option( BUILD_SHARED_LIB "Build Shared Library" OFF )
# EXPERIMENTAL - Turn ON to combine library into EXE - above shared library MUST be OFF
option( BUILD_AS_ONE "Build as one. Combine library into EXE" OFF )


if(CMAKE_COMPILER_IS_GNUCXX)
    set( WARNING_FLAGS -Wall )
endif(CMAKE_COMPILER_IS_GNUCXX)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang") 
   set( WARNING_FLAGS "-Wall -Wno-overloaded-virtual" )
endif() 

if(WIN32 AND MSVC)
    # turn off various warnings - none needed in this compile
    set(WARNING_FLAGS "${WARNING_FLAGS} /wd4996")
    # foreach(warning 4244 4251 4267 4275 4290 4786 4305)
    #     set(WARNING_FLAGS "${WARNING_FLAGS} /wd${warning}")
    # endforeach(warning)
    set( MSVC_FLAGS "-DNOMINMAX -D_USE_MATH_DEFINES -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D__CRT_NONSTDC_NO_WARNINGS" )
    # if (${MSVC_VERSION} EQUAL 1600)
    #    set( MSVC_LD_FLAGS "/FORCE:MULTIPLE" )
    # endif (${MSVC_VERSION} EQUAL 1600)
    #set( NOMINMAX 1 )
    # to distinguish between debug and release lib
    set( CMAKE_DEBUG_POSTFIX "d" )
else()
    # items for unix
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARNING_FLAGS} ${MSVC_FLAGS} -D_REENTRANT" )
set( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MSVC_LD_FLAGS}" )

# configuration file, if needed
# configure_file( ${CMAKE_SOURCE_DIR}/config.h.cmake ${CMAKE_BINARY_DIR}/config.h )
# add_definitions( -DHAVE_CONFIG_H )
# include_directories( ${CMAKE_BINARY_DIR} )

if(BUILD_SHARED_LIB)
   set(LIB_TYPE SHARED)
   message(STATUS "*** Building DLL library ${LIB_TYPE}")
else()
   message(STATUS "*** Option BUILD_SHARED_LIB is OFF ${LIB_TYPE}")
endif()
#################################################
##### LIBRARIES #####
#################################################

# dirent LIBRARY from [X:\dirent\src],
# have 1 C/C++ sources, 1 headers
set(name dirent)
set(dir src)
set(${name}_SRCS
    ${dir}/dirent.c
    )
set(${name}_HDRS
    ${dir}/dirent.h
    )
add_library( ${name} ${LIB_TYPE} ${${name}_SRCS} ${${name}_HDRS} )
# deal with install, if any...
install( TARGETS ${name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib )
install(FILES ${${name}_HDRS} DESTINATION include)
list(APPEND add_LIBS ${name})

# just a 'test' exe
set(name test-dir)
add_executable( ${name} ${dir}/${name}.cxx )
target_link_libraries( ${name} ${add_LIBS} )
if (MSVC)
    set_target_properties( ${name} PROPERTIES DEBUG_POSTFIX d )
endif ()
# deal with install, if any...
# install( TARGETS ${name} DESTINATION bin )

# eof
