#include "capi.h"
#include "csSegyHeaderInfo.h"
#include "geolib_defines.h"

#include <string>

using namespace cseis_geolib;

CS_CAPI int cseis_csNativeSegyHeaderInfo_getByteLoc(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->byteLoc;
}
CS_CAPI int cseis_csNativeSegyHeaderInfo_getByteSize(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->byteSize;
}
CS_CAPI type_t cseis_csNativeSegyHeaderInfo_getInType(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->inType;
}
CS_CAPI type_t cseis_csNativeSegyHeaderInfo_getOutType(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->outType;
}
CS_CAPI const char * cseis_csNativeSegyHeaderInfo_getName(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->name.c_str();
}
CS_CAPI const char * cseis_csNativeSegyHeaderInfo_getDescription(void *obj) {
	return static_cast<csSegyHeaderInfo*>(obj)->description.c_str();
}

CS_CAPI void cseis_csNativeSegyHeaderInfo_setByteLoc(void *obj, int val) {
	static_cast<csSegyHeaderInfo*>(obj)->byteLoc = val;
}
CS_CAPI void cseis_csNativeSegyHeaderInfo_setByteSize(void *obj, int val) {
	static_cast<csSegyHeaderInfo*>(obj)->byteSize = val;
}
CS_CAPI void cseis_csNativeSegyHeaderInfo_setInType(void *obj, type_t val) {
	static_cast<csSegyHeaderInfo*>(obj)->inType = val;
}
CS_CAPI void cseis_csNativeSegyHeaderInfo_setOutType(void *obj, type_t val) {
	static_cast<csSegyHeaderInfo*>(obj)->outType = val;
}
