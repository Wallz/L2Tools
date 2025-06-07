/*=============================================================================
	ALevelInfo.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// Constructors.
	ALevelInfo();

	// AActor interface.
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual void CheckForErrors();
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual void SetZone(UBOOL bTest, UBOOL bForceRefresh);
	virtual void SetVolumes();

	// Level functions
	APhysicsVolume* GetDefaultPhysicsVolume();
	APhysicsVolume* GetPhysicsVolume(FVector Loc, AActor *A, UBOOL bUseTouch);

	APhysicsVolume* L2GetPhysicsVolume(FVector, AActor*, INT);

	class AAirVolume* GetAirVolume(FVector, AActor*, INT);
	class AAirVolume* L2GetAirVolume(FVector, AActor*, INT);

	class AMusicVolume* GetMusicVolume(FVector, AActor*, INT);
	class AMusicVolume* L2GetMusicVolume(FVector, AActor*, INT);
	
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

