/*=============================================================================

	UnMesh.h: Unreal Engine mesh instance / abstract base object.
	Copyright 2001-2002 Epic Games, Inc. All Rights Reserved.

    Pseudo-abstract mesh base.

=============================================================================*/

#ifndef _INC_UNMESH
#define _INC_UNMESH

// Forward declaration
class UMesh;
class UMeshInstance;

// Flags
enum MInstanceFlags
{
	MINST_InUse        = 0x00000001,
	MINST_DeleteMe     = 0x00000002
};

enum GetFrameFlags
{
	GF_FullSkin  = 0,
	GF_RawVerts  = 1,
	GF_BonesOnly = 2,
	GF_RootOnly  = 3,
};


/*-----------------------------------------------------------------------------
	UMesh - pseudo-abstract mesh class.
-----------------------------------------------------------------------------*/
class ENGINE_API UMesh : public UPrimitive
{
	DECLARE_CLASS(UMesh, UPrimitive, 0, Engine)

protected:
    UMeshInstance* DefMeshInstance; // Default instance, used with null actor GetInstance calls.

public:
    // constructor
	UMesh();
	
    // Retrieve instance class associated with this mesh class.
	virtual UClass* MeshGetInstanceClass();
	virtual UClass* SubMeshGetInstanceClass();
	virtual UClass* ExtraMeshGetInstanceClass();
	
    // Get a mesh instance for a particular actor (default implementation exists but may be overridden)
    virtual UMeshInstance* MeshGetInstance(const AActor* InActor);
	virtual UMeshInstance* SubMeshGetInstance(AActor*, INT);
	virtual UMeshInstance* ExtraMeshGetInstance(AActor*, INT, INT, UMaterial*);
	virtual UMeshInstance* RightHandMeshGetInstance(AActor*);
	virtual UMeshInstance* LeftHandMeshGetInstance(AActor*);
	virtual UMeshInstance* RightArmMeshGetInstance(AActor*);
	virtual UMeshInstance* LeftArmMeshGetInstance(AActor*);
	virtual UMeshInstance* CloakMeshGetInstance(AActor*);
	virtual UMeshInstance* CloakCoverMeshGetInstance(AActor*);

	// Serialization ( garbage collection! )
	void Serialize( FArchive& Ar );		
};

/*----------------------------------------------------------------------------
  UMeshInstance - pseudo-abstract mesh instance class.
----------------------------------------------------------------------------*/
class ENGINE_API UMeshInstance : public UPrimitive
{
	DECLARE_CLASS( UMeshInstance, UPrimitive, 0, Engine)
	
	// Get or assign the owner for this meshinstance.
	virtual AActor* GetActor();
	virtual void SetActor(AActor* InActor);

	// Get/set the mesh associated with the mesh instance.
	virtual UMesh* GetMesh();
	virtual void SetMesh(UMesh* InMesh);
	virtual void SetSubMeshIndex(INT);

	// Status queries
	virtual void SetStatus(INT Flags);
	virtual INT GetStatus();

	// Animation methods

	// UpdateAnimation moves task of animation advancement to the actor's mesh instance.
	virtual UBOOL UpdateAnimation(FLOAT DeltaSeconds);
	// PlayAnim moves task of initializing animation to the actor's mesh instance.
	virtual UBOOL PlayAnim(INT Channel, FName SequenceName, FLOAT InRate, FLOAT InTweenTime, UBOOL InLooping);

	// Return number of animations supported by the mesh instance.
	virtual INT GetAnimCount();
	// Return animation for a given index.
	virtual HMeshAnim GetAnimIndexed(INT InIndex);
	// Return animation for a given name
	virtual HMeshAnim GetAnimNamed(FName InName);
	// Get the name of a given animation
	virtual FName AnimGetName(HMeshAnim InAnim);
	// Get the group of a given animation
	virtual FName AnimGetGroup(HMeshAnim InAnim);
	// Find if animation has certain group tag.
	virtual UBOOL AnimIsInGroup(HMeshAnim InAnim, FName Group);
	// Get the number of frames in an animation
	virtual FLOAT AnimGetFrameCount(HMeshAnim InAnim);
	// Get the play rate of the animation in frames per second
	virtual FLOAT AnimGetRate(HMeshAnim InAnim);
	// Get the number of notifications associated with this animation.
	virtual INT AnimGetNotifyCount(HMeshAnim InAnim);
	// Get the time of a particular notification.
	virtual FLOAT AnimGetNotifyTime(HMeshAnim InAnim, INT InNotifyIndex);
	// Get text associated with a given notify.
	virtual const TCHAR* AnimGetNotifyText(HMeshAnim InAnim, INT InNotifyIndex);
	// Get function associated with given notify.
	virtual UAnimNotify* AnimGetNotifyObject(HMeshAnim InAnim, INT InNotifyIndex);
	// change to void* => AnimGetNotifyObject() ? #debug
	virtual UBOOL IsAnimating(INT Channel = 0);
	// Stop all animation.
	virtual UBOOL StopAnimating();
	virtual UBOOL FreezeAnimAt(FLOAT Time, INT Channel);

	virtual UBOOL IsAnimTweening(INT Channel = 0);
	virtual UBOOL IsAnimPastLastFrame(INT Channel = 0);
	virtual UBOOL AnimStopLooping(INT Channel = 0);

	virtual FName GetActiveAnimSequence(INT Channel = 0);
	virtual FLOAT GetActiveAnimRate(INT Channel = 0);
	virtual FLOAT GetActiveAnimFrame(INT Channel = 0);

	// PostNetReceive animation state network reconstruction.
	virtual void SetAnimFrame(INT Channel, FLOAT NewFrame, INT UnitFlag = 0 ) {}
	virtual UBOOL AnimForcePose(FName SeqName, FLOAT AnimFrame, FLOAT Delta, INT Channel = 0);

	// UPrimitive interface. Default implementations point back to source mesh.
	virtual FBox GetCollisionBoundingBox(const AActor* Owner);

	virtual UMaterial* GetMaterial(INT Count, AActor* Owner);
	virtual UMaterial* GetAttachedWeaponTexture(INT, INT, AActor* Owner);
	virtual UMaterial* GetSubTexture(INT, INT, AActor* Owner);

	virtual void GetFrame(AActor* Owner, FLevelSceneNode* SceneNode, FVector* ResultVerts, INT Size, INT& LODRequest, DWORD TaskFlag);
	virtual void PoseFrame(INT, FLOAT);

	// Render support.
	virtual FMatrix MeshToWorld();
	virtual void Render(FDynamicActor* Owner,FLevelSceneNode* SceneNode,TList<FDynamicLight*>* Lights,TList<FProjectorRenderInfo*>* Projectors, FRenderInterface* RI);

	// Mesh creation methods. Usually point back to the mesh.
	// Set drawing scale.
	virtual void SetScale( FVector NewScale );
	// Bounds generation. 
	virtual void MeshBuildBounds();
	
	virtual INT ActiveVertStreamSize();
	virtual void MeshSkinVertsCallback(void*);
};


/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/
#endif

