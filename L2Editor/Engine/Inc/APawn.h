/*=============================================================================
	APawn.h: Class functions residing in the APawn class.
	Copyright 1997-2001 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

// declare type for node evaluation functions
typedef FLOAT ( *NodeEvaluator ) (ANavigationPoint*, APawn*, FLOAT);

	// Constructors.
	APawn();

	// AActor interface.
	virtual FLOAT GetNetPriority( AActor* Sent, FLOAT Time, FLOAT Lag );
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual void NotifyAnimEnd( int Channel );
	virtual void NotifyBump(AActor *Other);
	virtual UBOOL CheckOwnerUpdated();
	virtual void TickSimulated( FLOAT DeltaSeconds );
	virtual void TickSpecial( FLOAT DeltaSeconds );
	virtual UBOOL PlayerControlled();
	virtual void SetBase(AActor *NewBase, FVector NewFloor = FVector(0,0,1), int bNotifyActor=1);
	virtual void CheckForErrors();
	virtual UBOOL IsNetRelevantFor( APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation );
	virtual UBOOL CacheNetRelevancy(UBOOL bIsRelevant, APlayerController* RealViewer, AActor* Viewer);
	virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual void PostNetReceiveLocation();
	virtual void RenderEditorSelected(FLevelSceneNode* SceneNode,FRenderInterface* RI, FDynamicActor* FDA);
	virtual FLOAT GetAmbientVolume(FLOAT Attenuation);
	virtual void SetZone( UBOOL bTest, UBOOL bForceRefresh );
	virtual APawn* GetPawn();
	virtual APawn* GetPlayerPawn()const;
	virtual FLOAT GetDamageDist();
	virtual FName GetCapeBoneName();
	virtual FName GetHeadBoneName();
	virtual FName GetLArmBoneName();
	virtual FName GetLHandBoneName();
	virtual FName GetLowbodyBoneName();
	virtual FName GetRArmBoneName();
	virtual FName GetRHandBoneName();
	virtual FName GetSpineBoneName();
	virtual FRotator FindSlopeRotation(FVector, FRotator);
	virtual INT CanBeAttacked();
	virtual INT CanBeTold();
	virtual INT IsAPawn();
	virtual INT IsBroadcastObserverModeActor();
	virtual INT IsDamageAct();
	virtual INT IsLoaded();
	virtual INT IsObserverModeActor();
	virtual INT IsPlayer();
	virtual INT IsRendered();
	virtual INT IsSameAnim();
	virtual INT IsSpineRotation();
	virtual INT PostLoadProcess();
	virtual INT Tick(FLOAT, enum ELevelTick);
	virtual UMaterial* GetCloakSkin(INT);
	virtual UMaterial* GetLeftArmSkin(INT);
	virtual UMaterial* GetLeftHandSkin(INT);
	virtual UMaterial* GetRightArmSkin(INT);
	virtual UMaterial* GetRightHandSkin(INT);
	virtual UMaterial* GetSubSkin(INT);
	virtual UMesh* GetCloakCoverMesh();
	virtual UMesh* GetCloakMesh();
	virtual UMesh* GetLeftArmMesh();
	virtual UMesh* GetLeftHandMesh();
	virtual UMesh* GetRightArmMesh();
	virtual UMesh* GetRightHandMesh();
	virtual UMesh* GetSubMesh(INT);
	virtual UMeshInstance* GetCloakCoverMeshInstance();
	virtual UMeshInstance* GetCloakMeshInstance();
	virtual UMeshInstance* GetLeftArmMeshInstance();
	virtual UMeshInstance* GetLeftHandMeshInstance();
	virtual UMeshInstance* GetRightArmMeshInstance();
	virtual UMeshInstance* GetRightHandMeshInstance();
	virtual UMeshInstance* GetSubMeshInstance(INT);
	virtual USound* GetAttackItemSound(FLOAT&, FLOAT&);
	virtual USound* GetAttackVoiceSound(FLOAT);
	virtual USound* GetDamageSound(FLOAT&, FLOAT&);
	virtual USound* GetDefenseItemSound(FLOAT&, FLOAT&);
	virtual USound* GetShieldItemSound(FLOAT&, FLOAT&);
	virtual struct FNMagicInfo* GetMagicInfo();
	virtual void AssociateAttackedNotify(APawn*, AActor*, INT, INT, INT, INT, INT, INT, INT, INT, INT);
	virtual void Attacked();
	virtual void AttackedNotify(APawn*, AActor*, INT, INT, INT);
	virtual void CheckSameAnim();
	virtual void ClearL2Game();
	virtual void CreateSkin(FRenderInterface*);
	virtual void Destroy();
	virtual void LostChild(AActor*);
	virtual void PostBeginPlay();
	virtual void PostRender();
	virtual void PostScriptDestroyed();
	virtual void SetCapeBoneName(FName);
	virtual void SetCloakCoverMesh(UMesh*);
	virtual void SetCloakMesh(UMesh*);
	virtual void SetHeadBoneName(FName);
	virtual void SetLArmBoneName(FName);
	virtual void SetLHandBoneName(FName);
	virtual void SetLeftArmMesh(UMesh*);
	virtual void SetLeftHandMesh(UMesh*);
	virtual void SetLowbodyBoneName(FName);
	virtual void SetMeshes(INT, FName, INT);
	virtual void SetRArmBoneName(FName);
	virtual void SetRHandBoneName(FName);
	virtual void SetRightArmMesh(UMesh*);
	virtual void SetRightHandMesh(UMesh*);
	virtual void SetSpineBoneName(FName);
	virtual void SetSubMesh(INT, UMesh*);
	virtual void SetTexes(INT, FName, INT);
	virtual void SetVolumes();
	virtual void SmoothHitWall(FVector, AActor*);
	virtual void Spawned();
	virtual void UpdateAnimation(FLOAT);
	virtual void UpdateRenderData();
	virtual void performPhysics(FLOAT);
	virtual void physFalling(FLOAT, INT);
	virtual void processHitWall(FVector, AActor*);
	virtual void processLanded(FVector, AActor*, FLOAT, INT);
	virtual void stepUp(FVector, FVector, FVector, struct FCheckResult&);
	// Own Functions.
	virtual UBOOL HurtByVolume(AActor *A);
	virtual void physicsRotation(FLOAT deltaTime, FVector OldVelocity);
	virtual void PlayAnim(INT Channel, FName SequenceName, FLOAT PlayAnimRate, FLOAT TweenTime, INT Loop, INT);
	virtual void MoveToLocation(FVector, FVector);
	virtual void TickMoveInVehicle(FLOAT);

	// Latent movement
	void setMoveTimer(FLOAT MoveSize);
	FLOAT GetMaxSpeed();
	UBOOL moveToward(const FVector &Dest, AActor *GoalActor);
	void rotateToward(AActor *Focus, FVector FocalPoint);
	UBOOL PickWallAdjust(FVector WallHitNormal);
	void StartNewSerpentine(FVector Dir,FVector Start);
	void ClearSerpentine();

	// reach tests
	UBOOL ReachedDestination(FVector Dir, AActor* GoalActor);
	int pointReachable(FVector aPoint, int bKnowVisible=0);
	int actorReachable(AActor *Other, UBOOL bKnowVisible=0, UBOOL bNoAnchorCheck=0);
	int Reachable(FVector aPoint, AActor* GoalActor);
	int walkReachable(FVector Dest, int reachFlags, AActor* GoalActor);
	int jumpReachable(FVector Dest, int reachFlags, AActor* GoalActor);
	int flyReachable(FVector Dest, int reachFlags, AActor* GoalActor);
	int swimReachable(FVector Dest, int reachFlags, AActor* GoalActor);
	int ladderReachable(FVector Dest, int reachFlags, AActor* GoalActor);
	
	// movement component tests (used by reach tests)
	ETestMoveResult jumpLanding(FVector testvel, int moveActor);
	ETestMoveResult walkMove(FVector Delta, FCheckResult& Hit, AActor* GoalActor, FLOAT threshold);
	ETestMoveResult flyMove(FVector Delta, AActor* GoalActor, FLOAT threshold);
	ETestMoveResult swimMove(FVector Delta, AActor* GoalActor, FLOAT threshold);
	ETestMoveResult FindBestJump(FVector Dest); 
	ETestMoveResult FindJumpUp(FVector Direction); 
	ETestMoveResult HitGoal(AActor *GoalActor); 
	ETestMoveResult L2walkMove(FVector, FCheckResult&, AActor*, FLOAT);
	ETestMoveResult L2walkMove2(FVector, FCheckResult&, AActor*, FLOAT);
	FVector SuggestJumpVelocity(FVector Dest, FLOAT XYSpeed, FLOAT BaseZ);
	
	UBOOL CanCrouchWalk( const FVector& StartLocation, const FVector& EndLocation );

	// Path finding
	UBOOL ValidAnchor(); 
	FLOAT findPathToward(AActor *goal, FVector GoalLocation, NodeEvaluator NodeEval, FLOAT BestWeight, UBOOL bWeightDetours);
	ANavigationPoint* breadthPathTo(NodeEvaluator NodeEval, ANavigationPoint *start, int moveFlags, FLOAT *Weight, UBOOL bWeightDetours);
	ANavigationPoint* CheckDetour(ANavigationPoint* BestDest, ANavigationPoint* Start, UBOOL bWeightDetours);
	int calcMoveFlags(); // FIXME: This used to be inline, but that didn't compile with static linking.
	void SetAnchor(ANavigationPoint *NewAnchor);

	// Pawn physics modes
	FVector CheckForLedges(FVector AccelDir, FVector Delta, FVector GravDir, int &bCheckedFall, int &bMustJump );
	void physWalking(FLOAT deltaTime, INT Iterations);
	void physFlying(FLOAT deltaTime, INT Iterations);
	void physSwimming(FLOAT deltaTime, INT Iterations);
	void physSpider(FLOAT deltaTime, INT Iterations);
	void physLadder(FLOAT deltaTime, INT Iterations);
	void physNMover(FLOAT deltaTime, INT Iterations);
	void startNewPhysics(FLOAT deltaTime, INT Iterations);
	void startSwimming(FVector OldLocation, FVector OldVelocity, FLOAT timeTick, FLOAT remainingTime, INT Iterations);
	
	void Crouch(INT bTest=0);
	void UnCrouch(INT bTest=0);
	FVector NewFallVelocity(FVector OldVelocity, FVector OldAcceleration, FLOAT timeTick);

	// animation functions - temp FIXME - create animation objects with this functionality
	void UpdateMovementAnimation(FLOAT DeltaSeconds);
	void ZeroMovementAlpha(INT iStart, INT iEnd, FLOAT StepSize);

	// Controller interface
	UBOOL IsHumanControlled(); // FIXME: This used to be inline, but that didn't compile with static linking.
	UBOOL IsLocallyControlled(); // FIXME: This used to be inline, but that didn't compile with static linking.

	// Own Functions.
	AActor* AttachTrailer(AActor*, INT, FLOAT, INT);
	class ANProjectile* SetAtkArrow(INT);
	class ANSkillProjectile* SkillEffectShot();
	APawn* RiderEnd(FVector);
	APawn* RiderEnter(INT, INT, INT, FVector);
	FLOAT GetAtk01AnimRate();
	FLOAT GetAtk02AnimRate();
	FLOAT GetAtk03AnimRate();
	FLOAT GetAtkDownAnimRate();
	FLOAT GetAtkUpAnimRate();
	FLOAT GetAttackEffFrame();
	FLOAT GetAttackEndEffFrame();
	FLOAT GetAttackItemSoundFrame();
	FLOAT GetAttackRange();
	FLOAT GetCastLongAnimRate();
	FLOAT GetCastMidAnimRate();
	FLOAT GetCastShortAnimRate();
	FLOAT GetCastShortEndAnimRate();
	FLOAT GetCurAnimFrame(INT);
	FLOAT GetCurAnimRate(INT);
	FLOAT GetCurMoveAnimRate();
	FLOAT GetCurSpeed(INT);
	FLOAT GetMagicFriendAnimRate();
	FLOAT GetMagicNoTargetAnimRate();
	FLOAT GetMagicShotAnimRate();
	FLOAT GetMagicThrowAnimRate();
	FLOAT GetRunAnimRate();
	FLOAT GetShieldAtkAnimRate();
	FLOAT GetSitAnimRate();
	FLOAT GetSocialAnimDuration(INT);
	FLOAT GetSpAtk01AnimRate();
	FLOAT GetSpAtk02AnimRate();
	FLOAT GetSpAtk03AnimRate();
	FLOAT GetSpAtk04AnimRate();
	FLOAT GetStandAnimRate();
	FLOAT GetThrowAnimRate();
	FLOAT GetWalkAnimRate();
	FNPawnLight* AddPawnLight(FVector, FRotator, INT);
	FName GetAtk01AnimName();
	FName GetAtk02AnimName();
	FName GetAtk03AnimName();
	FName GetAtkDownAnimName();
	FName GetAtkUpAnimName();
	FName GetAtkWaitAnimName();
	FName GetBJumpAnimName();
	FName GetCastEndAnimName();
	FName GetCastLongAnimName();
	FName GetCastMidAnimName();
	FName GetCastShortAnimName();
	FName GetChairSitAnimName();
	FName GetChairStandAnimName();
	FName GetChairWaitAnimName();
	FName GetCurAnimName(INT);
	FName GetCurAtkAnimName();
	FName GetCurMoveAnimName();
	FName GetCurTurnAnimName();
	FName GetCurWaitAnimName();
	FName GetDamageAnimName();
	FName GetDamageFlyAnimName();
	FName GetDeathAnimName();
	FName GetDeathStandAnimName();
	FName GetDeathWaitAnimName();
	FName GetDefenceAnimName();
	FName GetDodgeAnimName();
	FName GetEnchantedWeaponEffect(FVector&, FLOAT&, FLOAT&, INT);
	FName GetFallAnimName();
	FName GetFishingControlAnimName();
	FName GetFishingEndAnimName();
	FName GetFishingPullAnimName();
	FName GetFishingStartAnimName();
	FName GetFishingWaitAnimName();
	FName GetJumpAnimName();
	FName GetLFootBoneName();
	FName GetMagicFriendAnimName();
	FName GetMagicNoTargetAnimName();
	FName GetMagicShotAnimName();
	FName GetMagicThrowAnimName();
	FName GetPicItemAnimName();
	FName GetRFootBoneName();
	FName GetRandomAtkAnimName();
	FName GetRiderAtkAnimName();
	FName GetRiderDeathAnimName();
	FName GetRiderDeathWaitAnimName();
	FName GetRiderRunAnimName();
	FName GetRiderWaitAnimName();
	FName GetRunAnimName();
	FName GetShieldAtkAnimName();
	FName GetSitAnimName();
	FName GetSitWaitAnimName();
	FName GetSocialAnimName(INT);
	FName GetSpAtk01AnimName();
	FName GetSpAtk02AnimName();
	FName GetSpAtk03AnimName();
	FName GetSpAtk04AnimName();
	FName GetSpAtk05AnimName();
	FName GetSpAtk06AnimName();
	FName GetSpAtk07AnimName();
	FName GetSpAtk08AnimName();
	FName GetSpAtk09AnimName();
	FName GetSpAtk10AnimName();
	FName GetSpAtk11AnimName();
	FName GetSpAtk12AnimName();
	FName GetSpAtk13AnimName();
	FName GetSpAtk14AnimName();
	FName GetSpAtk15AnimName();
	FName GetSpAtk16AnimName();
	FName GetSpAtk17AnimName();
	FName GetSpAtk18AnimName();
	FName GetSpAtk19AnimName();
	FName GetSpAtk20AnimName();
	FName GetSpAtk21AnimName();
	FName GetSpAtk22AnimName();
	FName GetSpAtk23AnimName();
	FName GetSpAtk24AnimName();
	FName GetSpAtk25AnimName();
	FName GetSpAtk26AnimName();
	FName GetSpAtk27AnimName();
	FName GetSpAtk28AnimName();
	FName GetStandAnimName();
	FName GetStunAnimName();
	FName GetSwimAnimName();
	FName GetSwimAttackWaitAnimName();
	FName GetSwimDeathAnimName();
	FName GetSwimDeathWaitAnimName();
	FName GetSwimWaitAnimName();
	FName GetThrowAnimName();
	FName GetTurnAnimName();
	FName GetVariationWeaponEffect(FVector&, FLOAT&, FLOAT&, INT);
	FName GetWaitAnimName();
	FName GetWalkAnimName();
	INT Action_Attack(AActor*);
	INT Action_Attack(AActor*, INT, INT, INT, INT, INT, INT, INT);
	INT AddNCubic(INT);
	INT AdjustLandPosition();
	INT CheckAbnormalState(INT);
	INT CheckAtkConsumeItem();
	INT CheckWaistRotation();
	INT GetAtkShotNum(FName);
	INT GetAtkType(FName);
	INT InitSkillProcess();
	INT IsAttackIntend();
	INT IsBigHead(FLOAT&);
	INT IsCameraWalkingMode();
	INT IsCastingMultiShotSkill();
	INT IsEnableFaceRotation();
	INT IsVariationEffectEnabled();
	INT L2walkReachable(FVector, INT, AActor*);
	INT L2walkReachable2(FVector, INT, AActor*);
	INT SetMagicInfo(INT, INT, FLOAT, FLOAT, AActor*, FLOAT, struct FL2MagicSkillData*);
	INT SetSameNormal(enum EPawnSubMeshStyle, enum EPawnSubMeshStyle, INT);
	INT SetSkillAnim(INT, INT);
	INT SetSkillType(INT, INT);
	INT SpawnNEffect(INT, INT, FLOAT);
	INT SpawnNTransientEffect(APawn*, INT);
	INT UpdateAbnormalState(FLOAT);
	UMesh* GetEnchantedWeaponMesh(FVector&, FVector&, INT);
	UMesh* GetHeroMesh();
	UMeshInstance* GetWeaponMeshInstance(INT);
	USound* GetBootsSound();
	FMeshAnimSeq* FindSeq(FName);
	void AddAppendixEffect(enum AppendixVisualEffectBitType);
	void AddAssociatedActorNotify(INT, INT, AActor*, INT);
	void AddMonsterRaceMgr(INT, INT, INT*, FVector, FVector, INT);
	void AttackStop();
	void BowAttackProcess(FLOAT);
	void CheckCubicStatus(short, short*);
	void CreateOverHeadAlarm(INT);
	void DeSpawnExitEvent();
	void DeleteAllAbnormalState();
	void DeleteAllAppendixEffect();
	void DeleteAllCursedWeaponEffect();
	void DeleteAllNCubic();
	void DeleteAllPawnLight();
	void DeleteAllWeaponEffect();
	void DeleteAppendixEffect(enum AppendixVisualEffectBitType);
	void DeleteDecoEffect();
	void DeleteNCubic(INT);
	void DeleteOverHeadAlarm();
	void DisableFaceRotation();
	void DisableSpineRotation();
	void EnableFaceRotation();
	void EnableSpineRotation();
	void FaceRotation(FRotator, FLOAT);
	void InitBoneScaler();
	void InitDecoEffect();
	void LoadAnimData(FName);
	void MagicCancel();
	void MagicProcess(FLOAT);
	void MagicStop();
	void NActionAnimEnd();
	void NActionProcess(FLOAT);
	void NActionStop();
	void NPrimeActionEnd();
	void NotifyDie();
	void PlaySkillSound(AActor*, AActor*, INT);
	void RefreshQuestMark();
	void SEtLFootBonName(FName);
	void SEtRFootBonName(FName);
	void SetAtkConsumeItem(INT, INT);
	void SetCloakSkin(INT, UMaterial*);
	void SetExMeshesNTexes(INT, FName, FName);
	void SetForcedLodModel(INT);
	void SetLeftArmSkin(INT, UMaterial*);
	void SetLeftHandSkin(UMaterial*);
	void SetLobbyWaitAnimName(TCHAR*);
	void SetRelativeCoords(AActor*);
	void SetRightArmSkin(UMaterial*);
	void SetRightHandSkin(UMaterial*);
	void SetRotationRate(FRotator);
	void SetSubSkin(INT, UMaterial*);
	void SetUseSameNormal(enum EPawnSubMeshStyle, INT, INT);
	void ShotAttackProcess(FLOAT);
	void SkillEffectChanneling();
	void SkillEffectFinalize();
	void SkillEffectInit();
	void SkillEffectPreShot();
	void SkillEffectUnionDDTarget(AActor*);
	void SpawnEnterEvent();
	void SpelledTargetDestroyNotify(AActor*);
	void SpineRotation(FLOAT);
	void StopSpellSound();
	void SwordAttackProcess(FLOAT);
	void ThrowAttackProcess(FLOAT);
	void UpdateBubbles(FLOAT, FVector const&);
	void UpdateFootRotation(FLOAT);
	void UpdateMeshSound(FName, FName, FLOAT, FLOAT);
	void UpdateWaistRotation(FLOAT);
	void UpdateWaterHitEffect(APhysicsVolume*);
	void UpdateWaterHitEffectMovement(FLOAT);
	void WaistRotation(FRotator, FLOAT, INT);

private:
	UBOOL Pick3DWallAdjust(FVector WallHitNormal);
	FLOAT Swim(FVector Delta, FCheckResult &Hit);
	FVector findWaterLine(FVector Start, FVector End);
	INT IsShowQuestMark();
	int checkFloor(FVector Dir, FCheckResult &Hit);
	void SpiderstepUp(FVector DesiredDir, FVector Delta, FCheckResult &Hit);
	void calcVelocity(FVector AccelDir, FLOAT deltaTime, FLOAT maxSpeed, FLOAT friction, INT bFluid, INT bBrake, INT bBuoyant);
	int findNewFloor(FVector OldLocation, FLOAT deltaTime, FLOAT remainingTime, INT Iterations);

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/


