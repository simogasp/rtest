/* 
 * File:   ARTagUtil.hpp
 * Author: sgaspari
 *
 * Created on December 11, 2014, 9:54 PM
 */

#ifndef ARTAGUTIL_HPP
#define	ARTAGUTIL_HPP

#include "OcvCamera.hpp"

#include <ARToolKitPlus/ar.h>
#include <ARToolKitPlus/CameraAdvImpl.h>


#define NOTAMARKER -1

typedef int MarkerID;

/// the maximum number of patterns that can be analyzed in a camera image
static const std::size_t kMaxTag = 30;

class MyARLogger : public ARToolKitPlus::Logger
{
    void artLog( const char* nStr );	
};

/**
 * Determine whether a detected marker is close to the border of the image
 * @param minfo the marker to process
 * @param w the image width
 * @param h the image height
 * @param d the threshold distance from the border
 * @return true if the marker is close to the image border
 */
bool verticesOnBorder( const ARToolKitPlus::ARMarkerInfo & minfo, const std::size_t w, const std::size_t h, const std::size_t d );


/**
 */
class ARTKCamera : public ARToolKitPlus::CameraAdvImpl
{
public:

    ARTKCamera( )
    {
	undist_iterations = 1;
    }

    ARTKCamera( const OcvCamera & cam )
    {
	loadFromK( cam );
	undist_iterations = 1;
    }

    void loadFromK( const OcvCamera & cam );
};

#endif	/* ARTAGUTIL_HPP */

