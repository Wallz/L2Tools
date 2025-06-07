//#ifdef __L2 //kurt
class AnimNotify_AttackShot extends AnimNotify
	native;

var() int TargetIndex;

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
