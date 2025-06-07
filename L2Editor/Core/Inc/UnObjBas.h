/*=============================================================================
	UnObjBas.h: Unreal object base class.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Core enumerations.
-----------------------------------------------------------------------------*/

//
// Flags for loading objects.
//
enum ELoadFlags
{
	LOAD_None			= 0x0000,	// No flags.
	LOAD_NoFail			= 0x0001,	// Critical error if load fails.
	LOAD_NoWarn			= 0x0002,	// Don't display warning if load fails.
	LOAD_Throw			= 0x0008,	// Throw exceptions upon failure.
	LOAD_Verify			= 0x0010,	// Only verify existance; don't actually load.
	LOAD_AllowDll		= 0x0020,	// Allow plain DLLs.
	LOAD_DisallowFiles  = 0x0040,	// Don't load from file.
	LOAD_NoVerify       = 0x0080,   // Don't verify imports yet.
	LOAD_Forgiving      = 0x1000,   // Forgive missing imports (set them to NULL).
	LOAD_Quiet			= 0x2000,   // No log warnings.
	LOAD_NoRemap        = 0x4000,   // No remapping of packages.
	LOAD_Propagate      = 0,
};

//
// Package flags.
//
enum EPackageFlags
{
	PKG_AllowDownload	= 0x0001,	// Allow downloading package.
	PKG_ClientOptional  = 0x0002,	// Purely optional for clients.
	PKG_ServerSideOnly  = 0x0004,   // Only needed on the server side.
	PKG_BrokenLinks     = 0x0008,   // Loaded from linker with broken import links.
	PKG_Unsecure        = 0x0010,   // Not trusted.
	PKG_Need			= 0x8000,	// Client needs to download this package.
};

//
// Internal enums.
//
enum ENativeConstructor    {EC_NativeConstructor};
enum EStaticConstructor    {EC_StaticConstructor};
enum EInternal             {EC_Internal};
enum ECppProperty          {EC_CppProperty};
enum EInPlaceConstructor   {EC_InPlaceConstructor};

//
// Result of GotoState.
//
enum EGotoState
{
	GOTOSTATE_NotFound		= 0,
	GOTOSTATE_Success		= 1,
	GOTOSTATE_Preempted		= 2,
};

//
// Flags describing a class.
//
enum EClassFlags
{
	// Base flags.
	CLASS_Abstract          = 0x00000001,  // Class is abstract and can't be instantiated directly.
	CLASS_Compiled			= 0x00000002,  // Script has been compiled successfully.
	CLASS_Config			= 0x00000004,  // Load object configuration at construction time.
	CLASS_Transient			= 0x00000008,	// This object type can't be saved; null it out at save time.
	CLASS_Parsed            = 0x00000010,	// Successfully parsed.
	CLASS_Localized         = 0x00000020,  // Class contains localized text.
	CLASS_SafeReplace       = 0x00000040,  // Objects of this class can be safely replaced with default or NULL.
	CLASS_RuntimeStatic     = 0x00000080,	// Objects of this class are static during gameplay.
	CLASS_NoExport          = 0x00000100,  // Don't export to C++ header.
	CLASS_Placeable         = 0x00000200,  // Allow users to create in the editor.
	CLASS_PerObjectConfig   = 0x00000400,  // Handle object configuration on a per-object basis, rather than per-class.
	CLASS_NativeReplication = 0x00000800,  // Replication handled in C++.
	CLASS_EditInlineNew		= 0x00001000,	// Class can be constructed from editinline New button.
	CLASS_CollapseCategories= 0x00002000,	// Display properties in the editor without using categories.
	CLASS_ExportStructs     = 0x00004000,  // amb: export structs to <package>Classes.h header file

    // sjs ---
    CLASS_IsAUProperty      = 0x00008000,  // IsA UProperty
    CLASS_IsAUObjectProperty= 0x00010000,  // IsA UObjectProperty
    CLASS_IsAUBoolProperty  = 0x00020000,  // IsA UBoolProperty
    CLASS_IsAUState         = 0x00040000,  // IsA UState
    CLASS_IsAUFunction      = 0x00080000,  // IsA UFunction
    // --- sjs
	
	CLASS_NeedsDefProps		= 0x00100000,  // Class needs its defaultproperties imported
	CLASS_AutoInstancedProps= 0x00200000,  // Object properties of this class are auto-CPF_NeedCtorLink'd (ie instanced on duplicate).
	CLASS_DynamicRecompile  = 0x00400000,


	// Flags to inherit from base class.
	CLASS_Inherit           = CLASS_Transient | CLASS_Config | CLASS_Localized | CLASS_SafeReplace | CLASS_RuntimeStatic | CLASS_PerObjectConfig | CLASS_Placeable | CLASS_IsAUProperty | CLASS_IsAUObjectProperty | CLASS_IsAUBoolProperty | CLASS_IsAUState | CLASS_IsAUFunction,
	CLASS_RecompilerClear   = CLASS_Inherit | CLASS_Abstract | CLASS_NoExport | CLASS_NativeReplication,
	CLASS_ScriptInherit		= CLASS_Inherit | CLASS_EditInlineNew | CLASS_CollapseCategories | CLASS_AutoInstancedProps,
};

//
// Flags associated with each property in a class, overriding the
// property's default behavior.
//
enum EPropertyFlags
{
	// Regular flags.
	CPF_Edit		 = 0x00000001, // Property is user-settable in the editor.
	CPF_Const		 = 0x00000002, // Actor's property always matches class's default actor property.
	CPF_Input		 = 0x00000004, // Variable is writable by the input system.
	CPF_ExportObject = 0x00000008, // Object can be exported with actor.
	CPF_OptionalParm = 0x00000010, // Optional parameter (if CPF_Param is set).
	CPF_Net			 = 0x00000020, // Property is relevant to network replication.
	CPF_EditConstArray		= 0x00000040,// Prevent adding/removing of items from dynamic a array in the editor.
	CPF_Parm		 = 0x00000080, // Function/When call parameter.
	CPF_OutParm		 = 0x00000100, // Value is copied out after function call.
	CPF_SkipParm	 = 0x00000200, // Property is a short-circuitable evaluation function parm.
	CPF_ReturnParm	 = 0x00000400, // Return value.
	CPF_CoerceParm	 = 0x00000800, // Coerce args into this function parameter.
	CPF_Native       = 0x00001000, // Property is native: C++ code is responsible for serializing it.
	CPF_Transient    = 0x00002000, // Property is transient: shouldn't be saved, zero-filled at load time.
	CPF_Config       = 0x00004000, // Property should be loaded/saved as permanent profile.
	CPF_Localized    = 0x00008000, // Property should be loaded as localizable text.
	CPF_Travel       = 0x00010000, // Property travels across levels/servers.
	CPF_EditConst    = 0x00020000, // Property is uneditable in the editor.
	CPF_GlobalConfig = 0x00040000, // Load config from base class, not subclass.
	CPF_OnDemand     = 0x00100000, // Object or dynamic array loaded on demand only.
	CPF_New			 = 0x00200000, // Automatically create inner object.
	CPF_NeedCtorLink = 0x00400000, // Fields need construction/destruction.
	CPF_NoExport     = 0x00800000, // Property should not be exported to the native class header file.
	CPF_Button       = 0x01000000, // String that has "Go" button which allows it to call functions from UEd.
	CPF_CommentString= 0x02000000, // Property has a comment string visible via the property browser
	CPF_EditInline	 = 0x04000000, // Edit this object reference inline.
	CPF_EdFindable	 = 0x08000000, // References are set by clicking on actors in the editor viewports.
	CPF_EditInlineUse= 0x10000000, // EditInline with Use button.
	CPF_Deprecated  		= 0x20000000, // Property is deprecated.  Read it from an archive, but don't save it.
	CPF_EditInlineNotify	= 0x40000000, // EditInline, notify outer object on editor change.
			

	// Combinations of flags.
	CPF_ParmFlags				= CPF_OptionalParm | CPF_Parm | CPF_OutParm | CPF_SkipParm | CPF_ReturnParm | CPF_CoerceParm,
	CPF_PropagateFromStruct		= CPF_Const | CPF_Native | CPF_Transient,
	CPF_PropagateToArrayInner	= CPF_ExportObject | CPF_EditInline | CPF_EditInlineUse | CPF_EditInlineNotify | CPF_Localized, // gam
};

//
// Flags describing an object instance.
//
enum EObjectFlags
{
	RF_Transactional    = 0x00000001,   // Object is transactional.
	RF_Unreachable		= 0x00000002,	// Object is not reachable on the object graph.
	RF_Public			= 0x00000004,	// Object is visible outside its package.
	RF_TagImp			= 0x00000008,	// Temporary import tag in load/save.
	RF_TagExp			= 0x00000010,	// Temporary export tag in load/save.
	RF_SourceModified   = 0x00000020,   // Modified relative to source files.
	RF_TagGarbage		= 0x00000040,	// Check during garbage collection.
	RF_Final			= 0x00000080,	// Object is not visible outside of class.
	RF_PerObjectLocalized=0x00000100,	// Object is localized by instance name, not by class.
	RF_NeedLoad			= 0x00000200,   // During load, indicates object needs loading.
	RF_HighlightedName  = 0x00000400,	// A hardcoded name which should be syntax-highlighted.
	RF_EliminateObject  = 0x00000400,   // NULL out references to this during garbage collecion.
	RF_InSingularFunc   = 0x00000800,	// In a singular function.
	RF_RemappedName     = 0x00000800,   // Name is remapped.
	RF_Suppress         = 0x00001000,	//warning: Mirrored in UnName.h. Suppressed log name.
	RF_StateChanged     = 0x00001000,   // Object did a state change.
	RF_InEndState       = 0x00002000,   // Within an EndState call.
	RF_Transient        = 0x00004000,	// Don't save object.
	RF_Preloading       = 0x00008000,   // Data is being preloaded from file.
	RF_LoadForClient	= 0x00010000,	// In-file load for client.
	RF_LoadForServer	= 0x00020000,	// In-file load for client.
	RF_LoadForEdit		= 0x00040000,	// In-file load for client.
	RF_Standalone       = 0x00080000,   // Keep object around for editing even if unreferenced.
	RF_NotForClient		= 0x00100000,	// Don't load this object for the game client.
	RF_NotForServer		= 0x00200000,	// Don't load this object for the game server.
	RF_NotForEdit		= 0x00400000,	// Don't load this object for the editor.
	RF_Destroyed        = 0x00800000,	// Object Destroy has already been called.
	RF_NeedPostLoad		= 0x01000000,   // Object needs to be postloaded.
	RF_HasStack         = 0x02000000,	// Has execution stack.
	RF_Native			= 0x04000000,   // Native (UClass only).
	RF_Marked			= 0x08000000,   // Marked (for debugging).
	RF_ErrorShutdown    = 0x10000000,	// ShutdownAfterError called.
	RF_DebugPostLoad    = 0x20000000,   // For debugging Serialize calls.
	RF_DebugSerialize   = 0x40000000,   // For debugging Serialize calls.
	RF_DebugDestroy     = 0x80000000,   // For debugging Destroy calls.
	RF_ContextFlags		= RF_NotForClient | RF_NotForServer | RF_NotForEdit, // All context flags.
	RF_LoadContextFlags	= RF_LoadForClient | RF_LoadForServer | RF_LoadForEdit, // Flags affecting loading.
	RF_Load  			= RF_ContextFlags | RF_LoadContextFlags | RF_Public | RF_Final | RF_Standalone | RF_Native | RF_SourceModified | RF_Transactional | RF_HasStack | RF_PerObjectLocalized, // Flags to load from Unrealfiles.
	RF_Keep             = RF_Native | RF_Marked | RF_PerObjectLocalized, // Flags to persist across loads.
	RF_ScriptMask		= RF_Transactional | RF_Public | RF_Final | RF_Transient | RF_NotForClient | RF_NotForServer | RF_NotForEdit | RF_Standalone // Script-accessible flags.
};

//NEW: trace facility
enum ProcessEventType
{
	PE_Toggle,
	PE_Suppress,
	PE_Enable,
};

/*----------------------------------------------------------------------------
	Core types.
----------------------------------------------------------------------------*/

//
// Globally unique identifier.
//
class CORE_API FGuid
{
public:
	DWORD A,B,C,D;
	FGuid();
	FGuid(DWORD InA, DWORD InB, DWORD InC, DWORD InD);

	friend UBOOL operator==(const FGuid& X, const FGuid& Y);
	friend UBOOL operator!=(const FGuid& X, const FGuid& Y);
	friend FArchive& operator<<(FArchive& Ar, FGuid& G);

	TCHAR* String() const;
};

inline INT CompareGuids( FGuid* A, FGuid* B )
{
	INT Diff;
	Diff = A->A-B->A; if( Diff ) return Diff;
	Diff = A->B-B->B; if( Diff ) return Diff;
	Diff = A->C-B->C; if( Diff ) return Diff;
	Diff = A->D-B->D; if( Diff ) return Diff;
	return 0;
}

//
// COM IUnknown interface.
//
class CORE_API FUnknown
{
public:
	virtual DWORD STDCALL QueryInterface(const FGuid& RefIID, void** InterfacePtr);
	virtual DWORD STDCALL AddRef();
	virtual DWORD STDCALL Release();
};

//
// Information about a driver class.
//
class CORE_API FRegistryObjectInfo
{
public:
	FString Object;
	FString Class;
	FString MetaClass;
	FString Description;
	FString Autodetect;

	FRegistryObjectInfo();
};

//
// Information about a preferences menu item.
//
class CORE_API FPreferencesInfo
{
public:
	FString Caption;
	FString ParentCaption;
	FString Class;
	FName Category;
	UBOOL Immediate;

	FPreferencesInfo();
};

/*----------------------------------------------------------------------------
	Core macros.
----------------------------------------------------------------------------*/

// Special canonical package for FindObject, ParseObject.
#define ANY_PACKAGE ((UPackage*)-1)

// Define private default constructor.
#define NO_DEFAULT_CONSTRUCTOR(cls) \
	protected: cls() {} public:

// Guard macros.
#define unguardobjSlow		unguardfSlow(( TEXT("(%s)"), GetFullName() ))
#define unguardobj			unguardf(( TEXT("(%s)"), GetFullName() ))

// Verify the a class definition and C++ definition match up.
#define VERIFY_CLASS_OFFSET(Pre,ClassName,Member) \
    {for( TFieldFlagIterator<UProperty,CLASS_IsAUProperty> It( FindObjectChecked<UClass>( Pre##ClassName::StaticClass()->GetOuter(), TEXT(#ClassName) ) ); It; ++It ) \
		if( appStricmp(It->GetName(),TEXT(#Member))==0 ) \
			if( It->Offset != STRUCT_OFFSET(Pre##ClassName,Member) ) \
				appErrorf(TEXT("Class %s Member %s problem: Script=%i C++=%i"), TEXT(#ClassName), TEXT(#Member), It->Offset, STRUCT_OFFSET(Pre##ClassName,Member) );}

// Verify that C++ and script code agree on the size of a class.
#define VERIFY_CLASS_SIZE(ClassName) \
	if (sizeof(ClassName)!=ClassName::StaticClass()->GetPropertiesSize()) \
	{ appErrorf(TEXT("VERIFY: Class %s size problem; Script=%i C++=%i"), TEXT(#ClassName), (int) ClassName::StaticClass()->GetPropertiesSize(), sizeof(ClassName));}

// Declare the base UObject class.

#if !__STATIC_LINK
#define DECLARE_BASE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
public: \
	/* Identification */ \
	enum {StaticClassFlags=TStaticFlags}; \
	private: static UClass PrivateStaticClass; public: \
	typedef TSuperClass Super;\
	typedef TClass ThisClass;\
	static UClass* StaticClass() \
		{ return &PrivateStaticClass; } \
	void* operator new( size_t Size, UObject* Outer=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0 ) \
		{ return StaticAllocateObject( StaticClass(), Outer, Name, SetFlags ); } \
	void* operator new( size_t Size, EInternal* Mem ) \
		{ return (void*)Mem; }
#else
#define DECLARE_BASE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
public: \
	/* Identification */ \
	enum {StaticClassFlags=TStaticFlags}; \
	private: \
	static UClass* PrivateStaticClass; \
	public: \
	typedef TSuperClass Super;\
	typedef TClass ThisClass;\
	static UClass* GetPrivateStaticClass##TClass( TCHAR* Package ); \
	static void InitializePrivateStaticClass##TClass(); \
	static UClass* StaticClass() \
	{ \
		if (!PrivateStaticClass) \
		{ \
			PrivateStaticClass = GetPrivateStaticClass##TClass( TEXT(#TPackage) ); \
			InitializePrivateStaticClass##TClass(); \
		} \
		return PrivateStaticClass; \
	} \
	void* operator new( size_t Size, UObject* Outer=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0 ) \
		{ return StaticAllocateObject( StaticClass(), Outer, Name, SetFlags ); } \
	void* operator new( size_t Size, EInternal* Mem ) \
		{ return (void*)Mem; }
#endif

// Declare a concrete class.
#define DECLARE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
	DECLARE_BASE_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
	friend FArchive &operator<<( FArchive& Ar, TClass*& Res ) \
		{ return Ar << *(UObject**)&Res; } \
	virtual ~TClass() \
		{ ConditionalDestroy(); } \
	static void InternalConstructor( void* X ) \
		{ new( (EInternal*)X )TClass(); } \

// Declare an abstract class.
#define DECLARE_ABSTRACT_CLASS( TClass, TSuperClass, TStaticFlags, TPackage ) \
	DECLARE_BASE_CLASS( TClass, TSuperClass, TStaticFlags | CLASS_Abstract, TPackage ) \
	friend FArchive &operator<<( FArchive& Ar, TClass*& Res ) \
		{ return Ar << *(UObject**)&Res; } \
	virtual ~TClass() \
		{ ConditionalDestroy(); } \

// Declare that objects of class being defined reside within objects of the specified class.
#define DECLARE_WITHIN( TWithinClass ) \
	typedef TWithinClass WithinClass; \
	TWithinClass* GetOuter##TWithinClass() const { return (TWithinClass*)GetOuter(); }

// Register a class at startup time.

#if __STATIC_LINK
	#define IMPLEMENT_CLASS(TClass) \
		UClass* TClass::PrivateStaticClass = NULL; \
		UClass* TClass::GetPrivateStaticClass##TClass( TCHAR* Package ) \
		{ \
			UClass* ReturnClass; \
			ReturnClass = ::new UClass \
			( \
				EC_StaticConstructor, \
				sizeof(TClass), \
				StaticClassFlags, \
				FGuid(GUID1,GUID2,GUID3,GUID4), \
				TEXT(#TClass)+1, \
				Package, \
				StaticConfigName(), \
				RF_Public | RF_Standalone | RF_Transient | RF_Native, \
				(void(*)(void*))TClass::InternalConstructor, \
				(void(UObject::*)())&TClass::StaticConstructor \
			); \
			check(ReturnClass); \
			return ReturnClass; \
		} \
		/* Called from ::StaticClass after GetPrivateStaticClass */ \
		void TClass::InitializePrivateStaticClass##TClass() \
		{ \
			/* No recursive ::StaticClass calls allowed. Setup extras. */ \
			if (TClass::Super::StaticClass() != TClass::PrivateStaticClass) \
				TClass::PrivateStaticClass->SuperField = TClass::Super::StaticClass(); \
			else \
				TClass::PrivateStaticClass->SuperField = NULL; \
			TClass::PrivateStaticClass->ClassWithin = TClass::WithinClass::StaticClass(); \
			TClass::PrivateStaticClass->SetClass(UClass::StaticClass()); \
			/* Perform UObject native registration. */ \
			if( TClass::PrivateStaticClass->GetInitialized() && TClass::PrivateStaticClass->GetClass()==TClass::PrivateStaticClass->StaticClass() ) \
				TClass::PrivateStaticClass->Register(); \
		}
#elif _MSC_VER
	#define IMPLEMENT_CLASS(TClass) \
		UClass TClass::PrivateStaticClass \
		( \
			EC_NativeConstructor, \
			sizeof(TClass), \
			TClass::StaticClassFlags, \
			TClass::Super::StaticClass(), \
			TClass::WithinClass::StaticClass(), \
			FGuid(TClass::GUID1,TClass::GUID2,TClass::GUID3,TClass::GUID4), \
			TEXT(#TClass)+1, \
			GPackage, \
			StaticConfigName(), \
			RF_Public | RF_Standalone | RF_Transient | RF_Native, \
			(void(*)(void*))TClass::InternalConstructor, \
			(void(UObject::*)())&TClass::StaticConstructor \
		); \
		extern "C" DLL_EXPORT UClass* autoclass##TClass;\
		DLL_EXPORT UClass* autoclass##TClass = TClass::StaticClass();
#else
	#define IMPLEMENT_CLASS(TClass) \
		UClass TClass::PrivateStaticClass \
		( \
			EC_NativeConstructor, \
			sizeof(TClass), \
			TClass::StaticClassFlags, \
			TClass::Super::StaticClass(), \
			TClass::WithinClass::StaticClass(), \
			FGuid(TClass::GUID1,TClass::GUID2,TClass::GUID3,TClass::GUID4), \
			TEXT(#TClass)+1, \
			GPackage, \
			StaticConfigName(), \
			RF_Public | RF_Standalone | RF_Transient | RF_Native, \
			(void(*)(void*))TClass::InternalConstructor, \
			(void(UObject::*)())&TClass::StaticConstructor \
		); \
		DLL_EXPORT {UClass* autoclass##TClass = TClass::StaticClass();}
#endif

// Define the package of the current DLL being compiled.
#if __STATIC_LINK
	#define IMPLEMENT_PACKAGE(pkg) \
		extern "C" {TCHAR G##pkg##Package[] = TEXT(#pkg);} \
		IMPLEMENT_PACKAGE_PLATFORM(pkg)
#elif _MSC_VER
	#define IMPLEMENT_PACKAGE(pkg) \
		extern "C" DLL_EXPORT TCHAR GPackage[]; \
		extern "C" DLL_EXPORT TCHAR GPackage[] = TEXT(#pkg); \
		IMPLEMENT_PACKAGE_PLATFORM(pkg)
#else
	#define IMPLEMENT_PACKAGE(pkg) \
	extern "C" DLL_EXPORT {TCHAR GPackage[] = TEXT(#pkg);} \
		IMPLEMENT_PACKAGE_PLATFORM(pkg)
#endif

/*-----------------------------------------------------------------------------
	FScriptDelegate.
-----------------------------------------------------------------------------*/
struct CORE_API FScriptDelegate
{
	UObject* Object;
	FName FunctionName;

	friend FArchive& operator<<(FArchive& Ar, FScriptDelegate& D);
};
/*-----------------------------------------------------------------------------
	UObject.
-----------------------------------------------------------------------------*/

// gam --- Forgive me :(

enum EDrawPivot
{
    DP_UpperLeft            =0,
    DP_UpperMiddle          =1,
    DP_UpperRight           =2,
    DP_MiddleRight          =3,
    DP_LowerRight           =4,
    DP_LowerMiddle          =5,
    DP_LowerLeft            =6,
    DP_MiddleLeft           =7,
    DP_MiddleMiddle         =8,
    DP_MAX                  =9,
};

// --- gam

//
// The base class of all objects.
//
class CORE_API UObject : public FUnknown
{
#if __MWERKS__
public:
	static UPackage* GetTransientPackage();
protected:
#endif

	// Declarations.
	DECLARE_BASE_CLASS(UObject,UObject,CLASS_Abstract,Core)
	typedef UObject WithinClass;
	enum {GUID1=0,GUID2=0,GUID3=0,GUID4=0};
	static const TCHAR* StaticConfigName();

	// Friends.
	friend class FObjectIterator;
	friend class ULinkerLoad;
	friend class ULinkerSave;
	friend class UPackageMap;
	friend class FArchiveTagUsed;
	friend struct FObjectImport;
	friend struct FObjectExport;

protected:
	// Internal per-object variables.
	INT						Index;				// Index of object into table.
	UObject*				HashNext;			// Next object in this hash bin.
	FStateFrame*			StateFrame;			// Main script execution stack.
	ULinkerLoad*			_Linker;			// Linker it came from, or NULL if none.
	INT						_LinkerIndex;		// Index of this object in the linker's export map.
	UObject*				Outer;				// Object this object resides in.
	DWORD					ObjectFlags;		// Private EObjectFlags used by object manager.
	FName					Name;				// Name of the object.
	UClass*					Class;	  			// Class the object belongs to.
	INT						CacheIndex;
	INT						HashNextBuffer;
	INT						IndexBuffer;

	// Private systemwide variables.
	static UBOOL			GObjInitialized;	// Whether initialized.
	static UBOOL            GObjNoRegister;		// Registration disable.
	static INT				GObjBeginLoadCount;	// Count for BeginLoad multiple loads.
	static INT				GObjRegisterCount;  // ProcessRegistrants entry counter.
	static INT				GImportCount;		// Imports for EndLoad optimization.
	static UObject*			GObjHash[4096];		// Object hash.
	static UObject*			GAutoRegister;		// Objects to automatically register.
	static TArray<UObject*> GObjLoaded;			// Objects that might need preloading.
	static TArray<UObject*>	GObjRoot;			// Top of active object graph.
	static TArray<UObject*>	GObjObjects;		// List of all objects.
	static TArray<INT>      GObjAvailable;		// Available object indices.
	static TArray<UObject*>	GObjLoaders;		// Array of loaders.
	static UPackage*		GObjTransientPkg;	// Transient package.
	static TCHAR			GObjCachedLanguage[32]; // Language;
	static TArray<UObject*> GObjRegistrants;		// Registrants during ProcessRegistrants call.
	static TArray<FPreferencesInfo> GObjPreferences; // Prefereces cache.
	static TArray<FRegistryObjectInfo> GObjDrivers; // Drivers cache.
	static TMultiMap<FName,FName>* GObjPackageRemap; // Remap table for loading renamed packages.
	static TCHAR GLanguage[64];
	//L2
protected:
	static class TArray<class UObject *> GObjLoadedBuffer;
	static class TArray<class UObject *> GObjObjectsBuffer;
	static class UObject** GObjHashBuffer;
	static int GSWObjBeginLoadCount;

public:
	static class FCriticalSection * GDissociateImportsCS;
	static class FCriticalSection * GLoadingCS;
	static class FCriticalSection * GScriptingCS;
	static int GActorCurCount;
	static int GActorNewCount;
	static int GActorPeakCount;
	static int GEmitterCurCount;
	static int GEmitterNewCount;
	static int GEmitterPeakCount;
	static int GObjCurCount;
	static int GObjNewCount;
	static int GObjPeakCount;
	static int GPawnCurCount;
	static int GPawnNewCount;
	static int GPawnPeakCount;

	// Private functions.
protected:
	void AddObject( INT Index );
	void HashObject();
	void HashObjectBuffer();
	void UnhashObject( INT OuterIndex );
	void UnhashObjectBuffer(INT);
	void SetLinker( ULinkerLoad* L, INT I );

	// Private systemwide functions.
	static ULinkerLoad* GetLoader( INT i );
	static FName MakeUniqueObjectName( UObject* Outer, UClass* Class );
	static UBOOL ResolveName( UObject*& Outer, const TCHAR*& Name, UBOOL Create, UBOOL Throw );
	static void SafeLoadError( DWORD LoadFlags, const TCHAR* Error, const TCHAR* Fmt, ... );
	static void PurgeGarbage();
	static void CacheDrivers( UBOOL ForceRefresh );

public:
	// Constructors.
	UObject();
	UObject( const UObject& Src );
	UObject( ENativeConstructor, UClass* InClass, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags );
	UObject( EStaticConstructor, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags );
	UObject( EInPlaceConstructor, UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags );
	UObject& operator=( const UObject& );
	void StaticConstructor();
	static void InternalConstructor(void* X);

	// Destructors.
	virtual ~UObject();
	void operator delete(void* Object, size_t Size);

	// FUnknown interface.
	virtual DWORD STDCALL QueryInterface( const FGuid& RefIID, void** InterfacePtr );
	virtual DWORD STDCALL AddRef();
	virtual DWORD STDCALL Release();

	// UObject interface.
	virtual void ProcessEvent( UFunction* Function, void* Parms, void* Result=NULL );
	virtual void ProcessDelegate( FName DelegateName, FScriptDelegate* Delegate, void* Parms, void* Result=NULL );
	virtual void ProcessState( FLOAT DeltaSeconds );
	virtual UBOOL ProcessRemoteFunction( UFunction* Function, void* Parms, FFrame* Stack );
	virtual void Modify();
	virtual void PostLoad();
	virtual void Destroy();
	virtual void Serialize( FArchive& Ar );
	virtual UBOOL IsPendingKill();
	virtual EGotoState GotoState( FName State );
	virtual INT GotoLabel( FName Label );
	virtual void InitExecution();
	virtual void ShutdownAfterError();
	virtual void PostEditChange();
	virtual void CallFunction( FFrame& TheStack, RESULT_DECL, UFunction* Function );
	virtual UBOOL ScriptConsoleExec( const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor );
	virtual void Register();
	virtual void LanguageChange();
	virtual void ClearL2Game();

	//NEW: trace facility
	static void EventToggle( FName Event );
	static void EventEnable( FName Event, UBOOL bEnable );
	static void EventToggleAndLog( FName Event, FOutputDevice& Ar );
	static void EventEnableAndLog( FName Event, UBOOL bEnable, FOutputDevice& Ar );
	static void ProcessEventCommand( ProcessEventType PE, const TCHAR* EventList, FOutputDevice& Ar  );

	// Systemwide functions.
	static UObject* StaticFindObject( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticFindObjectChecked( UClass* Class, UObject* InOuter, const TCHAR* Name, UBOOL ExactClass=0 );
	static UObject* StaticLoadObject( UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UObject* StaticMemLoadObject(UClass*, UObject *, const TCHAR*, void*, int, DWORD, UPackageMap*);
	static UClass* StaticLoadClass( UClass* BaseClass, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox );
	static UClass* StaticMemLoadClass(UClass*, UObject *, const TCHAR*, void*, int, DWORD, UPackageMap*);
	static UObject* StaticAllocateObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError, UObject* Ptr=NULL, UObject* SuperObject=NULL );
	static UObject* StaticConstructObject( UClass* Class, UObject* InOuter=(UObject*)GetTransientPackage(), FName Name=NAME_None, DWORD SetFlags=0, UObject* Template=NULL, FOutputDevice* Error=GError, UObject* SuperObject=NULL );
	static UBOOL ObjectChecked(UClass*, UObject*, const TCHAR *, const TCHAR*, DWORD, UPackageMap*);
	static void StaticInit();
	static void StaticExit();
	static UBOOL StaticExec( const TCHAR* Cmd, FOutputDevice& Ar=*GLog );
	static void StaticTick();
	static UObject* LoadPackage( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags, INT=0 );
	static UBOOL CheckSafePackage(const TCHAR*);
	static UBOOL SavePackage( UObject* InOuter, UObject* Base, DWORD TopLevelFlags, const TCHAR* Filename, FOutputDevice* Error=GError, ULinkerLoad* Conform=NULL, INT=0 );
	static UBOOL DeletePackage( const TCHAR* Filename );
	static void FinishThreadLoad();
	static void CollectGarbage( DWORD KeepFlags );
	static void SerializeRootSet( FArchive& Ar, DWORD KeepFlags, DWORD RequiredFlags );
	static UBOOL IsReferenced( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static UBOOL AttemptDelete( UObject*& Res, DWORD KeepFlags, UBOOL IgnoreReference );
	static void BeginLoad();
	static void EndLoad();
	static void InitProperties( BYTE* Data, INT DataCount, UClass* DefaultsClass, BYTE* Defaults, INT DefaultsCount, UObject* DestObject=NULL, UObject* SuperObject=NULL );
	static void ExitProperties( BYTE* Data, UClass* Class );
	static void ResetLoaders( UObject* InOuter, UBOOL DynamicOnly, UBOOL ForceLazyLoad );
	static UPackage* CreatePackage( UObject* InOuter, const TCHAR* PkgName );
	static ULinkerLoad* GetMemPackageLinker(class UObject*, DWORD, class UPackageMap *, class FGuid*, void*, int);
	static ULinkerLoad* GetPackageLinker( UObject* InOuter, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox, FGuid* CompatibleGuid );
	static void StaticShutdownAfterError();
	static UObject* GetIndexedObject( INT Index );
	static void GlobalSetProperty( const TCHAR* Value, UClass* Class, UProperty* Property, INT Offset, UBOOL Immediate );
	static void ExportProperties( FOutputDevice& Out, UClass* ObjectClass, BYTE* Object, INT Indent, UClass* DiffClass, BYTE* Diff );
	static void ResetConfig( UClass* Class );
	static void GetRegistryObjects( TArray<FRegistryObjectInfo>& Results, UClass* Class, UClass* MetaClass, UBOOL ForceRefresh );
	static void GetPreferences( TArray<FPreferencesInfo>& Results, const TCHAR* Category, UBOOL ForceRefresh );
	static UBOOL GetInitialized();

	static TArray<UObject*> GetLoaderList();
	static INT GetImportCount();
	static void SetImportCount(INT);
	static void DissociateImports();
#ifndef __MWERKS__
	static UPackage* GetTransientPackage();
#endif
	static void VerifyLinker( ULinkerLoad* Linker );
	static void ProcessRegistrants();
	static void BindPackage( UPackage* Pkg );
	static const TCHAR* GetLanguage();
	static void SetLanguage( const TCHAR* LanguageExt );
	static INT GetObjectHash(FName ObjName, INT Outer);

	static void SWBeginLoad();
	static void SWEndLoad();

	// Functions.
	void AddToRoot();
	void RemoveFromRoot();
	const TCHAR* GetFullName( TCHAR* Str=NULL ) const;
	const TCHAR* GetPathName( UObject* StopOuter=NULL, TCHAR* Str=NULL ) const;
	UBOOL IsValid();
	void ConditionalRegister();
	UBOOL ConditionalDestroy();
	void ConditionalPostLoad();
	void ConditionalShutdownAfterError();
	UBOOL IsA( UClass* SomeBaseClass ) const;
	UBOOL IsIn( UObject* SomeOuter ) const;
	UBOOL IsProbing( FName ProbeName );
	virtual void Rename( const TCHAR* NewName=NULL, UObject* NewOuter=NULL );
	UField* FindObjectField( FName InName, UBOOL Global=0 );
	UFunction* FindFunction( FName InName, UBOOL Global=0 );
	UFunction* FindFunctionChecked( FName InName, UBOOL Global=0 );
	UState* FindState( FName InName );
	void SaveConfig( DWORD Flags=CPF_Config, const TCHAR* Filename=NULL );
	void LoadConfig( UBOOL Propagate=0, UClass* Class=NULL, const TCHAR* Filename=NULL );
	void LoadLocalized(); // gam
	void InitClassDefaultObject( UClass* InClass, UBOOL SetOuter = 0 );
	void ProcessInternal( FFrame& TheStack, void*const Result );
	void ParseParms( const TCHAR* Parms );
	virtual void NetDirty(UProperty* property);

	virtual UBOOL IsLoaded();

	// By-name arbitrary field accessors.
	
	UBOOL FindArrayProperty( FString Name, FArray** Array, INT* ElementSize );	
	UBOOL FindStructProperty( FString Name, UStruct** Struct );
	UBOOL FindObjectProperty( FString Name, UObject** Object );
	UBOOL FindFloatProperty( FString Name, FLOAT* FloatVar );
	UBOOL FindIntProperty( FString Name, INT* IntVar );
	UBOOL FindBoolProperty( FString Name, UBOOL* BoolVar );
	UBOOL FindFNameProperty( FString Name, FName* FNameVar );

	// Accessors.
	UClass* GetClass() const;
	void SetClass(UClass* NewClass);

	DWORD GetFlags() const;
	void SetFlags(DWORD NewFlags);
	void ClearFlags(DWORD NewFlags);

	const TCHAR* GetName() const;
	const FName GetFName() const;

	UObject* GetOuter() const;
	DWORD GetIndex() const;

	DWORD GetCacheIndex() const;

	ULinkerLoad* GetLinker();
	INT GetLinkerIndex();

	FStateFrame* GetStateFrame();

	ULinkerLoad* L2GetLoader(INT);
	UObject* LinkerCreate(ULinkerLoad*, UClass*, FName, DWORD, INT);


	// delegates
	#define DELEGATE_IS_SET(del) (__##del##__Delegate.Object!=NULL && !__##del##__Delegate.Object->IsPendingKill())

	// UnrealScript intrinsics.
	#define DECLARE_FUNCTION(func) void func( FFrame& TheStack, RESULT_DECL );
	DECLARE_FUNCTION(execAbs);
	DECLARE_FUNCTION(execAcos);
	DECLARE_FUNCTION(execAdd_FloatFloat);
	DECLARE_FUNCTION(execAdd_IntInt);
	DECLARE_FUNCTION(execAdd_RotatorRotator);
	DECLARE_FUNCTION(execAdd_VectorVector);
	DECLARE_FUNCTION(execAddAdd_Byte);
	DECLARE_FUNCTION(execAddAdd_Int);
	DECLARE_FUNCTION(execAddAdd_PreByte);
	DECLARE_FUNCTION(execAddAdd_PreInt);
	DECLARE_FUNCTION(execAddEqual_ByteByte);
	DECLARE_FUNCTION(execAddEqual_FloatFloat);
	DECLARE_FUNCTION(execAddEqual_IntInt);
	DECLARE_FUNCTION(execAddEqual_RotatorRotator);
	DECLARE_FUNCTION(execAddEqual_VectorVector);
	DECLARE_FUNCTION(execAnd_IntInt);
	DECLARE_FUNCTION(execAndAnd_BoolBool);
	DECLARE_FUNCTION(execAsc);
	DECLARE_FUNCTION(execAsin);
	DECLARE_FUNCTION(execAt_StrStr);
	DECLARE_FUNCTION(execAtan);
	DECLARE_FUNCTION(execCaps);
	DECLARE_FUNCTION(execChr);
	DECLARE_FUNCTION(execClamp);
	DECLARE_FUNCTION(execClassIsChildOf);
	DECLARE_FUNCTION(execClockwiseFrom_IntInt);
	DECLARE_FUNCTION(execComplement_PreInt);
	DECLARE_FUNCTION(execComplementEqual_FloatFloat);
	DECLARE_FUNCTION(execComplementEqual_StrStr);
	DECLARE_FUNCTION(execConcat_StrStr);
	DECLARE_FUNCTION(execCos);
	DECLARE_FUNCTION(execCross_VectorVector);
	DECLARE_FUNCTION(execDisable);
	DECLARE_FUNCTION(execDivide_FloatFloat);
	DECLARE_FUNCTION(execDivide_IntInt);
	DECLARE_FUNCTION(execDivide_RotatorFloat);
	DECLARE_FUNCTION(execDivide_VectorFloat);
	DECLARE_FUNCTION(execDivideEqual_ByteByte);
	DECLARE_FUNCTION(execDivideEqual_FloatFloat);
	DECLARE_FUNCTION(execDivideEqual_IntFloat);
	DECLARE_FUNCTION(execDivideEqual_RotatorFloat);
	DECLARE_FUNCTION(execDivideEqual_VectorFloat);
	DECLARE_FUNCTION(execDot_VectorVector);
	DECLARE_FUNCTION(execDynamicLoadObject);
	DECLARE_FUNCTION(execEnable);
	DECLARE_FUNCTION(execEqualEqual_BoolBool);
	DECLARE_FUNCTION(execEqualEqual_FloatFloat);
	DECLARE_FUNCTION(execEqualEqual_IntInt);
	DECLARE_FUNCTION(execEqualEqual_NameName);
	DECLARE_FUNCTION(execEqualEqual_ObjectObject);
	DECLARE_FUNCTION(execEqualEqual_RotatorRotator);
	DECLARE_FUNCTION(execEqualEqual_StrStr);
	DECLARE_FUNCTION(execEqualEqual_VectorVector);
	DECLARE_FUNCTION(execExp);
	DECLARE_FUNCTION(execFClamp);
	DECLARE_FUNCTION(execFindObject);
	DECLARE_FUNCTION(execFMax);
	DECLARE_FUNCTION(execFMin);
	DECLARE_FUNCTION(execFRand);
	DECLARE_FUNCTION(execGetAxes);
	DECLARE_FUNCTION(execGetEnum);
	DECLARE_FUNCTION(execGetPropertyText);
	DECLARE_FUNCTION(execGetStateName);
	DECLARE_FUNCTION(execGetUnAxes);
	DECLARE_FUNCTION(execGotoState);
	DECLARE_FUNCTION(execGreater_FloatFloat);
	DECLARE_FUNCTION(execGreater_IntInt);
	DECLARE_FUNCTION(execGreater_StrStr);
	DECLARE_FUNCTION(execGreaterEqual_FloatFloat);
	DECLARE_FUNCTION(execGreaterEqual_IntInt);
	DECLARE_FUNCTION(execGreaterEqual_StrStr);
	DECLARE_FUNCTION(execGreaterGreater_IntInt);
	DECLARE_FUNCTION(execGreaterGreater_VectorRotator);
	DECLARE_FUNCTION(execGreaterGreaterGreater_IntInt);
	DECLARE_FUNCTION(execInStr);
	DECLARE_FUNCTION(execInterpCurveEval);
	DECLARE_FUNCTION(execInterpCurveGetInputDomain);
	DECLARE_FUNCTION(execInterpCurveGetOutputRange);
	DECLARE_FUNCTION(execInvert);
	DECLARE_FUNCTION(execIsA);
	DECLARE_FUNCTION(execIsInState);
	DECLARE_FUNCTION(execLeft);
	DECLARE_FUNCTION(execLen);
	DECLARE_FUNCTION(execLerp);
	DECLARE_FUNCTION(execLess_FloatFloat);
	DECLARE_FUNCTION(execLess_IntInt);
	DECLARE_FUNCTION(execLess_StrStr);
	DECLARE_FUNCTION(execLessEqual_FloatFloat);
	DECLARE_FUNCTION(execLessEqual_IntInt);
	DECLARE_FUNCTION(execLessEqual_StrStr);
	DECLARE_FUNCTION(execLessLess_IntInt);
	DECLARE_FUNCTION(execLessLess_VectorRotator);
	DECLARE_FUNCTION(execLocalize);
	DECLARE_FUNCTION(execLog);
	DECLARE_FUNCTION(execLoge);
	DECLARE_FUNCTION(execMax);
	DECLARE_FUNCTION(execMid);
	DECLARE_FUNCTION(execMin);
	DECLARE_FUNCTION(execMirrorVectorByNormal);
	DECLARE_FUNCTION(execMultiply_FloatFloat);
	DECLARE_FUNCTION(execMultiply_FloatRotator);
	DECLARE_FUNCTION(execMultiply_FloatVector);
	DECLARE_FUNCTION(execMultiply_IntInt);
	DECLARE_FUNCTION(execMultiply_RotatorFloat);
	DECLARE_FUNCTION(execMultiply_VectorFloat);
	DECLARE_FUNCTION(execMultiply_VectorVector);
	DECLARE_FUNCTION(execMultiplyEqual_ByteByte);
	DECLARE_FUNCTION(execMultiplyEqual_FloatFloat);
	DECLARE_FUNCTION(execMultiplyEqual_IntFloat);
	DECLARE_FUNCTION(execMultiplyEqual_RotatorFloat);
	DECLARE_FUNCTION(execMultiplyEqual_VectorFloat);
	DECLARE_FUNCTION(execMultiplyEqual_VectorVector);
	DECLARE_FUNCTION(execMultiplyMultiply_FloatFloat);
	DECLARE_FUNCTION(execNormal);
	DECLARE_FUNCTION(execNormalize);
	DECLARE_FUNCTION(execNot_PreBool);
	DECLARE_FUNCTION(execNotEqual_BoolBool);
	DECLARE_FUNCTION(execNotEqual_FloatFloat);
	DECLARE_FUNCTION(execNotEqual_IntInt);
	DECLARE_FUNCTION(execNotEqual_NameName);
	DECLARE_FUNCTION(execNotEqual_ObjectObject);
	DECLARE_FUNCTION(execNotEqual_RotatorRotator);
	DECLARE_FUNCTION(execNotEqual_StrStr);
	DECLARE_FUNCTION(execNotEqual_VectorVector);
	DECLARE_FUNCTION(execOr_IntInt);
	DECLARE_FUNCTION(execOrOr_BoolBool);
	DECLARE_FUNCTION(execOrthoRotation);
	DECLARE_FUNCTION(execPercent_FloatFloat);
	DECLARE_FUNCTION(execQuatFindBetween);
	DECLARE_FUNCTION(execQuatFromAxisAndAngle);
	DECLARE_FUNCTION(execQuatInvert);
	DECLARE_FUNCTION(execQuatProduct);
	DECLARE_FUNCTION(execQuatRotateVector);
	DECLARE_FUNCTION(execRand);
	DECLARE_FUNCTION(execRight);
	DECLARE_FUNCTION(execRotator2Vector);
	DECLARE_FUNCTION(execRotRand);
	DECLARE_FUNCTION(execSaveConfig);
	DECLARE_FUNCTION(execSetPropertyText);
	DECLARE_FUNCTION(execSin);
	DECLARE_FUNCTION(execSmerp);
	DECLARE_FUNCTION(execSqrt);
	DECLARE_FUNCTION(execSquare);
	DECLARE_FUNCTION(execStaticSaveConfig);
	DECLARE_FUNCTION(execSubtract_FloatFloat);
	DECLARE_FUNCTION(execSubtract_IntInt);
	DECLARE_FUNCTION(execSubtract_PreFloat);
	DECLARE_FUNCTION(execSubtract_PreInt);
	DECLARE_FUNCTION(execSubtract_PreVector);
	DECLARE_FUNCTION(execSubtract_RotatorRotator);
	DECLARE_FUNCTION(execSubtract_VectorVector);
	DECLARE_FUNCTION(execSubtractEqual_ByteByte);
	DECLARE_FUNCTION(execSubtractEqual_FloatFloat);
	DECLARE_FUNCTION(execSubtractEqual_IntInt);
	DECLARE_FUNCTION(execSubtractEqual_RotatorRotator);
	DECLARE_FUNCTION(execSubtractEqual_VectorVector);
	DECLARE_FUNCTION(execSubtractSubtract_Byte);
	DECLARE_FUNCTION(execSubtractSubtract_Int);
	DECLARE_FUNCTION(execSubtractSubtract_PreByte);
	DECLARE_FUNCTION(execSubtractSubtract_PreInt);
	DECLARE_FUNCTION(execTan);
	DECLARE_FUNCTION(execVector2Rotator);
	DECLARE_FUNCTION(execVRand);
	DECLARE_FUNCTION(execVSize);
	DECLARE_FUNCTION(execWarn);
	DECLARE_FUNCTION(execXor_IntInt);
	DECLARE_FUNCTION(execXorXor_BoolBool);
	DECLARE_FUNCTION(execResetConfig);

	// UnrealScript calling stubs.
	void eventBeginState();
	void eventEndState();
};

/*----------------------------------------------------------------------------
	Core templates.
----------------------------------------------------------------------------*/

// Hash function.
inline DWORD GetTypeHash( const UObject* A )
{
	return A ? A->GetIndex() : 0;
}

// Parse an object name in the input stream.
template< class T > UBOOL ParseObject( const TCHAR* Stream, const TCHAR* Match, T*& Obj, UObject* Outer )
{
	return ParseObject( Stream, Match, T::StaticClass(), *(UObject **)&Obj, Outer );
}

// Find an optional object.
template< class T > T* FindObject( UObject* Outer, const TCHAR* Name, UBOOL ExactClass=0 )
{
	return (T*)UObject::StaticFindObject( T::StaticClass(), Outer, Name, ExactClass );
}

// Find an object, no failure allowed.
template< class T > T* FindObjectChecked( UObject* Outer, const TCHAR* Name, UBOOL ExactClass=0 )
{
	return (T*)UObject::StaticFindObjectChecked( T::StaticClass(), Outer, Name, ExactClass );
}

// Dynamically cast an object type-safely.
class UClass;
template< class T > T* Cast( UObject* Src )
{
	return Src && Src->IsA(T::StaticClass()) ? (T*)Src : NULL;
}
template< class T, class U > T* CastChecked( U* Src )
{
	if( !Src || !Src->IsA(T::StaticClass()) )
		appErrorf( TEXT("Cast of %s to %s failed"), Src ? Src->GetFullName() : TEXT("NULL"), T::StaticClass()->GetName() );
	return (T*)Src;
}
template< class T, EClassFlags Flag > T* FlagCast( UObject* Src ) // sjs
{
	return Src && (Src->GetClass()->ClassFlags & Flag) ? (T*)Src : NULL;
}

// Construct an object of a particular class.
template< class T > T* ConstructObject( UClass* Class, UObject* Outer=(UObject*)-1, FName Name=NAME_None, DWORD SetFlags=0 )
{
	check(Class->IsChildOf(T::StaticClass()));
	if( Outer==(UObject*)-1 )
		Outer = UObject::GetTransientPackage();

	T* Result = (T*)UObject::StaticConstructObject(Class, Outer, Name, SetFlags);

	if (sizeof(T) != Class->GetPropertiesSize()) {
		debugf(TEXT("VERIFY: Class %s size problem; Script=%i C++=%i"), Class->GetName(), (int)Class->GetPropertiesSize(), sizeof(T));
	}

	return Result;
}

// Load an object.
template< class T > T* LoadObject( UObject* Outer, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox )
{
	return (T*)UObject::StaticLoadObject( T::StaticClass(), Outer, Name, Filename, LoadFlags, Sandbox );
}

// Load a class object.
template< class T > UClass* LoadClass( UObject* Outer, const TCHAR* Name, const TCHAR* Filename, DWORD LoadFlags, UPackageMap* Sandbox )
{
	return UObject::StaticLoadClass( T::StaticClass(), Outer, Name, Filename, LoadFlags, Sandbox );
}

// Get default object of a class.
template< class T > T* GetDefault()
{
	return (T*)&T::StaticClass()->Defaults(0);
}

/*----------------------------------------------------------------------------
	Object iterators.
----------------------------------------------------------------------------*/

//
// Class for iterating through all objects.
//
class FObjectIterator
{
public:
	FObjectIterator( UClass* InClass=UObject::StaticClass() )
	:	Class( InClass ), Index( -1 )
	{
		check(Class);
		++*this;
	}
	void operator++()
	{
		while( ++Index<UObject::GObjObjects.Num() && (!UObject::GObjObjects(Index) || !UObject::GObjObjects(Index)->IsA(Class)) );
	}
	UObject* operator*()
	{
		return UObject::GObjObjects(Index);
	}
	UObject* operator->()
	{
		return UObject::GObjObjects(Index);
	}
	operator UBOOL()
	{
		return Index<UObject::GObjObjects.Num();
	}
protected:
	UClass* Class;
	INT Index;
};

//
// Class for iterating through all objects which inherit from a
// specified base class.
//
template< class T > class TObjectIterator : public FObjectIterator
{
public:
	TObjectIterator()
	:	FObjectIterator( T::StaticClass() )
	{}
	T* operator* ()
	{
		return (T*)FObjectIterator::operator*();
	}
	T* operator-> ()
	{
		return (T*)FObjectIterator::operator->();
	}
};

#define AUTO_INITIALIZE_REGISTRANTS \
	UObject::StaticClass(); \
	UClass::StaticClass(); \
	USubsystem::StaticClass(); \
	USystem::StaticClass(); \
	UProperty::StaticClass(); \
	UStructProperty::StaticClass(); \
	UField::StaticClass(); \
	UMapProperty::StaticClass(); \
	UArrayProperty::StaticClass(); \
	UFixedArrayProperty::StaticClass(); \
	UStrProperty::StaticClass(); \
	UNameProperty::StaticClass(); \
	UClassProperty::StaticClass(); \
	UObjectProperty::StaticClass(); \
	UFloatProperty::StaticClass(); \
	UBoolProperty::StaticClass(); \
	UIntProperty::StaticClass(); \
	UByteProperty::StaticClass(); \
	UDelegateProperty::StaticClass(); \
	ULanguage::StaticClass(); \
	UTextBufferFactory::StaticClass(); \
	UFactory::StaticClass(); \
	UPackage::StaticClass(); \
	UCommandlet::StaticClass(); \
	ULinkerSave::StaticClass(); \
	ULinker::StaticClass(); \
	ULinkerLoad::StaticClass(); \
	UEnum::StaticClass(); \
	UTextBuffer::StaticClass(); \
	UPackageMap::StaticClass(); \
	UConst::StaticClass(); \
	UFunction::StaticClass(); \
	UStruct::StaticClass();

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

