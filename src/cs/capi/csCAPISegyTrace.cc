#include "csSegyTrace.h"
#include "geolib_defines.h"
#include "capi.h"

#include <string>

using namespace cseis_geolib;

CS_CAPI void *cseis_csNativeSegyTrace_createInstance(int numSamples) {
	return reinterpret_cast<void*>(new csSegyTrace(numSamples));
}

CS_CAPI float const *cseis_csNativeSegyTrace_getSamples(void *obj) {
	return reinterpret_cast<csSegyTrace*>(obj)->getSamples();
}

//CS_CAPI void const *cseis_csNativeSegyTrace_getHeader(void *obj) {
//	return reinterpret_cast<csSegyTrace*>(obj)->getHeader();
//}

//CS_CAPI int cseis_csNativeSegyTrace_getInt(void *obj, int bytePos, int byteSize) {
//	return reinterpret_cast<csSegyTrace*>(obj)->getInt(bytePos, byteSize);
//}
//CS_CAPI float cseis_csNativeSegyTrace_getFloat(void *obj, int bytePos, int byteSize) {
//	return reinterpret_cast<csSegyTrace*>(obj)->getFloat(bytePos, byteSize);
//}
//CS_CAPI double cseis_csNativeSegyTrace_getDouble(void *obj, int bytePos, int byteSize) {
//	return reinterpret_cast<csSegyTrace*>(obj)->getDouble(bytePos, byteSize);
//}

/*
CS_CAPI std::string cseis_csNativeSegyTrace_getString(void *obj, int bytePos, int byteSize) {
	return reinterpret_cast<csSegyTrace*>(obj)->
}
*/