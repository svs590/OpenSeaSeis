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

CS_CAPI void *cseis_csNativecsSegyTraceHeader_createInstance(const void *hdrMap) {
	auto map = reinterpret_cast<const csSegyHdrMap*>(hdrMap);
	auto newHeader = new csSegyTraceHeader(map);
	return reinterpret_cast<void*>(newHeader);
}

CS_CAPI void *cseis_csNativecsSegyTraceHeader_copyInstance(const void *obj) {
	auto to_copy = reinterpret_cast<const csSegyTraceHeader*>(obj);
	auto newHeader = new csSegyTraceHeader(*to_copy);
	return reinterpret_cast<void*>(newHeader);
}

CS_CAPI void cseis_csNativecsSegyTraceHeader_deleteInstance(const void *obj) {
	delete reinterpret_cast<const csSegyTraceHeader*>(obj);
}

CS_CAPI int cseis_csNativecsSegyTraceHeader_numHeaders(const void *obj) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->numHeaders();
}

CS_CAPI char const* cseis_csNativecsSegyTraceHeader_headerDesc(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->headerDesc(hdrIndex);
}

CS_CAPI char const* cseis_csNativecsSegyTraceHeader_headerName(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->headerName(hdrIndex);
}

CS_CAPI int cseis_csNativecsSegyTraceHeader_headerIndex(const void *obj, const char *name) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->headerIndex(std::string(name));
}

CS_CAPI type_t cseis_csNativecsSegyTraceHeader_headerType(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->headerType(hdrIndex);
}

CS_CAPI int cseis_csNativecsSegyTraceHeader_intValue(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->intValue(hdrIndex);
}

CS_CAPI float cseis_csNativecsSegyTraceHeader_floatValue(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->floatValue(hdrIndex);
}

CS_CAPI double cseis_csNativecsSegyTraceHeader_doubleValue(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->doubleValue(hdrIndex);
}

CS_CAPI char const* cseis_csNativecsSegyTraceHeader_stringValue(const void *obj, int hdrIndex) {
	return reinterpret_cast<const csSegyTraceHeader*>(obj)->stringValue(hdrIndex).c_str();
}

// Not implemented
CS_CAPI int cseis_csNativecsSegyTraceHeader_byteLoc(const void *obj, int hdrIndex);
CS_CAPI int cseis_csNativecsSegyTraceHeader_bytePos(const void *obj, int hdrIndex);

CS_CAPI void cseis_csNativecsSegyTraceHeader_setIntValue(int hdrIndex, int value);
CS_CAPI void cseis_csNativecsSegyTraceHeader_setFloatValue(int hdrIndex, float value);
CS_CAPI void cseis_csNativecsSegyTraceHeader_setDoubleValue(int hdrIndex, double value);
CS_CAPI void cseis_csNativecsSegyTraceHeader_setStringValue(int hdrIndex, std::string const& value);

