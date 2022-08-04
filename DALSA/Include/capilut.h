//
// Coreco API - LUT functions.
//

#ifndef _CAPILUT_H_
#define _CAPILUT_H_

#include <capidef.h>

/****************************************************************************
*	LUT Capabilities
*
*	Last capability #: 0
****************************************************************************/
#define CORLUT_CAP_FIRST			0
#define CORLUT_CAP( n, size)	    ((((n) + CORLUT_CAP_FIRST) << 16) | (size))

/****************************************************************************
*	LUT Parameters
*
*	Last parameter #: 5
****************************************************************************/
#define CORLUT_PRM_FIRST        0
#define CORLUT_PRM( n, size)	((((n) + CORLUT_PRM_FIRST) << 16) | (size))

#define CORLUT_PRM_ADDRESS			CORLUT_PRM( 0, sizeof(void *))
#define CORLUT_PRM_DATASIZE		CORLUT_PRM( 1, 4)
#define CORLUT_PRM_FORMAT			CORLUT_PRM( 2, 4)
#define CORLUT_PRM_NPAGES			CORLUT_PRM( 3, 4)
#define CORLUT_PRM_NENTRIES		CORLUT_PRM( 4, 4)
#define CORLUT_PRM_SIZE				CORLUT_PRM( 5, 4)
#define CORLUT_PRM_PHYSADDRESS	CORLUT_PRM( 6, sizeof(ULONG_PTR))
#define CORLUT_PRM_SIGNED		   CORLUT_PRM( 7, 4)

#define CORLUT_VAL_FORMAT_UNSIGNED		CORDATA_FORMAT_UNSIGNED
#define CORLUT_VAL_FORMAT_SIGNED			CORDATA_FORMAT_SIGNED

#define CORLUT_VAL_FORMAT_MONO			CORDATA_FORMAT_MONO
#define CORLUT_VAL_FORMAT_COLOR		   CORDATA_FORMAT_COLOR

#define CORLUT_VAL_FORMAT_NINTRL			CORDATA_FORMAT_NINTRL
#define CORLUT_VAL_FORMAT_INTRL			CORDATA_FORMAT_INTRL

#define CORLUT_VAL_FORMAT_COLORI			(CORDATA_FORMAT_COLOR | CORDATA_FORMAT_INTRL)
#define CORLUT_VAL_FORMAT_COLORNI		(CORDATA_FORMAT_COLOR | CORDATA_FORMAT_NINTRL)

/* LUT Types	*/
#define CORLUT_TYPE_MONO8			0x00000000
#define CORLUT_TYPE_MONO16			0x00000001
#define CORLUT_TYPE_COLORI8		0x00000002
#define CORLUT_TYPE_COLORI16		0x00000003
#define CORLUT_TYPE_COLORNI8		0x00000004
#define CORLUT_TYPE_COLORNI16		0x00000005

/* Combined Format Definitions */

// Monochrome formats
#define CORLUT_VAL_FORMAT_MONO8	   CORDATA_FORMAT_UINT8
#define CORLUT_VAL_FORMAT_INT8	   CORDATA_FORMAT_INT8
#define CORLUT_VAL_FORMAT_UINT8	   CORDATA_FORMAT_UINT8

#define CORLUT_VAL_FORMAT_MONO9	   CORDATA_FORMAT_UINT9
#define CORLUT_VAL_FORMAT_INT9	   CORDATA_FORMAT_INT9
#define CORLUT_VAL_FORMAT_UINT9	   CORDATA_FORMAT_UINT9

#define CORLUT_VAL_FORMAT_MONO10	   CORDATA_FORMAT_UINT10
#define CORLUT_VAL_FORMAT_INT10	   CORDATA_FORMAT_INT10
#define CORLUT_VAL_FORMAT_UINT10	   CORDATA_FORMAT_UINT10

#define CORLUT_VAL_FORMAT_MONO11	   CORDATA_FORMAT_UINT11
#define CORLUT_VAL_FORMAT_INT11	   CORDATA_FORMAT_INT11
#define CORLUT_VAL_FORMAT_UINT11	   CORDATA_FORMAT_UINT11

#define CORLUT_VAL_FORMAT_MONO12	   CORDATA_FORMAT_UINT12
#define CORLUT_VAL_FORMAT_INT12	   CORDATA_FORMAT_INT12
#define CORLUT_VAL_FORMAT_UINT12	   CORDATA_FORMAT_UINT12

#define CORLUT_VAL_FORMAT_MONO13	   CORDATA_FORMAT_UINT13
#define CORLUT_VAL_FORMAT_INT13	   CORDATA_FORMAT_INT13
#define CORLUT_VAL_FORMAT_UINT13	   CORDATA_FORMAT_UINT13

#define CORLUT_VAL_FORMAT_MONO14	   CORDATA_FORMAT_UINT14
#define CORLUT_VAL_FORMAT_INT14	   CORDATA_FORMAT_INT14
#define CORLUT_VAL_FORMAT_UINT14	   CORDATA_FORMAT_UINT14

#define CORLUT_VAL_FORMAT_MONO15	   CORDATA_FORMAT_UINT15
#define CORLUT_VAL_FORMAT_INT15	   CORDATA_FORMAT_INT15
#define CORLUT_VAL_FORMAT_UINT15	   CORDATA_FORMAT_UINT15

#define CORLUT_VAL_FORMAT_MONO16	   CORDATA_FORMAT_UINT16
#define CORLUT_VAL_FORMAT_INT16	   CORDATA_FORMAT_INT16
#define CORLUT_VAL_FORMAT_UINT16	   CORDATA_FORMAT_UINT16

#define CORLUT_VAL_FORMAT_MONO32	   CORDATA_FORMAT_UINT32
#define CORLUT_VAL_FORMAT_INT32	   CORDATA_FORMAT_INT32
#define CORLUT_VAL_FORMAT_UINT32	   CORDATA_FORMAT_UINT32

// Color interlaced formats
#define CORLUT_VAL_FORMAT_COLORI8		CORDATA_FORMAT_COLORI8
#define CORLUT_VAL_FORMAT_COLORI9		CORDATA_FORMAT_COLORI9
#define CORLUT_VAL_FORMAT_COLORI10		CORDATA_FORMAT_COLORI10
#define CORLUT_VAL_FORMAT_COLORI11		CORDATA_FORMAT_COLORI11
#define CORLUT_VAL_FORMAT_COLORI12		CORDATA_FORMAT_COLORI12
#define CORLUT_VAL_FORMAT_COLORI13		CORDATA_FORMAT_COLORI13
#define CORLUT_VAL_FORMAT_COLORI14		CORDATA_FORMAT_COLORI14
#define CORLUT_VAL_FORMAT_COLORI15		CORDATA_FORMAT_COLORI15
#define CORLUT_VAL_FORMAT_COLORI16		CORDATA_FORMAT_COLORI16

// Color non-interlaced formats
#define CORLUT_VAL_FORMAT_COLORNI8		CORDATA_FORMAT_COLORNI8
#define CORLUT_VAL_FORMAT_COLORNI9		CORDATA_FORMAT_COLORNI9
#define CORLUT_VAL_FORMAT_COLORNI10		CORDATA_FORMAT_COLORNI10
#define CORLUT_VAL_FORMAT_COLORNI11		CORDATA_FORMAT_COLORNI11
#define CORLUT_VAL_FORMAT_COLORNI12		CORDATA_FORMAT_COLORNI12
#define CORLUT_VAL_FORMAT_COLORNI13		CORDATA_FORMAT_COLORNI13
#define CORLUT_VAL_FORMAT_COLORNI14		CORDATA_FORMAT_COLORNI14 
#define CORLUT_VAL_FORMAT_COLORNI15		CORDATA_FORMAT_COLORNI15 
#define CORLUT_VAL_FORMAT_COLORNI16		CORDATA_FORMAT_COLORNI16

/* Format Field Access Macros */
#define CORLUT_FORMAT_IS_COLOR( format)		CORDATA_FORMAT_IS_COLOR( format)
#define CORLUT_FORMAT_IS_INTRL( format)		CORDATA_FORMAT_IS_INTRL( format)
#define CORLUT_FORMAT_IS_SIGNED( format)		CORDATA_FORMAT_IS_SIGNED( format)
#define CORLUT_FORMAT_DATADEPTH( format)		CORDATA_FORMAT_DATADEPTH( format)
#define CORLUT_FORMAT_DATASIZE( format)		CORDATA_FORMAT_DATASIZE( format)
#define CORLUT_FORMAT_NPAGES( format)			CORDATA_FORMAT_NPAGES( format)
#define CORLUT_FORMAT_SET_SIGN(format, sign)	CORDATA_FORMAT_SET_SIGN(format, sign)

#define CORLUT_VAL_MAX_NENTRIES			0

#ifdef __cplusplus
extern "C" {
#endif

/* Application programming interface */

/*----------------------------------------------------------------------------*/
/* Lut creation and destruction */
CORSTATUS CORAPIFUNC CorLutFree( CORLUT hLut);
CORSTATUS CORAPIFUNC CorLutNew( CORSERVER hServer, UINT32 nEntries, UINT32 format, CORLUT *hLut);
CORSTATUS CORAPIFUNC CorLutNewFromFile( CORSERVER hServer, PCSTR filename, CORLUT *hLut);

/*----------------------------------------------------------------------------*/
/* Get/Set attributes */
CORSTATUS CORAPIFUNC CorLutGetPrm( CORLUT hLut, UINT32 prm, void *value);
CORSTATUS CORAPIFUNC CorLutSetPrm( CORLUT hLut, UINT32 prm, UINT32 value);
CORSTATUS CORAPIFUNC CorLutSetPrmEx( CORLUT hLut, UINT32 prm, const void *value);
CORSTATUS CORAPIFUNC CorLutGetCap( CORLUT hLut, UINT32 cap, void *value);

/*----------------------------------------------------------------------------*/
/* Operations */
CORSTATUS CORAPIFUNC CorLutBit( CORLUT hLut, UINT32 bit, CORDATA color);
CORSTATUS CORAPIFUNC CorLutClip( CORLUT hLut,  INT32 imin,  INT32 imax, CORDATA omin, CORDATA omax);
CORSTATUS CORAPIFUNC CorLutCopy( CORLUT hSrc, CORLUT hDst);
CORSTATUS CORAPIFUNC CorLutNormal( CORLUT hLut);
CORSTATUS CORAPIFUNC CorLutPseudoColor( CORLUT hLut);
CORSTATUS CORAPIFUNC CorLutReverse( CORLUT hLut);
CORSTATUS CORAPIFUNC CorLutShift( CORLUT hLut, INT32 nShift);
CORSTATUS CORAPIFUNC CorLutSlope( CORLUT hLut, INT32 i1, CORDATA c1, INT32 i2, CORDATA c2);
CORSTATUS CORAPIFUNC CorLutGamma( CORLUT hLut, FLOAT factor);
CORSTATUS CORAPIFUNC CorLutAdd( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutAnd(CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutASub( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutSub( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutScale( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutMax( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutMin( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutOr( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutXor( CORLUT hLut, CORDATA k);
CORSTATUS CORAPIFUNC CorLutRoll( CORLUT hLut, INT32 rol);
CORSTATUS CORAPIFUNC CorLutThreshold1( CORLUT hLut, CORDATA thrs);
CORSTATUS CORAPIFUNC CorLutThreshold2( CORLUT hLut, CORDATA thrs1, CORDATA thrs2);

/*----------------------------------------------------------------------------*/
/* Read/Write access */
CORSTATUS CORAPIFUNC CorLutRead( CORLUT hLut, UINT32 offset, void *array, UINT32 size);
CORSTATUS CORAPIFUNC CorLutWrite( CORLUT hLut, UINT32 offset, const void *array, UINT32 size);

CORSTATUS CORAPIFUNC CorLutReadEx( CORLUT hLut, UINT32 offset, PCORDATA pData);
CORSTATUS CORAPIFUNC CorLutWriteEx( CORLUT hLut, UINT32 offset, CORDATA data);

/*----------------------------------------------------------------------------*/
/* Load/Save from/to file */
CORSTATUS CORAPIFUNC CorLutLoad( CORLUT hLut, PCSTR filename);
CORSTATUS CORAPIFUNC CorLutSave( CORLUT hLut, PCSTR filename);

#ifdef __cplusplus
}
#endif

#endif   // _CAPILUT_H_
