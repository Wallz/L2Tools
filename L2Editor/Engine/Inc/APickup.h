/*=============================================================================
	APickup.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// Constructors.
	APickup() {}

	// AActor interface.
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual INT AddMyMarker(AActor *S);
	virtual void ClearMarker();
	virtual void CheckForErrors();
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

