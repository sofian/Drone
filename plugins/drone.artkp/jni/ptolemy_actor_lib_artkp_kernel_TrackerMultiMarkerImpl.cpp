#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "ARToolKitPlus/Tracker.h"
#include "ARToolKitPlus/TrackerMultiMarker.h"
#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"
#include "ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.h"
#include "common.h"

/*
 * Class:     ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl
 * Method:    _createTrackerHandle
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl__1createTrackerHandle
  (JNIEnv *env, jobject obj)
{
  // return default constructor
  ARToolKitPlus::TrackerMultiMarker *tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, 1, 16>();
  return (jlong) tracker;
}
