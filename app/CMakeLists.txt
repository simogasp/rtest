add_executable( calibration calibration.cpp )
target_link_libraries( calibration ${OpenCV_LIBS} )

add_executable( circledetection circledetection.cpp )
target_link_libraries( circledetection ${OpenCV_LIBS} )

add_executable( delaunay delaunay.cpp )
target_link_libraries( delaunay ${OpenCV_LIBS} )

add_executable( simpleDetection simpleDetection.cpp )
target_link_libraries( simpleDetection ${OpenCV_LIBS} tracker)

include_directories( ${LIBGLM_INCLUDE_DIRS})

add_executable( videoOGLTracking videoOGLTracking.cpp )
target_link_libraries( videoOGLTracking ${OpenCV_LIBS} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} tracker)
