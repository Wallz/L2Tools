/*=============================================================================
	UnClass.h: UClass definition.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney.
=============================================================================*/
#ifdef _MSC_VER
#pragma warning( disable : 4121 )
#endif
// vogel: alignment of a member was sensitive to packing

/*-----------------------------------------------------------------------------
	Constants.
-----------------------------------------------------------------------------*/

// Boundary to align class properties on.
enum {PROPERTY_ALIGNMENT=4 };

/*-----------------------------------------------------------------------------
	FRepRecord.
-----------------------------------------------------------------------------*/

//
// Information about a property to replicate.
//
struct FRepRecord
{
	UProperty* Property;
	INT Index;
	FRepRecord(UProperty* InProperty,INT InIndex)
	: Property(InProperty), Index(InIndex)
	{}
};

/*-----------------------------------------------------------------------------
	FDependency.
-----------------------------------------------------------------------------*/

//
// One dependency record, for incremental compilation.
//
class CORE_API FDependency
{
public:
	// Variables.
	UClass*		Class;
	UBOOL		Deep;
	DWORD		ScriptTextCRC;

	// Functions.
	FDependency();
	FDependency( UClass* InClass, UBOOL InDeep );
	UBOOL IsUpToDate();

	CORE_API friend FArchive& operator<<( FArchive& Ar, FDependency& Dep );
};

/*-----------------------------------------------------------------------------
	FRepLink.
-----------------------------------------------------------------------------*/

//
// A tagged linked list of replicatable variables.
//
class FRepLink
{
public:
	UProperty*	Property;		// Replicated property.
	FRepLink*	Next;			// Next replicated link per class.
	FRepLink( UProperty* InProperty, FRepLink* InNext )
	:	Property	(InProperty)
	,	Next		(InNext)
	{}
};

/*-----------------------------------------------------------------------------
	FLabelEntry.
-----------------------------------------------------------------------------*/

//
// Entry in a state's label table.
//
struct CORE_API FLabelEntry
{
	// Variables.
	FName	Name;
	INT		iCode;

	// Functions.
	FLabelEntry( FName InName, INT iInCode );

	CORE_API friend FArchive& operator<<( FArchive& Ar, FLabelEntry &Label );
};

/*-----------------------------------------------------------------------------
	UField.
-----------------------------------------------------------------------------*/

//
// Base class of UnrealScript language objects.
//
class CORE_API UField : public UObject
{
	DECLARE_ABSTRACT_CLASS(UField,UObject,0,Core)
	NO_DEFAULT_CONSTRUCTOR(UField)

	// Constants.
	enum {HASH_COUNT = 256};

	// Variables.
	UField*			SuperField;
	UField*			Next;
	UField*			HashNext;

	// Constructors.
	UField( ENativeConstructor, UClass* InClass, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags, UField* InSuperField );
	UField( EStaticConstructor, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags );
	UField( UField* InSuperField );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void PostLoad();
	virtual void Register();

	// UField interface.
	virtual void AddCppProperty( UProperty* Property );
	virtual UBOOL MergeBools();
	virtual void Bind();
	virtual UClass* GetOwnerClass();
	virtual INT GetPropertiesSize();
};

/*-----------------------------------------------------------------------------
	TFieldIterator.
-----------------------------------------------------------------------------*/

//
// For iterating through a linked list of fields.
//
template <class T> class TFieldIterator
{
public:
	TFieldIterator( UStruct* InStruct )
	: Struct( InStruct )
	, Field( InStruct ? InStruct->Children : NULL )
	{
		IterateToNext();
	}
	operator UBOOL()
	{
		return Field != NULL;
	}
	void operator++()
	{
		checkSlow(Field);
		Field = Field->Next;
		IterateToNext();
	}
	T* operator*()
	{
		checkSlow(Field);
		return (T*)Field;
	}
	T* operator->()
	{
		checkSlow(Field);
		return (T*)Field;
	}
	UStruct* GetStruct()
	{
		return Struct;
	}
protected:
	void IterateToNext()
	{
		while( Struct )
		{
			while( Field )
			{
				if( Field->IsA(T::StaticClass()) )
					return;
				Field = Field->Next;
			}
			Struct = Struct->GetInheritanceSuper();
			if( Struct )
				Field = Struct->Children;
		}
	}
	UStruct* Struct;
	UField* Field;
};

template <class T, EClassFlags Flag> class TFieldFlagIterator // sjs
{
public:
	TFieldFlagIterator( UStruct* InStruct )
	: Struct( InStruct )
	, Field( InStruct ? InStruct->Children : NULL )
	{
		IterateToNext();
	}
	inline operator UBOOL()
	{
		return Field != NULL;
	}
	inline void operator++()
	{
		checkSlow(Field);
		Field = Field->Next;
		IterateToNext();
	}
	inline T* operator*()
	{
		checkSlow(Field);
		return (T*)Field;
	}
	inline T* operator->()
	{
		checkSlow(Field);
		return (T*)Field;
	}
	inline UStruct* GetStruct()
	{
		return Struct;
	}
protected:
	inline void IterateToNext()
	{
		while( Struct )
		{
			while( Field )
			{
                if( (Field->GetClass()->ClassFlags & Flag) )
                    return;
				Field = Field->Next;
			}
			Struct = Struct->GetInheritanceSuper();
			if( Struct )
				Field = Struct->Children;
		}
	}
	UStruct* Struct;
	UField* Field;
};

/*-----------------------------------------------------------------------------
	UStruct.
-----------------------------------------------------------------------------*/

enum EStructFlags
{
	// State flags.
	STRUCT_Native		= 0x00000001,	
	STRUCT_Export		= 0x00000002,
	// gam ---
	STRUCT_Long	        = 0x00000004, // will get shown as "..." in editactor until expanded.
	STRUCT_Inherit      = STRUCT_Long,
	// --- gam
};

//
// An UnrealScript structure definition.
//
class CORE_API UStruct : public UField
{
	DECLARE_CLASS(UStruct,UField,0,Core)
	NO_DEFAULT_CONSTRUCTOR(UStruct)

	// Variables.
	UTextBuffer*		ScriptText;
	UTextBuffer*		CppText;
	UField*				Children;
	INT					PropertiesSize;
	FName				FriendlyName;
	TArray<BYTE>		Script;

	// Compiler info.
	INT					TextPos;
	INT					Line;
	DWORD					StructFlags;

	// In memory only.
	UProperty*			RefLink;
	UProperty*			PropertyLink;
	UProperty*			ConfigLink;
	UProperty*			ConstructorLink;

	// Constructors.
	UStruct( ENativeConstructor, INT InSize, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags, UStruct* InSuperStruct );
	UStruct( EStaticConstructor, INT InSize, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags );
	UStruct( UStruct* InSuperStruct );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void PostLoad();
	virtual void Destroy();
	virtual void Register();

	// UField interface.
	virtual void AddCppProperty( UProperty* Property );
	virtual INT GetPropertiesSize();

	// UStruct interface.
	virtual UStruct* GetInheritanceSuper() {return GetSuperStruct();}
	virtual void Link( FArchive& Ar, UBOOL Props );
	virtual void SerializeBin( FArchive& Ar, BYTE* Data, INT MaxReadBytes );
	virtual void SerializeTaggedProperties( FArchive& Ar, BYTE* Data, UClass* DefaultsClass );
	virtual void CleanupDestroyed( BYTE* Data );
	virtual EExprToken SerializeExpr( INT& iCode, FArchive& Ar );
	virtual TCHAR* GetNameCPP();

	void SetPropertiesSize(INT NewSize);

	UBOOL IsChildOf(const UStruct* SomeBase) const;
	
	UStruct* GetSuperStruct() const;

	UBOOL StructCompare( const void* A, const void* B );

	DWORD GetScriptTextCRC();
	FString FunctionMD5();		// Returns the Quick MD5 hash for this package	

protected:

	// Cheat Protection
		
	BYTE FunctionMD5Digest[16];		// Holds a MD5 digest for this function
};

/*-----------------------------------------------------------------------------
	UFunction.
-----------------------------------------------------------------------------*/

//
// An UnrealScript function.
//
class CORE_API UFunction : public UStruct
{
	DECLARE_CLASS(UFunction,UStruct,CLASS_IsAUFunction,Core)
	DECLARE_WITHIN(UState)
	NO_DEFAULT_CONSTRUCTOR(UFunction)

	// Persistent variables.
	DWORD FunctionFlags;
	_WORD iNative;
	_WORD RepOffset;
	BYTE  OperPrecedence;

	// Variables in memory only.
	BYTE  NumParms;
	_WORD ParmsSize;
	_WORD ReturnValueOffset;
	void (UObject::*Func)( FFrame& TheStack, RESULT_DECL );
#if DO_GUARD_SLOW
	SQWORD Calls,Cycles;
#endif

	// Constructors.
	UFunction( UFunction* InSuperFunction );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void PostLoad();

	// UField interface.
	virtual void Bind();

	// UStruct interface.
	virtual UBOOL MergeBools();
	virtual UStruct* GetInheritanceSuper();
	virtual void Link( FArchive& Ar, UBOOL Props );

	// UFunction interface.
	UFunction* GetSuperFunction() const;
	UProperty* GetReturnProperty();
};

/*-----------------------------------------------------------------------------
	UState.
-----------------------------------------------------------------------------*/

//
// An UnrealScript state.
//
class CORE_API UState : public UStruct
{
	DECLARE_CLASS(UState,UStruct,CLASS_IsAUState,Core)
	NO_DEFAULT_CONSTRUCTOR(UState)

	// Variables.
	QWORD ProbeMask;
	QWORD IgnoreMask;
	DWORD StateFlags;
	_WORD LabelTableOffset;
	UField* VfHash[HASH_COUNT];

	// Constructors.
	UState( ENativeConstructor, INT InSize, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags, UState* InSuperState );
	UState( EStaticConstructor, INT InSize, const TCHAR* InName, const TCHAR* InPackageName, DWORD InFlags );
	UState( UState* InSuperState );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void Destroy();

	// UStruct interface.
	virtual UBOOL MergeBools();
	virtual UStruct* GetInheritanceSuper();
	virtual void Link( FArchive& Ar, UBOOL Props );

	// UState interface.
	UState* GetSuperState() const;
};

/*-----------------------------------------------------------------------------
	UEnum.
-----------------------------------------------------------------------------*/

//
// An enumeration, a list of names usable by UnrealScript.
//
class CORE_API UEnum : public UField
{
	DECLARE_CLASS(UEnum,UField,0,Core)
	DECLARE_WITHIN(UStruct)
	NO_DEFAULT_CONSTRUCTOR(UEnum)

	// Variables.
	TArray<FName> Names;

	// Constructors.
	UEnum(UEnum* InSuperEnum);

	// UObject interface.
	virtual void Serialize( FArchive& Ar );

	// UEnum interface.
	UEnum* GetSuperEnum() const;
};

/*-----------------------------------------------------------------------------
	UClass.
-----------------------------------------------------------------------------*/
#define TRACK_ISA 0
//
// An object class.
//
class CORE_API UClass : public UState
{
	DECLARE_CLASS(UClass,UState,0,Core)
	DECLARE_WITHIN(UPackage)

	// Variables.
	DWORD				ClassFlags;
	INT					ClassUnique;
	FGuid				ClassGuid;
	UClass*				ClassWithin;
	FName				ClassConfigName;
	TArray<FRepRecord>	ClassReps;
	TArray<UField*>		NetFields;
	TArray<FDependency> Dependencies;
	TArray<FName>		PackageImports;
	TArray<BYTE>		Defaults;
	TArray<FName>		HideCategories;
    TArray<FName>       DependentOn; //amb,gam
	void(*ClassConstructor)(void*);
	void(UObject::*ClassStaticConstructor)();

	// In memory only.
	FString				DefaultPropText;
#if TRACK_ISA
    INT IsACount;
#endif

	// Constructors.
	UClass();
	UClass( UClass* InSuperClass );
	UClass( ENativeConstructor, DWORD InSize, DWORD InClassFlags, UClass* InBaseClass, UClass* InWithinClass, FGuid InGuid, const TCHAR* InNameStr, const TCHAR* InPackageName, const TCHAR* InClassConfigName, DWORD InFlags, void(*InClassConstructor)(void*), void(UObject::*InClassStaticConstructor)() );
	UClass( EStaticConstructor, DWORD InSize, DWORD InClassFlags, FGuid InGuid, const TCHAR* InNameStr, const TCHAR* InPackageName, const TCHAR* InClassConfigName, DWORD InFlags, void(*InClassConstructor)(void*), void(UObject::*InClassStaticConstructor)() );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );
	virtual void PostLoad();
	virtual void Destroy();
	virtual void Register();

	// UField interface.
	virtual void Bind();

	// UStruct interface.
	virtual UBOOL MergeBools() {return 1;}
	virtual UStruct* GetInheritanceSuper() {return GetSuperClass();}
	virtual TCHAR* GetNameCPP();
	virtual void Link( FArchive& Ar, UBOOL Props );

	// UClass interface.
	void AddDependency(UClass* InClass, UBOOL InDeep);
	UClass* GetSuperClass() const;
	UObject* GetDefaultObject();
	class AActor* GetDefaultActor();
	class ULevelObject* GetDefaultLevelObject();
	INT HasNativesToExport(UObject*);

private:
	// Hide IsA because calling IsA on a class almost always indicates
	// an error where the caller should use IsChildOf.
	UBOOL IsA(UClass* Parent) const;
};

/*-----------------------------------------------------------------------------
	UConst.
-----------------------------------------------------------------------------*/

//
// An UnrealScript constant.
//
class CORE_API UConst : public UField
{
	DECLARE_CLASS(UConst,UField,0,Core)
	DECLARE_WITHIN(UStruct)
	NO_DEFAULT_CONSTRUCTOR(UConst)

	// Variables.
	FString Value;

	// Constructors.
	UConst( UConst* InSuperConst, const TCHAR* InValue );

	// UObject interface.
	virtual void Serialize( FArchive& Ar );

	// UConst interface.
	UConst* GetSuperConst() const;
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

