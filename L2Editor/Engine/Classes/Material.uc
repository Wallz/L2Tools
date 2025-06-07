//=============================================================================
// Material: Abstract material class
// This is a built-in Unreal class and it shouldn't be modified.
//=============================================================================
class Material extends Object
	native
	hidecategories(Object)
	collapsecategories
	noexport;

#exec Texture Import File=Textures\DefaultTexture.pcx 

var() Material FallbackMaterial;

var Material DefaultMaterial;
var const transient bool UseFallback;	// Render device should use the fallback.
var const transient bool Validated;		// Material has been validated as renderable.

//#ifdef __L2//kurt
var int Reserved;	
//#endif

//#ifdef __L2//yohan
//var int ShaderPropertyID;
//var int ShaderCodeID;
//#endif
var byte TextureTranform;	
var byte MAX_SAMPLER_NUM;
var byte MAX_TEXMAT_NUM;
var byte MAX_PASS_NUM;
var byte TwoPassRenderState;
var byte AlphaRef;		
var	int	SrcBlend;
var	int	DestBlend;
var	int	OverriddenFogColor;
//var	matrix matTexMatrix[8];
var	matrix matTexMatrix[16];
var	int FC_Color1;
var int FC_Color2;
var int FC_FadePeriod;
var int FC_FadePhase;
var int FC_ColorFadeType;
var int BitmapMaterials[16];
var string strTex[16];

var string ShaderCode;

function Reset()
{
	if( FallbackMaterial != None )
		FallbackMaterial.Reset();
}

function Trigger( Actor Other, Actor EventInstigator )
{
	if( FallbackMaterial != None )
		FallbackMaterial.Trigger( Other, EventInstigator );
}

defaultproperties
{
     DefaultMaterial=Texture'Engine.DefaultTexture'
}
