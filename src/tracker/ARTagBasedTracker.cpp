/* 
 * File:   ARTagBasedTracker.cpp
 * Author: sgaspari
 * 
 * Created on December 8, 2014, 11:13 PM
 */

#include "tracker/ARTagUtil.hpp"
#include "tracker/ARTagBasedTracker.hpp"
#include "tracker/utility.hpp"

#include <opencv2/core/core.hpp>

#include <iostream>

using namespace std;
using namespace cv;



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
	
	// RPP is more robust than ARToolKit's standard pose estimator
	_arTagTracker.setPoseEstimator( ARToolKitPlus::POSE_ESTIMATOR_ORIGINAL );

	_currLenForDebug = 0;

	_params = params;
}

std::size_t ARTagBasedTracker::detectMarkers( const Mat & img )
{

	size_t numMark = 0;
	MarkerID best = 0;
	cout << "getting the pose" << endl;
	best = _arTagTracker.calc( (unsigned char*) img.data, -1, true, &_infosForDebug, &_currLenForDebug );
	cout << "best marker ID: "<< best << endl;

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

void ARTagBasedTracker::getPoseMatrix( cv::Mat &pose, const OcvCamera &cam )
{
	if(_infosForDebug )
	{
////		cout << "getting the pose" << endl;
//		const ARFloat* ARMa = _arTagTracker.getModelViewMatrix();
//		pose = Mat::eye(3, 4, CV_32FC1);
//
//		for( int i = 0; i < 3; i++ )
//		{
//			for( int j = 0; j < 4; j++ )
//			{
//				pose.at<float>(i,j) = ARMa[j*4+i];
//				PRINTVAR(ARMa[j*4+i]);
//				PRINTVAR(j*4+i);
//			}
//		}
		
		// get the 4 vertices
		vector<Point2f> vx;
		vector<Point2f> vgt;
		vx.reserve(4);
		vgt.reserve(4);
		vgt.push_back(Point2f(0.0, 0.0));
		vgt.push_back(Point2f(0.0, 80.0));
		vgt.push_back(Point2f(80.0, 80.0));
		vgt.push_back(Point2f(80.0, 0.0));
		for(int i = 0; i < 4; ++i )
		{
			vx.push_back(Point2f(_infosForDebug[0].vertex[i][0], _infosForDebug[0].vertex[i][1]));
			
		}
		
		PRINTVAR(vx);
		PRINTVAR(vgt);
		Mat K;
		cam.matK.convertTo(K, CV_32F);
		computePose(vgt, vx, K, pose );
		
		PRINTVAR(pose);
	}
	else
	{
		pose = Mat::eye(3,4,CV_32FC1);
	}
}

ARTagBasedTracker::~ARTagBasedTracker( ) { }

