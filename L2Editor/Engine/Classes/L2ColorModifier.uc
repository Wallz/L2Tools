class L2ColorModifier extends Modifier
	noteditinlinenew
	native;

enum EL2TextureOp
{
	P_DISABLE,
    P_SELECTARG1,
    P_SELECTARG2,
    P_MODULATE,
    P_MODULATE2X,
    P_MODULATE4X,
    P_ADD,
    P_ADDSIGNED,
    P_ADDSIGNED2X,
    P_SUBTRACT,
    P_ADDSMOOTH,
    P_BLENDDIFFUSEALPHA,
    P_BLENDTEXTUREALPHA,
    P_BLENDFACTORALPHA,
    P_BLENDTEXTUREALPHAPM,
    P_BLENDCURRENTALPHA,
    P_PREMODULATE,
    P_MODULATEALPHA_ADDCOLOR,
    P_MODULATECOLOR_ADDALPHA,
    P_MODULATEINVALPHA_ADDCOLOR,
    P_MODULATEINVCOLOR_ADDALPHA,
    P_BUMPENVMAP,
    P_BUMPENVMAPLUMINANCE,
    P_DOTPRODUCT3,
    P_MULTIPLYADD,
};

var() color Color;
var() bool	RenderTwoSided;
var() bool	AlphaBlend;
var() EL2TextureOp AlphaOp;
var() EL2TextureOp ColorOp;

defaultproperties
{
     Color=(B=255,G=255,R=255,A=255)
     RenderTwoSided=True
     AlphaBlend=True
}
