/* 
 * File:   ARTagBasedTracker.hpp
 * Author: sgaspari
 *
 * Created on December 8, 2014, 11:13 PM
 */

#ifndef _ARTAGBASEDTRACKER_HPP
#define	_ARTAGBASEDTRACKER_HPP

#include "ARTagUtil.hpp"

#include <ARToolKitPlus/CameraAdvImpl.h>
#include <ARToolKitPlus/TrackerSingleMarkerImpl.h>
#include <opencv2/core/core.hpp>


	
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

    void init( const Parameters & args, const OcvCamera &cam );

    std::size_t detectMarkers( const cv::Mat & img );

    void visualDebug( cv::Mat & img );

    void ignoreMarkerNextDetection( const MarkerID id );
    
    void getPoseMatrix( cv::Mat &pose );

    virtual ~ARTagBasedTracker( );

private:
    ARToolKitPlus::TrackerSingleMarkerImpl<6, 6, 6, 1, kMaxTag> _arTagTracker;
    ARToolKitPlus::ARMarkerInfo *_infosForDebug;
    MyARLogger _logger;
    int _currLenForDebug;
    Parameters _params;

};

#endif	/* _ARTAGBASEDTRACKER_HPP */

