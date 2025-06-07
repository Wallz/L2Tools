class HDREffect extends CameraEffect
	native
	noexport
	editinlinenew
	collapsecategories;

var const int	RenderTargets[7];
var float HDRTimeStamp;
// var	int	TrigAccum;

var int	idxCurLum;
//var float RGBBias;

var float GrayLum;
var float FinalCoef;

var float ExpBase;
var float ExpCoef;

var float ClampMin;
var float ClampMax;

defaultproperties
{
     idxCurLum=5
     GrayLum=0.600000
     FinalCoef=3.500000
     ExpBase=0.980000
     ExpCoef=30.000000
     ClampMin=0.300000
     ClampMax=0.750000
}
