/*=============================================================================
	UnArc.h: Unreal archive class.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Archive.
-----------------------------------------------------------------------------*/

//
// Archive class. Used for loading, saving, and garbage collecting
// in a byte order neutral way.
//
class CORE_API FArchive
{
public:
	// FArchive interface.
	virtual ~FArchive();

	virtual void Serialize(void* V, INT Length);
	virtual void SerializeBits(void* V, INT LengthBits);
	virtual void SerializeInt(DWORD& Value, DWORD Max);

	virtual void Preload(UObject* Object);
	virtual void CountBytes(SIZE_T InNum, SIZE_T InMax);

	virtual FArchive& operator<<(class UObject*& Res);
	virtual FArchive& operator<<(class FName& N);

	virtual INT MapName(FName* Name);
	virtual INT MapObject(UObject* Object);
	virtual INT Tell();
	virtual INT TotalSize();
	virtual UBOOL AtEnd();
	virtual UBOOL AtStopper();
	virtual void SetStopper(INT InStopper = INDEX_NONE);
	virtual void Seek(INT InPos);

	virtual void AttachLazyLoader(FLazyLoader* LazyLoader);
	virtual void DetachLazyLoader(FLazyLoader* LazyLoader);
	virtual void AttachRoughLoader(FRoughLoader*) {}
	virtual void DetachRoughLoader(FRoughLoader*) {}

	virtual void Precache(INT HintCount);
	virtual void Flush();
	virtual UBOOL Close();

	virtual UBOOL GetError();

	virtual INT GetCurrentEncryptVersion();

	// Hardcoded datatype routines that may not be overridden.
	FArchive& ByteOrderSerialize(void* V, INT Length);

	void ThisContainsCode();
	// Constructor.
	FArchive();

	// Status accessors.
	INT Ver();
	INT NetVer();
	INT LicenseeVer();
	UBOOL IsLoading();
	UBOOL IsSaving();
	UBOOL IsTrans();
	UBOOL IsNet();
	UBOOL IsPersistent();
	UBOOL IsError();
	UBOOL ForEdit();
	UBOOL ForClient();
	UBOOL ForServer();
	UBOOL ContainsCode();

	// Friend archivers.
	friend FArchive& operator<<( FArchive& Ar, ANSICHAR& C )
	{
		Ar.Serialize( &C, 1 );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, BYTE& B )
	{
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, SBYTE& B )
	{
		Ar.Serialize( &B, 1 );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, _WORD& W )
	{
		Ar.ByteOrderSerialize( &W, sizeof(W) );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, SWORD& S )
	{
		Ar.ByteOrderSerialize( &S, sizeof(S) );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, DWORD& D )
	{
		Ar.ByteOrderSerialize( &D, sizeof(D) );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, INT& I )
	{
		Ar.ByteOrderSerialize( &I, sizeof(I) );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, FLOAT& F )
	{
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
#ifndef __PSX2_EE__
	friend FArchive& operator<<( FArchive& Ar, DOUBLE& F )
	{
		Ar.ByteOrderSerialize( &F, sizeof(F) );
		return Ar;
	}
#endif
	friend FArchive& operator<<( FArchive &Ar, QWORD& Q )
	{
		Ar.ByteOrderSerialize( &Q, sizeof(Q) );
		return Ar;
	}
	friend FArchive& operator<<( FArchive& Ar, SQWORD& S )
	{
		Ar.ByteOrderSerialize( &S, sizeof(S) );
		return Ar;
	}

protected:
	// Status variables.
	INT ArVer;
	INT ArNetVer;
	INT ArLicenseeVer;
	UBOOL ArIsLoading;
	UBOOL ArIsSaving;
	UBOOL ArIsTrans;
	UBOOL ArIsPersistent;
	UBOOL ArForEdit;
	UBOOL ArForClient;
	UBOOL ArForServer;
	UBOOL ArIsError;
	UBOOL ArContainsCode;			// Quickly tell if an archive contains script code

	// Prevent archive passing a set stopper position
	INT Stopper;
};

/*-----------------------------------------------------------------------------
	FArchive macros.
-----------------------------------------------------------------------------*/

//
// Class for serializing objects in a compactly, mapping small values
// to fewer bytes.
//
class CORE_API FCompactIndex
{
public:
	INT Value;
	CORE_API friend FArchive& operator<<( FArchive& Ar, FCompactIndex& I );
};

//
// Archive constructor.
//
template <class T> T Arctor( FArchive& Ar )
{
	T Tmp;
	Ar << Tmp;
	return Tmp;
}

// Macro to serialize an integer as a compact index.
#define AR_INDEX(intref) \
	(*(FCompactIndex*)&(intref))

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/

