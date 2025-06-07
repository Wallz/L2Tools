//#ifdef __L2 // by nonblock
class AnimNotify_ScreenFade extends AnimNotify
	native;

var() float	FadeOutDuration;
var() color FadeOutColor;
var() float BlackOutDuration;
var() float FadeInDuration;

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
     FadeOutDuration=3000.000000
     FadeOutColor=(B=255,G=255,R=255,A=255)
     FadeInDuration=1000.000000
}
