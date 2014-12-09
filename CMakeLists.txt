cmake_minimum_required(VERSION 2.8)
include(ExternalProject)

project( AR_test )

# set the path where we can find the findXXX.cmake
SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/.cmake)

# set the search path for OCV - it could be set also running ccmake
# or giving the  -DOpenCV_DIR="..." option in cmake
# it is where the OpenCVConfig.cmake can be found 
if(NOT DEFINED OpenCV_DIR)
    # this is quite a wild guess...
    #set( OpenCV_DIR "/usr/share/OpenCV")
    if(UNIX)
        execute_process(COMMAND locate share/OpenCV/OpenCVConfig.cmake COMMAND grep -v ^$ OUTPUT_VARIABLE OUTPUT_LOCATE)
        # message( STATUS "OUTPUT_LOCATE= ${OUTPUT_LOCATE}" )
        string(REPLACE "\n" ";" OUTPUT_LOCATE ${OUTPUT_LOCATE})
        # message( STATUS "OUTPUT_LOCATE= ${OUTPUT_LOCATE}" )
        list(LENGTH OUTPUT_LOCATE RES)
        # message( STATUS "RES= ${RES}" )
        # there is always a blank line
        if(RES EQUAL 2)
            list(GET OUTPUT_LOCATE 0 OUTPUT_LOCATE)
            get_filename_component(OpenCV_DIR ${OUTPUT_LOCATE} PATH)
            message( STATUS "OpenCV_DIR= ${OpenCV_DIR}" )            
        elseif(RES GREATER 2)
            #string(REPLACE ";" " \n" OUTPUT_LOCATE ${OUTPUT_LOCATE})
            message( "OpenCVConfig.cmake found: ${OUTPUT_LOCATE}" )
            message( SEND_ERROR "Too many OpenCVConfig.cmake found. Please provide one of these paths in OpenCV_DIR" )
        else()
            message( ERROR "Could not find OpenCVConfig.cmake found. If OpenCV are not installed in a system folder, please provide its path in OpenCV_DIR" )
        endif(RES EQUAL 2)
    else()
        message( STATUS "OpenCV_DIR is not defined, if OpenCV are not installed in a system folder, please provide its path in OpenCV_DIR" )
    endif()

endif()
list(APPEND CMAKE_MODULE_PATH "${OpenCV_DIR}")

# add a search path for the glm libraries
# it will work if they are already compiled in /3rdparty
if(NOT DEFINED GLM_DIR)
    set(GLM_DIR ${PROJECT_SOURCE_DIR}/3rdparty/glm/build)
endif() 
list(APPEND CMAKE_LIBRARY_PATH "${GLM_DIR}/lib")
list(APPEND CMAKE_INCLUDE_PATH "${GLM_DIR}/include")


# set the output path for the generated files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib )


#########################################################
#
# EXTERNAL LIBRARIES
#
#########################################################

#########################################################
# LIB GLM
#########################################################


#try to find the libraries
find_package(GLM REQUIRED)

if(NOT LIBGLM_FOUND)

    #just to be sure
    set(GLM_DIR ${PROJECT_SOURCE_DIR}/3rdparty/glm)

    # set(EP_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdpary)
    # set_property(DIRECTORY PROPERTY EP_BASE ${PROJECT_SOURCE_DIR}/3rdparty)
    ExternalProject_Add( glm
                        URL http://devernay.free.fr/hacks/glm/glm-0.3.2.tar.gz
                        PREFIX ${GLM_DIR}
                        BUILD_IN_SOURCE 1
                        INSTALL_DIR ${GLM_DIR}/build
                        CONFIGURE_COMMAND ./configure --prefix=${GLM_DIR}/build --enable-shared
                        BUILD_COMMAND make -j 4
                        INSTALL_COMMAND make install)

    ExternalProject_Get_Property(glm install_dir)
    message( "lib glm has not be found, it will be installed in: ${install_dir}")
    set(LIBGLM_INCLUDE_DIRS ${install_dir}/include)   
    # message( "lib glm has not be found, it will be installed in: ${LIBGLM_INCLUDE_DIRS}")

    add_library(glm_lib SHARED IMPORTED)
    set_property(TARGET glm_lib PROPERTY IMPORTED_LOCATION ${install_dir}/lib/libglm.so)
    add_dependencies(glm_lib glm)

    set(LIBGLM_LIBRARIES glm_lib)

else()
    message( "-- Found glm" )
    message( ${LIBGLM_INCLUDE_DIRS} )
    message( ${LIBGLM_LIBRARIES} )
endif(NOT LIBGLM_FOUND)



#########################################################
# FIND OPENCV
#########################################################
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
add_definitions(${OpenCV_DEFINITIONS})
# MESSAGE( "${OpenCV_LIB_DIR}" )
# MESSAGE( "${OpenCV_VERSION}" )
# MESSAGE( "${OpenCV_FOUND}" )

if(NOT OpenCV_FOUND)
    message(ERROR " OpenCV not found!")
else()
    message( "-- Found OpenCV version: ${OpenCV_VERSION}" )
endif(NOT OpenCV_FOUND)


#########################################################
# FIND ARTOOLKITPLUS
#########################################################
find_package(ARToolKitPlus REQUIRED)
include_directories(${ARTOOLKITPLUS_INCLUDE_DIR})
link_directories(${ARTOOLKITPLUS_LIBRARIES})
 MESSAGE( "${ARTOOLKITPLUS_INCLUDE_DIR}" )
 MESSAGE( "${ARTOOLKITPLUS_LIBRARIES}" )
 MESSAGE( "${ARTOOLKITPLUS_FOUND}" )

if(NOT ARTOOLKITPLUS_FOUND)
    message(ERROR " ARToolKitPlus not found!")
else()
    message( "-- Found ARToolKitPlus" )
endif(NOT ARTOOLKITPLUS_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
# MESSAGE( "${OPENGL_LIBRARIES}" )
# MESSAGE( "${OPENGL_LIBRARIES_DIRS}" )
# MESSAGE( "${OPENGL_DEFINITIONS}" )
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
# message( "glut ${GLUT_INCLUDE_DIR}" )
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
else()
    message( "-- Found GLUT" )
endif(NOT GLUT_FOUND)


## FreeGLUT ?
# find_package(FreeGLUT REQUIRED)
# include_directories(${FreeGLUT_INCLUDE_DIRS})
# link_directories(${FreeGLUT_LIBRARY_DIRS})
# add_definitions(${FreeGLUT_DEFINITIONS})
# MESSAGE( "${FREEGLUT_INCLUDE_PATH}" )
# MESSAGE( "${FREEGLUT_LIBRARY}" )
# MESSAGE( "${FreeGLUT_DEFINITIONS}" )
# if(NOT FREEGLUT_FOUND)
#     message(ERROR " FreeGLUT not found!")
# endif(NOT FREEGLUT_FOUND)

#########################################################
# Doxygen
#########################################################

# add a target to generate API documentation with Doxygen
find_package(Doxygen QUIET)
# message( "${DOXYGEN_EXECUTABLE}" )
#if(DOXYGEN_FOUND)
#    set(CMAKE_DOC_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc)
#    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in ${CMAKE_DOC_OUTPUT_DIRECTORY}/Doxyfile @ONLY)
#    add_custom_target(doc
#            ${DOXYGEN_EXECUTABLE} ${CMAKE_DOC_OUTPUT_DIRECTORY}/Doxyfile
#            WORKING_DIRECTORY ${CMAKE_DOC_OUTPUT_DIRECTORY}
#            COMMENT "Generating API documentation with Doxygen" VERBATIM)
#endif(DOXYGEN_FOUND)



#########################################################
#
# PROJECT LIBRARIES
#
#########################################################

#########################################################
# TRACKER LIBRARY
#########################################################

add_subdirectory(src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/tracker)

get_property(inc_dirs DIRECTORY PROPERTY INCLUDE_DIRECTORIES)
# message("inc_dirs = ${inc_dirs}")



add_subdirectory(app)


