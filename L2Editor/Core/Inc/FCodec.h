/*=============================================================================
UnCodec.h: Data compression codecs.
Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

Revision history:
* Created by Tim Sweeney.
=============================================================================*/

/*-----------------------------------------------------------------------------
Coder/decoder base class.
-----------------------------------------------------------------------------*/

#ifndef FCODEC_H
#define FCODEC_H

class CORE_API FCodec
{
public:
	virtual UBOOL Encode(void*, INT);
	virtual UBOOL Encode(FArchive&, FArchive&);
	virtual UBOOL Encode(char const*, char const*, BYTE*, DWORD, BYTE*, DWORD&, INT);
	virtual UBOOL Decode(FArchive&, BYTE**, INT&);
	virtual UBOOL Decode(FArchive&, FArchive&);
	virtual UBOOL Decode(char const*, char const*, BYTE*, DWORD, BYTE*, DWORD&, INT, INT);
};

class CORE_API FDummyCodec : public FCodec
{
public:
	virtual UBOOL Encode(FArchive&, FArchive&);
	virtual UBOOL Decode(FArchive&, BYTE**, INT&);
	virtual UBOOL Decode(FArchive&, FArchive&);
};

#endif
/*-----------------------------------------------------------------------------
The End.
-----------------------------------------------------------------------------*/
