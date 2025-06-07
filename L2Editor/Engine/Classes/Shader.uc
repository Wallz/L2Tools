class Shader extends RenderedMaterial
	editinlinenew
	native;

var() editinlineuse Material Diffuse;
var() editinlineuse Material Opacity;

var() editinlineuse Material Specular;
var() editinlineuse Material SpecularityMask;

var() editinlineuse Material SelfIllumination;
var() editinlineuse Material SelfIlluminationMask;

var() editinlineuse Material Detail;
var() float DetailScale;

var() enum EOutputBlending
{
	OB_Normal,
	OB_Masked,
	OB_Modulate,
	OB_Translucent,
	OB_Invisible,
	OB_Brighten,
	OB_Darken
} OutputBlending;

var() bool TwoSided;
var() bool Wireframe;
var   bool ModulateStaticLighting2X;
var() bool PerformLightingOnSpecularPass;
//#ifdef __L2 zodiac
var() bool TreatAsTwoSided;
var() bool ZWrite;
var() bool AlphaTest;
var() byte AlphaRef;
//#endif
var() editinlineuse Material NormalMap;
var() float BumpOffsetScaleFactor;
var() float BumpOffsetBiasFactor;

function Reset()
{
	if(Diffuse != None)
		Diffuse.Reset();
	if(Opacity != None)
		Opacity.Reset();
	if(Specular != None)
		Specular.Reset();
	if(SpecularityMask != None)
		SpecularityMask.Reset();
	if(SelfIllumination != None)
		SelfIllumination.Reset();
	if(SelfIlluminationMask != None)
		SelfIlluminationMask.Reset();
	if(FallbackMaterial != None)
		FallbackMaterial.Reset();
	if(NormalMap != None)
		NormalMap.Reset();
}

function Trigger( Actor Other, Actor EventInstigator )
{
	if(Diffuse != None)
		Diffuse.Trigger(Other,EventInstigator);
	if(Opacity != None)
		Opacity.Trigger(Other,EventInstigator);
	if(Specular != None)
		Specular.Trigger(Other,EventInstigator);
	if(SpecularityMask != None)
		SpecularityMask.Trigger(Other,EventInstigator);
	if(SelfIllumination != None)
		SelfIllumination.Trigger(Other,EventInstigator);
	if(SelfIlluminationMask != None)
		SelfIlluminationMask.Trigger(Other,EventInstigator);
	if(FallbackMaterial != None)
		FallbackMaterial.Trigger(Other,EventInstigator);
	if(NormalMap !=None)
		NormalMap.Trigger(Other,EventInstigator);
}

defaultproperties
{
     DetailScale=8.000000
}
