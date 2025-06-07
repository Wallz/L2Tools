/*=============================================================================
	AController.h: AI or player.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );

	virtual void StopMove(void);
	virtual INT HitWallNotify(FVector, AActor *);
	virtual void ResetMusicControll();
	virtual void SetVehicleStatus(INT);
	virtual UBOOL IsObserverMode();
	virtual UBOOL IsBroadcastObserverMode();
	virtual void SetBroadcastObserverMode(INT);
	virtual void CheckHearSound(AActor* SoundMaker, INT Id, USound* Sound, FVector Parameters, FLOAT Radius, UBOOL Attenuate);
	virtual AActor* GetViewTarget();
	virtual INT AcceptNearbyPath(AActor* goal);
	virtual void AdjustFromWall(FVector HitNormal, AActor* HitActor);
	virtual void SetAdjustLocation(FVector NewLoc);
	virtual UBOOL LocalPlayerController();
	virtual UBOOL WantsLedgeCheck();
	virtual UBOOL StopAtLedge();
	virtual AActor* GetSelectedActor();
	virtual void SetSelectedActor(AActor*);
	virtual INT GetSelectedCreatureID();
	virtual void SetSelectedCreatureID(INT);

	// Seeing and hearing checks
	int CanHear(FVector NoiseLoc, FLOAT Loudness, AActor *Other); 
	void ShowSelf();
	DWORD SeePawn(APawn *Other, UBOOL bMaySkipChecks=true);
	DWORD LineOfSightTo(AActor *Other, INT bUseLOSFlag=0);
	void CheckEnemyVisible();
	void StartAnimPoll();
	UBOOL CheckAnimFinished(INT Channel);
	UBOOL CanHearSound(FVector HearSource, AActor* SoundMaker, FLOAT Radius);
	
	AActor* HandleSpecial(AActor *bestPath);
	
	void SetRouteCache(ANavigationPoint *EndPath, FLOAT StartDist, FLOAT EndDist);
	AActor* FindPath(FVector point, AActor* goal, UBOOL bWeightDetours);
	AActor* SetPath(INT bInitialPath=1);
	
	void CheckFears();

	void AddMoveTo(FVector, FVector);
	void AddMoveToward(FVector, AActor*, FLOAT);
	void MoveBackTo(FVector, AActor*, FLOAT);
	void MoveTo(FVector, AActor*, FLOAT);
	void MoveToNotRotation(FVector, AActor*, FLOAT);
	void MoveToward(AActor*, AActor*, FLOAT);
	void StopMoveToward();
	void StopMoveWithLocation(FVector);

	void AddFlyTo(FVector, FVector, INT);
	void FlyTo(FVector, INT, AActor*, FLOAT);

	void L2NetCommand(INT);
	void ClearL2NetCommand();
	void PendingL2NetCommand();

	// Natives.
	DECLARE_FUNCTION(execPollWaitForLanding)
	DECLARE_FUNCTION(execPollMoveTo)
	DECLARE_FUNCTION(execPollMoveToward)
	DECLARE_FUNCTION(execPollFinishRotation)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

