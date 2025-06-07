	virtual INT Tick(FLOAT, enum ELevelTick);
	virtual void PostScriptDestroyed();

	virtual void StopMove();
	virtual INT HitWallNotify(FVector, AActor*);
	virtual void ResetMusicControll();
	virtual INT IsBroadcastObserverMode();
	virtual INT IsObserverMode();
	virtual void SetBroadcastObserverMode(INT);
	virtual void SetVehicleStatus(INT);
	virtual void PlayerCalcView(AActor*, FVector*, FRotator*);
	virtual void SetRequestedServerMusic(TCHAR*, FLOAT);
	virtual void SetRequestedServerVoice(TCHAR*, FLOAT, INT);
	// Own Functions.
	virtual void CalcVolumeCamera(FVector*, FRotator*);
	virtual void CalcBehindView(FVector*, FRotator*);
	virtual void SetViewTarget(AActor*);

	FNViewShake* AddViewShakeState(INT, FLOAT, FLOAT, FLOAT, FLOAT, FVector, FVector, FVector, FVector, FLOAT, FLOAT, AActor*);
	FVector RotationToLocationWithDist(FRotator, FVector, FLOAT);
	FVector ViewToLocationWithDist(FVector, FVector, FLOAT);
	INT IntToShort(INT);
	INT IsBlockRotation(FRotator, FVector, FLOAT, INT);

	void DisableCameraManuallyRotationg(INT);
	void InvalidateZoneName();
	void ReleaseSpecialViewTarget();
	void ResetJoypadMoving(FLOAT, FLOAT);
	void ResetKeyboardMoving(INT, INT, INT);
	void SetSpecialViewTarget(AActor*, FLOAT, FRotator, FLOAT, FLOAT, FRotator, INT, INT, INT);
	void ShakeView(FVector*, FRotator*);
	void SpecialViewTick(FLOAT);
	void UpdateShakeState(FLOAT);