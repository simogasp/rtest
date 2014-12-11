/* 
 * File:   simpleDetection.cpp
 * Author: sgaspari
 *
 * Created on December 9, 2014, 2:07 PM
 */



#include "tracker/ARTagBasedTracker.hpp"
#include "tracker/utility.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace std;
using namespace cv;


// Display the help for the programm
void help( const char* programName );

// parse the input command line arguments
bool parseArgs( int argc, char**argv, string &inputFilename, int &cameraID, bool &useCamera, string &calibFile );

/*
 * 
 */
int main( int argc, char** argv )
{

	// the name of the window
	const string WINDOW_NAME = "Image View";

    // it will contains the filename of the image file
    string inputFilename;
	
	// it will contains the filename of the image file
    string calibFilename;

	int cameraID = 0;
	bool useCamera = false;
	
	// Used to load the video and get the frames
    VideoCapture capture;

    // Camera object containing the calibration parameters
    OcvCamera cam;

    // Camera Tracker object
    ARTagBasedTracker tracker;

    if( !parseArgs( argc, argv, inputFilename, cameraID, useCamera, calibFilename) )
    {
    	cerr << "Aborting..." << endl;
    	return EXIT_FAILURE;
    }

	namedWindow( WINDOW_NAME, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO );

	if( useCamera )
	{
		capture.open( cameraID );
		cerr << "live" << endl;
	}
	else
	{
		capture.open(  inputFilename  );
		cerr << "file " << inputFilename << endl;
	}


	// check it is really opened
	if( !capture.isOpened() )
	{
		cerr << "Could not open video file " << inputFilename << endl;
		return EXIT_FAILURE;
	}
	else if( useCamera )
	{
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	}

	// init the Camera loading the calibration parameters
	cam.init( calibFilename );
	
	tracker.init( ARTagBasedTracker::Parameters(), cam );


	// processing loop
	while(1)
	{
        Mat view;
		
		// get the new frame from capture and copy it to view
        capture >> view;

       // if no more images to process exit the loop
        if(view.empty())
        	break;

        // detect the markers
        if( tracker.detectMarkers( view ) > 0)
        {
            //******************************************************************/   	
            // draw the reference on top of the image
			//******************************************************************/
        	tracker.visualDebug( view );
			Mat pose;
			tracker.getPoseMatrix( pose );
			PRINTVAR(pose);
			drawReferenceSystem( view, cam, pose, 4, 15, true );
        }

		/******************************************************************/
		// show the image inside the window --> see imshow
		/******************************************************************/
		imshow( WINDOW_NAME, view);
		
		// wait 20ms for user input before processing the next frame
		// Any user input will stop the execution
//#if DEBUGGING		
//		if (( waitKey( -1 ) & 0xff ) == 'q')
//			break;
//#else
//		if( waitKey( 10 ) >= 0) 
//			break;
//#endif
	}

	/******************************************************************/
	// release the video resource
	/******************************************************************/
	capture.release();
    
                                               
    return EXIT_SUCCESS;
}

// Display the help for the programm
void help( const char* programName )
{
    cout << "Detect the artags" << endl
        << "Usage: " << programName  << endl
        << "     -c <calib file>                                   # the name of the calibration file" << endl
		<< "     <video file|camera index>                         # the name of the video file or the camera id number for a live feed" << endl
        << endl;
}

// parse the input command line arguments
bool parseArgs( int argc, char**argv, string &inputFilename, int &cameraID, bool &useCamera, string &calibFile )
{
	// check the minimum number of arguments
    if( argc != 4 )
    {
        help( argv[0] );
        return false;
    }


	// Read the input arguments
    for( int i = 1; i < argc; i++ )
    {
		const char* s = argv[i];
        if( s[0] != '-' )
        {
			if( isdigit(s[0]) )
			{
                sscanf(s, "%d", &cameraID);
				useCamera = true;
				cerr << "using live feed on camera #" << cameraID << endl;
			}
            else
			{
				inputFilename.assign(s);
				useCamera = false;
			}
        }        
        else if( strcmp( s, "-c" ) == 0 )
        {
            if( i+1<argc )
    			calibFile.assign(argv[++i]);
			else
			{
				cerr << "Missing argument for option " << s << endl;
           		return false;
			}	
        }
        else
        { 
        	cerr << "Unknown option " << s << endl;
           	return false;
        }
    }

    return true;
}

