/*=============================================================================
	AEmitter.h.
	Copyright 2001-2002 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

	// UObject/ AActor Functions.
	virtual UBOOL Tick( FLOAT DeltaTime, enum ELevelTick TickType );
	virtual void Spawned();
	virtual void Destroy();
	virtual void ClearL2Game();
	virtual void PostScriptDestroyed();
	virtual void L2EventShow(INT);
	virtual void NotifyAnimEnd(INT);
	virtual INT IsAEmitter();
	virtual AEmitter* GetEmitter();
	virtual FVector GetTrailerPrePivot();
	virtual void RenderEditorInfo(FLevelSceneNode* SceneNode, FRenderInterface* RI, FDynamicActor* FDA);
	
	// AEmitter Functions.
	virtual void  Initialize();
	virtual INT   CheckForProjectors();
	virtual void  Kill();
	virtual void  PreDestroyEvent();

	FLOAT GetParticleMaxLifeTimeRange();
	INT CheckFirstSpawnParticle();
	INT SpawnEmitterLight();
	INT SpawnEmitterQuake();
	void AdjustparticleLife(FLOAT);
	void EmitterRotation(FLOAT);
	void SetDelayed(FLOAT, INT, INT);
	void SetDisabled(INT, INT, INT);
	void SetMaxParticles(FLOAT);
	void SetOpacity(FLOAT);
	void SetOpacityRatio(FLOAT);
	void SetParticleLifeTimeRange(FLOAT);
	void SetParticleMaxParticles(FLOAT);
	void SetSizeScale(FLOAT);
	void SetSpeedRate(FLOAT);
	void SetSpeedScale(FLOAT);

	void Render(FDynamicActor* Actor,class FLevelSceneNode* SceneNode,TList<class FDynamicLight*>* Lights,FRenderInterface* RI);

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

