/*=============================================================================
	UnName.h: Unreal global name types.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*----------------------------------------------------------------------------
	Definitions.
----------------------------------------------------------------------------*/

// Maximum size of name.
enum {NAME_SIZE	= 64};

// Name index.
typedef INT NAME_INDEX;

// Enumeration for finding name.
enum EFindName
{
	FNAME_Find,			// Find a name; return 0 if it doesn't exist.
	FNAME_Add,			// Find a name or add it if it doesn't exist.
	FNAME_Intrinsic,	// Find a name or add it intrinsically if it doesn't exist.
};

/*----------------------------------------------------------------------------
	FNameEntry.
----------------------------------------------------------------------------*/

//
// A global name, as stored in the global name table.
//
struct FNameEntry
{
	// Variables.
	NAME_INDEX	Index;				// Index of name in hash.
	DWORD		Flags;				// RF_TagImp, RF_TagExp, RF_Native.
	FNameEntry*	HashNext;			// Pointer to the next entry in this hash bin's linked list.

	// The name string.
	TCHAR		Name[NAME_SIZE];	// Name, variable-sized.

	// Functions.
	CORE_API friend FArchive& operator<<( FArchive& Ar, FNameEntry& E );
	CORE_API friend FNameEntry* AllocateNameEntry( const TCHAR* Name, DWORD Index, DWORD Flags, FNameEntry* HashNext );
};
template <> struct TTypeInfo<FNameEntry*> : public TTypeInfoBase<FNameEntry*>
{
	static UBOOL NeedsDestructor() {return 0;}
};

/*----------------------------------------------------------------------------
	FName.
----------------------------------------------------------------------------*/

//
// Public name, available to the world.  Names are stored as WORD indices
// into the name table and every name in Unreal is stored once
// and only once in that table.  Names are case-insensitive.
//
#define checkName checkSlow
class CORE_API FName 
{
public:
	// Accessors.
	const TCHAR* operator*() const;
	NAME_INDEX GetIndex() const;
	DWORD GetFlags() const;
	void SetFlags(DWORD Set) const;
	void ClearFlags(DWORD Clear) const;

	UBOOL operator==(const FName& Other) const;
	UBOOL operator!=(const FName& Other) const;

	UBOOL IsValid();

	// Constructors.
	FName(enum EName N);
	FName();
	FName( const TCHAR* Name, EFindName FindType=FNAME_Add );

	// Name subsystem.
	static void StaticInit();
	static void StaticExit();
	static void DeleteEntry( int i );
	static void DisplayHash( class FOutputDevice& Ar );
	static void Hardcode( FNameEntry* AutoName );

	// Name subsystem accessors.
	static const TCHAR* SafeString(EName Index);
	static UBOOL SafeSuppressed(EName Index);
	static int GetMaxNames();
	static FNameEntry* GetEntry(int i);
	static UBOOL GetInitialized();

private:
	// Name index.
	NAME_INDEX Index;

	// Static subsystem variables.
	static TArray<FNameEntry*>	Names;			 // Table of all names.
	static TArray<INT>          Available;       // Indices of available names.
	static FNameEntry*			NameHash[4096];  // Hashed names.
	static UBOOL				Initialized;	 // Subsystem initialized.
};

inline DWORD GetTypeHash( const FName N )
{
	return N.GetIndex();
}

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

