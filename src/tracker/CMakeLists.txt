FILE(
	GLOB
	trackerHeaders_hpp
	./tracker/*.hpp
)

add_library( tracker STATIC utility.cpp ChessboardCameraTracker.cpp ChessboardCameraTrackerKLT.cpp Camera.cpp)
target_link_libraries( tracker ${OpenCV_LIBS} )

install(TARGETS tracker LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
install(FILES ${trackerHeaders_hpp} DESTINATION include/tracker)
