#ifndef CS_FILTER_TOOL_H
#define CS_FILTER_TOOL_H

#include "csFFTTools.h"

namespace cseis_jni {

class csFilterTool : public cseis_geolib::csFFTTools {
 public:
  csFilterTool( int numSamples, float sampleInt );
  ~csFilterTool();
  void setParam( float freqLowPass, float slopeLowPass, float freqHighPass, float slopeHighPass );
  float* retrieveSamplesPointer();
  void applyFilter();
 private:
  csFilterTool( int numSamples );
  csFilterTool( int numSamplesIn, int numSamplesOut, float sampleIntIn, float sampleIntOut );

  float* mySamples;
  float myFreqHighPass;
  float myFreqLowPass;
  float myOrderLowPass;
  float myOrderHighPass;
};

} // END namespace
#endif
