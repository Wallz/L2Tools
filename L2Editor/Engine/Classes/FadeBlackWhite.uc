class FadeBlackWhite extends CameraEffect
	native
	noexport
	editinlinenew
	collapsecategories;

var() float FadeTime;
var() float FadeDir;

// Used internally
var const float LastAlpha;
var const float	LastFrameTime;

defaultproperties
{
     FadeTime=2.000000
     FadeDir=1.000000
}
