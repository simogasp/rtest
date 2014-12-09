#ifndef _CAMERA_HPP_
#define	_CAMERA_HPP_

#include <opencv2/core/core.hpp>

class OcvCamera
{
public:
    
	OcvCamera() {}

	/**
	 * Initialize the camera loading the internal parameters from the given file
	 *
	 * @param[in] calibFilename the calibration file
	 * @return true if success
	 */
	bool init( std::string calibFilename );

	~OcvCamera( ) {}

       
    /**
     * Return the OpenGL projection matrix for the camera
     * @param[out] proj the OGL projection matrix (ready to be passed, ie in col major format)
     * @param znear near clipping plane
     * @param zfar far clipping plane
     * \note using http://strawlab.org/2011/11/05/augmented-reality-with-OpenGL/
     */
    void getOGLProjectionMatrix( float *proj, const float znear, const float zfar ) const;
	

public:
	cv::Mat matK;

	cv::Mat distCoeff;

	cv::Size imageSize;

};

#endif	/* _CAMERA_HPP_ */
