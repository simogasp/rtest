#include "tracker/ChessboardCameraTracker.hpp"
#include "tracker/utility.hpp"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

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
bool ChessboardCameraTracker::process( cv::Mat &view, cv::Mat &pose, const Camera & cam, const cv::Size &boardSize, const Pattern &pattern  )
{
	// true if the chessboard is found
	bool found = false;

	// contains the points detected on the chessboard
	vector<Point2f> corners;
	
	//******************************************************************/
	// undistort the input image. view at the end must contain the undistorted version
	// of the image.
	//******************************************************************/
	Mat temp = view.clone();
    undistort( temp, view, cam.matK, cam.distCoeff );

	//******************************************************************/
	// detect the chessboard
	//******************************************************************/
	found = detectChessboard( view, corners, boardSize, pattern );

	// cout << ( (!found ) ? ( "No " ) : ("") ) << "chessboard detected!" << endl;

	//******************************************************************/
	// if a chessboard is found estimate the homography 
	//******************************************************************/
	if( found )
	{

		// contains the points on the chessboard
		vector<Point2f> objectPoints;

		//******************************************************************/
		// generate the points on the chessboard
		//******************************************************************/
		calcChessboardCorners( boardSize, 25, objectPoints, pattern );

		//******************************************************************/
		// estimate the homography
		// --> see findHomography 
		// http://docs.opencv.org/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html?highlight=homography#findhomography
		//******************************************************************/
		Mat H = findHomography( objectPoints, corners, CV_RANSAC );  //cor ~ H * obj

		cout << "H = " << H << endl << endl;
		cout << "corners =" << corners << endl << endl;
		cout << "ptsOb =" << objectPoints << endl << endl;

		//******************************************************************/
		// decompose the homography
		//******************************************************************/
		decomposeHomography( H, cam.matK, pose );

	}

	return found;
}
