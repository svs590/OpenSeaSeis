#include "csStandardHeaders.h"
#include "csSegyTraceHeader.h"
#include "csSegyHdrMap.h"
#include "csSegyReader.h"
#include "csSegyHeaderInfo.h"
#include "csSegyBinHeader.h"
#include "csException.h"
#include "csVector.h"
#include "csFlexHeader.h"
#include "csFlexNumber.h"
#include "geolib_defines.h"
#include "capi.h"

#include <string>

using namespace cseis_geolib;

CS_CAPI void *cseis_csNativeSegyBinHeader_createInstance(bool doSwapEndian) {
	return reinterpret_cast<void*>(new csSegyBinHeader(doSwapEndian));
}

CS_CAPI int cseis_csNativeSegyBinHeader_jobID(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->jobID;
}

CS_CAPI int cseis_csNativeSegyBinHeader_lineNum(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->lineNum;
}

CS_CAPI int cseis_csNativeSegyBinHeader_reelNum(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->reelNum;
}

CS_CAPI int cseis_csNativeSegyBinHeader_numTraces(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->numTraces;
}

CS_CAPI int cseis_csNativeSegyBinHeader_numAuxTraces(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->numAuxTraces;
}

CS_CAPI int cseis_csNativeSegyBinHeader_sampleIntUS(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sampleIntUS;
}

CS_CAPI int cseis_csNativeSegyBinHeader_sampleIntOrigUS(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sampleIntOrigUS;
}

CS_CAPI int cseis_csNativeSegyBinHeader_numSamples(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->numSamples;
}

CS_CAPI int cseis_csNativeSegyBinHeader_numSamplesOrig(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->numSamplesOrig;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_dataSampleFormat(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->dataSampleFormat;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_fold(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->fold;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_sortCode(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sortCode;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_vertSumCode(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->vertSumCode;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_sweepFreqStart(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sweepFreqStart;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_sweepFreqEnd(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sweepFreqEnd;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_sweepCode(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->sweepCode;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_taperType(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->taperType;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_correlatedTraces(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->correlatedTraces;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_gainRecovered(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->gainRecovered;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_ampRecoveryMethod(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->ampRecoveryMethod;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_unitSystem(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->unitSystem;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_polarity(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->polarity;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_vibPolarityCode(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->vibPolarityCode;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_revisionNum(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->revisionNum;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_fixedTraceLengthFlag(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->fixedTraceLengthFlag;
}

CS_CAPI unsigned short cseis_csNativeSegyBinHeader_numExtendedBlocks(const void *obj) {
	return reinterpret_cast<const csSegyBinHeader*>(obj)->numExtendedBlocks;
}