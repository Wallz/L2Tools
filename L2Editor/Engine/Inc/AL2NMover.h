	virtual void PostScriptDestroyed();
	// Own Functions.	
	virtual void MoveStart(FLOAT, INT);
	virtual void MoveStop();
	virtual void MovePlay();
	virtual void MovePause();
	virtual INT IsStarted();
	virtual INT IsEnded();
	virtual INT IsEnded(AActor**);
	virtual INT IsPaused();
	virtual INT MoveTick(FLOAT);
	virtual void AddMoveTarget(FVector);
	virtual void AddMoveTarget(AActor*, INT);
	virtual void ChangeTargetToLoc(AActor*);
	virtual void DeleteTarget(FVector);
	virtual void DeleteTarget(AActor*);
	