/*=============================================================================
	UnLinker.h: Unreal object linker.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	FObjectExport.
-----------------------------------------------------------------------------*/

//
// Information about an exported object.
//
struct CORE_API FObjectExport
{
	// Variables.
	INT         ClassIndex;		// Persistent.
	INT         SuperIndex;		// Persistent (for UStruct-derived objects only).
	INT			PackageIndex;	// Persistent.
	FName		ObjectName;		// Persistent.
	DWORD		ObjectFlags;	// Persistent.
	INT         SerialSize;		// Persistent.
	INT         SerialOffset;	// Persistent (for checking only).
	UObject*	_Object;		// Internal.
	INT			_iHashNext;		// Internal.

	// Functions.
	FObjectExport();
	FObjectExport( UObject* InObject );
	
	friend FArchive& operator<<( FArchive& Ar, FObjectExport& E )
	{
		guard(FObjectExport<<);

		Ar << AR_INDEX(E.ClassIndex);
		Ar << AR_INDEX(E.SuperIndex);
		Ar << E.PackageIndex;
		Ar << E.ObjectName;
		Ar << E.ObjectFlags;
		Ar << AR_INDEX(E.SerialSize);
		if( E.SerialSize )
			Ar << AR_INDEX(E.SerialOffset);

		return Ar;
		unguard;
	}
};

/*-----------------------------------------------------------------------------
	FObjectImport.
-----------------------------------------------------------------------------*/

//
// Information about an imported object.
//
struct CORE_API FObjectImport
{
	// Variables.
	FName			ClassPackage;	// Persistent.
	FName			ClassName;		// Persistent.
	INT				PackageIndex;	// Persistent.
	FName			ObjectName;		// Persistent.
	UObject*		XObject;		// Internal (only really needed for saving, can easily be gotten rid of for loading).
	ULinkerLoad*	SourceLinker;	// Internal.
	INT             SourceIndex;	// Internal.

	// Functions.
	FObjectImport();
	FObjectImport( UObject* InObject );
	
	friend FArchive& operator<<( FArchive& Ar, FObjectImport& I )
	{
		guard(FObjectImport<<);

		Ar << I.ClassPackage << I.ClassName;
		Ar << I.PackageIndex;
		Ar << I.ObjectName;
		if( Ar.IsLoading() )
		{
			I.SourceIndex = INDEX_NONE;
			I.XObject     = NULL;
		}
		return Ar;

		unguard;
	}
};

/*----------------------------------------------------------------------------
	Items stored in Unrealfiles.
----------------------------------------------------------------------------*/

//
// Unrealfile summary, stored at top of file.
//
struct CORE_API FGenerationInfo
{
	INT ExportCount, NameCount;

	FGenerationInfo( INT InExportCount, INT InNameCount );

	friend FArchive& operator<<( FArchive& Ar, FGenerationInfo& Info )
	{
		guard(FGenerationInfo<<);
		return Ar << Info.ExportCount << Info.NameCount;
		unguard;
	}
};

struct FPackageFileSummary
{
	// Variables.
	INT		Tag;
protected:
	INT		FileVersion;
public:
	DWORD	PackageFlags;
	INT		NameCount,		NameOffset;
	INT		ExportCount,	ExportOffset;
	INT     ImportCount,	ImportOffset;
	FGuid	Guid;
	TArray<FGenerationInfo> Generations;

	// Constructor.
	FPackageFileSummary();

	INT GetFileVersion() const;
	INT GetFileVersionLicensee() const;
	void SetFileVersions(INT Epic, INT Licensee);
};

/*----------------------------------------------------------------------------
	ULinker.
----------------------------------------------------------------------------*/

//
// A file linker.
//
class CORE_API ULinker : public UObject
{
	DECLARE_CLASS(ULinker,UObject,CLASS_Transient,Core)
	NO_DEFAULT_CONSTRUCTOR(ULinker)

	// Variables.
	UObject*				LinkerRoot;			// The linker's root object.
	FPackageFileSummary		Summary;			// File summary.
	TArray<FName>			NameMap;			// Maps file name indices to name table indices.
	TArray<FObjectImport>	ImportMap;			// Maps file object indices >=0 to external object names.
	TArray<FObjectExport>	ExportMap;			// Maps file object indices >=0 to external object names.
	INT						Success;			// Whether the object was constructed successfully.
	FString					Filename;			// Filename.
	DWORD					_ContextFlags;		// Load flag mask.

	// Constructors.
	ULinker( UObject* InRoot, const TCHAR* InFilename );

	virtual void Serialize( FArchive& Ar );

	virtual UBOOL LinksToCode();	// True if this Linker contains code

	FString GetImportFullName( INT i );
	FString GetExportFullName( INT i, const TCHAR* FakeRoot=NULL );

	// Cheat Protection

	// The QuickMD5 hash is a check of 4 major tables for this package.  It looks at 
	// the Header, Name, Import and Export tables.  Any changes here will result in a failure.

	FString QuickMD5();		// Returns the Quick MD5 hash for this package

protected:

	BYTE QuickMD5Digest[16];	// Holds a MD5 of the 3 Tables and Summary};
};
/*----------------------------------------------------------------------------
	ULinkerLoad.
----------------------------------------------------------------------------*/

//
// A file loader.
//
class ULinkerLoad : public ULinker, public FArchive
{
	DECLARE_CLASS(ULinkerLoad,ULinker,CLASS_Transient,Core)
	NO_DEFAULT_CONSTRUCTOR(ULinkerLoad)

	// Friends.
	friend class UObject;
	friend class UPackageMap;

	// Variables.
	DWORD					LoadFlags;
	UBOOL					Verified;
	INT						ExportHash[256];
	TArray<FLazyLoader*>	LazyLoaders;
	FArchive*				Loader;

	ULinkerLoad(UObject*, void*, INT, DWORD);
	ULinkerLoad(UObject* InParent, const TCHAR* InFilename, DWORD InLoadFlags);

	void Verify();
	void VerifyImport(INT);

	INT Check(UClass*, FName, DWORD, INT);

	FName GetExportClassPackage( INT i );
	FName GetExportClassName( INT i );

	void VerifyImport( INT i );

	void LoadAllObjects();
	void LoadObject();
	void LoadOnSystemMemory();

	INT FindExportIndex( FName ClassName, FName ClassPackage, FName ObjectName, INT PackageIndex );

	UObject* Create( UClass* ObjectClass, FName ObjectName, DWORD LoadFlags, UBOOL Checked );
	void Preload( UObject* Object );

	FArchive* GetReader();

	void AskDestroy();

	virtual UBOOL LinksToCode();	// True if this Linker contains code
	
private:
	UObject* CreateExport( INT Index );
	UObject* CreateImport( INT Index );

	UObject* IndexToObject( INT Index );

	virtual void DetachExport( INT i );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void Destroy();

	// FArchive interface.
	virtual void AttachLazyLoader( FLazyLoader* LazyLoader );
	virtual void DetachLazyLoader( FLazyLoader* LazyLoader );
	virtual void DetachAllLazyLoaders(UBOOL Load);

	virtual void AttachRoughLoader(FRoughLoader*);
	virtual void DetachRoughLoader(FRoughLoader*);
	virtual void DetachAllRoughLoaders(UBOOL Load);

	virtual void Seek( INT InPos );
	virtual INT Tell();
	virtual INT TotalSize();
	virtual void Serialize( void* V, INT Length );

	virtual FArchive& operator<<(UObject*& Object);
	virtual FArchive& operator<<(FName& Name);
};

/*----------------------------------------------------------------------------
	ULinkerSave.
----------------------------------------------------------------------------*/

//
// A file saver.
//
//TODO:lordofdest: Implement full FArchive interface
class ULinkerSave : public ULinker, public FArchive
{
	DECLARE_CLASS(ULinkerSave,ULinker,CLASS_Transient,Core);
	NO_DEFAULT_CONSTRUCTOR(ULinkerSave);

	// Variables.
	FArchive* Saver;
	TArray<INT> ObjectIndices;
	TArray<INT> NameIndices;

	// Constructor.
	ULinkerSave( UObject* InParent, const TCHAR* InFilename );
	void Destroy();

	// FArchive interface.
	INT MapName( FName* Name );
	INT MapObject( UObject* Object );
	FArchive& operator<<( FName& Name )
	{
		guardSlow(ULinkerSave<<FName);
		INT Save = NameIndices(Name.GetIndex());
		return *this << AR_INDEX(Save);
		unguardobjSlow;
	}
	FArchive& operator<<( UObject*& Obj )
	{
		guardSlow(ULinkerSave<<UObject);
		INT Save = Obj ? ObjectIndices(Obj->GetIndex()) : 0;
		return *this << AR_INDEX(Save);
		unguardobjSlow;
	}
	void Seek( INT InPos );
	INT Tell();
	void Serialize( void* V, INT Length );
};

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

