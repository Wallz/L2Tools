/*=============================================================================
	APlayerController.h: A player pawn.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.
=============================================================================*/
	// AActor interface.
	virtual UBOOL Tick(FLOAT DeltaTime, enum ELevelTick TickType);
	virtual void PostScriptDestroyed();
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual UBOOL IsNetRelevantFor( APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation );
	virtual AActor* GetViewTarget();
	virtual INT IsAPlayerController();
	virtual UBOOL LocalPlayerController();
	virtual UBOOL WantsLedgeCheck();
	virtual UBOOL StopAtLedge();
	virtual void CheckHearSound(AActor* SoundMaker, INT Id, USound* Sound, FVector Parameters, FLOAT Radius, UBOOL Attenuate);
	// Own Functions.
	virtual void PlayerCalcView(AActor*, FVector*, FRotator*);
	virtual void SetRequestedServerMusic(TCHAR*, FLOAT);
	virtual void SetRequestedServerVoice(TCHAR*, FLOAT, INT);
	virtual UPlayerInput* GetPlayerInput();
	virtual void SetPlayerInput(UPlayerInput*);
	virtual UCheatManager* GetCheatManager();
	virtual void SetCheatManager(UCheatManager*);
	
	//  Player Pawn interface.
	void SetPlayer(UPlayer* Player);
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

