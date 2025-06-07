/*=============================================================================
	AProjector.h: Class functions residing in the Projector class.
	Copyright 2000-2002 Epic Games, Inc. All Rights Reserved.
=============================================================================*/
	// Actor interface.
	virtual UPrimitive* GetPrimitive();
	virtual void PostEditChange();
	virtual void PostEditLoad();
	virtual void PostEditMove();
	virtual void Destroy();
	virtual void TickSpecial(FLOAT DeltaSeconds);
	virtual void RenderEditorSelected(FLevelSceneNode* SceneNode, FRenderInterface* RI, FDynamicActor* FDA);

	virtual UBOOL ShouldTrace(AActor *SourceActor, DWORD TraceFlags);

	// Projector interface.
	virtual void Attach();
	virtual void Detach( UBOOL Force );
	virtual void Abandon();
	virtual void CalcMatrix();
	virtual void UpdateParticleMaterial(class UParticleMaterial* ParticleMaterial, INT ProjectorIndex );
	
	void RenderWireframe(FRenderInterface* RI);
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

