#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>

using namespace cv;
using namespace std;

// Display the help for the programm
void help( const char* programName );

// parse the input command line arguments
bool parseArgs( int argc, char**argv, string &inputFilename, bool &useCamera, int &cameraID );


int main( int argc, char** argv )
{
	/******************************************************************/
	/* CONSTANTS to use                         					  */
    /******************************************************************/

	// the name of the window
	const string WINDOW_NAME = "Image View";

	/******************************************************************/
	/* VARIABLES to use                         					  */
    /******************************************************************/
	
	vector<Point2f> pointbuf;	// it will contain the detected corners on the chessboard
	
	bool useCamera = false;					// it will be true if a chessboard is found, false otherwise
	int cameraId = 0;
    // it will contains the filename of the image file
    string inputFilename;
	
	// Used to load the video and get the frames
    VideoCapture capture;


	/******************************************************************/
	/* READ THE INPUT PARAMETERS - DO NOT MODIFY					  */
    /******************************************************************/

    if( !parseArgs( argc, argv, inputFilename, useCamera, cameraId ) )
    {
    	cerr << "Aborting..." << endl;
    	return EXIT_FAILURE;
    }

	
	/******************************************************************/
	// create a window to display the image --> see namedWindow
	/******************************************************************/
	namedWindow( WINDOW_NAME, CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO );

	/******************************************************************/
	// read the input video with capture
	/******************************************************************/
	if( useCamera )
	{
		capture.open( cameraId );
		
	}
	else
	{
		capture.open( string( inputFilename ) );
	}
	
	/******************************************************************/
	// check it is really opened
	/******************************************************************/
	if( !capture.isOpened() )
	{
		cerr << "Could not open video from " << inputFilename << endl;
		return EXIT_FAILURE;
	}
	else if ( useCamera )
	{
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	}

	SimpleBlobDetector blobDetector = SimpleBlobDetector();

	// processing loop
	while(1)
	{
        Mat view;
       	/******************************************************************/
		// get the new frame from capture and copy it to view
		/******************************************************************/
        capture >> view;

		/******************************************************************/
        // if no more images to process exit the loop
		/******************************************************************/
        if(!view.data)
        	break;

		Mat gray;
		cvtColor(view, gray, CV_RGB2GRAY);

//		vector<Vec3f> circles;
//		HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 2, 50, 200, 100 );
//
//
//		for( size_t i = 0; i < circles.size(); i++ )
//	   {
//			Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//			int radius = cvRound(circles[i][2]);
//			// draw the circle center
//			circle( view, center, 3, Scalar(0,255,0), -1, 8, 0 );
//			// draw the circle outline
//			circle( view, center, radius, Scalar(0,0,255), 3, 8, 0 );
//	   }
		
		std::vector<KeyPoint> keypoints;

		blobDetector.detect(view, keypoints);
		for (size_t i = 0; i < keypoints.size(); i++)
		{
			// draw the circle center
			circle( view, keypoints[i].pt, 3, Scalar(0,255,0), -1, 8, 0 );
			// draw the circle outline
			circle( view, keypoints[i].pt, keypoints[i].size, Scalar(0,0,255), 3, 8, 0 );
		}

		/******************************************************************/
		// show the image inside the window --> see imshow
		/******************************************************************/
		imshow( WINDOW_NAME, view);
		
		// wait 20ms for user input before processing the next frame
		// Any user input will stop the execution
		if( waitKey( 10 ) >= 0) 
			break;

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
    cout << "Detect circles in a given video" << endl
        << "Usage: " << programName  << endl
 		<< "     <video file/camera id> " << endl
        << endl;
}

// parse the input command line arguments
bool parseArgs( int argc, char**argv, string &inputFilename, bool &useCamera, int &cameraID )
{
	// check the minimum number of arguments
    if( argc != 2 )
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
			}
            else
			{
                inputFilename = s;
				useCamera = false;
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

