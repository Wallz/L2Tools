/*=============================================================================
	UnStationaryWeapon: 
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Joe Wilcox
=============================================================================*/

virtual void PostNetReceive();
virtual INT* GetOptimizedRepList( BYTE* Recent, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );