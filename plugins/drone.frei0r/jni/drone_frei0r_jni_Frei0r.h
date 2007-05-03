/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class drone_frei0r_jni_Frei0r */

#ifndef _Included_drone_frei0r_jni_Frei0r
#define _Included_drone_frei0r_jni_Frei0r
#ifdef __cplusplus
extern "C" {
#endif
#undef drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_FILTER
#define drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_FILTER 0L
#undef drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_SOURCE
#define drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_SOURCE 1L
#undef drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_MIXER2
#define drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_MIXER2 2L
#undef drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_MIXER3
#define drone_frei0r_jni_Frei0r_F0R_PLUGIN_TYPE_MIXER3 3L
#undef drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_BGRA8888
#define drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_BGRA8888 0L
#undef drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_RGBA8888
#define drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_RGBA8888 1L
#undef drone_frei0r_jni_Frei0r_F0R_PARAM_BOOL
#define drone_frei0r_jni_Frei0r_F0R_PARAM_BOOL 0L
#undef drone_frei0r_jni_Frei0r_F0R_PARAM_DOUBLE
#define drone_frei0r_jni_Frei0r_F0R_PARAM_DOUBLE 1L
#undef drone_frei0r_jni_Frei0r_F0R_PARAM_COLOR
#define drone_frei0r_jni_Frei0r_F0R_PARAM_COLOR 2L
#undef drone_frei0r_jni_Frei0r_F0R_PARAM_POSITION
#define drone_frei0r_jni_Frei0r_F0R_PARAM_POSITION 3L
#undef drone_frei0r_jni_Frei0r_F0R_PARAM_STRING
#define drone_frei0r_jni_Frei0r_F0R_PARAM_STRING 4L
#undef drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_PACKED32
#define drone_frei0r_jni_Frei0r_F0R_COLOR_MODEL_PACKED32 2L
/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getName
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getName
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getAuthor
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getAuthor
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getPluginType
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getPluginType
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getColorModel
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getColorModel
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getFrei0rVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getFrei0rVersion
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getMajorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getMajorVersion
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getMinorVersion
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getMinorVersion
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    nParams
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_nParams
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getExplanation
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getExplanation
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    _openLibrary
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r__1openLibrary
  (JNIEnv *, jobject, jstring);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    _closeLibrary
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r__1closeLibrary
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_init
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    deinit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_deinit
  (JNIEnv *, jobject);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    construct
 * Signature: (II)J
 */
JNIEXPORT jlong JNICALL Java_drone_frei0r_jni_Frei0r_construct
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    destruct
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_destruct
  (JNIEnv *, jobject, jlong);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    update
 * Signature: (JD[I[I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_update
  (JNIEnv *, jobject, jlong, jdouble, jintArray, jintArray);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    update2
 * Signature: (JD[I[I[I[I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_update2
  (JNIEnv *, jobject, jlong, jdouble, jintArray, jintArray, jintArray, jintArray);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getParamName
  (JNIEnv *, jobject, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamType
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_drone_frei0r_jni_Frei0r_getParamType
  (JNIEnv *, jobject, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamExplanation
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_drone_frei0r_jni_Frei0r_getParamExplanation
  (JNIEnv *, jobject, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    setParamValue
 * Signature: (JLjava/lang/Object;I)V
 */
JNIEXPORT void JNICALL Java_drone_frei0r_jni_Frei0r_setParamValue
  (JNIEnv *, jobject, jlong, jobject, jint);

/*
 * Class:     drone_frei0r_jni_Frei0r
 * Method:    getParamValue
 * Signature: (JI)Ljava/lang/Object;
 */
JNIEXPORT jobject JNICALL Java_drone_frei0r_jni_Frei0r_getParamValue
  (JNIEnv *, jobject, jlong, jint);

#ifdef __cplusplus
}
#endif
#endif
