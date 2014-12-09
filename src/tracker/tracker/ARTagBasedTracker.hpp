/* 
 * File:   ARTagBasedTracker.hpp
 * Author: sgaspari
 *
 * Created on December 8, 2014, 11:13 PM
 */

#ifndef _ARTAGBASEDTRACKER_HPP
#define	_ARTAGBASEDTRACKER_HPP

#include "Camera.hpp"

#include <ARToolKitPlus/CameraAdvImpl.h>
#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>


#define NOTAMARKER -1

typedef int MarkerID;

class MyARLogger : public ARToolKitPlus::Logger
{

    void artLog( const char* nStr )
    {
	ROM_COUT( nStr );
    }
};

class ARTKCamera : public ARToolKitPlus::CameraAdvImpl
{
public:

    ARTKCamera( )
    {
	undist_iterations = 1;
    }

    ARTKCamera( const Camera & cam )
    {
	loadFromK( cam );
	undist_iterations = 1;
    }

    void loadFromK( Camera & cam )
    {
	cc[0] = cam.matK<double>at( 0, 2 );
	cc[1] = cam.matK<double>at( 1, 2 );
	fc[0] = cam.matK<double>at( 0, 0 );
	fc[1] = cam.matK<double>at( 1, 1 );

	memset( kc, 0, 6 * sizeof (ARFloat ) );
	for ( std::size_t i = 0; i < cam.distCoeff.rows; ++i )
	{
	    kc[i] = ( float ) cam.distCoeff<double>at( i );
	}

	for ( std::size_t i = 0; i < 3; ++i )
	    for ( std::size_t j = 0; j < 4; ++j )
		mat[i][j] = 0.0;

	mat[0][0] = fc[0]; // fc_x
	mat[1][1] = fc[1]; // fc_y
	mat[0][2] = cc[0]; // cc_x
	mat[1][2] = cc[1]; // cc_y
	mat[2][2] = 1.0;
    }
};

class ARTagBasedTracker
{
public:

    struct Parameters
    {

	Parameters( )
	: _pixelFormat( ARToolKitPlus::PIXEL_FORMAT_BGR )
	, _distortion( ARToolKitPlus::UNDIST_NONE )
	, _markerMode( ARToolKitPlus::MARKER_ID_SIMPLE )
	, _threshold( -1.0f )
	, _thresholdRetries( 4 )
	, _borderWidth( 0.25f )
	, _patternWidth( 80 )
	, _calibrationFilename( )
	, _width( 0 )
	, _height( 0 ) { }

	ARToolKitPlus::PIXEL_FORMAT _pixelFormat;
	ARToolKitPlus::UNDIST_MODE _distortion;
	ARToolKitPlus::MARKER_MODE _markerMode;
	float _threshold;
	std::size_t _thresholdRetries;
	float _borderWidth;
	std::size_t _patternWidth;
	std::string _calibrationFilename;
	std::size_t _width;
	std::size_t _height;
    };



    ARTagBasedTracker( );

    void init( const Parameters & args, const Camera &cam );

    std::size_t detectMarkers( const Mat & img );

    void visualDebug( const Mat & img );

    void ignoreMarkerNextDetection( const MarkerID id );

    virtual ~ARTagBasedTracker( );

private:
    ARToolKitPlus::TrackerSingleMarkerImpl<6, 6, 6, 1, kMaxTag> _arTagTracker;
    ARToolKitPlus::ARMarkerInfo *_infosForDebug;
    MyARLogger _logger;
    int _currLenForDebug;
    Parameters _params;

};

#endif	/* _ARTAGBASEDTRACKER_HPP */

