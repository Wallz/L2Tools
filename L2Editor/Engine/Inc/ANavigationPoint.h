/*=============================================================================
	ANavigationPoint.h: Class functions residing in the ANavigationPoint class.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	virtual void PostEditMove();
	virtual void Spawned();
	virtual void Destroy();
	virtual INT AddMyMarker(AActor *S);
	virtual void CheckForErrors();

	virtual INT ProscribedPathTo(ANavigationPoint *Dest);
	virtual void addReachSpecs(APawn * Scout, UBOOL bOnlyChanged=false);
	virtual void PostaddReachSpecs(APawn * Scout);
	virtual void InitForPathFinding();
	virtual void SetupForcedPath(APawn* Scout, UReachSpec* Path);
	virtual void ClearPaths();
	virtual void FindBase();
	virtual UBOOL IsIdentifiedAs(FName ActorName);
	virtual UBOOL ReviewPath(APawn* Scout);
	virtual void CheckSymmetry(ANavigationPoint* Other);
	virtual void ClearForPathFinding();
	virtual class AInventorySpot* GetAInventorySpot();
	
	void CleanUpPruned();
	INT PrunePaths();
	UBOOL FindAlternatePath(UReachSpec* StraightPath, INT AccumulatedDistance);
	UReachSpec* GetReachSpecTo(ANavigationPoint *Nav);
	UBOOL ShouldBeBased();
	
	UBOOL CanReach(ANavigationPoint *Dest, FLOAT Dist);

