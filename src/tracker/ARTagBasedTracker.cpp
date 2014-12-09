/* 
 * File:   ARTagBasedTracker.cpp
 * Author: sgaspari
 * 
 * Created on December 8, 2014, 11:13 PM
 */

#include "tracker/ARTagBasedTracker.hpp"

#include <opencv2/core/core.hpp>

#include <iostream>

using namespace std;
using namespace cv;

bool verticesOnBorder( const ARToolKitPlus::ARMarkerInfo & minfo, const std::size_t w, const std::size_t h, const std::size_t d )
{
	for(std::size_t k = 0; k < 4; ++k)
	{
		if ( minfo.vertex[k][0] <= d || minfo.vertex[k][0] >= ( w - d ) )
		{
			return true;
		}
		if ( minfo.vertex[k][1] <= d || minfo.vertex[k][1] >= ( h - d ) )
		{
			return true;
		}
	}
	return false;
}

void MyARLogger::artLog( const char* nStr )
{
	printf("%s\n", nStr );
}


void ARTKCamera::loadFromK( const OcvCamera & cam )
{
	cc[0] = (float) cam.matK.at<double>( 0, 2 );
	cc[1] = (float)cam.matK.at<double>( 1, 2 );
	fc[0] = (float)cam.matK.at<double>( 0, 0 );
	fc[1] = (float)cam.matK.at<double>( 1, 1 );

	memset( kc, 0, 6 * sizeof (ARFloat ) );
	for ( std::size_t i = 0; i < cam.distCoeff.rows; ++i )
	{
		kc[i] = ( float ) cam.distCoeff.at<double>( i );
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

ARTagBasedTracker::ARTagBasedTracker( )
: _arTagTracker( 640, 480 )
, _infosForDebug( NULL )
, _currLenForDebug( 0 )
{ }

void ARTagBasedTracker::init( const Parameters & params, const OcvCamera &cam )
{
	// load a camera file. two types of camera files are supported:
	//  - Std. ARToolKit
	//  - MATLAB Camera Calibration Toolbox
	if ( !_arTagTracker.init( NULL, 1.0f, 1000.0f ) )
	{
		cerr << "ERROR: cannot init ar tracker!" << endl;
		return;
	}

	_arTagTracker.setCamera( new ARTKCamera( cam ) );

	_arTagTracker.setPixelFormat( params._pixelFormat );

	// define size of the marker
	_arTagTracker.setPatternWidth( params._patternWidth );

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

	_currLenForDebug = 0;

	_params = params;
}

std::size_t ARTagBasedTracker::detectMarkers( const Mat & img )
{

	size_t numMark = 0;
	_arTagTracker.calc( (unsigned char*) img.data, -1, false, &_infosForDebug, &_currLenForDebug );

	for( int i = 0; i < _currLenForDebug; ++i )
	{
		if ( verticesOnBorder( _infosForDebug[i], img.cols, img.rows, 16 ) )
		{
			_infosForDebug[i].id = NOTAMARKER;
		}
		if ( _infosForDebug[i].id != NOTAMARKER )
		{
			++numMark;
		}
	}

	return numMark;
}

void ARTagBasedTracker::visualDebug(  Mat & img )
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

void ARTagBasedTracker::ignoreMarkerNextDetection( const MarkerID id )
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

ARTagBasedTracker::~ARTagBasedTracker( ) { }

