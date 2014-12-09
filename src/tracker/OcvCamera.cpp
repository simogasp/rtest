#include "tracker/OcvCamera.hpp"

#include <iostream>

using namespace std;
using namespace cv;

/**
 * Initialize the camera loading the internal parameters from the given file
 *
 * @param[in] calibFilename the calibration file
 * @return true if success
 */
bool OcvCamera::init( std::string calibFilename )
{
	/******************************************************************/ 
	// open the file storage with the given filename
	/******************************************************************/ 
	FileStorage fs = FileStorage( calibFilename, CV_STORAGE_READ );

	/******************************************************************/ 
	// check if the file storage has been opened correclty
	/******************************************************************/ 
	if( !fs.isOpened() )
	{
		cerr << "Cannot open calibration file " << calibFilename << endl;
		return false;
	}

	/******************************************************************/ 
	// load the camera_matrix in matK
	/******************************************************************/ 
	fs["camera_matrix"] >> matK;
	/******************************************************************/ 
	// load the distortion_coefficients in distCoeff
	/******************************************************************/ 
	fs["distortion_coefficients"] >> distCoeff;
	/******************************************************************/ 
	// load image_width and image_height in imageSize.[width|height]
	/******************************************************************/ 
	fs["image_width"] >> imageSize.width;
	fs["image_height"] >> imageSize.height;

	cout << matK << endl;
	cout << distCoeff << endl;

	return true;
}


/**
 * Return the OpenGL projection matrix for the left camera
 * @param[out] proj the OGL projection matrix (ready to be passed, ie in col major format)
 * @param znear near clipping plane
 * @param zfar far clipping plane
 * \note using http://strawlab.org/2011/11/05/augmented-reality-with-OpenGL/
 */
void OcvCamera::getOGLProjectionMatrix( float *proj, const float znear, const float zfar ) const
{

	
//	With window_coords==’y down’, we have:
//
//	[2*K00/width, -2*K01/width,    (width - 2*K02 + 2*x0)/width,                            0]
//	[          0, 2*K11/height, (-height + 2*K12 + 2*y0)/height,                            0]
//	[          0,            0,  (-zfar - znear)/(zfar - znear), -2*zfar*znear/(zfar - znear)]
//	[          0,            0,                              -1,                            0]
	
	//corrected with artoolkitpluss src/Tracker::349
	
	proj[0]  = (float)(2*matK.at<double>(0,0))/imageSize.width;
	proj[1*4 + 0] = (float)(2*matK.at<double>(0,1))/imageSize.width;
	proj[2*4 + 0] = -(float)(imageSize.width - 2*matK.at<double>(0,2) )/imageSize.width;
	proj[3*4 + 0] = 0;
	
	proj[0*4 + 1] = 0;
	// minus -(float)(2*matK.at<double>(1,1))/imageSize.height;
	proj[1*4 + 1] = -(float)(2*matK.at<double>(1,1))/imageSize.height;
	proj[2*4 + 1] = -(float)(-imageSize.height + 2*matK.at<double>(1,2) )/imageSize.height;
	proj[3*4 + 1] = 0;
	
	proj[0*4 + 2] = 0;
	proj[1*4 + 2] = 0;
	proj[2*4 + 2] = (zfar + znear)/(zfar - znear);
	proj[3*4 + 2] = -2*zfar*znear/(zfar - znear);
	
	proj[0*4 + 3] = 0;
	proj[1*4 + 3] = 0;
	proj[2*4 + 3] = 1;
	proj[3*4 + 3] = 0;
	
}
