/******************************************************************************
* Misc_lib.h - header file for the misc. library.			      *
* This header is also the interface header to the world.		      *
*******************************************************************************
* (C) Gershon Elber, Technion, Israel Institute of Technology                 *
*******************************************************************************
* Written by Gershon Elber, Oct. 94.					      *
******************************************************************************/

#ifndef MISC_LIB_H
#define MISC_LIB_H

#include <stdio.h>
#ifdef __WINCE__
#   include <memory.h>
#endif
#include "irit_sm.h"
#ifdef USE_VARARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif /* USE_VARARGS */

typedef enum {
    MISC_ERR_MALLOC_FAILED,
    MISC_ERR_CONFIG_FILE_NO_FOUND,
    MISC_ERR_CONFIG_ERROR,
    MISC_ERR_UNKNOWN_CONFIG,

    MISC_ERR_UNDEFINE_ERR
} MiscFatalErrorType;

typedef enum {
    IC_NONE_TYPE = 0,
    IC_BOOLEAN_TYPE,
    IC_INTEGER_TYPE,
    IC_REAL_TYPE,
    IC_STRING_TYPE,
    IC_MULTI_STR_TYPE
} IrtCfgDataType;

typedef enum {
    IRIT_IMAGE_UNKNOWN_TYPE,
    IRIT_IMAGE_RLE_TYPE,
    IRIT_IMAGE_PPM3_TYPE,
    IRIT_IMAGE_PPM6_TYPE,
    IRIT_IMAGE_PNG_TYPE
} IrtImgImageType;

typedef struct IritConfigStruct {
    char *VarName;
    char *SomeInfo;
    VoidPtr VarData;
    IrtCfgDataType VarType;
} IritConfigStruct;

typedef struct IrtImgPixelStruct {
    IrtBType r, g, b;
} IrtImgPixelStruct;

typedef struct IrtImgRGBAPxlStruct {
    IrtBType r, g, b, a;
} IrtImgRGBAPxlStruct;

typedef float IrtImgRealClrType;
typedef struct IrtImgRealPxlStruct {
    IrtImgRealClrType r, g, b, a;
} IrtImgRealPxlStruct;

typedef struct IritPriorQue {
    struct IritPriorQue *Right, *Left; /* Pointers to two sons of this node. */
    VoidPtr Data;			     /* Pointers to the data itself. */
} IritPriorQue;

typedef struct IritHashElementStruct {
    struct IritHashElementStruct *Pnext;
    VoidPtr Data;
    IrtRType Key;
} IritHashElementStruct;

typedef struct IritHashTableStruct {
    IrtRType MinKeyVal, MaxKeyVal, DKey, KeyEps;
    IritHashElementStruct **Vec;
    int VecSize;
} IritHashTableStruct;

typedef void (*IritFatalMsgFuncType)(char *Msg);
typedef void (*IritWarningMsgFuncType)(char *Msg);
typedef void (*IritInfoMsgFuncType)(char *Msg);
typedef void (*MiscSetErrorFuncType)(MiscFatalErrorType, char *);

typedef int (*IritPQCompFuncType)(VoidPtr, VoidPtr);/* Comparison functions. */
typedef int (*IritHashCmpFuncType)(VoidPtr Data1, VoidPtr Data2);
typedef void IritLevenEvalFuncType(IrtRType *CurPoint,
				   IrtRType ModelParams[],
				   IrtRType *YPointer,
				   IrtRType YdParams[]);
typedef void IritLevenNumerProtectionFuncType(IrtRType InternalModelParams[]);
typedef int IritLevenIsModelValidFuncType(IrtRType InternalModelParams[]);

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

/* Basic dynamic memory allocation routines: */
#ifdef DEBUG_IRIT_MALLOC
VoidPtr IritMalloc(unsigned Size, char *ObjType, char *FileName, int LineNum);
#define IritMalloc(x)	IritMalloc((x), #x, __FILE__, __LINE__)
void IritFree(VoidPtr p);
#define IritFree(x)	{ IritFree(x); x = NULL; }
void IritTestAllDynMemory(void);
void IritInitTestDynMemory(void);
void IritInitTestDynMemory2(int DebugMalloc,
			    IritIntPtrSizeType DebugSearchPtr,
			    int DebugSearchPtrAbort);
void IritCheckMarkDynMemory(IrtRType *Start);
void IritDebugMallocSearchPtr(VoidPtr p, int Abort);
#else
#define IritMalloc(Size)	malloc(Size)
#define IritFree(Ptr)		free(Ptr)
#endif /* DEBUG_IRIT_MALLOC */
VoidPtr IritRealloc(VoidPtr p, unsigned OldSize, unsigned NewSize);

/* Prototype of the configuration routines: */
int IritConfig(const char *PrgmName,
	       const IritConfigStruct *SetUp,
	       int NumVar);
void IritConfigPrint(const IritConfigStruct *SetUp, int NumVar);
int IritConfigSave(const char *FileName,
		   const IritConfigStruct *SetUp,
		   int NumVar);

/* Get command line arguments. */
#ifdef USE_VARARGS
int GAGetArgs(int va_alist, ...);
#else
int GAGetArgs(int argc, ...);
#endif /* USE_VARARGS */
char *GAStringErrMsg(int Error, char *OutStr);
void GAPrintErrMsg(int Error);
char *GAStringHowTo(const char *CtrlStr, char *OutStr);
void GAPrintHowTo(const char *CtrlStr);

/* Homogeneous 4x4 matrix routines. */
void MatGenUnitMat(IrtHmgnMatType Mat);
int MatIsUnitMatrix(IrtHmgnMatType Mat, IrtRType Eps);
void MatGenMatTrans(IrtRType Tx, IrtRType Ty, IrtRType Tz, IrtHmgnMatType Mat);
void MatGenMatUnifScale(IrtRType Scale, IrtHmgnMatType Mat);
void MatGenMatScale(IrtRType Sx, IrtRType Sy, IrtRType Sz, IrtHmgnMatType Mat);
void MatGenMatRotX1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotX(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);
void MatGenMatRotY1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotY(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);
void MatGenMatRotZ1(IrtRType Teta, IrtHmgnMatType Mat);
void MatGenMatRotZ(IrtRType CosTeta, IrtRType SinTeta, IrtHmgnMatType Mat);

void MatMultTwo4by4(IrtHmgnMatType MatRes,
		    IrtHmgnMatType Mat1,
		    IrtHmgnMatType Mat2);
void MatAddTwo4by4(IrtHmgnMatType MatRes,
		   IrtHmgnMatType Mat1,
		   IrtHmgnMatType Mat2);
void MatSubTwo4by4(IrtHmgnMatType MatRes,
		   IrtHmgnMatType Mat1,
		   IrtHmgnMatType Mat2);
void MatScale4by4(IrtHmgnMatType MatRes,
		  IrtHmgnMatType Mat,
		  const IrtRType *Scale);

void MatMultVecby4by4(IrtVecType VecRes,
		      const IrtVecType Vec,
		      IrtHmgnMatType Mat);
void MatMultPtby4by4(IrtPtType PtRes,
		     const IrtPtType Pt,
		     IrtHmgnMatType Mat);
void MatMultWVecby4by4(IrtRType VRes[4],
		       const IrtRType Vec[4],
		       IrtHmgnMatType Mat);

IrtRType MatDeterminantMatrix(IrtHmgnMatType Mat);
int MatInverseMatrix(IrtHmgnMatType M, IrtHmgnMatType InvM);
void MatTranspMatrix(IrtHmgnMatType M, IrtHmgnMatType TranspM);
IrtRType MatScaleFactorMatrix(IrtHmgnMatType M);
void MatRotateFactorMatrix(IrtHmgnMatType M, IrtHmgnMatType RotMat);
void MatTranslateFactorMatrix(IrtHmgnMatType M, IrtVecType Trans);

/* General matrix routines. */
void MatGnrlCopy(IrtGnrlMatType Dst, IrtGnrlMatType Src, int n);
void MatGnrlUnitMat(IrtGnrlMatType Mat, int n);
int MatGnrlIsUnitMatrix(IrtGnrlMatType Mat, IrtRType Eps, int n);
void MatGnrlMultTwoMat(IrtGnrlMatType MatRes,
		       IrtGnrlMatType Mat1,
		       IrtGnrlMatType Mat2,
		       int n);
void MatGnrlAddTwoMat(IrtGnrlMatType MatRes,
		      IrtGnrlMatType Mat1,
		      IrtGnrlMatType Mat2,
		      int n);
void MatGnrlSubTwoMat(IrtGnrlMatType MatRes,
		      IrtGnrlMatType Mat1,
		      IrtGnrlMatType Mat2,
		      int n);
void MatGnrlScaleMat(IrtGnrlMatType MatRes,
		     IrtGnrlMatType Mat,
		     IrtRType *Scale,
		     int n);
void MatGnrlMultVecbyMat(IrtVecGnrlType VecRes,
			 IrtGnrlMatType Mat,
			 IrtVecGnrlType Vec,
			 int n);
void MatGnrlMultVecbyMat2(IrtVecGnrlType VecRes,
			  IrtVecGnrlType Vec,
			  IrtGnrlMatType Mat,
			  int n);
int MatGnrlInverseMatrix(IrtGnrlMatType M,
			 IrtGnrlMatType InvM,
			 int n);
void MatGnrlTranspMatrix(IrtGnrlMatType M,
			 IrtGnrlMatType TranspM,
			 int n);
IrtRType MatGnrlDetMatrix(IrtGnrlMatType M, int n);
int MatGnrlOrthogonalSubspace(IrtGnrlMatType M, int n);
void MatGnrlPrintMatrix(IrtGnrlMatType M, int n, FILE *F);

/* QR matrix factorization. */
int IritQRFactorization(IrtRType *A,
			int n,
			int m,
			IrtRType *Q,
			IrtRType *R);
int IritSolveUpperDiagMatrix(const IrtRType *A,
			     int n,
			     const IrtRType *b,
			     IrtRType *x);
int IritSolveLowerDiagMatrix(const IrtRType *A,
			     int n,
			     const IrtRType *b,
			     IrtRType *x);
int IritQRUnderdetermined(IrtRType *A,
			  IrtRType *x,
			  const IrtRType *b,
			  int m,
			  int n);

/* Gauss Jordan matrix solver and Levenberg Marquardt local minimum finder. */
int IritGaussJordan(IrtRType *A, IrtRType *B, unsigned N, unsigned M);
IrtRType IritLevenMarMin(IrtRType **x,
			 IrtRType y[],
			 IrtRType Sigma[],
			 unsigned NumberOfDataElements,
			 IrtRType ModelParams[],
			 IritLevenEvalFuncType *ShapeFunc,
			 IritLevenNumerProtectionFuncType *ProtectionFunc,
			 IritLevenIsModelValidFuncType *ModelValidatorFunc,
			 unsigned NumberOfModelParams,
			 IrtRType Tolerance);
IrtRType IritLevenMarMinSig1(IrtRType **XVals,
			     IrtRType YVals[],
			     unsigned NumberOfDataElements,
			     IrtRType ModelParams[],
			     IritLevenEvalFuncType *ShapeFunc,
			     IritLevenNumerProtectionFuncType *ProtectionFunc,
			     IritLevenIsModelValidFuncType *ModelValidatorFunc,
			     unsigned NumberOfMedelParams,
			     IrtRType Tolerance);
unsigned int IritLevenMarSetMaxIterations(unsigned NewVal);

/* An implementation of a priority queue. */
void IritPQInit(IritPriorQue **PQ);
int IritPQEmpty(IritPriorQue *PQ);
void IritPQCompFunc(IritPQCompFuncType NewCompFunc);
VoidPtr IritPQFirst(IritPriorQue **PQ, int Delete);
VoidPtr IritPQInsert(IritPriorQue **PQ, VoidPtr NewItem);
VoidPtr IritPQDelete(IritPriorQue **PQ, VoidPtr NewItem);
VoidPtr IritPQFind(IritPriorQue *PQ, VoidPtr OldItem);
VoidPtr IritPQNext(IritPriorQue *PQ, VoidPtr CmpItem, VoidPtr BiggerThan);
int IritPQSize(IritPriorQue *PQ);
void IritPQPrint(IritPriorQue *PQ, void (*PrintFunc)(VoidPtr));
void IritPQFree(IritPriorQue *PQ, int FreeItems);
void IritPQFreeFunc(IritPriorQue *PQ, void (*FreeFunc)(VoidPtr));

/* An implementation of a hashing table. */
IritHashTableStruct *IritHashTableCreate(IrtRType MinKeyVal,
					 IrtRType MaxKeyVal,
					 IrtRType KeyEps,
					 int VecSize);
int IritHashTableInsert(IritHashTableStruct *IHT,
			VoidPtr Data,
			IritHashCmpFuncType HashCmpFunc,
			IrtRType Key,
			int RplcSame);
VoidPtr IritHashTableFind(IritHashTableStruct *IHT,
			  VoidPtr Data,
			  IritHashCmpFuncType HashCmpFunc,
			  IrtRType Key);
int IritHashTableRemove(IritHashTableStruct *IHT,
			VoidPtr Data,
			IritHashCmpFuncType HashCmpFunc,
			IrtRType Key);
void IritHashTableFree(IritHashTableStruct *IHT);

/* Read/Write of images. */
int IrtImgReadImageXAlign(int Alignment);
IrtImgPixelStruct *IrtImgReadImage(const char *ImageFileName,
				   int *MaxX,
				   int *MaxY,
				   int *Alpha);
IrtImgPixelStruct *IrtImgReadImage2(const char *ImageFileName,
				    int *MaxX,
				    int *MaxY,
				    int *Alpha);
void IrtImgReadClrCache(void);
IrtImgImageType IrtImgWriteSetType(const char *ImageType);
int IrtImgWriteOpenFile(const char **argv,
			const char *FName,
			int Alpha,
			int XSize,
			int YSize);
void IrtImgWritePutLine(IrtBType *Alpha, IrtImgPixelStruct *Pixels);
void IrtImgWriteCloseFile(void);
IrtImgPixelStruct *IrtImgFlipXYImage(IrtImgPixelStruct *Img,
				     int MaxX,
				     int MaxY,
				     int Alpha);
int IrtImgParsePTextureString(const char *PTexture,
			      char *FName,
			      IrtRType *Scale,
			      int *Flip);

/* Searching routines. */
VoidPtr IritSearch2DInit(IrtRType XMin,
			 IrtRType XMax,
			 IrtRType YMin,
			 IrtRType YMax,
			 IrtRType Tol,
			 int DataSize);
void IritSearch2DFree(VoidPtr S2D);
void IritSearch2DInsertElem(VoidPtr S2D,
			    IrtRType XKey,
			    IrtRType YKey,
			    VoidPtr Data);
int IritSearch2DFindElem(VoidPtr S2D,
			 IrtRType XKey,
			 IrtRType YKey,
			 VoidPtr Data);

/* Set cover related routines. */
VoidPtr IritRLNew(void);
void IritRLAdd(VoidPtr RLC, IrtRType l, IrtRType r, int attr);
int *IritRLFindCyclicCover(VoidPtr RLC, IrtRType Tol);
void IritRLDelete(VoidPtr RLC);
int IritRLSetGaurdiansNumber(int g);

/* XGeneral routine - compatibility between Unix and Win95/WinNT/OS2/etc. */
char *IritStrdup(const char *s);
char *IritStrUpper(char *s);
void IritSleep(int MiliSeconds);
void IritRandomInit(long Seed);
IrtRType IritRandom(IrtRType Min, IrtRType Max);
IrtRType IritCPUTime(int Reset);
char *IritRealTimeDate(void);
IrtRType IritApproxStrStrMatch(const char *Str1,
			       const char *Str2,
			       int IgnoreCase);

#ifndef AMIGA
void movmem(VoidPtr Src, VoidPtr Dest, int Len);
#endif /* AMIGA */
const char *searchpath(const char *Name);

#ifdef STRICMP
int strnicmp(const char *s1, const char *s2, int n);
int stricmp(const char *s1, const char *s2);
#else
#if !(defined(__WINNT__) || defined(__WINCE__) || defined(OS2GCC) || defined(AMIGA) || defined(__CYGWIN__))
#   define strnicmp(s1, s2, n) strncasecmp((s1), (s2), (n))
#   define stricmp(s1, s2)     strcasecmp((s1), (s2))
#endif /* !(__WINNT__ || __WINCE__|| OS2GCC || AMIGA) */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#       define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#       define stricmp(s1, s2)     _stricmp((s1), (s2))
#   endif /* _MSC_VER >= 1400 */
#endif /*  __WINNT__ */

#ifdef __WINCE__
#   ifdef strnicmp
#       undef strnicmp
#       undef stricmp
#   endif /* strnicmp */
#   define strnicmp(s1, s2, n) _strnicmp((s1), (s2), (n))
#   define stricmp(s1, s2)     _stricmp((s1), (s2))
#endif /* __WINCE__ */
#endif /* STRICMP */

#ifdef __WINNT__
#   if _MSC_VER >= 1400  /* Visual 8, 2005 */
#   ifdef getcwd
#       undef getcwd
#   endif /* getcwd */
#   define getcwd(buf, maxlen)	_getcwd(buf, maxlen)
#   define chdir(dir)		_chdir(dir)
#   define setmode(fd, mode)	_setmode(fd, mode)
#   define putenv(str)		_putenv(str)
#   endif /* _MSC_VER >= 1400 */
#endif /*  __WINNT__ */

const char *IritStrIStr(const char *s, const char *Pattern);

#ifdef STRSTR
char *strstr(const char *s, const char *Pattern);
#endif /* STRSTR */

#ifdef GETCWD
char *getcwd(char *s, int Len);
#endif /* GETCWD */

/* Error handling. */

MiscSetErrorFuncType MiscSetFatalErrorFunc(MiscSetErrorFuncType ErrorFunc);
void MiscFatalError(MiscFatalErrorType ErrID, char *ErrDesc);
const char *MiscDescribeError(MiscFatalErrorType ErrorNum);

int IritLineHasCntrlChar(const char *Line);
IritFatalMsgFuncType IritSetFatalErrorFunc(IritFatalMsgFuncType FatalMsgFunc);
void IritFatalError(char *Msg);
IritWarningMsgFuncType IritSetWarningMsgFunc(IritWarningMsgFuncType WrnMsgFunc);
void IritWarningMsg(char *Msg);
IritInfoMsgFuncType IritSetInfoMsgFunc(IritInfoMsgFuncType InfoMsgFunc);
void IritInformationMsg(char *Msg);

#ifdef USE_VARARGS
void IritFatalErrorPrintf(const char *va_alist, ...);
void IritWarningMsgPrintf(const char *va_alist, ...);
void IritInformationMsgPrintf(const char *va_alist, ...);
#else
void IritFatalErrorPrintf(const char *Format, ...);
void IritWarningMsgPrintf(const char *Format, ...);
void IritInformationMsgPrintf(const char *Format, ...);
#endif /* USE_VARARGS */

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* MISC_LIB_H */
