FILE(
	GLOB
	trackerHeaders_hpp
	./tracker/*.hpp
)

add_library( tracker STATIC utility.cpp ChessboardCameraTracker.cpp ChessboardCameraTrackerKLT.cpp OcvCamera.cpp ARTagBasedTracker.cpp ARTagUtil.cpp MultiARTagBasedTracker.cpp )
target_link_libraries( tracker ${OpenCV_LIBS} ${ARTOOLKITPLUS_LIBRARIES})

install(TARGETS tracker LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES ${trackerHeaders_hpp} DESTINATION include/tracker)
