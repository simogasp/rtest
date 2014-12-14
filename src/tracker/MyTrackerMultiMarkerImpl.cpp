/* 
 * File:   MyTrackerMultiMarkerImpl.cpp
 * Author: sgaspari
 * 
 * Created on December 12, 2014, 9:37 AM
 */

#include "tracker/MyTrackerMultiMarkerImpl.hpp"

#include <ARToolKitPlus/TrackerMultiMarker.h>
#include <ARToolKitPlus/TrackerImpl.h>
#include <ARToolKitPlus/Logger.h>


MYARMM_TEMPL_FUNC
MYARMM_TEMPL_TRACKER::MyTrackerMultiMarkerImpl(int nWidth, int nHeight)
{
	this->logger = NULL;

	this->screenWidth = nWidth;
	this->screenHeight = nHeight;
	
	this->useDetectLite = true;
	this->numDetected = 0;

	this->config = 0;

	this->thresh = 150;
}


MYARMM_TEMPL_FUNC
MYARMM_TEMPL_TRACKER::~MyTrackerMultiMarkerImpl()
{
	this->cleanup();
	if(this->config)
		this->arMultiFreeConfig(this->config);
}

/// initializes ARToolKit
/// initializes TrackerSingleMarker
/**
 *  nCamParamFile is the name of the camera parameter file
 *  nNearClip & nFarClip are near and far clipping values for the OpenGL projection matrix
 *  nLogger is an instance which implements the ARToolKit::Logger interface
 */
MYARMM_TEMPL_FUNC
bool MYARMM_TEMPL_TRACKER::init(const char* nCamParamFile, const char* nMultiFile, ARFloat nNearClip, ARFloat nFarClip,
		  ARToolKitPlus::Logger* nLogger )
{
		// init some "static" from TrackerMultiMarker
	//
	if(this->marker_infoTWO==NULL)
		this->marker_infoTWO = ARToolKitPlus::artkp_Alloc<ARToolKitPlus::ARMarkerInfo2>(MYARMM_TEMPL_TRACKER::MAX_IMAGE_PATTERNS);

	this->logger = nLogger;

	if(this->config)
		this->arMultiFreeConfig(this->config);

    if((this->config = this->arMultiReadConfigFile(nMultiFile)) == NULL )
        return false;

	if(this->logger)
		this->logger->artLogEx("INFO: %d markers loaded from config file", this->config->marker_num);
	
		//initialize applications
	if(nCamParamFile)
		return this->loadCameraFile(nCamParamFile, nNearClip, nFarClip);


    return true;
}
