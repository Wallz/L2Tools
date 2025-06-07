
virtual void PostScriptDestroyed();
virtual INT Tick(FLOAT, enum ELevelTick);
virtual struct FNMagicInfo* GetMagicInfo();

virtual void Initialize();

// Own Functions.
void NCubicRotation(FLOAT);
void NCubicSkillInit(APawn*, APawn*, INT);
FVector GetVelocity(APawn*, FLOAT);
INT GetMovementState(APawn*);
INT NCubicSkillProcess(APawn*, FLOAT);
