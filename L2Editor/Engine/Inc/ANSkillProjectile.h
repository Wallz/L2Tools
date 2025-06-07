	virtual void Destroy();
	virtual void PostScriptDestroyed();

	virtual INT Tick(FLOAT, enum ELevelTick);
	virtual void processHitWall(FVector, AActor*);

	virtual void TargetDestroyNotify(AActor*);
	virtual void PreDestroy();

	void SetSkillID(INT);
	void SkillEffectExplosion(FVector, AActor*);
