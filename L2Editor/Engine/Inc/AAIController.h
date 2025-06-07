	virtual INT AcceptNearbyPath(AActor *goal);
	virtual void AdjustFromWall(FVector HitNormal, AActor* HitActor);
	virtual void SetAdjustLocation(FVector NewLoc);

	DECLARE_FUNCTION(execPollWaitToSeeEnemy)
