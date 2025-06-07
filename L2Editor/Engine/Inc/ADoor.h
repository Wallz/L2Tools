/*=============================================================================
	ADoor.h: Class functions residing in the ADoor class.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.
=============================================================================*/
	
	virtual void PrePath();
	virtual void PostPath();
	virtual AActor* AssociatedLevelGeometry();
	virtual UBOOL HasAssociatedLevelGeometry(AActor *Other);

	virtual void PostaddReachSpecs(APawn * Scout);
	virtual void InitForPathFinding();
	virtual void FindBase();
	virtual UBOOL IsIdentifiedAs(FName ActorName);

