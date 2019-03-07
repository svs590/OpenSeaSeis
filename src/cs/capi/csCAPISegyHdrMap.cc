#include "capi.h"
#include "csSegyHdrMap.h"
#include "geolib_defines.h"

#include <string>

using namespace cseis_geolib;

CS_CAPI int cseis_csNativeSegyHdrMap_getMapID(void *obj){
	return static_cast<csSegyHdrMap*>(obj)->mapID();
}
CS_CAPI bool cseis_csNativeSegyHdrMap_copyHeader(void *obj, void *hdr){
	return static_cast<csSegyHdrMap*>(obj)->addHeader(*(static_cast<csSegyHeaderInfo*>(hdr)));
}
CS_CAPI bool cseis_csNativeSegyHdrMap_addHeader(void *obj, const char *name, int byteLoc, int byteSize, type_t inType, const char *desc){
	return static_cast<csSegyHdrMap*>(obj)->addHeader(
		std::string(name),
		byteLoc,
		byteSize,
		inType,
		std::string(desc)
	);
}
CS_CAPI bool cseis_csNativeSegyHdrMap_removeHeaderByIndex(void *obj, int hdrIndex){
	return static_cast<csSegyHdrMap*>(obj)->removeHeader(hdrIndex);
}
CS_CAPI bool cseis_csNativeSegyHdrMap_removeHeaderByName(void *obj, const char *name){
	return static_cast<csSegyHdrMap*>(obj)->removeHeader(std::string(name));
}
CS_CAPI void cseis_csNativeSegyHdrMap_removeAll(void *obj){
	static_cast<csSegyHdrMap*>(obj)->removeAll();
}
CS_CAPI int cseis_csNativeSegyHdrMap_headerIndex(void *obj, const char *name){
	return static_cast<csSegyHdrMap*>(obj)->headerIndex(std::string(name));
}
CS_CAPI bool cseis_csNativeSegyHdrMap_contains(void *obj, const char *name, int* hdrIndex){
	return static_cast<csSegyHdrMap*>(obj)->contains(std::string(name), hdrIndex);
}

CS_CAPI const void * const cseis_csNativeSegyHdrMap_headerByIndex(void *obj, int hdrIndex){
	csSegyHdrMap *ths = static_cast<csSegyHdrMap*>(obj);
	return static_cast<const void*>(ths->header(hdrIndex));
}
CS_CAPI const void * const cseis_csNativeSegyHdrMap_headerByName(void *obj, const char *name){
	csSegyHdrMap *ths = static_cast<csSegyHdrMap*>(obj);
	return static_cast<const void*>(ths->header(std::string(name)));
}

CS_CAPI int cseis_csNativeSegyHdrMap_numHeaders(void *obj){
	return static_cast<csSegyHdrMap*>(obj)->numHeaders();
}

CS_CAPI void cseis_csNativeSegyHdrMap_lockUserDefenition(void *obj) {
	static_cast<csSegyHdrMap*>(obj)->lockUserDefenition();
}

CS_CAPI void cseis_csNativeSegyHdrMap_unlockUserDefenition(void *obj) {
	static_cast<csSegyHdrMap*>(obj)->unlockUserDefenition();
}