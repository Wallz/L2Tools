	virtual void Destroy();
	virtual void PostScriptDestroyed();

	virtual INT Tick(FLOAT, enum ELevelTick);
	virtual struct FNMagicInfo* GetMagicInfo();
	virtual void processHitWall(FVector, AActor*);

	// Own Functions.
	virtual void TargetDestroyNotify(AActor*);
	virtual void PreDestroy();

	FLOAT PrepareInterpolation(INT, FVector);
	void SetTargetActor(AActor*);
