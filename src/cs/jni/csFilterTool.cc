#include "csFFTTools.h"
#include "csFilterTool.h"
#include <cmath>
#include <cstring>

using namespace cseis_jni;
using namespace cseis_geolib;

csFilterTool::csFilterTool( int numSamples, float sampleInt )  : cseis_geolib::csFFTTools( numSamples, sampleInt ) {
  mySamples = new float[numSamples];
}
csFilterTool::csFilterTool( int numSamples ) : cseis_geolib::csFFTTools( numSamples ) {
  mySamples = new float[numSamples];
}
csFilterTool::csFilterTool( int numSamplesIn, int numSamplesOut, float sampleIntIn, float sampleIntOut ) : cseis_geolib::csFFTTools(numSamplesIn, numSamplesOut, sampleIntIn, sampleIntOut) {
  mySamples = new float[numSamplesIn];
}

csFilterTool::~csFilterTool() {
  if( mySamples != NULL ) {
    delete [] mySamples;
    mySamples = NULL;
  }
}

void csFilterTool::setParam( float freqLowPass, float slopeLowPass, float freqHighPass, float slopeHighPass ) {
  myFreqLowPass   = freqLowPass;
  myFreqHighPass  = freqHighPass;
  myOrderLowPass  = fabs(slopeLowPass) / 6.0;
  myOrderHighPass = fabs(slopeHighPass) / 6.0;
}

float* csFilterTool::retrieveSamplesPointer() {
  return mySamples;
}

void csFilterTool::applyFilter() {
  if( myFreqLowPass > 0 ) {
    lowPass( mySamples, myOrderLowPass, myFreqLowPass, false );
  }
  if( myFreqHighPass > 0 ) {
    highPass( mySamples, myOrderHighPass, myFreqHighPass, false );
  }
}
