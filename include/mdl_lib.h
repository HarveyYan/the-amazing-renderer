/******************************************************************************
* Mdl_lib.h - header file for the Model library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Bogdanov Alexander   July, 1996				      *
******************************************************************************/

#ifndef MDL_LIB_H
#define MDL_LIB_H

#include "cagd_lib.h" 
#include "trim_lib.h" 

typedef enum {
    MDL_BOOL_UNION = 6000,
    MDL_BOOL_INTERSECTION,
    MDL_BOOL_SUBTRACTION,
    MDL_BOOL_ADDITION
} MdlIrtBType;

typedef enum {
    MDL_ERR_PTR_REF = 1000,
    MDL_ERR_TSEG_NO_SRF,

    MDL_ERR_UNDEFINE_ERR
} MdlFatalErrorType;

typedef struct MdlTrimSegStruct {
    struct MdlTrimSegStruct *Pnext;
    struct IPAttributeStruct *Attr;
    struct MdlTrimSrfStruct *SrfFirst;
    struct MdlTrimSrfStruct *SrfSecond;
    CagdCrvStruct *UVCrvFirst;   /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *UVCrvSecond;  /* Trim crv segment in srf's param. domain. */
    CagdCrvStruct *EucCrv;       /* Trimming curve as an E3 Euclidean curve. */
} MdlTrimSegStruct;

typedef struct MdlTrimSegRefStruct {
    struct MdlTrimSegRefStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegStruct *TrimSeg;
    int Reversed;
} MdlTrimSegRefStruct;


typedef struct MdlLoopStruct {
    struct MdlLoopStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSegRefStruct *SegRefList;
} MdlLoopStruct;

typedef struct MdlTrimSrfStruct {
    struct MdlTrimSrfStruct *Pnext;
    struct IPAttributeStruct *Attr;
    CagdSrfStruct *Srf;                /* Surface trimmed by MdlTrimSegList. */
    MdlLoopStruct *LoopList;
} MdlTrimSrfStruct;

typedef struct MdlModelStruct {
    struct MdlModelStruct *Pnext;
    struct IPAttributeStruct *Attr;
    MdlTrimSrfStruct *TrimSrfList;
    MdlTrimSegStruct *TrimSegList;       /* List of trimming curve segments. */
} MdlModelStruct;

typedef void (*MdlSetErrorFuncType)(MdlFatalErrorType);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

void MdlTrimSegFree(MdlTrimSegStruct *MTSeg);
void MdlTrimSegFreeList(MdlTrimSegStruct *MTSegList);
void MdlTrimSegRefFree(MdlTrimSegRefStruct *MTSegRef);
void MdlLoopFree(MdlLoopStruct *MdlLoop);
void MdlTrimSrfFree(MdlTrimSrfStruct *TrimSrf);
void MdlTrimSrfFreeList(MdlTrimSrfStruct *MdlTrimSrfList);
void MdlModelFree(MdlModelStruct *Model);
void MdlModelFreeList(MdlModelStruct *Model);


#ifdef DEBUG
#define MdlTrimSegFree(MTSeg)         { MdlTrimSegFree(MTSeg); MTSeg = NULL; }
#define MdlTrimSegFreeList(MTSegList) { MdlTrimSegFreeList(MTSegList); \
					MTSegList = NULL; }
#define MdlTrimSegRefFree(MTSegRef)   { MdlTrimSegRefFree(MTSegRef); \
					MTSegRef = NULL; }
#define MdlLoopFree(MdlLoop)          { MdlLoopFree(MdlLoop); MdlLoop = NULL; }
#define MdlTrimSrfFree(TrimSrf)       { MdlTrimSrfFree(TrimSrf); \
					TrimSrf = NULL; }
#define MdlTrimSrfFreeList(MdlTrimSrfList) { MdlTrimSrfFreeList(MdlTrimSrfList); \
					     MdlTrimSrfList = NULL; }
#define MdlModelFree(Model)           { MdlModelFree(Model); Model = NULL; }
#define MdlModelFreeList(Model)       { MdlModelFreeList(Model); Model = NULL; }
#endif /* DEBUG */

MdlTrimSegStruct *MdlTrimSegCopy(MdlTrimSegStruct *MdlTrimSeg,
				 MdlTrimSrfStruct *TrimSrfList);
MdlTrimSegStruct *MdlTrimSegCopyList(MdlTrimSegStruct *MdlTrimSegList,
				     MdlTrimSrfStruct *TrimSrfList);
MdlTrimSegRefStruct *MdlTrimSegRefCopy(MdlTrimSegRefStruct *SegRefList,
				       MdlTrimSegStruct *TrimSegList);
MdlTrimSegRefStruct *MdlTrimSegRefCopyList(MdlTrimSegRefStruct *SegRefList,
					   MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *MdlLoopCopy(MdlLoopStruct *MdlLoop, 
			   MdlTrimSegStruct *TrimSegList);
MdlLoopStruct *MdlLoopCopyList(MdlLoopStruct *MdlLoopList, 
			       MdlTrimSegStruct *TrimSegList);
MdlTrimSrfStruct *MdlTrimSrfCopy(MdlTrimSrfStruct *MdlTrimSrf, 
				 MdlTrimSegStruct *TrimSegList);
MdlTrimSrfStruct *MdlTrimSrfCopyList(MdlTrimSrfStruct *MdlTrimSrfList, 
				     MdlTrimSegStruct *TrimSegList);
MdlModelStruct *MdlModelCopy(MdlModelStruct *Model);
MdlModelStruct *MdlModelCopyList(MdlModelStruct *ModelList);
MdlTrimSegStruct *MdlTrimSegNew(CagdCrvStruct *UVCrv1,
				CagdCrvStruct *UVCrv2,
                                CagdCrvStruct *EucCrv1,
                                MdlTrimSrfStruct *SrfFirst,
                                MdlTrimSrfStruct *SrfSecond);
MdlTrimSegRefStruct *MdlTrimSegRefNew(MdlTrimSegStruct *MdlTrimSeg);
MdlLoopStruct *MdlLoopNew(MdlTrimSegRefStruct *MdlTrimSegRefList);
MdlTrimSrfStruct *MdlTrimSrfNew(CagdSrfStruct *Srf,
				MdlLoopStruct *LoopList, 
				CagdBType HasTopLvlTrim);
MdlTrimSrfStruct *MdlTrimSrfNew2(CagdSrfStruct *Srf,
	 		         CagdCrvStruct *LoopList, 
			         CagdBType HasTopLvlTrim);
MdlModelStruct *MdlModelNew(CagdSrfStruct *Srf,
		            CagdCrvStruct *LoopList,
		            CagdBType HasTopLvlTrim);
MdlModelStruct *MdlModelNew2(MdlTrimSrfStruct *TrimSrfs,
			     MdlTrimSegStruct *TrimSegs);
void MdlPatchTrimmingSegPointers(MdlModelStruct *Model);
IritIntPtrSizeType MdlGetLoopSegIndex(MdlTrimSegRefStruct *TrimSeg,
				      MdlTrimSegStruct *TrimSegList);
IritIntPtrSizeType MdlGetSrfIndex(MdlTrimSrfStruct *Srf,
				  MdlTrimSrfStruct *TrimSrfList);

void MdlModelTransform(MdlModelStruct *Model,
		       CagdRType *Translate,
		       CagdRType Scale);
void MdlModelMatTransform(MdlModelStruct *Model, CagdMType Mat);

/******************************************************************************
* Bounding boxes routines.						      *
******************************************************************************/

void MdlModelBBox(MdlModelStruct *Mdl, CagdBBoxStruct *BBox);
void MdlModelListBBox(MdlModelStruct *Mdls, CagdBBoxStruct *BBox);

/******************************************************************************
* Boolean operations.							      *
******************************************************************************/

CagdCrvStruct *MdlExtractUVCrv(MdlTrimSrfStruct *Srf, MdlTrimSegStruct *Seg);
CagdCrvStruct *MdlExtructReversUVCrv(MdlTrimSrfStruct *MdlSrf, 
				     MdlTrimSegStruct *MdlSeg);
MdlModelStruct *MdlBooleanOperator(MdlModelStruct *Model1, 
				   MdlModelStruct *Model2,
				   MdlIrtBType BoolOperator);
MdlModelStruct *MdlBooleanUnion(MdlModelStruct *Model1, 
				MdlModelStruct *Model2);
MdlModelStruct *MdlBooleanIntersection(MdlModelStruct *Model1, 
				       MdlModelStruct *Model2);
MdlModelStruct *MdlBooleanSubtraction(MdlModelStruct *Model1, 
				      MdlModelStruct *Model2);
MdlModelStruct *MdlBooleanAddition(MdlModelStruct *Model1, 
				   MdlModelStruct *Model2);
TrimSrfStruct *MdlTrimConvert(MdlModelStruct *Model);

/******************************************************************************
* Error handling.							      *
******************************************************************************/
MdlSetErrorFuncType MdlSetFatalErrorFunc(MdlSetErrorFuncType ErrorFunc);
void MdlFatalError(MdlFatalErrorType ErrID);
const char *MdlDescribeError(MdlFatalErrorType ErrID);

void MdlDbg(void *Obj);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* MDL_LIB_H */
