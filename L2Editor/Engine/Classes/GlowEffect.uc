class GlowEffect extends CameraEffect
	native
	noexport
	editinlinenew
	collapsecategories;
	
var const int	RenderTargets[5];
var float		Luminance;			
var float		MiddleGray;
var float		WhiteCutoff;
var float		Threshold;		
var float		BloomScale;
var int			BlurNum;
var int			GlowType;
var float		RGBCutoff;		

var int			FinalBlendBlurType;		// 1 - gauss, 2 - 16box, 3 - ?
var int			FinalBlendOpacity;
var int	        AspectBlendOpacity;

defaultproperties
{
     Luminance=0.080000
     MiddleGray=0.180000
     WhiteCutoff=0.800000
     Threshold=0.350000
     BloomScale=1.500000
     BlurNum=2
     GlowType=1
     RGBCutoff=0.800000
     FinalBlendBlurType=1
     FinalBlendOpacity=85
     AspectBlendOpacity=192
}
