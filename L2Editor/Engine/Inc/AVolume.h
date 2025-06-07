/*=============================================================================
	AVolume.h: Class functions residing in the AVolume class.
	Copyright 2001 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

virtual void PostBeginPlay();

virtual UBOOL IsAVolume();
virtual void SetVolumes();
virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);

//Own
INT Encompasses(FVector point);

