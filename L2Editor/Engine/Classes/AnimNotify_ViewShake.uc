//#ifdef __L2 //kurt
class AnimNotify_ViewShake extends AnimNotify
	native;

var () enum EViewShakeType
{
	VST_DAMAGE,
	VST_VIBRATION,	
	VST_USER,
	VST_UP,
	VST_DOWN,
	VST_UPDOWN,
	VST_DOWNUP,
	
} ShakeType;

var() float  ShakeIntensity;
var() vector ShakeVector;
var() float  ShakeRange;
var() int    ShakeCount;

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
