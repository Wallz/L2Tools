/*=============================================================================
	AActor.h.
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

// !!! FIXME: a big round of applause for quailty GNU software!  --ryan.
#ifndef BUGGYINLINE
#ifdef __GNUC__
#define BUGGYINLINE
#else
#define BUGGYINLINE inline
#endif
#endif

	// Constructors.
	AActor() {}
	AActor& operator=(AActor const&);
	virtual void Destroy();

	// UObject interface.
	virtual INT* GetOptimizedRepList( BYTE* InDefault, FPropertyRetirement* Retire, INT* Ptr, UPackageMap* Map, UActorChannel* Channel );
	virtual APawn* GetPlayerPawn() const;
	virtual UBOOL IsPlayer();
	virtual APawn* GetPawn();
	virtual class ALight* GetLight();
	virtual class AEmitter* GetEmitter();
	virtual FLOAT GetNetPriority(AActor* Sent, FLOAT Time, FLOAT Lag);
	virtual FLOAT WorldLightRadius() const;
	virtual UBOOL Tick(FLOAT DeltaTime, enum ELevelTick TickType);
	virtual void PostEditMove() {}
	virtual void PostEditLoad() {}
	virtual void PreRaytrace() {}
	virtual void PostRaytrace() {}
	virtual void Spawned() {}
	virtual void PreNetReceive();
	virtual void PostNetReceive();
	virtual void PostNetReceiveLocation();
	virtual UMaterial* GetSkin(INT Index);
	virtual UBOOL ShouldTickInEntry() { return false; }
	virtual void L2EventShow(INT);
	virtual void L2EventPlay(INT);
	virtual void LostChild(AActor *);
	virtual void GainedChild(AActor *);
	virtual UBOOL CanBeAttacked(void);
	virtual UBOOL CanBeTold(void);
	virtual void PostRender(void);
	virtual void SetMeshes(INT, FName, INT);
	virtual void SetTexes(INT, FName, INT);
	virtual UMaterial* GetSubSkin(INT);
	virtual UMaterial* GetRightHandSkin(INT);
	virtual UMaterial* GetLeftHandSkin(INT);
	virtual UMaterial* GetRightArmSkin(INT);
	virtual UMaterial* GetLeftArmSkin(INT);
	virtual UMaterial* GetCloakSkin(INT);
	virtual void CheckSameAnim();
	virtual UBOOL IsSameAnim();
	virtual void CreateSkin(FRenderInterface*);
	virtual void ClearTexModifier();
	virtual void SetAlphaTexModifier(BYTE);
	virtual void ClearAlphaTexModifier();
	virtual void SetOverlayTexModifier(BYTE, BYTE, BYTE, BYTE);
	virtual void SetOverlayDependAlphaTexModifier(BYTE, BYTE, BYTE);
	virtual void SetSubtractTexModifier(BYTE, BYTE, BYTE);
	virtual void SetAddTexModifier(FColor);
	virtual void SetAddTexModifier(BYTE, BYTE, BYTE);
	virtual void SetChangeColorTexModifier(BYTE, BYTE, BYTE);
	virtual UBOOL IsModifiedTexture();
	virtual UModifier* GetModifiedTextureMaterial();
	virtual void SetActorViewType(EActorViewType, INT, BYTE);
	virtual UBOOL IsNeedTick();
	virtual void SetStaticMeshActorViewType(INT, INT);
	virtual USound* GetStepSoundData();
	virtual FMatrix ConvertOrthMatrix(FMatrix, int);
	virtual FMatrix CollisionToWorld();
	virtual FCoords ToLocal() const;
	virtual FCoords ToWorld() const;
	virtual FMatrix LocalToWorld() const;
	virtual FMatrix BillBoardLocalToWorld(FRotator) const;
	virtual FMatrix WorldToLocal() const;
	virtual FDynamicLight* GetLightRenderData();
	virtual void UpdateRenderData();
	virtual void PostScriptDestroyed() {} // C++ notification that the script Destroyed() function has been called.
	virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);
	virtual UPrimitive* GetPrimitive();
	virtual void NotifyBump(AActor *Other);
	virtual void SetBase(AActor *NewBase, FVector NewFloor = FVector(0, 0, 1), int bNotifyActor = 1);
	virtual void NotifyAnimEnd(int Channel);
	virtual void UpdateAnimation(FLOAT DeltaSeconds);
	virtual void StartAnimPoll();
	virtual UBOOL CheckAnimFinished(int Channel);
	virtual UBOOL CheckOwnerUpdated();
	virtual void TickAuthoritative(FLOAT DeltaSeconds);
	virtual void TickSimulated(FLOAT DeltaSeconds);
	virtual void TickSpecial(FLOAT DeltaSeconds);
	virtual UBOOL PlayerControlled();
	virtual UBOOL IsNetRelevantFor(APlayerController* RealViewer, AActor* Viewer, FVector SrcLocation);
	virtual void Attacked();
	virtual UBOOL IsDamageAct();
	virtual UBOOL IsSpineRotation();
	virtual FLOAT GetDamageDist();
	virtual FVector GetTrailerPrePivot();
	virtual void SetTrailerPrePivot(FVector);
	virtual void RenderEditorInfo(FLevelSceneNode* SceneNode, FRenderInterface* RI, FDynamicActor* FDA);
	virtual void RenderEditorSelected(FLevelSceneNode* SceneNode, FRenderInterface* RI, FDynamicActor* FDA);
	virtual FLOAT GetAmbientVolume(FLOAT Attenuation);
	virtual void SetZone(UBOOL bTest, UBOOL bForceRefresh);
	virtual void SetVolumes();
	virtual void PostBeginPlay();
	virtual void setPhysics(BYTE NewPhysics, AActor *NewFloor = NULL, FVector NewFloorV = FVector(0, 0, 1));
	virtual void performPhysics(FLOAT DeltaSeconds);
	virtual void BoundProjectileVelocity();
	virtual void processHitWall(FVector HitNormal, AActor *HitActor);
	virtual void processLanded(FVector HitNormal, AActor *HitActor, FLOAT remainingTime, INT Iterations);
	virtual void physFalling(FLOAT deltaTime, INT Iterations);
	virtual FRotator FindSlopeRotation(FVector FloorNormal, FRotator NewRotation);
	virtual void SmoothHitWall(FVector HitNormal, AActor *HitActor);
	virtual void stepUp(FVector GravDir, FVector DesiredDir, FVector Delta, FCheckResult &Hit);
	virtual UBOOL ShrinkCollision(AActor *HitActor);
	virtual void L2HitWall(FVector, AActor *);
	virtual void L2Touch(AActor *);
	virtual void L2Explode(FVector, AActor *);
	virtual USound* GetAttackItemSound(FLOAT&, FLOAT&);
	virtual USound* GetDefenseItemSound(FLOAT&, FLOAT&);
	virtual USound* GetShieldItemSound(FLOAT&, FLOAT&);
	virtual USound* GetDamageSound(FLOAT&, FLOAT&);
	virtual USound* GetAttackVoiceSound(FLOAT);
	virtual void AttackedNotify(APawn*, AActor*, int, int, int);
	virtual void AssociateAttackedNotify(APawn*, AActor*, int, int, int, int, int, int, int, int, int);
	virtual UBOOL IsRendered();
	virtual FName GetRHandBoneName();
	virtual FName GetLHandBoneName();
	virtual FName GetRArmBoneName();
	virtual FName GetLArmBoneName();
	virtual FName GetCapeBoneName();
	virtual FName GetSpineBoneName();
	virtual FName GetLowbodyBoneName(void);
	virtual FName GetHeadBoneName(void);
	virtual void SetRHandBoneName(FName);
	virtual void SetLHandBoneName(FName);
	virtual void SetRArmBoneName(FName);
	virtual void SetLArmBoneName(FName);
	virtual void SetCapeBoneName(FName);
	virtual void SetSpineBoneName(FName);
	virtual void SetLowbodyBoneName(FName);
	virtual void SetHeadBoneName(FName);
	virtual UMesh* GetSubMesh(INT);
	virtual UMesh* GetRightHandMesh();
	virtual UMesh* GetLeftHandMesh();
	virtual UMesh* GetRightArmMesh();
	virtual UMesh* GetLeftArmMesh();
	virtual UMesh* GetCloakMesh();
	virtual UMesh* GetCloakCoverMesh();
	virtual void SetSubMesh(INT, UMesh *);
	virtual void SetRightHandMesh(UMesh *);
	virtual void SetLeftHandMesh(UMesh *);
	virtual void SetRightArmMesh(UMesh *);
	virtual void SetLeftArmMesh(UMesh *);
	virtual void SetCloakMesh(UMesh *);
	virtual void SetCloakCoverMesh(UMesh *);
	virtual UMeshInstance* GetSubMeshInstance(INT);
	virtual UMeshInstance*GetRightHandMeshInstance();
	virtual UMeshInstance*GetLeftHandMeshInstance();
	virtual UMeshInstance*GetRightArmMeshInstance();
	virtual UMeshInstance*GetLeftArmMeshInstance();
	virtual UMeshInstance*GetCloakMeshInstance();
	virtual UMeshInstance*GetCloakCoverMeshInstance();
	virtual FNMagicInfo* GetMagicInfo();
	virtual void GetTargetLocation(FVector, FVector &);
	virtual void physL2Movement(float, int);
	virtual INT AddMyMarker(AActor *S) { return 0; };
	virtual void ClearMarker() {};
	virtual AActor* AssociatedLevelGeometry();
	virtual UBOOL HasAssociatedLevelGeometry(AActor *Other);
	virtual void PlayAnim(INT Channel, FName SequenceName, FLOAT PlayAnimRate, FLOAT TweenTime, INT Loop);
	virtual void CheckForErrors();
	virtual void PrePath();
	virtual void PostPath();
	virtual AActor* GetProjectorBase();
	virtual UBOOL IsABrush();
	virtual UBOOL IsAMover();
	virtual UBOOL IsAVolume();
	virtual UBOOL IsAPlayerController();
	virtual UBOOL IsAPawn();
	virtual UBOOL IsAProjectile();
	virtual UBOOL IsAAmbientSound(void);
	virtual UBOOL IsAEmitter(void);
	virtual UBOOL IsALight(void);
	virtual UBOOL IsObserverModeActor(void);
	virtual UBOOL IsBroadcastObserverModeActor(void);
	virtual FName GetL2MovementEventName(void);
	virtual FName GetL2MovementTagName(int);
	virtual void AttachL2MovementActor(AActor*);
	virtual INT PostLoadProcess(void);
	virtual APlayerController* GetTopPlayerController();

	void ProcessEvent( UFunction* Function, void* Parms, void* Result=NULL );
	void ProcessState( FLOAT DeltaSeconds );
	UBOOL ProcessRemoteFunction( UFunction* Function, void* Parms, FFrame* Stack );
	void ProcessDemoRecFunction( UFunction* Function, void* Parms, FFrame* Stack );
	void Serialize( FArchive& Ar );
	void InitExecution();
	void PostEditChange();
	void PostLoad();
	void NetDirty(UProperty* property); 

	// AActor interface.
	class ULevel* GetLevel() const;
	
	UBOOL IsOwnedBy( const AActor *TestOwner ) const;
	inline UBOOL IsBlockedBy( const AActor* Other ) const;
	UBOOL IsInZone( const AZoneInfo* Other ) const;
	UBOOL IsBasedOn( const AActor *Other ) const;
	inline UBOOL IsJoinedTo( const AActor *Other) const;
	
	// Editor specific
	UBOOL IsHiddenEd();
	
	FDynamicActor* GetActorRenderData();
	void ClearRenderData();

	FLOAT LifeFraction();
	FVector GetCylinderExtent() const;

	AActor* GetTopOwner();
	UBOOL IsPendingKill();
	
	// AActor collision functions.
	UBOOL IsOverlapping( AActor *Other, FCheckResult* Hit=NULL );

	UBOOL IsInOctree();

	// AActor general functions.
	void BeginTouch(AActor *Other);
	void EndTouch(AActor *Other, UBOOL NoNotifySelf);
	void SetOwner( AActor *Owner );
	UBOOL IsBrush() const;
	UBOOL IsStaticBrush() const;
	UBOOL IsMovingBrush() const;
	UBOOL IsVolumeBrush() const;
	UBOOL IsEncroacher() const;
	UBOOL IsAnimating(int Channel=0) const;
	
	void SetCollision( UBOOL NewCollideActors, UBOOL NewBlockActors, UBOOL NewBlockPlayers);
	void SetCollisionSize( FLOAT NewRadius, FLOAT NewHeight );
	void SetDrawScale( FLOAT NewScale);
	void SetDrawScale3D( FVector NewScale3D);
	void SetStaticMesh( UStaticMesh* NewStaticMesh );
	void SetDrawType( EDrawType NewDrawType );
	
	FRotator GetViewRotation();
	
	void ReplicateAnim(INT channel, FName SequenceName, FLOAT Rate, FLOAT Frame, FLOAT TweenR, FLOAT Last, UBOOL bLoop);
	void PlayReplicatedAnim();
	
	void UpdateTimers(FLOAT DeltaSeconds);

    void UpdateOverlay(FLOAT DeltaSeconds);
	UBOOL AttachToBone( AActor* Attachment, FName BoneName, int Unk );
	UBOOL DetachFromBone( AActor* Attachment );
	inline AActor* GetAmbientLightingActor() { return bUseLightingFromBase && Base ? Base->GetAmbientLightingActor() : this; }

	void FindBase();
	
	void physProjectile(FLOAT deltaTime, INT Iterations);

	void physicsRotation(FLOAT deltaTime);
	int fixedTurn(int current, int desired, int deltaRate); 
	void TwoWallAdjust(FVector &DesiredDir, FVector &Delta, FVector &HitNormal, FVector &OldHitNormal, FLOAT HitTime);
	void physTrailer(FLOAT DeltaTime);
	void physRootMotion(FLOAT DeltaTime);
	UBOOL moveSmooth(FVector Delta);
	
	void UpdateRelativeRotation();
	void GetNetBuoyancy(FLOAT &NetBuoyancy, FLOAT &NetFluidFriction);
    
#ifdef WITH_KARMA
    McdModelID getKModel() const;
    
	void physKarma(FLOAT DeltaTime);
	BUGGYINLINE void physKarma_internal(FLOAT DeltaTime);

	void preKarmaStep_skeletal(FLOAT DeltaTime);
	void postKarmaStep_skeletal();

	void preKarmaStep(FLOAT DeltaTime);
	void postKarmaStep();

    void physKarmaRagDoll(FLOAT DeltaTime);
    BUGGYINLINE void physKarmaRagDoll_internal(FLOAT DeltaTime);
	void KFreezeRagdoll();
#endif
    
	// AI functions.
	void CheckNoiseHearing(FLOAT Loudness);
	int TestCanSeeMe(class APlayerController *Viewer);
	FVector SuggestFallVelocity(FVector Dest, FVector Start, FLOAT XYSpeed, FLOAT BaseZ, FLOAT JumpZ, FLOAT MaxXYSpeed);

	// Special editor behavior
	AActor* GetHitActor();

	// Projectors
	void AttachProjector( AProjector* Projector );
	void DetachProjector( AProjector* Projector );

	// Natives.
	DECLARE_FUNCTION(execPollSleep)
	DECLARE_FUNCTION(execPollFinishAnim)
	DECLARE_FUNCTION(execPollFinishInterpolation)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

