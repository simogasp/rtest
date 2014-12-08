#ifndef _UTILITY_HPP_
#define	_UTILITY_HPP_

#include "Camera.hpp"

#include <opencv2/core/core.hpp>

#include <iostream>

#define DEBUGGING 1

#if DEBUGGING
#define PRINTVAR( a ) std::cout << #a << " = " << a << endl << endl;
#else
#define PRINTVAR( a )
#endif

// Enumerative type containing the possible patterns for the chessboard
enum Pattern { CHESSBOARD, CIRCLES_GRID, ASYMMETRIC_CIRCLES_GRID };

/**
 * Detect a chessboard in a given image
 *
 * @param[in] rgbimage The rgb image to process
 * @param[out] pointbuf the set of 2D image corner detected on the chessboard 
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] patternType The type of chessboard pattern to look for
 * @return true if the chessboard is detected inside the image, false otherwise 
 */
bool detectChessboard( const cv::Mat &rgbimage, std::vector<cv::Point2f> &pointbuf, const cv::Size &boardSize, const Pattern patternType );

/**
 * Decompose the homography into its components R and t
 *
 * @param[in] H The homography H = [r1 r2 t]
 * @param[in] matK The 3x3 calibration matrix K
 * @param[out] poseMat the 3x4 pose matrix [R t]
 */
void decomposeHomography( const cv::Mat &H, const cv::Mat& matK, cv::Mat& poseMat );

/**
 * 
 * @param[in,out] rgbimage The image on which to draw the reference system
 * @param[in] cam The camera
 * @param[in] projMat The projection matrix of the camera
 * @param[in] thickness The thickness of the line
 * @param[in] scale A scale factor for the unit vectors to draw
 * @param[in] alreadyUndistorted A boolean value that tells if the input image rgbimage is already undistorted or we are working on a distorted image
 */
void drawReferenceSystem( cv::Mat &rgbimage, const Camera& cam, const cv::Mat &projMat, const int &thickness, const double &scale, const bool alreadyUndistorted = true );

/**
 * Wrapper around the original opencv's projectPoints
 * 
 * @param[in] objectPoints the 3D points
 * @param[in] poseMat the pose matrix
 * @param[in] cameraMatrix the calibration matrix
 * @param[in] distCoeffs the distortion coefficients
 * @param[out] imagePoints the projected points
 */
void myProjectPoints( cv::InputArray objectPoints, const cv::Mat &poseMat, cv::InputArray cameraMatrix, cv::InputArray distCoeffs, cv::OutputArray imagePoints);

/**
 * Wrapper around the original opencv's solvePnPRansac
 * 
 * @param[in] objectPoints the 3D points
 * @param[in] imagePoints the image points
 * @param[in] cameraMatrix the calibration matrix
 * @param[in] distCoeffs the distortion coefficients
 * @param[out] poseMat the pose matrix
 * @param[out] inliers the list of indices of the inliers points
 */
void mySolvePnPRansac(cv::InputArray objectPoints, cv::InputArray imagePoints, cv::InputArray cameraMatrix, cv::InputArray distCoeffs, cv::Mat &poseMat, cv::OutputArray inliers=cv::noArray());

/**
 * Generate the set of 3D points of a chessboard
 *
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] squareSize the size in mm of the each square of the chessboard
 * @param[out] corners the set of 2D points on the chessboard 
 * @param[in] patternType The type of chessboard pattern to look for
 */
void calcChessboardCorners( const cv::Size &boardSize, const float &squareSize, std::vector<cv::Point2f>& corners, const Pattern patternType = CHESSBOARD );


/**
 * Generate the set of 3D points of a chessboard
 *
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] squareSize the size in mm of the each square of the chessboard
 * @param[out] corners the set of 3D points on the chessboard 
 * @param[in] patternType The type of chessboard pattern to look for
 */
void calcChessboardCorners3D( const cv::Size &boardSize, const float &squareSize, std::vector<cv::Point3f>& corners, const Pattern patternType = CHESSBOARD );

/**
 * Filter a generic vector against a list of index of the elements to be deleted,
 *
 * @param[in,out] inout the vector to filter
 * @param[in] idx list of indices of the element to remove
 */
template<typename T>
void filterVector( std::vector<T> &inout, const std::vector<int> &idx )
{
	std::vector<T> temp;
	temp.reserve( idx.size() );

	for( size_t i = 0; i < idx.size(); ++i )
	{
		assert( idx[i] < inout.size() );
		temp.push_back( inout[ idx[i] ] );
	}

	inout.clear();  //necessary??
	inout = temp;
}

#endif /*_UTILITY_HPP_*/
