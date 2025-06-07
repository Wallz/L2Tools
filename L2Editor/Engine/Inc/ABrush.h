/*=============================================================================
	ABrush.h.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

// Constructors.
ABrush() {}

// UObject interface.
virtual void PostLoad();
virtual void PostEditChange();

// AActor interface.
virtual void CheckForErrors();
virtual UPrimitive* GetPrimitive();

virtual FCoords ToLocal() const;
virtual FCoords ToWorld() const;

virtual UBOOL IsABrush();

// ABrush interface.
virtual void CopyPosRotScaleFrom(ABrush* Other);
virtual void InitPosRotScale();

FLOAT BuildCoords(FModelCoords* Coords, FModelCoords* Uncoords);

// OLD
// OLD
// These functions exist for the "ucc mapconvert" commandlet.  The engine/editor should NOT
// be using them otherwise.
FCoords OldToLocal();
FCoords OldToWorld() const;
FLOAT OldBuildCoords(FModelCoords* Coords, FModelCoords* Uncoords);
// OLD
// OLD


/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

