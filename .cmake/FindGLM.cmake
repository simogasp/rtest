# - Try to find Libglm
# Once done this will define
# LIBGLM_FOUND
# LIBGLM_INCLUDE_DIRS
# LIBGLM_LIBRARIES

# message("CMAKE_INCLUDE_PATH = ${CMAKE_INCLUDE_PATH}")
if (DEFINED GLM_DIR)
	#message("GLM_DIR = ${GLM_DIR}" )
	if(EXISTS "${GLM_DIR}/include/glm.h" AND EXISTS "${GLM_DIR}/lib/libglm.so")
		#message("glm found manually = ${GLM_DIR}" )
		set(GLM_LIBRARY ${GLM_DIR}/lib/libglm.so )
		set(GLM_INCLUDE_PATH ${GLM_DIR}/include/ )
	else()
		message("Can't find glm files in ${GLM_DIR}!!" )		
	endif()
else()
	message("-- GLM_DIR not defined")
# endif()

FIND_PATH(GLM_INCLUDE_PATH 
	NAMES glm.h
	HINTS
    ${GLM_DIR}/include
    $ENV{GLM_HOME}/include
	PATHS
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where glm.h resides")

message("GLM_INCLUDE_PATH = ${GLM_INCLUDE_PATH}")

FIND_LIBRARY(GLM_LIBRARY
	NAMES glm 
	PATHS
	$ENV{GLM_DIR}/lib
	$ENV{GLM_HOME}/lib
	/usr/lib
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The GLM library")


endif()
#message("GLM_LIBRARY = ${GLM_LIBRARY}")

set(LIBGLM_LIBRARIES ${GLM_LIBRARY} )
set(LIBGLM_INCLUDE_DIRS ${GLM_INCLUDE_PATH} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBGLM_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args( libglm  DEFAULT_MSG
                                  GLM_LIBRARY GLM_INCLUDE_PATH )

mark_as_advanced(GLM_INCLUDE_PATH GLM_LIBRARY)
