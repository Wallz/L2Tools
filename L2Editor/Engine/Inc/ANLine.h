
class ENGINE_API ANLine : public AActor
{
	virtual INT Tick(FLOAT, enum ELevelTick);
	virtual void Destroy();

	FBox GetBoundingBox();
	INT SetEndPositions(APawn*, INT);

	void Render(FDynamicActor*, FLevelSceneNode*, TList<FDynamicLight*>*, FRenderInterface*);

	DECLARE_CLASS(ANLine, AActor, 0, Engine)
	NO_DEFAULT_CONSTRUCTOR(ANLine)
};