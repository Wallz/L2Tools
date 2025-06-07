//#ifdef __L2 // idearain

class AnimNotify_BoneScale extends AnimNotify
	native;

enum L2PartialBoneScalerType
{
	LPBST_LINEAR,
	LPBST_SIN_SOUT,
	LPBST_SIN_FOUT,
	LPBST_FIN_SOUT,
};

var()	int		BoneIndex;
var()	float	StartScale;
var()	float	EndScale;
var()	float	StartFrameIndex;
var()	float	EndFrameIndex;
var()	bool	ApplySubBones;
var()	L2PartialBoneScalerType	BoneScalerType;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)


//#endif

cpptext
{
	// AnimNotify interface.
	virtual void Notify( UMeshInstance *Instance, AActor *Owner );
}


defaultproperties
{
}
