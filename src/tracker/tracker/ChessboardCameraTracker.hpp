#ifndef _CHESSBOARDCAMERATRACKER_HPP_
#define	_CHESSBOARDCAMERATRACKER_HPP_

#include "ICameraTracker.hpp"

class ChessboardCameraTracker : public ICameraTracker
{
public:

	ChessboardCameraTracker() {}
	

	/**
	* It detects a chessboard inside an image and if found it returns the pose of the camera wrt the chessboard
	*
    * @param[in,out] view the original image
	* @param[out] pose the pose of the camera 
	* @param[in] cam the camera
	* @param[in] boardSize the size of the chessboard to detect
    * @param[in] pattern the type of pattern to detect
	* @return true if the chessboard has been found
	*/
    bool process( cv::Mat &view, cv::Mat &pose, const Camera & cam, const cv::Size &boardSize, const Pattern &pattern  );

	virtual ~ChessboardCameraTracker() {}

};

#endif /*_CHESSBOARDCAMERATRACKER_HPP_*/
