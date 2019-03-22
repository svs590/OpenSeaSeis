/* Copyright (c) Colorado School of Mines, 2013.*/
/* All rights reserved.                       */

#include "csSegyTraceHeader.h"
#include "csSegyHdrMap.h"
#include "csStandardHeaders.h"
#include "csSegyHeaderInfo.h"
#include "csSegyHeader.h"
#include "csFlexNumber.h"
#include "csFlexHeader.h"
#include "csByteConversions.h"
#include <cstdlib>
#include <cstring>
#include <string>

using namespace cseis_geolib;

csSegyTraceHeader::csSegyTraceHeader( csSegyHdrMap const* segyHdrMap ) {
  myHdrMapPtr = segyHdrMap;
  myHdrValues.resize(myHdrMapPtr->numHeaders());
  // Initialize
  for( int ihdr = 0; ihdr < myHdrMapPtr->numHeaders(); ihdr++ ) {
    myHdrValues[ihdr] = 0;
  }
}

csSegyTraceHeader::~csSegyTraceHeader() {
  if( !myHdrValues.empty() ) {
    myHdrValues.clear();
  }
}

void csSegyTraceHeader::readHeaderValues( byte_t const* buffer, bool doSwapEndian, bool autoScaleHeaders ) {
  if (myHdrMapPtr->numHeaders() > (int)myHdrValues.size())
    myHdrValues.resize(myHdrMapPtr->numHeaders());
	
  int nHeaders = numHeaders();
  for( int ihdr = 0; ihdr < nHeaders; ihdr++ ) {
    csSegyHeaderInfo const* info = myHdrMapPtr->header(ihdr);
    int byteLoc  = info->byteLoc;
    int byteSize = info->byteSize;
    if( info->inType == TYPE_SHORT || (info->inType == TYPE_INT && byteSize == 2) ) {
      if( doSwapEndian ) {
        myHdrValues[ihdr].setIntValue( (int)byte2Short_SWAP( &buffer[byteLoc] ) );
      }
      else {
        myHdrValues[ihdr].setIntValue( (int)byte2Short( &buffer[byteLoc] ) );
      }
    }
    else if( info->inType == TYPE_INT ) {
      if( doSwapEndian ) {
        myHdrValues[ihdr].setIntValue( byte2Int_SWAP( &buffer[byteLoc] ) );
      }
      else {
        myHdrValues[ihdr].setIntValue( byte2Int( &buffer[byteLoc] ) );
      }
    }
    else if( info->inType == TYPE_USHORT ) {
      if( doSwapEndian ) {
        myHdrValues[ihdr].setIntValue( (int)(unsigned short)byte2Short_SWAP( &buffer[byteLoc] ) );
      }
      else {
        myHdrValues[ihdr].setIntValue( (int)(unsigned short)byte2Short( &buffer[byteLoc] ) );
      }
    }
    else if( info->inType == TYPE_FLOAT ) { // FLOAT, 4 bytes
/*
      float f1 = 0.0;
      float f2 = 0.0;
      float f3 = 0.0;
      char c2[4];
      char c3[4];
      memcpy( &f1, &buffer[byteLoc], 4 );
      c2[0] = buffer[byteLoc+2];
      c2[1] = buffer[byteLoc+3];
      c2[2] = buffer[byteLoc+0];
      c2[3] = buffer[byteLoc+1];
      memcpy( &f2, c2, 4 );
      c3[0] = buffer[byteLoc+3];
      c3[1] = buffer[byteLoc+2];
      c3[2] = buffer[byteLoc+1];
      c3[3] = buffer[byteLoc+0];
      memcpy( &f3, c3, 4 );
      fprintf(stdout,"Header '%s': %f %f %f\n", info->name.c_str(), f1, f2, f3);
*/

      if( doSwapEndian ) {
        myHdrValues[ihdr].setFloatValue( byte2Float_SWAP( &buffer[byteLoc] ) );
      }
      else {
        myHdrValues[ihdr].setFloatValue( byte2Float( &buffer[byteLoc] ) );
      }
    }
    else if( info->inType == TYPE_STRING ) { // STRING
      char* text1 = new char[byteSize+1];
      memcpy(text1,&buffer[byteLoc],byteSize);
      text1[byteSize] = '\0';
      std::string text = text1;
      myHdrValues[ihdr].setStringValue( text );
      delete [] text1;
      /*
      char* text2 = new char[byteSize+1];
      for( int i = 0; i < byteSize;i++ ) {
        text2[byteSize-i-1] = buffer[byteLoc+i];
      }
      text2[byteSize] = '\0';
      delete [] text2;
      */
    }
    else if( info->inType == csSegyHdrMap::SEGY_HDR_TYPE_6BYTE ) { // Special 6-byte format
      int value;
      int power_of_10;
      if( doSwapEndian ) {
        value = byte2Int_SWAP( &buffer[byteLoc] );
        power_of_10 = (int)byte2Short_SWAP( &buffer[byteLoc+4] );
      }
      else {
        value = byte2Int( &buffer[byteLoc] );
        power_of_10 = (int)byte2Short( &buffer[byteLoc+4] );
      }
      myHdrValues[ihdr].setDoubleValue( (double)value * pow(10,power_of_10) );
    }
    //    fprintf(stdout,"Header %3d, byteLoc %3d, byteSize %2d: %d\n", ihdr, byteLoc, byteSize, myHdrValues[ihdr].intValue() );
  }

  // Special treatment for headers that need to be scaled:
  if( autoScaleHeaders ) {
    myHdrMapPtr->applyCoordinateScalar( myHdrValues );
  }

}

void csSegyTraceHeader::dump( byte_t const* buffer, bool doSwapEndian, FILE* fout ) {
/*  for( int i = 0; i < 240; i+= byteSize ) {
    int value;
    if( doSwapEndian ) {
      if( byteSize == 2 ) {
        value = (int)byte2Short_SWAP( &buffer[i] );
      }
      else if( byteSize == 4 ) {
        value = byte2Int_SWAP( &buffer[i] );
      }
    }
    else {
      if( byteSize == 2 ) {
        value = (int)byte2Short( &buffer[i] );
      }
      else if( byteSize == 4 ) {
        value = byte2Int( &buffer[i] );
      }
    }
    fprintf(stdout,"#%3d:  %d\n", i+1, value);
  }
*/
  fprintf(fout,"%5s  %8s  %8s\n","Byte#","2-byte","4-byte");
  for( int i = 0; i < 240; i+= 2 ) {
    short int value2;
    int value4 = 0;
//    bool isByte4 = (i % 4 == 0);
    bool isByte4 = (i < 237);
    if( doSwapEndian ) {
      value2 = byte2Short_SWAP( &buffer[i] );
      if( isByte4 ) {
        value4 = byte2Int_SWAP( &buffer[i] );
      }
    }
    else {
      value2 = byte2Short( &buffer[i] );
      if( isByte4 ) {
        value4 = byte2Int( &buffer[i] );
      }
    }
//    if( isByte4 ) {
      fprintf(fout,"# %-3d  %8d  %8d\n", i+1, (int)value2, value4);
//    }
//    else {
//      fprintf(fout,"# %-3d  %8d\n", i+1, (int)value2);
//    }
  }

}


void csSegyTraceHeader::writeHeaderValues( byte_t* buffer, bool doSwapEndian, bool autoScaleHeaders ) {
  int nHeaders = numHeaders();

  // Special treatment for headers that need to be scaled:
  if( autoScaleHeaders ) {
    myHdrMapPtr->applyCoordinateScalarWriting( myHdrValues );
  }

  for( int ihdr = 0; ihdr < nHeaders; ihdr++ ) {
    csSegyHeaderInfo const* info = myHdrMapPtr->header(ihdr);
    int byteLoc  = info->byteLoc;

    if( info->inType == TYPE_SHORT || (info->inType == TYPE_INT && info->byteSize == 2) ) {
      if( doSwapEndian ) {
        short2Byte_SWAP( (short)myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
      else {
        short2Byte( (short)myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
    }
    else if( info->inType == TYPE_INT ) {
      if( doSwapEndian ) {
        int2Byte_SWAP( myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
      else {
        int2Byte( myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
    }
    else if( info->inType == TYPE_USHORT ) {
      if( doSwapEndian ) {
        short2Byte_SWAP( (short)(unsigned short)myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
      else {
        short2Byte( (short)(unsigned short)myHdrValues[ihdr].intValue(), &buffer[byteLoc] );
      }
    }
    else if( info->inType == TYPE_FLOAT ) { // FLOAT, 4 bytes
      if( doSwapEndian ) {
        float2Byte_SWAP( (float)myHdrValues[ihdr].doubleValue(), &buffer[byteLoc] );
      }
      else {
        float2Byte( (float)myHdrValues[ihdr].doubleValue(), &buffer[byteLoc] );
      }
    }
  }

  // Special treatment for scalars:
  // Reverse polarity for output scalars, so that on input the data is inversely scaled
  if( autoScaleHeaders ) {
    int scalarCoord = -myHdrValues[myHdrMapPtr->getCoordScalarHeaderIndex()].intValue();
    int scalarElev  = -myHdrValues[myHdrMapPtr->getElevScalarHeaderIndex()].intValue();
    int scalarStat  = -myHdrValues[myHdrMapPtr->getStatScalarHeaderIndex()].intValue();
    if( doSwapEndian ) {
      short2Byte_SWAP( scalarCoord, &buffer[csSegyHeader::BYTE_LOC_SCALAR_COORD] );
      short2Byte_SWAP( scalarElev, &buffer[csSegyHeader::BYTE_LOC_SCALAR_ELEV] );
      short2Byte_SWAP( scalarStat, &buffer[csSegyHeader::BYTE_LOC_SCALAR_STAT] );
    }
    else {
      short2Byte( scalarCoord, &buffer[csSegyHeader::BYTE_LOC_SCALAR_COORD] );
      short2Byte( scalarElev, &buffer[csSegyHeader::BYTE_LOC_SCALAR_ELEV] );
      short2Byte( scalarStat, &buffer[csSegyHeader::BYTE_LOC_SCALAR_STAT] );
    }
  }

}

int csSegyTraceHeader::numHeaders() const {
  return myHdrMapPtr->numHeaders();
}

int csSegyTraceHeader::intValue( int hdrIndex ) const {
  if( hdrIndex < numHeaders() ) {
    return myHdrValues[hdrIndex].intValue();
  }
  else {
    throw( csException("hdrIntValue: Wrong header index passed") );
  }
}
float csSegyTraceHeader::floatValue( int hdrIndex ) const {
  if( hdrIndex < numHeaders() ) {
    return myHdrValues[hdrIndex].floatValue();
  }
  else {
    throw( csException("hdrFloatValue: Wrong header index passed") );
  }
}
double csSegyTraceHeader::doubleValue( int hdrIndex ) const {
  if( hdrIndex < numHeaders() ) {
    return myHdrValues[hdrIndex].doubleValue();
  }
  else {
    throw( csException("hdrDoubleValue: Wrong header index passed") );
  }
}

std::string csSegyTraceHeader::stringValue( int hdrIndex ) const {
  return myHdrValues[hdrIndex].stringValue();
}

cseis_geolib::csFlexHeader* csSegyTraceHeader::getHandleHdrValues() {
  return myHdrValues.data();
}


//-----------------------------------------------------------------------------------------
char const* csSegyTraceHeader::headerDesc( int hdrIndex ) const {
  if( hdrIndex >= myHdrMapPtr->numHeaders() ) {
    throw( csException("csSegyTraceHeader::headerDesc: Passed wrong hdrIndex.") );
  }
  return myHdrMapPtr->header( hdrIndex )->description.c_str();
}
//-----------------------------------------------------------------------------------------
int csSegyTraceHeader::headerIndex( std::string name ) const {
  return myHdrMapPtr->headerIndex(name);
}
//-----------------------------------------------------------------------------------------
char const* csSegyTraceHeader::headerName( int hdrIndex ) const {
  if( hdrIndex >= myHdrMapPtr->numHeaders() ) {
    throw( csException("csSegyTraceHeader::headerName: Passed wrong hdrIndex %d.", hdrIndex) );
  }
  return myHdrMapPtr->header( hdrIndex )->name.c_str();
}
//-----------------------------------------------------------------------------------------
type_t csSegyTraceHeader::headerType( int hdrIndex ) const {
  if( hdrIndex >= myHdrMapPtr->numHeaders() ) {
    throw( csException("csSegyTraceHeader::headerType: Passed wrong hdrIndex.") );
  }
  return myHdrMapPtr->header( hdrIndex )->outType;
}

void csSegyTraceHeader::setIntValue( int hdrIndex, int value ) {
  if( hdrIndex < myHdrMapPtr->numHeaders() ) {
    myHdrValues[hdrIndex].setIntValue( value );
  }
  else { 
    throw( csException("setIntValue: Wrong header index passed") );
  }
}
void csSegyTraceHeader::setFloatValue( int hdrIndex, float value ) {
  if( hdrIndex < myHdrMapPtr->numHeaders() ) {
    myHdrValues[hdrIndex].setFloatValue( value );
  }
  else {
    throw( csException("setFloatValue: Wrong header index passed") );
  }
}
void csSegyTraceHeader::setDoubleValue( int hdrIndex, double value ) {
  if( hdrIndex < myHdrMapPtr->numHeaders() ) {
    myHdrValues[hdrIndex].setDoubleValue( value );
  }
  else {
    throw( csException("setDoubleValue: Wrong header index passed") );
  }
}
void csSegyTraceHeader::setStringValue( int hdrIndex, std::string const& value ) {
  throw( csException("setStringValue: Not implemented yet") );
}

csSegyTraceHeader::csSegyTraceHeader(const csSegyTraceHeader &trcHdr) {
	myHdrValues = trcHdr.myHdrValues;
	myHdrMapPtr = trcHdr.myHdrMapPtr;
}

