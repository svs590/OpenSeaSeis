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
#include <iostream>
using namespace std;

using namespace cseis_geolib;


/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_createInstance
 */
CS_CAPI void *cseis_csNativeSegyReader_createInstance(
	const char *filename_in,
	int nTracesBuffer,
	int segyHeaderMap,
	bool reverseByteOrderData_in,
	bool reverseByteOrderHdr_in,
	bool autoscale_hdrs_in)
{
	csSegyReader::SegyReaderConfig config;
	config.enableRandomAccess = true;

	if (reverseByteOrderData_in) {
		config.reverseByteOrderData = true;
	}
	else {
		config.reverseByteOrderData = false;
	}
	if (reverseByteOrderHdr_in) {
		config.reverseByteOrderHdr = true;
	}
	else {
		config.reverseByteOrderHdr = false;
	}
	if (autoscale_hdrs_in) {
		config.autoscaleHdrs = true;
	}
	else {
		config.autoscaleHdrs = false;
	}
	config.segyHeaderMapping = segyHeaderMap;
	config.numTracesBuffer = nTracesBuffer;
	// Force SU format when SU header map is selected:
	config.isSUFormat = (segyHeaderMap == csSegyHdrMap::SEGY_SU || segyHeaderMap == csSegyHdrMap::SEGY_SU_BOTH);

	csSegyReader* reader = nullptr;
	try {
		reader = new csSegyReader(std::string(filename_in), config);
	}
	catch (csException& e) {
		fprintf(stderr, "Error when opening SEGY file: %s\n", e.getMessage());
		fflush(stderr);
		
		return nullptr;
	}

	try {
		reader->initialize();
	}
	catch (csException& e) {
		delete reader;
		fprintf(stderr, "Error when initializing SEGY reader object.\nSystem message: %s", e.getMessage());
		fflush(stderr);

		return nullptr;
	}

	return (reinterpret_cast<void*>(reader));
}

CS_CAPI void cseis_csNativeSegyReader_deleteInstance(void *obj) {
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	delete ptr;
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_closeFile
 */
CS_CAPI void cseis_csNativeSegyReader_closeFile
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	ptr->closeFile();
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_sampleByteSize
 */
CS_CAPI int cseis_SegyReader_sampleByteSize
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return ptr->sampleByteSize();
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_numTraces
 */
CS_CAPI int cseis_SegyReader_numTraces
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->numTraces());
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_numSamples
 */
CS_CAPI int cseis_SegyReader_numSamples
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->numSamples());
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_numHeaders
 */
CS_CAPI int cseis_SegyReader_numHeaders
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return ptr->numTraceHeaders();
}

/*
 * Class:     cseis_csNativeSegyReader
 * Method:    native_sampleInt
 */
CS_CAPI float cseis_SegyReader_sampleInterval
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->sampleIntMS());
}

CS_CAPI const void *cseis_SegyReader_binHeader(void *obj)
{
	return reinterpret_cast<csSegyReader*>(obj)->binHdr();
}

CS_CAPI const char *cseis_csSegyReader_1charHeader(void *obj)
{
	return reinterpret_cast<csSegyReader*>(obj)->charHdrBlock();
}

CS_CAPI int cseis_csReader_1hdrIntValue(void *obj, int hdrIndex)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->getTraceHeader()->intValue(hdrIndex));
}

CS_CAPI float cseis_csSegyReader_1hdrFloatValue(void *obj, int hdrIndex)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->getTraceHeader()->floatValue(hdrIndex));
}

CS_CAPI double cseis_csSegyReader_1hdrDoubleValue(void *obj, int hdrIndex)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	return(ptr->getTraceHeader()->doubleValue(hdrIndex));
}

CS_CAPI bool cseis_csSegyReader_1moveToTrace
(void *obj, int firstTraceIndex, int numTracesToRead)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	try {
		if (ptr->moveToTrace(firstTraceIndex, numTracesToRead)) {
			return true;
		}
	}
	catch (csException& e) {
		printf("Error when moving to new position in SEGY file: %s\n", e.getMessage());
		fflush(stderr);
	}
	return false;
}

CS_CAPI bool cseis_csSegyReader_2moveToTrace
(void *obj, int firstTraceIndex)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	try {
		if (ptr->moveToTrace(firstTraceIndex)) {
			return true;
		}
	}
	catch (csException& e) {
		printf("Error when moving to new position in SEGY file: %s\n", e.getMessage());
		fflush(stderr);
	}
	return false;
}

CS_CAPI const float *cseis_csSegyReader_1getNextTrace
(void *obj)
{
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);

	try {
		return ptr->getNextTracePointer();
	}
	catch (csException& e) {
		printf("Error when reading from SEGY file: %s\n", e.getMessage());
		fflush(stderr);
		
		return nullptr;
	}
}

CS_CAPI const void * const cseis_csSegyReader_getTrcHdrMap(void *obj) {
	csSegyReader* ths = reinterpret_cast<csSegyReader*>(obj);
	return static_cast<const void*>(ths->getTrcHdrMap());
}

CS_CAPI void cseis_csNativeSegyReader_charBinHeader(void *obj, char *buf) {
	csSegyReader* ptr = reinterpret_cast<csSegyReader*>(obj);
	memcpy(buf, ptr->charHdrBlock(), csSegyHeader::SIZE_CHARHDR * sizeof(char));
}

