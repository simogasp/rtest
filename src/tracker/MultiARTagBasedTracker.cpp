/* 
 * File:   MultiARTagBasedTracker.cpp
 * Author: sgaspari
 * 
 * Created on December 11, 2014, 9:52 PM
 */

#include "tracker/MultiARTagBasedTracker.hpp"
#include "tracker/ARTagUtil.hpp"
#include "tracker/utility.hpp"

using namespace std;
using namespace cv;

MultiARTagBasedTracker::MultiARTagBasedTracker( )
: _arTagTracker( 640, 480 )
, _infosForDebug( NULL )
, _currLenForDebug( 0 )
{ }

void MultiARTagBasedTracker::visualDebug(  Mat & img )
{

	for ( int i = 0; i < _currLenForDebug; i++ )
	{
		if ( ( _infosForDebug + i )->id != NOTAMARKER )
		{
			Scalar color( 0, 255, 0 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[0][0], ( float ) ( _infosForDebug + i )->vertex[0][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[1][0], ( float ) ( _infosForDebug + i )->vertex[1][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[1][0], ( float ) ( _infosForDebug + i )->vertex[1][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[2][0], ( float ) ( _infosForDebug + i )->vertex[2][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[2][0], ( float ) ( _infosForDebug + i )->vertex[2][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[3][0], ( float ) ( _infosForDebug + i )->vertex[3][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[0][0], ( float ) ( _infosForDebug + i )->vertex[0][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[3][0], ( float ) ( _infosForDebug + i )->vertex[3][1] ),
					color, 2 );

			string str = static_cast<ostringstream*>( &(ostringstream() << (_infosForDebug + i )->id ) )->str();
			putText( img, 
					str, 
					Point2f( ( float ) ( _infosForDebug + i )->pos[0], ( float ) ( _infosForDebug + i )->pos[1] ), 
					FONT_HERSHEY_SIMPLEX, 
					1.0,
					color,
					2);
		}
		else
		{
			Scalar color( 0, 0, 255 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[0][0], ( float ) ( _infosForDebug + i )->vertex[0][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[1][0], ( float ) ( _infosForDebug + i )->vertex[1][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[1][0], ( float ) ( _infosForDebug + i )->vertex[1][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[2][0], ( float ) ( _infosForDebug + i )->vertex[2][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[2][0], ( float ) ( _infosForDebug + i )->vertex[2][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[3][0], ( float ) ( _infosForDebug + i )->vertex[3][1] ),
					color, 2 );
			line( img,
					Point2f( ( float ) ( _infosForDebug + i )->vertex[0][0], ( float ) ( _infosForDebug + i )->vertex[0][1] ),
					Point2f( ( float ) ( _infosForDebug + i )->vertex[3][0], ( float ) ( _infosForDebug + i )->vertex[3][1] ),
					color, 2 );
		}
	}	
}

void MultiARTagBasedTracker::ignoreMarkerNextDetection( const MarkerID id )
{
	for( int i = 0; i < _currLenForDebug; ++i )
	{
		if ( _infosForDebug[i].id == (int)id )
		{
			cout << "removing marker ID " << id << " from artk+" << endl;
			_infosForDebug[i].id = NOTAMARKER;
			return;
		}
	}
}



void MultiARTagBasedTracker::init( const Parameters & params, const OcvCamera &cam )
{
	// load a camera file. two types of camera files are supported:
	//  - Std. ARToolKit
	//  - MATLAB Camera Calibration Toolbox
	if ( !_arTagTracker.init( NULL, params._multiPatternFilename.c_str(), 1.0f, 1000.0f ) )
	{
		cerr << "ERROR: cannot init ar tracker!" << endl;
		return;
	}

	_arTagTracker.setCamera( new ARTKCamera( cam ) );

	_arTagTracker.setPixelFormat( params._pixelFormat );


	// the marker in the BCH test image has a thin border...
	_arTagTracker.setBorderWidth( params._borderWidth );

	// set a threshold. alternatively we could also activate automatic thresholding
	if( params._threshold > 0 )
	{
		_arTagTracker.setThreshold( params._threshold );
	}
	else
	{
		_arTagTracker.activateAutoThreshold( true );
		_arTagTracker.setNumAutoThresholdRetries( params._thresholdRetries );
	}

	// let's use lookup-table undistortion for high-speed
	// note: LUT only works with images up to 1024x1024
	_arTagTracker.setUndistortionMode( params._distortion );

	// switch to simple ID based markers
	// use the tool in tools/IdPatGen to generate markers
	_arTagTracker.setMarkerMode( params._markerMode );
	
	// RPP is more robust than ARToolKit's standard pose estimator
	_arTagTracker.setPoseEstimator( ARToolKitPlus::POSE_ESTIMATOR_ORIGINAL );

	_currLenForDebug = 0;

	_params = params;
}

std::size_t MultiARTagBasedTracker::detectMarkers( const Mat & img )
{

	size_t numMark = 0;
	MarkerID best = 0;
	cout << "getting the pose" << endl;
	best = _arTagTracker.calc( (unsigned char*) img.data );
	cout << "best marker ID: "<< best << endl;
	
	*_infosForDebug = _arTagTracker.getDetectedMarker(0);
	_currLenForDebug = _arTagTracker.getNumDetectedMarkers();

	return numMark;
}



void MultiARTagBasedTracker::getPoseMatrix( cv::Mat &pose )
{
	if(_infosForDebug )
	{
//		cout << "getting the pose" << endl;
		const ARFloat* ARMa = _arTagTracker.getModelViewMatrix();
		pose = Mat::eye(3, 4, CV_32FC1);

		for( int i = 0; i < 3; i++ )
		{
			for( int j = 0; j < 4; j++ )
			{
				pose.at<float>(i,j) = ARMa[j*4+i];
				PRINTVAR(ARMa[j*4+i]);
				PRINTVAR(j*4+i);
			}
		}
		
//		PRINTVAR(pose.inv());
	}
	else
	{
		pose = Mat::eye(3,4,CV_32FC1);
	}
}


MultiARTagBasedTracker::~MultiARTagBasedTracker( ) { }