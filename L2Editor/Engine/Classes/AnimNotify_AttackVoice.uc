//#ifdef __L2 //kurt
class AnimNotify_AttackVoice extends AnimNotify
	native;

var() float Volume;
var() float Radius;
var() int Random;

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
