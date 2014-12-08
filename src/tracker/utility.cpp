#include "tracker/utility.hpp"

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;



/******************************************************************/
/* FUNCTIONS TO DEVELOP											  */
/******************************************************************/
/**
 * Detect a chessboard in a given image
 *
 * @param[in] rgbimage The rgb image to process
 * @param[out] pointbuf the set of 2D image corner detected on the chessboard 
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] patternType The type of chessboard pattern to look for
 * @return true if the chessboard is detected inside the image, false otherwise 
 */
bool detectChessboard( const Mat &rgbimage, vector<Point2f> &pointbuf, const Size &boardSize, const Pattern patternType )
{
	// it contains the value to return
	bool found = false;

	switch( patternType )
	{
		// detect a classic chessboard
		case CHESSBOARD:
		
			/******************************************************************/
			// detect the chessboard --> see findChessboardCorners
			// found = ...
			/******************************************************************/
			found = findChessboardCorners( rgbimage, boardSize, pointbuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FAST_CHECK | CV_CALIB_CB_NORMALIZE_IMAGE);
			
			// if a chessboard is found refine the position of the points in a window 11x11 pixel
			// use the default value for the termination criteria --> TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 )
			if ( found )
			{
				Mat viewGrey;		// it will contain the graylevel version of the image
				
				/******************************************************************/
				// convert the image in "rgbimage" to gray level and save it in "viewGrey"
				// --> cvtColor with CV_BGR2GRAY option
				/******************************************************************/
				cvtColor(rgbimage, viewGrey, CV_BGR2GRAY); 
				
				/******************************************************************/
				// refine the corner location in "pointbuf" using "viewGrey" 
				// --> see cornerSubPix
				/******************************************************************/
				cornerSubPix( viewGrey, pointbuf, Size(11,11),  Size(-1,-1), TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 30, 0.1 ));
			}
			break;
			
		// detect a regular grid made of circles	
		case CIRCLES_GRID:
			/******************************************************************/
			// detect the circles --> see findCirclesGrid
			// found = ...
			/******************************************************************/
			found = findCirclesGrid( rgbimage, boardSize, pointbuf );
			
			break;
			
		// detect an asymmetric grid made of circles	
		case ASYMMETRIC_CIRCLES_GRID:
			/******************************************************************/	
			// detect the circles --> see findCirclesGrid using the options CALIB_CB_ASYMMETRIC_GRID | CALIB_CB_CLUSTERING 
			// found = ...
			/******************************************************************/
			found = findCirclesGrid( rgbimage, boardSize, pointbuf, CALIB_CB_ASYMMETRIC_GRID | CALIB_CB_CLUSTERING );
			
			break;
			
			
		default:
			cerr << "Unknown pattern type" << endl;
			return found;
	}

	return found;
}



/**
 *
 * @param[in,out] rgbimage The image on which to draw the reference system
 * @param[in] cam The camera
 * @param[in] projMat The projection matrix of the camera
 * @param[in] thickness The thickness of the line
 * @param[in] scale A scale factor for the unit vectors to draw
 * @param[in] alreadyUndistorted A boolean value that tells if the input image rgbimage is already undistorted or we are working on a distorted image
 */
void drawReferenceSystem( cv::Mat &rgbimage, const Camera& cam, const cv::Mat &projMat, const int &thickness, const double &scale, const bool alreadyUndistorted )
{
	
	// contains the points to project to draw the 3 axis
	vector<Point3f> vertex3D;
	
	//******************************************************************/
	// Add the four 3D points (Point3f) that we can use to draw 
	// the reference system to vertex3D. Use <scale> as unit
	//******************************************************************/
	vertex3D.push_back( Point3f( 0, 0, 0) );
	vertex3D.push_back( Point3f( 0, scale, 0) );
	vertex3D.push_back( Point3f( scale, 0, 0) );
	vertex3D.push_back( Point3f( 0, 0, -scale) );

	// contains the projected 3D points on the image
	vector<Point2f> imgRefPts;

	//******************************************************************/
	// Project the 3D points using myProjectPoints. Attention, check the
	// flag alreadyUndistorted to see if we have to apply the distortion:
	// if it is true we pass a 1x5 zero vector, otherwise the distortion
	//  parameter of cam
	//******************************************************************/
	if( alreadyUndistorted )
	{
		myProjectPoints( Mat( vertex3D ), projMat, cam.matK, Mat::zeros(1,5,CV_32F), imgRefPts );
	}
	else
	{
		myProjectPoints( Mat( vertex3D ), projMat, cam.matK, cam.distCoeff, imgRefPts );
	}

//	cout << "vertex3D" << vertex3D << endl;
//	cout << "imgRefPts" << imgRefPts << endl;

	//******************************************************************/
	// draw the line of the x-axis and put "X" at the end
	//******************************************************************/
	line( rgbimage, imgRefPts[0], imgRefPts[1], Scalar( 255, 0, 0 ), thickness );
	putText( rgbimage, "X", imgRefPts[1], FONT_HERSHEY_SIMPLEX, 1.0, Scalar( 255, 0, 0 ), 2 );

	//******************************************************************/
	// draw the line of the y-axis and put "Y" at the end
	//******************************************************************/
	line( rgbimage, imgRefPts[0], imgRefPts[2], Scalar( 0, 255, 0 ), thickness );
	putText( rgbimage, "Y", imgRefPts[2], FONT_HERSHEY_SIMPLEX, 1.0, Scalar( 0, 255, 0 ), 2 );

	//******************************************************************/
	// draw the line of the z-axis and put "Z" at the end
	//******************************************************************/
	line( rgbimage, imgRefPts[0], imgRefPts[3], Scalar( 0, 0, 255 ), thickness );
	putText( rgbimage, "Z", imgRefPts[3], FONT_HERSHEY_SIMPLEX, 1.0, Scalar( 0, 0, 255 ), 2 );

}

/**
 * Wrapper around the original opencv's projectPoints
 * 
 * @param objectPoints the 3D points
 * @param poseMat the pose matrix
 * @param cameraMatrix the calibration matrix
 * @param distCoeffs the distortion coeffi
 * @param imagePoints
 */
void myProjectPoints( InputArray objectPoints, const Mat &poseMat, InputArray cameraMatrix, InputArray distCoeffs, OutputArray imagePoints)
{
	Mat rvec;
	Rodrigues( poseMat.colRange(0,3), rvec );
	// projectPoints( Mat( vertex3D.t( ) ).reshape( 3, 1 ), rvec, Tvec, K, dist, imgRefPts );
	projectPoints( objectPoints, rvec, poseMat.col(3), cameraMatrix, distCoeffs, imagePoints );
}


/**
 * Generate the set of 3D points of a chessboard
 *
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] squareSize the size in mm of the each square of the chessboard
 * @param[out] corners the set of 2D points on the chessboard 
 * @param[in] patternType The type of chessboard pattern to look for
 */
void calcChessboardCorners( const Size &boardSize, const float &squareSize, vector<Point2f>& corners, const Pattern patternType )
{
    corners.resize(0);
    corners.reserve( boardSize.height* boardSize.width );
    
    switch(patternType)
    {
      case CHESSBOARD:
      case CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
            {
            	/******************************************************************/
            	// create a Point2f(x,y) according to the position j,i and a square 
            	// size of squareSize. Add it to corners (using push_back...)
            	/******************************************************************/
                corners.push_back( Point2f( float( j*squareSize ), 		// x
                                            float( i*squareSize )));	// y
            }
        break;

      case ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
            {
            	/******************************************************************/
            	// create a Point2f(x,y) according to the position j,i considering 
            	// that x is generate using the formula (2*j + i % 2)*squareSize
            	//  Add it to corners (using push_back...)
            	/******************************************************************/
                corners.push_back(Point2f( float((2*j + i % 2)*squareSize),
                                           float(i*squareSize)));
            }
        break;

      default:
        CV_Error(CV_StsBadArg, "Unknown pattern type\n");
    }
}

/**
 * Decompose the homography into its components R and t
 *
 * @param[in] H The homography H = [r1 r2 t]
 * @param[in] matK The 3x3 calibration matrix K
 * @param[out] poseMat the 3x4 pose matrix [R t]
 */
void decomposeHomography( const Mat &H, const Mat& matK, Mat& poseMat )
{

  	Mat temp;
	
	//******************************************************************/
	//temp contains inv(K)*H
	//******************************************************************/
	temp = matK.inv()*H;
	PRINTVAR( temp );
  	
	Mat r1, r2,  r3, t;
	//******************************************************************/
	// get r1 and r2 from temp
	//******************************************************************/
  	r1 = temp.col(0);
	r2 = temp.col(1); 
	
	//******************************************************************/
	// compute lambda
	//******************************************************************/
  	float lambda = norm(r1);
	
	//******************************************************************/
	// normalize r1 and r2
	//******************************************************************/
  	r1 = r1 / norm(r1);  
  	r2 = r2 / norm(r2);  

	//******************************************************************/
	// compute r3
	//******************************************************************/
  	r3 = r1.cross(r2);
	PRINTVAR( r3 );
	
	//******************************************************************/
	// compute t
	//******************************************************************/
  	t = temp.col(2) / lambda;

	//******************************************************************/
	// create a 3x4 matrix (float) for poseMat
	//******************************************************************/
  	poseMat = Mat(3, 4, CV_32F );

	//******************************************************************/
	// fill the columns of poseMat with r1 r2 r3 and t
	//******************************************************************/
#if CV_MINOR_VERSION < 4		
// apparently older versions does not support direct copy
	Mat a1 = poseMat.col(0);
  	r1.copyTo( a1 );
	a1 = poseMat.col(1);
  	r2.copyTo( a1 );
	a1 = poseMat.col(2);
  	r3.copyTo( a1 );
	a1 = poseMat.col(3);
  	t.copyTo( a1 );
#else
  	r1.copyTo( poseMat.col(0) );
  	r2.copyTo( poseMat.col(1) );
  	r3.copyTo( poseMat.col(2) );
  	t.copyTo( poseMat.col(3) );
#endif

//  	PRINTVAR( poseMat );

}


/******************************************************************************/
//KLT ONLY

/**
 * Generate the set of 3D points of a chessboard
 *
 * @param[in] boardSize the size of the board in terms of corners (width X height)
 * @param[in] squareSize the size in mm of the each square of the chessboard
 * @param[out] corners the set of 3D points on the chessboard 
 * @param[in] patternType The type of chessboard pattern to look for
 */
void calcChessboardCorners3D( const Size &boardSize, const float &squareSize, vector<Point3f>& corners, const Pattern patternType  )
{
    corners.resize(0);
    corners.reserve( boardSize.height* boardSize.width );
    
    switch(patternType)
    {
      case CHESSBOARD:
      case CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
			{
				/******************************************************************/
            	// create a Point3f(x,y,0) according to the position j,i and a square 
            	// size of squareSize. Add it to corners (using push_back...)
            	/******************************************************************/
                corners.push_back( Point3f( float( j*squareSize ), 	// x
                                            float( i*squareSize ),	// y
											0.0 ));					// z
			}

        break;

      case ASYMMETRIC_CIRCLES_GRID:
        for( int i = 0; i < boardSize.height; i++ )
            for( int j = 0; j < boardSize.width; j++ )
			{
				/******************************************************************/
            	// create a Point3f(x,y,0) according to the position j,i considering 
            	// that x is generate using the formula (2*j + i % 2)*squareSize
            	//  Add it to corners (using push_back...)
            	/******************************************************************/
                corners.push_back(Point3f( float((2*j + i % 2)*squareSize),
                                           float(i*squareSize),
											0.0 )); //z
			}
        break;

      default:
        CV_Error(CV_StsBadArg, "Unknown pattern type\n");
    }	
}


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
void mySolvePnPRansac(cv::InputArray objectPoints, cv::InputArray imagePoints, cv::InputArray cameraMatrix, cv::InputArray distCoeffs, cv::Mat &poseMat, OutputArray inliers )
{
	Mat currR, currT;
	solvePnPRansac( objectPoints, imagePoints, cameraMatrix, distCoeffs, currR, currT, false, 100, 4, 100, inliers );

	poseMat = Mat(3, 4, CV_32F );

	Mat Rot;
	Rodrigues( currR, Rot );
#if CV_MINOR_VERSION < 4
// apparently older versions does not support direct copy
	Mat temp;
	Rot.convertTo( temp, CV_32F );
	Mat a1 = poseMat.colRange(0,3);
	temp.copyTo( a1 );
	a1 = poseMat.col(3);
	currT.convertTo( temp, CV_32F );
	temp.copyTo( a1 );
#else
	Rot.copyTo( poseMat.colRange(0,3) );
	currT.copyTo( poseMat.col(3) );
#endif
}
