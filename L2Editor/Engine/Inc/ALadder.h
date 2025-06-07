/*=============================================================================
	ALadder.h: Class functions residing in the ALadder class.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	virtual INT ProscribedPathTo(ANavigationPoint *Dest);
	virtual void addReachSpecs(APawn * Scout, UBOOL bOnlyChanged=false);
	virtual void InitForPathFinding();
	virtual void ClearPaths();