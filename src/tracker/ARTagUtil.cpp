/* 
 * File:   ARTagUtil.cpp
 * Author: sgaspari
 * 
 * Created on December 11, 2014, 9:54 PM
 */

#include "tracker/ARTagUtil.hpp"

/**
 * Determine whether a detected marker is close to the border of the image
 * @param minfo the marker to process
 * @param w the image width
 * @param h the image height
 * @param d the threshold distance from the border
 * @return true if the marker is close to the image border
 */
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

