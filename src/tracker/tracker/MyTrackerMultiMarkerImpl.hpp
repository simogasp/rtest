/* 
 * File:   MyTrackerMultiMarkerImpl.hpp
 * Author: sgaspari
 *
 * Created on December 12, 2014, 9:37 AM
 */

#ifndef MYTRACKERMULTIMARKERIMPL_HPP
#define	MYTRACKERMULTIMARKERIMPL_HPP


#include <ARToolKitPlus/TrackerMultiMarkerImpl.h>

#define MYARMM_TEMPL_FUNC template <int __PATTERN_SIZE_X, int __PATTERN_SIZE_Y, int __PATTERN_SAMPLE_NUM, int __MAX_LOAD_PATTERNS, int __MAX_IMAGE_PATTERNS>
#define MYARMM_TEMPL_TRACKER MyTrackerMultiMarkerImpl<__PATTERN_SIZE_X, __PATTERN_SIZE_Y, __PATTERN_SAMPLE_NUM, __MAX_LOAD_PATTERNS, __MAX_IMAGE_PATTERNS>


/// MyTrackerMultiMarkerImpl is just a derived class reimplementing the init method so that a
/// NULL cameraFile can be passed 
/**
 *  __PATTERN_SIZE_X describes the pattern image width (16 by default).
 *  __PATTERN_SIZE_Y describes the pattern image height (16 by default).
 *  __PATTERN_SAMPLE_NUM describes the maximum resolution at which a pattern is sampled from the camera image
 *  (64 by default, must a a multiple of __PATTERN_SIZE_X and __PATTERN_SIZE_Y).
 *  __MAX_LOAD_PATTERNS describes the maximum number of pattern files that can be loaded.
 *  __MAX_IMAGE_PATTERNS describes the maximum number of patterns that can be analyzed in a camera image.
 *  Reduce __MAX_LOAD_PATTERNS and __MAX_IMAGE_PATTERNS to reduce memory footprint.
 */
template <int __PATTERN_SIZE_X, int __PATTERN_SIZE_Y, int __PATTERN_SAMPLE_NUM, int __MAX_LOAD_PATTERNS=32, int __MAX_IMAGE_PATTERNS=32>
class MyTrackerMultiMarkerImpl : public ARToolKitPlus::TrackerMultiMarkerImpl<__PATTERN_SIZE_X,__PATTERN_SIZE_Y, __PATTERN_SAMPLE_NUM, __MAX_LOAD_PATTERNS, __MAX_IMAGE_PATTERNS>
{
public:
	MyTrackerMultiMarkerImpl(int nWidth=320, int nHeight=240);
	~MyTrackerMultiMarkerImpl();

	/// initializes ARToolKit
	/// initializes TrackerSingleMarker
	/**
	 *  nCamParamFile is the name of the camera parameter file
	 *  nNearClip & nFarClip are near and far clipping values for the OpenGL projection matrix
	 *  nLogger is an instance which implements the ARToolKit::Logger interface
	 */
	bool init(const char* nCamParamFile, const char* nMultiFile, ARFloat nNearClip, ARFloat nFarClip,
			  ARToolKitPlus::Logger* nLogger=NULL);
	
};

#include "../MyTrackerMultiMarkerImpl.cpp"

#endif	/* MYTRACKERMULTIMARKERIMPL_HPP */

