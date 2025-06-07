/*=============================================================================
	AMover.h: Class functions residing in the AMover class.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// Constructors.
	AMover();

	// UObject interface.
	virtual void PostLoad();
	virtual void PostEditChange();

	// AActor interface.
	virtual void Spawned();
	virtual void PostEditMove();
	virtual void PreRaytrace();
	virtual void PostRaytrace();
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual void physMovingBrush(FLOAT DeltaTime);
	virtual void performPhysics(FLOAT DeltaSeconds);
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);
	virtual INT AddMyMarker(AActor *S);
	virtual void ClearMarker();
	virtual INT CanBeAttacked();
	virtual INT IsAMover();
	virtual USound* GetAttackItemSound(FLOAT&, FLOAT&);
	virtual USound* GetAttackVoiceSound(FLOAT);
	virtual USound* GetDamageSound(FLOAT&, FLOAT&);
	virtual USound* GetDefenseItemSound(FLOAT&, FLOAT&);
	virtual USound* GetShieldItemSound(FLOAT&, FLOAT&);
	virtual void AssociateAttackedNotify(APawn*, AActor*, INT, INT, INT, INT, INT, INT, INT, INT, INT);
	virtual void AttackedNotify(APawn*, AActor*, INT, INT, INT);
	virtual void GetTargetLocation(FVector, FVector&);

	// AMover interface.
	virtual void SetWorldRaytraceKey();
	virtual void SetBrushRaytraceKey();

	USound* GetBrokenSound(FLOAT&, FLOAT&);

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

