/*=============================================================================
	Core.h: Unreal core public header file.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

#ifndef _INC_CORE
#define _INC_CORE

/*----------------------------------------------------------------------------
	Low level includes.
----------------------------------------------------------------------------*/

// API definition.
#ifndef CORE_API
#define CORE_API DLL_IMPORT
#endif

// Unwanted Intel C++ level 4 warnings/ remarks.
#if __ICL
#pragma warning(disable : 873)
#pragma warning(disable : 981)
#pragma warning(disable : 522)
#pragma warning(disable : 271)
#pragma warning(disable : 424)
#pragma warning(disable : 193)
#pragma warning(disable : 444)
#pragma warning(disable : 440)
#pragma warning(disable : 171)
#pragma warning(disable : 1125)
#pragma warning(disable : 488)
#pragma warning(disable : 858)
#pragma warning(disable : 82)
#pragma warning(disable : 1)
#pragma warning(disable : 177)
#pragma warning(disable : 279)
#endif

// Build options.
#include "UnBuild.h"

#if !__PSX2_EE__ && !__GCN__

#define FTime DOUBLE

#else
// Time (needed so that the PS2 doesn't use doubles)
// Note that this is only used on the PC in UTexture::LastUpdateTime
#define FIXTIME 4294967296.f
class FTime
{
#if __GNUG__
#define TIMETYP long long
#else // __MWERKS__
#define TIMETYP unsigned long
#endif
public:

	        FTime      ()               {v=0;}
	        FTime      (float f)        {v=(TIMETYP)(f*FIXTIME);}
//	        FTime      (double d)       {v=(TIMETYP)(d*FIXTIME);}
	        FTime      (TIMETYP i,int)  {v=i;}
	float   GetFloat   ()               {return v/FIXTIME;}
	FTime   operator+  (float f) const  {return FTime(v+(TIMETYP)(f*FIXTIME),0);}
	float   operator-  (FTime t) const  {return (v-t.v)/FIXTIME;}
	FTime   operator*  (float f) const  {return FTime(v*f);}
	FTime   operator/  (float f) const  {return FTime(v/f);}
	FTime&  operator+= (float f)        {v=v+(TIMETYP)(f*FIXTIME); return *this;}
	FTime&  operator*= (float f)        {v=(TIMETYP)(v*f); return *this;}
	FTime&  operator/= (float f)        {v=(TIMETYP)(v/f); return *this;}
	int     operator== (FTime t)        {return v==t.v;}
	int     operator!= (FTime t)        {return v!=t.v;}
	int     operator>  (FTime t)        {return v>t.v;}
	FTime&  operator=  (const FTime& t) {v=t.v; return *this;}
	TIMETYP v;
};
#endif

// Are we on a console or not?
#if defined(_XBOX) || defined(__PSX2_EE__) || defined(__GCN__)
#define CONSOLE 1
#endif

#if _MSC_VER || __ICC || __LINUX__
	#define SUPPORTS_PRAGMA_PACK 1
#else
	#define SUPPORTS_PRAGMA_PACK 0
#endif

// Compiler specific include.
#if __GCN__
	#ifdef EMU
		typedef char TCHAR;
		#include "UnGCNEmu.h"
	#else
		#include <string.h>
		#include "UnMWerks.h" // Using UnVcWin32.h for GCN emulator, should be UnGnuG.h or something.
	#endif
#elif _MSC_VER
	#include "UnVcWin32.h"
#elif __GNUG__
	#include <string.h>
	#include "UnGnuG.h"
#elif __MWERKS__
	#include <string.h>
	#include "UnMWerks.h"
#elif __ICC
	#if __LINUX_X86__
		#include <string.h>
		#include "UnGnuG.h"
	#else
		#error Not yet supported
	#endif
#else
	#error Unknown Compiler
#endif


// OS specific include.
#if __UNIX__
	#include "UnUnix.h"
#ifndef __MWERKS__
	#include <signal.h>
#endif
#endif


// Global constants.
enum {MAXBYTE		= 0xff       };
enum {MAXWORD		= 0xffffU    };
enum {MAXDWORD		= 0xffffffffU};
enum {MAXSBYTE		= 0x7f       };
enum {MAXSWORD		= 0x7fff     };
enum {MAXINT		= 0x7fffffff };
enum {INDEX_NONE	= -1         };
enum {UNICODE_BOM   = 0xfeff     };
enum ENoInit {E_NoInit = 0};

enum ERunningOS 
{ 
	OS_WIN95	= 0,
	OS_WIN98,
	OS_WINME,
	OS_WIN2K,
	OS_WINXP,
	OS_WINNT,
	OS_LINUX_X86,
	OS_FREEBSD_X86,
	OS_LINUX_X86_64,
	OS_MAC_OSX_PPC,
	OS_UNKNOWN	= 255
};

// Unicode or single byte character set mappings.
#ifdef _UNICODE
	#ifndef _TCHAR_DEFINED
		typedef UNICHAR  TCHAR;
		typedef UNICHARU TCHARU;
	#endif

    #ifndef _TEXT_DEFINED
	#undef TEXT
	#define TEXT(s) L##s
    #endif

    #ifndef _US_DEFINED
	#undef US
	#define US FString(L"")
    #endif

	inline TCHAR    FromAnsi   ( ANSICHAR In ) { return (BYTE)In;                        }
	inline TCHAR    FromUnicode( UNICHAR In  ) { return In;                              }
	inline ANSICHAR ToAnsi     ( TCHAR In    ) { return (_WORD)In<0x100 ? In : MAXSBYTE; }
	inline UNICHAR  ToUnicode  ( TCHAR In    ) { return In;                              }
#else
	#ifndef _TCHAR_DEFINED
		typedef ANSICHAR  TCHAR;
		typedef ANSICHARU TCHARU;
	#endif
	#undef TEXT
	#define TEXT(s) s
	#undef US
	#define US FString("")
	inline TCHAR    FromAnsi   ( ANSICHAR In ) { return In;                              }
	inline TCHAR    FromUnicode( UNICHAR In  ) { return (_WORD)In<0x100 ? In : MAXSBYTE; }
	inline ANSICHAR ToAnsi     ( TCHAR In    ) { return (_WORD)In<0x100 ? In : MAXSBYTE; }
	inline UNICHAR  ToUnicode  ( TCHAR In    ) { return (BYTE)In;                        }
#endif

#if __linux__
// !!! FIXME: dirty, dirty, dirty.  --ryan.
extern TCHAR *GUnixSpawnOnExit;
#endif

/*----------------------------------------------------------------------------
	Forward declarations.
----------------------------------------------------------------------------*/

// Objects.
class	UObject;
class		UExporter;
class		UFactory;
class		UField;
class			UConst;
class			UEnum;
class			UProperty;
class				UByteProperty;
class				UIntProperty;
class				UBoolProperty;
class				UFloatProperty;
class				UObjectProperty;
class					UClassProperty;
class				UNameProperty;
class				UStructProperty;
class               UStrProperty;
class               UArrayProperty;
class				UDelegateProperty;
class			UStruct;
class				UFunction;
class				UState;
class					UClass;
class		ULinker;
class			ULinkerLoad;
class			ULinkerSave;
class		UPackage;
class		USubsystem;
class			USystem;
class		UTextBuffer;
class       URenderDevice;
class		UPackageMap;
class		UDebugger; //DEBUGGER

// Structs.
class FName;
class FArchive;
class FCompactIndex;
class FExec;
class FGuid;
class FMemCache;
class FMemStack;
class FPackageInfo;
class FTransactionBase;
class FUnknown;
class FRepLink;
class FArray;
class FLazyLoader;
class FRoughLoader;
class FString;
class FMalloc;
class FCodec;
class L2ThreadUtil;

// Templates.
template<class T> class TArray;
template<class T> class TTransArray;
template<class T> class TLazyArray;
template<class TK, class TI> class TMap;
template<class TK, class TI> class TMultiMap;

// Globals.
CORE_API extern class FOutputDevice* GNull;

// EName definition.
#include "UnNames.h"

class CORE_API FCriticalSection
{
public:
	FCriticalSection();
	FCriticalSection(void* handle);
	~FCriticalSection();

	void Lock();
	void Unlock();

private:
	void* Handle;
	bool  OwnHandle;
};

class CORE_API FScopeCriticalSection
{
public:
	FScopeCriticalSection(FCriticalSection* InCriticalSection);
	~FScopeCriticalSection();

private:
	FCriticalSection* CriticalSection;
};

/*-----------------------------------------------------------------------------
	Abstract interfaces.
-----------------------------------------------------------------------------*/

// An output device.
class CORE_API FOutputDevice
{
public:
	// FOutputDevice interface.
	virtual void Serialize( const TCHAR* V, EName Event )=0;
	virtual void Flush();
	virtual void Release();

	// Simple text printing.
	void Log( const TCHAR* S );
	void Log( enum EName Type, const TCHAR* S );
	void Log( const FString& S );
	void Log( enum EName Type, const FString& S );
	void Logf( const TCHAR* Fmt, ... );
	void Logf( enum EName Type, const TCHAR* Fmt, ... );
	void Logf2(enum EName type, const TCHAR* Fmt, ...);
	void Logf2(const TCHAR* Fmt, ...);

// rcg04262002 Not sure what the ramifications are for other platforms yet.
#if ((defined __LINUX__) && (UNICODE))
	void Log( const ANSICHAR* S );
	void Log( enum EName Type, const ANSICHAR* S );
	void Logf( const ANSICHAR* Fmt, ... );
	void Logf( enum EName Type, const ANSICHAR* Fmt, ... );
#endif
};

// Error device.
class CORE_API FOutputDeviceError : public FOutputDevice
{
public:
	virtual void HandleError()=0;
};

class CORE_API FLogOutError : public FOutputDevice
{
public:
	FLogOutError();
	FLogOutError(FLogOutError const&);

	virtual void Serialize(const TCHAR* V, EName Event);
};

class CORE_API FNullOutError : public FOutputDevice
{
public:
	FNullOutError();
	FNullOutError(FNullOutError const&);

	virtual void Serialize(const TCHAR* V, EName Event);
};

class CORE_API FThrowOut : public FOutputDevice
{
public:
	FThrowOut();
	FThrowOut(FThrowOut const&);

	virtual void Serialize(const TCHAR* V, EName Event);
};

// Memory allocator.
class CORE_API FMalloc
{
public:
	virtual void* Malloc( DWORD Count, const TCHAR* Tag )=0;
	virtual void* Realloc( void* Original, DWORD Count, const TCHAR* Tag )=0;
	virtual void Free( void* Original )=0;
	virtual void DumpAllocs()=0;
	virtual void HeapCheck()=0;
	virtual void Init()=0;
	virtual void Exit()=0;
	virtual int GetOsPeak();
	virtual int GetOsCurrent();
	virtual int GetUsedPeak();
	virtual int GetUsedCurrent();
	virtual int GetCurrentAllocs();
	virtual int GetTotalAllocs();
};

// Configuration database cache.
class FConfigCache
{
public:
	virtual UBOOL GetBool( const TCHAR* Section, const TCHAR* Key, UBOOL& Value, const TCHAR* Filename=NULL )=0;
	virtual UBOOL GetInt( const TCHAR* Section, const TCHAR* Key, INT& Value, const TCHAR* Filename=NULL )=0;
	virtual UBOOL GetFloat( const TCHAR* Section, const TCHAR* Key, FLOAT& Value, const TCHAR* Filename=NULL )=0;
	virtual UBOOL GetString( const TCHAR* Section, const TCHAR* Key, TCHAR* Value, INT Size, const TCHAR* Filename=NULL )=0;
	virtual UBOOL GetString( const TCHAR* Section, const TCHAR* Key, class FString& Str, const TCHAR* Filename=NULL )=0;
	virtual const TCHAR* GetStr( const TCHAR* Section, const TCHAR* Key, const TCHAR* Filename=NULL )=0;
	virtual UBOOL GetSection( const TCHAR* Section, TCHAR* Value, INT Size, const TCHAR* Filename=NULL )=0;
	virtual TMultiMap<FString,FString>* GetSectionPrivate( const TCHAR* Section, UBOOL Force, UBOOL Const, const TCHAR* Filename=NULL )=0;
	virtual void EmptySection( const TCHAR* Section, const TCHAR* Filename=NULL )=0;
	virtual void SetBool( const TCHAR* Section, const TCHAR* Key, UBOOL Value, const TCHAR* Filename=NULL )=0;
	virtual void SetInt( const TCHAR* Section, const TCHAR* Key, INT Value, const TCHAR* Filename=NULL )=0;
	virtual void SetFloat( const TCHAR* Section, const TCHAR* Key, FLOAT Value, const TCHAR* Filename=NULL )=0;
	virtual void SetString( const TCHAR* Section, const TCHAR* Key, const TCHAR* Value, const TCHAR* Filename=NULL )=0; // gam
	virtual void Flush( UBOOL Read, const TCHAR* Filename=NULL )=0;
	virtual void UnloadFile( const TCHAR* Filename )=0;
	virtual void Detach( const TCHAR* Filename )=0;
	virtual void Init( const TCHAR* InSystem, const TCHAR* InUser, UBOOL RequireConfig )=0;
	virtual void Exit()=0;
	virtual void Dump( FOutputDevice& Ar )=0;
	virtual ~FConfigCache() {};
};

// Any object that is capable of taking commands.
class CORE_API FExec
{
public:
	virtual UBOOL Exec( const TCHAR* Cmd, FOutputDevice& Ar )=0;
};

// Notification hook.
class CORE_API FNotifyHook
{
public:
	virtual void NotifyDestroy( void* Src ) {}
	virtual void NotifyPreChange( void* Src ) {}
	virtual void NotifyPostChange( void* Src ) {}
	virtual void NotifyExec( void* Src, const TCHAR* Cmd ) {}
};

// Interface for returning a context string.
class FContextSupplier
{
public:
	virtual FString GetContext()=0;
};

// A context for displaying modal warning messages.
class CORE_API FFeedbackContext : public FOutputDevice
{
public:
	virtual UBOOL YesNof( const TCHAR* Fmt, ... )=0;
	virtual void BeginSlowTask( const TCHAR* Task, UBOOL StatusWindow )=0;
	virtual void EndSlowTask()=0;
	virtual UBOOL VARARGS StatusUpdatef( INT Numerator, INT Denominator, const TCHAR* Fmt, ... )=0;
	virtual void SetContext( FContextSupplier* InSupplier )=0;
	virtual void MapCheck_Show();
	virtual void MapCheck_ShowConditionally();
	virtual void MapCheck_Hide();
	virtual void MapCheck_Clear();
	virtual void MapCheck_Add( INT InType, void* InActor, const TCHAR* InMessage );
	
	INT WarningCount;
	INT ErrorCount;

	FFeedbackContext();
};

// Class for handling undo/redo transactions among objects.
typedef void( *STRUCT_AR )( FArchive& Ar, void* TPtr );
typedef void( *STRUCT_DTOR )( void* TPtr );
class CORE_API FTransactionBase
{
public:
	virtual void SaveObject( UObject* Object )=0;
	virtual void SaveArray( UObject* Object, FArray* Array, INT Index, INT Count, INT Oper, INT ElementSize, STRUCT_AR Serializer, STRUCT_DTOR Destructor )=0;
	virtual void Apply()=0;
};

// File manager.
enum EFileTimes
{
	FILETIME_Create      = 0,
	FILETIME_LastAccess  = 1,
	FILETIME_LastWrite   = 2,
};
enum EFileWrite
{
	FILEWRITE_NoFail            = 0x01,
	FILEWRITE_NoReplaceExisting = 0x02,
	FILEWRITE_EvenIfReadOnly    = 0x04,
	FILEWRITE_Unbuffered        = 0x08,
	FILEWRITE_Append			= 0x10,
	FILEWRITE_AllowRead         = 0x20,
};
enum EFileRead
{
	FILEREAD_NoFail             = 0x01,
};
enum ECopyCompress
{
	FILECOPY_Normal				= 0x00,
	FILECOPY_Compress			= 0x01,
	FILECOPY_Decompress			= 0x02,
};
enum ECopyResult
{
	COPY_OK						= 0x00,
	COPY_MiscFail				= 0x01,
	COPY_ReadFail				= 0x02,
	COPY_WriteFail				= 0x03,
	COPY_CompFail				= 0x04,
	COPY_DecompFail				= 0x05,
	COPY_Canceled				= 0x06,
};
#define COMPRESSED_EXTENSION	TEXT(".uz2")

struct FCopyProgress
{
	virtual UBOOL Poll( FLOAT Fraction )=0;
};


CORE_API const TCHAR* appBaseDir();

class CORE_API FFileManager
{
public:
	virtual void Init(UBOOL Startup) {}
	virtual FArchive* CreateMemReader(INT, INT)=0; ///?????
	virtual FArchive* CreateFileReader( const TCHAR* Filename, DWORD ReadFlags=0, FOutputDevice* Error=GNull )=0;
	virtual FArchive* CreateFileWriter( const TCHAR* Filename, DWORD WriteFlags=0, FOutputDevice* Error=GNull )=0;
	virtual INT FileSize( const TCHAR* Filename )=0;
	virtual UBOOL Delete( const TCHAR* Filename, UBOOL RequireExists=0, UBOOL EvenReadOnly=0 )=0;
	virtual DWORD Copy( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace=1, UBOOL EvenIfReadOnly=0, UBOOL Attributes=0, DWORD Compress=FILECOPY_Normal, FCopyProgress* Progress=NULL )=0;
	virtual UBOOL Move( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace=1, UBOOL EvenIfReadOnly=0, UBOOL Attributes=0 )=0;
	virtual SQWORD GetGlobalTime( const TCHAR* Filename )=0;
	virtual UBOOL SetGlobalTime( const TCHAR* Filename )=0;
	virtual UBOOL MakeDirectory( const TCHAR* Path, UBOOL Tree=0 )=0;
	virtual UBOOL DeleteDirectory( const TCHAR* Path, UBOOL RequireExists=0, UBOOL Tree=0 )=0;
	virtual TArray<FString> FindFiles( const TCHAR* Filename, UBOOL Files, UBOOL Directories )=0;
	virtual UBOOL SetDefaultDirectory( const TCHAR* Filename )=0;
	virtual FString GetDefaultDirectory()=0;
    virtual const TCHAR *CalcHomeDir(void) { return(appBaseDir()); }
};

//
// File Streaming.
//

enum EFileStreamType
{
	ST_Regular		= 0,
	ST_Ogg			= 1,
	ST_OggLooping	= 2
};

struct FStream
{
	void*	Data;
	void*	Handle;
	void*	TDD;		// type dependent data
	INT		FileSeek;
	INT		ChunkSize;
	INT		ChunksRequested;
	INT		Locked;
	INT		Used;
	INT		EndOfFile;
	EFileStreamType	Type;
};

class CORE_API FFileStream
{
public:
	static FFileStream* Init( INT MaxStreams );
	static void Destroy();

	// Interface functions.
	INT CreateStream( const TCHAR* Filename, INT ChunkSize, INT InitialChunks, void* Data, EFileStreamType Type, void* TDD );
	void RequestChunks( INT StreamId, INT Chunks, void* Data );
	UBOOL QueryStream( INT StreamId, INT& ChunksQueued );
	void DestroyStream( INT StreamId, UBOOL ReadQueuedChunks );

	// Only use the below functions in the thread's main loop.
	UBOOL Read( INT StreamId, INT Bytes );
	UBOOL Create( INT StreamId, const TCHAR* Filename );
	UBOOL Destroy( INT StreamId );
	void Enter(INT StreamId);
	void Leave(INT StreamId);

	static FFileStream* Instance;
	static INT StreamIndex;
	static INT MaxStreams;
	static FStream* Streams;
	static INT Destroyed;

private:
	FFileStream() {}
	~FFileStream() {}
};

class FEdLoadError;

/*----------------------------------------------------------------------------
	Global variables.
----------------------------------------------------------------------------*/

// Core globals.
CORE_API extern FMemStack				GMem;
CORE_API extern FOutputDevice*			GLog;
CORE_API extern FOutputDevice*			GNull;
CORE_API extern FOutputDevice*		    GThrow;
CORE_API extern FOutputDeviceError*		GError;
CORE_API extern FFeedbackContext*		GWarn;
CORE_API extern FConfigCache*			GConfig;
CORE_API extern FTransactionBase*		GUndo;
CORE_API extern FOutputDevice*			GLogHook;
CORE_API extern FExec*					GExec;
CORE_API extern FMalloc*				GMalloc;
CORE_API extern FFileManager*			GFileManager;
CORE_API extern USystem*				GSys;
CORE_API extern UProperty*				GProperty;
CORE_API extern BYTE*					GPropAddr;
CORE_API extern UObject*				GPropObject;
CORE_API extern DWORD					GRuntimeUCFlags;
CORE_API extern USubsystem*				GWindowManager;
CORE_API extern TCHAR				    GErrorHist[4096];
CORE_API extern TCHAR                   GTrue[64], GFalse[64], GYes[64], GNo[64], GNone[64];
CORE_API extern TCHAR					GCdPath[];
CORE_API extern	DOUBLE					GSecondsPerCycle;
CORE_API extern	DOUBLE					GTempDouble;
CORE_API extern void					(*GTempFunc)(void*);
CORE_API extern SQWORD					GTicks;
CORE_API extern INT                     GScriptCycles;
CORE_API extern DWORD					GPageSize;
CORE_API extern DWORD					GProcessorCount;
CORE_API extern DWORD					GPhysicalMemory;
CORE_API extern DWORD                   GUglyHackFlags;
CORE_API extern UBOOL					GIsScriptable;
CORE_API extern UBOOL					GIsEditor;
CORE_API extern UBOOL					GIsUCC;
CORE_API extern UBOOL					GEdShowFogInViewports;
CORE_API extern UBOOL					GEdSelectionLock;
CORE_API extern UBOOL					GIsClient;
CORE_API extern UBOOL					GIsServer;
CORE_API extern UBOOL					GIsCriticalError;
CORE_API extern UBOOL					GIsStarted;
CORE_API extern UBOOL					GIsRunning;
CORE_API extern UBOOL					GIsSlowTask;
CORE_API extern UBOOL					GIsGuarded;
CORE_API extern UBOOL					GIsRequestingExit;
CORE_API extern UBOOL					GIsStrict;
CORE_API extern UBOOL                   GScriptEntryTag;
CORE_API extern UBOOL                   GLazyLoad;
CORE_API extern UBOOL					GUnicode;
CORE_API extern UBOOL					GUnicodeOS;
CORE_API extern UBOOL					GShowBuildLabel;
CORE_API extern class FGlobalMath		GMath;
CORE_API extern class FArchive*         GDummySave;
CORE_API extern FFileStream*			GFileStream;
CORE_API extern FLOAT					GAudioMaxRadiusMultiplier; //!! vogel: TODO: merge audio globals into one struct
CORE_API extern FLOAT					GAudioDefaultRadius;
CORE_API extern TArray<FEdLoadError>	GEdLoadErrors;
CORE_API extern	UDebugger*				GDebugger; //DEBUGGER
CORE_API extern UBOOL					GIsBenchmarking;
CORE_API extern QWORD					GMakeCacheIDIndex;

CORE_API extern TCHAR                   GBuildLabel[1024];
CORE_API extern TCHAR                   GMachineOS[1024];
CORE_API extern TCHAR                   GMachineCPU[1024];
CORE_API extern TCHAR                   GMachineVideo[1024];

CORE_API extern	TCHAR					GIni[1024];
CORE_API extern	TCHAR					GUserIni[1024];
CORE_API extern FLOAT					NEAR_CLIPPING_PLANE;
CORE_API extern FLOAT					FAR_CLIPPING_PLANE;
CORE_API extern UBOOL					GIsOpenGL;
CORE_API extern ERunningOS				GRunningOS;

//L2
CORE_API extern FCodec*					GL2CurrentTxtCodec;
CORE_API extern FCodec*					GL2DESCodec;
CORE_API extern FOutputDevice*			GCheckGrpLog;
CORE_API extern FOutputDevice*			GL2DebugLog;
CORE_API extern FOutputDevice* GLogHook2;
CORE_API extern L2ThreadUtil GThreadUtil;
CORE_API extern DOUBLE GSecondsPerCycleQPF;
CORE_API extern INT/*enum GCSTATE*/ GL2GCState;
CORE_API extern FLOAT GL2ActorCR;
CORE_API extern FLOAT GL2AntiPortalCR;
CORE_API extern FLOAT GL2FogEnd;
CORE_API extern FLOAT GL2FogSpeed;
CORE_API extern FLOAT GL2FogStart;
CORE_API extern FLOAT GL2MaxPawnResourceLoad;
CORE_API extern FLOAT GL2OriginPawnCR;
CORE_API extern FLOAT GL2OriginStaticMeshCR;
CORE_API extern FLOAT GL2OriginStaticMeshLodCR;
CORE_API extern FLOAT GL2OriginTerrainCR;
CORE_API extern FLOAT GL2PawnCR;
CORE_API extern FLOAT GL2ProjectorCR;
CORE_API extern FLOAT GL2SeamlessMaginotLine;
CORE_API extern FLOAT GL2StaticMeshCR;
CORE_API extern FLOAT GL2StaticMeshLodCR;
CORE_API extern FLOAT GL2TerrainCR;
CORE_API extern INT GCastDuplicate;
CORE_API extern INT GColorBits;
CORE_API extern INT GEnableEngSelection;
CORE_API extern INT GExitPurge;
CORE_API extern INT GIsArrowMode;
CORE_API extern INT GIsDynamicRecompile;
CORE_API extern INT GIsEnterChatting;
CORE_API extern INT GIsKeyboardSecurity;
CORE_API extern INT GIsL22DQuestMark;
CORE_API extern INT GIsL2Console;
CORE_API extern INT GIsL2HideName;
CORE_API extern INT GIsL2Mark;
CORE_API extern INT GIsL2NetLog;
CORE_API extern INT GIsL2NetLogin;
CORE_API extern INT GIsL2Projector;
CORE_API extern INT GIsL2Seamless;
CORE_API extern INT GIsL2WaterEffect;
CORE_API extern INT GIsL2WaterEffectType;
CORE_API extern INT GIsLeftHandedMouse;
CORE_API extern INT GIsLoadL2WaterEffect;
CORE_API extern INT GIsNative;
CORE_API extern INT GIsOldChat;
CORE_API extern INT GIsOnlyL2Console;
CORE_API extern INT GIsOnlyMSN;
CORE_API extern INT GIsOnlyXMLUI;
CORE_API extern INT GIsRejectingDuel;
CORE_API extern INT GIsSEK;
CORE_API extern INT GIsStrict;
CORE_API extern INT GIsTestHtml;
CORE_API extern INT GIsTransparencyMode;
CORE_API extern INT GIsTrueFontUse;
CORE_API extern INT GIsUseXMLUI;
CORE_API extern INT GL2ActorAppearingType;
CORE_API extern INT GL2Antialiasing;
CORE_API extern INT GL2CanUseJoystick;
CORE_API extern INT GL2CheckLevelOfWeather;
CORE_API extern INT GL2ClientOrder;
CORE_API extern INT GL2ClientStandAlone;
CORE_API extern INT GL2DefaultShadow;
CORE_API extern INT GL2DisableGCLobby;
CORE_API extern INT GL2DrawTargetName;
CORE_API extern INT GL2EnablePurgeLevel;
CORE_API extern INT GL2EnableStaticMeshShadow;
CORE_API extern INT GL2FFEffectUse;
CORE_API extern INT GL2GPUSkinning;
CORE_API extern INT GL2GlowEffect;
CORE_API extern INT GL2HDREffect;
CORE_API extern INT GL2HideDropMesh;
CORE_API extern INT GL2IsFirstRun;
CORE_API extern INT GL2KeepMinFrameRate;
CORE_API extern INT GL2LevelOfWeather;
CORE_API extern INT GL2NVPerfHUD;
CORE_API extern INT GL2NeedEditChange;
CORE_API extern INT GL2NoGC;
CORE_API extern INT GL2PartyLooting;
CORE_API extern INT GL2PostProcessType;
CORE_API extern INT GL2RenderActorLimit;
CORE_API extern INT GL2RenderActorLimitOpt;
CORE_API extern INT GL2RenderDeco;
CORE_API extern INT GL2ReplayLoadResource;
CORE_API extern INT GL2ReplayMode;
CORE_API extern INT GL2SaveLog;
CORE_API extern INT GL2ScreenShotQuality;
CORE_API extern INT GL2Shader;
CORE_API extern INT GL2SharedSky;
CORE_API extern INT GL2ShowCMViewer;
CORE_API extern INT GL2ShowGroupName;
CORE_API extern INT GL2ShowMyName;
CORE_API extern INT GL2ShowNPCName;
CORE_API extern INT GL2ShowOtherPCName;
CORE_API extern INT GL2ShowPartyMemberName;
CORE_API extern INT GL2ShowPledgeMemberName;
CORE_API extern INT GL2ShowWindowBox;
CORE_API extern INT GL2ShowWindowName;
CORE_API extern INT GL2TestServer;
CORE_API extern INT GL2TextCapture;
CORE_API extern INT GL2TextureDetail;
CORE_API extern INT GL2ThaiHtmlMode;
CORE_API extern INT GL2ThreadFilter;
CORE_API extern INT GL2ThreadFilterBacku;
CORE_API extern INT GL2UnderWaterEffect;
CORE_API extern INT GL2UseAutoTrackingPawn;
CORE_API extern INT GL2UseBoneSimul;
CORE_API extern INT GL2UseColorCursor;
CORE_API extern INT GL2UseCommand;
CORE_API extern INT GL2UseGameGuard;
CORE_API extern INT GL2UseHitWateEffect;
CORE_API extern INT GL2UseKeyCrypt;
CORE_API extern INT GL2UseReducePawnLOD;
CORE_API extern INT GL2UseReplayManager;
CORE_API extern INT GL2UseSameVS;
CORE_API extern INT GL2UseSkipAnim;
CORE_API extern INT GL2UseTrilinear;
CORE_API extern INT GL2UsingJoystick;
CORE_API extern INT GL2WireSkeletalMesh;
CORE_API extern INT GLanguageType;
CORE_API extern INT GMsnDontDisturb;
CORE_API extern INT GMsnHideAll;
CORE_API extern INT GMsnHideWarnMsg;
CORE_API extern INT GMsnLoginAlarm;
CORE_API extern INT GMsnMessageAlarm;
CORE_API extern INT GMsnNewEMailAlarm;
CORE_API extern INT GMsnSoundAlarm;
CORE_API extern INT GNativeDuplicate;
CORE_API extern INT GRefreshRate;
CORE_API extern INT GSEKScreenX;
CORE_API extern INT GSEKScreenY;
CORE_API extern DWORD GCPUCoreCount;
CORE_API extern DWORD GCPUType;
CORE_API extern DWORD GNewMemoryCount;
CORE_API extern TCHAR* GCountryCode;
CORE_API extern TCHAR* GCountryName;

// Per module globals.
extern "C" DLL_EXPORT TCHAR GPackage[];

// Normal includes.
#include "UnFile.h"			// Low level utility code.
#include "UnObjVer.h"		// Object version info.
#include "UnArc.h"			// Archive class.
#include "UnTemplate.h"     // Dynamic arrays.
#include "UnName.h"			// Global name subsystem.
#include "UnStack.h"		// Script stack definition.
#include "UnObjBas.h"		// Object base class.
#include "UnCoreNet.h"		// Core networking.
#include "UnCorObj.h"		// Core object class definitions.
#include "UnClass.h"		// Class definition.
#include "UnType.h"			// Base property type.
#include "UnScript.h"		// Script class.
#include "UFactory.h"		// Factory definition.
#include "UExporter.h"		// Exporter definition.
#include "UnCache.h"		// Cache based memory management.
#include "UnMem.h"			// Stack based memory management.
#include "UnCId.h"          // Cache ID's.
#include "UnBits.h"         // Bitstream archiver.
#include "UnMath.h"         // Vector math functions.
#include "UnL2Support.h"

// Worker class for tracking loading errors in the editor
class CORE_API FEdLoadError
{
public:
	FEdLoadError();
	FEdLoadError(INT InType, TCHAR* InDesc);
	~FEdLoadError();

	// The types of things that could be missing.
	enum
	{
		TYPE_FILE		= 0,	// A totally missing file
		TYPE_RESOURCE	= 1,	// Texture/Sound/StaticMesh/etc
	};

	INT Type;		// TYPE_
	FString Desc;	// Description of the error

	UBOOL operator==(const FEdLoadError& LE) const;
	FEdLoadError& operator=(const FEdLoadError Other);
};

//
// Archive for counting memory usage.
//
class CORE_API FArchiveCountMem : public FArchive
{
public:
	FArchiveCountMem(UObject* Src);

	SIZE_T GetNum();
	SIZE_T GetMax();
	void CountBytes(SIZE_T InNum, SIZE_T InMax);

protected:
	SIZE_T Num, Max;
};

enum
{
	MCTYPE_ERROR	= 0,
	MCTYPE_WARNING	= 1,
	MCTYPE_NOTE		= 2
};

typedef struct {
	INT Type;
	AActor* Actor;
	FString Message;
} MAPCHECK;

// A convenience to allow referring to axis' by name instead of number
enum EAxis
{
	AXIS_X	= 0,
	AXIS_Y	= 1,
	AXIS_Z	= 2,
};

// Very basic abstract debugger class.
class UDebugger //DEBUGGER
{
public:
	virtual void DebugInfo( UObject* Debugee, FFrame* Stack, FString InfoType, int LineNumber, int InputPos )=0;
	virtual void NotifyAccessedNone()=0;
};

#include "UnCoreNative.h"

#ifdef __PSX2_EE__
	#include "UnPSX2.h"
#endif

#ifdef __GCN__
	#include "UnGCN.h"
#endif

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
#endif

