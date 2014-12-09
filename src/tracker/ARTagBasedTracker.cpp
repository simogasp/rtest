/* 
 * File:   ARTagBasedTracker.cpp
 * Author: sgaspari
 * 
 * Created on December 8, 2014, 11:13 PM
 */

#include "tracker/ARTagBasedTracker.hpp"

using namespace std;
using namespace cv;

ARTagBasedTracker::ARTagBasedTracker( ) { }

void ARTagBasedTracker::init( const Parameters & args, const Camera &cam )
{
	
}

std::size_t ARTagBasedTracker::detectMarkers( const Mat & img )
{
	
}

void ARTagBasedTracker::visualDebug( const Mat & img )
{
	CvFont font1, font2;
	cvInitFont( &font1, CV_FONT_HERSHEY_SIMPLEX, 0.8, 0.8, 0, 2 );
	cvInitFont( &font2, CV_FONT_HERSHEY_PLAIN, 1, 1, 0, 2 );

	for ( int i = 0; i < _currLenForDebug; i++ )
	{
		if ( ( _infosForDebug + i )->id != NOTAMARKER )
		{
			char str[10];

			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[0][0], ( int ) ( _infosForDebug + i )->vertex[0][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[1][0], ( int ) ( _infosForDebug + i )->vertex[1][1] ),
					CV_RGB( 255, 255, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[1][0], ( int ) ( _infosForDebug + i )->vertex[1][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[2][0], ( int ) ( _infosForDebug + i )->vertex[2][1] ),
					CV_RGB( 255, 255, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[2][0], ( int ) ( _infosForDebug + i )->vertex[2][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[3][0], ( int ) ( _infosForDebug + i )->vertex[3][1] ),
					CV_RGB( 255, 255, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[0][0], ( int ) ( _infosForDebug + i )->vertex[0][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[3][0], ( int ) ( _infosForDebug + i )->vertex[3][1] ),
					CV_RGB( 255, 255, 0 ), 2 );

			sprintf( str, "%d", ( _infosForDebug + i )->id );
			cvPutText( img, str, cvPoint( ( int ) ( _infosForDebug + i )->pos[0], ( int ) ( _infosForDebug + i )->pos[1] ), &font1, CV_RGB( 255, 255, 0 ) );
		}
		else
		{
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[0][0], ( int ) ( _infosForDebug + i )->vertex[0][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[1][0], ( int ) ( _infosForDebug + i )->vertex[1][1] ),
					CV_RGB( 255, 0, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[1][0], ( int ) ( _infosForDebug + i )->vertex[1][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[2][0], ( int ) ( _infosForDebug + i )->vertex[2][1] ),
					CV_RGB( 255, 0, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[2][0], ( int ) ( _infosForDebug + i )->vertex[2][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[3][0], ( int ) ( _infosForDebug + i )->vertex[3][1] ),
					CV_RGB( 255, 0, 0 ), 2 );
			cvLine( img,
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[0][0], ( int ) ( _infosForDebug + i )->vertex[0][1] ),
					cvPoint( ( int ) ( _infosForDebug + i )->vertex[3][0], ( int ) ( _infosForDebug + i )->vertex[3][1] ),
					CV_RGB( 255, 0, 0 ), 2 );
		}
	}	
}

void ARTagBasedTracker::ignoreMarkerNextDetection( const MarkerID id )
{
	for( int i = 0; i < _currLenForDebug; ++i )
	{
		if ( _infosForDebug[i].id == (int)id )
		{
			cout << "removing marker ID " << id << " from artk+" );
			_infosForDebug[i].id = NOTAMARKER;
			return;
		}
	}
}

ARTagBasedTracker::~ARTagBasedTracker( ) { }

