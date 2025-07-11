class Projector extends Actor
	placeable
	native;

#exec Texture Import File=Textures\Proj_IconMasked.pcx Name=Proj_Icon Mips=Off MASKED=1
#exec Texture Import file=Textures\GRADIENT_Fade.tga Name=GRADIENT_Fade Mips=Off UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP DXT=3
#exec Texture Import file=Textures\GRADIENT_Clip.tga Name=GRADIENT_Clip Mips=Off UCLAMPMODE=CLAMP VCLAMPMODE=CLAMP DXT=3


// Projector blending operation.

enum EProjectorBlending
{
	PB_None,
	PB_Modulate,
	PB_AlphaBlend,
	PB_Add
};

var() EProjectorBlending	MaterialBlendingOp,		// The blending operation between the material being projected onto and ProjTexture.
							FrameBufferBlendingOp;	// The blending operation between the framebuffer and the result of the base material blend.

// Projector properties.

var() Material	ProjTexture;
var() int		FOV;
var() int		MaxTraceDistance;
var() bool		bProjectBSP;
var() bool		bProjectTerrain;
var() bool		bProjectStaticMesh;
var() bool		bProjectParticles;
var() bool		bProjectActor;
var() bool		bLevelStatic;
var() bool		bClipBSP;
var() bool		bClipStaticMesh;
var() bool		bProjectOnUnlit;
var() bool		bGradient;
var() bool		bProjectOnBackfaces;
var() bool		bProjectOnAlpha;
var() bool		bProjectOnParallelBSP;
var() name		ProjectTag;
var() bool		bDynamicAttach;


// Internal state.

var const transient plane FrustumPlanes[6];
var const transient vector FrustumVertices[8];
var const transient Box Box;
var const transient ProjectorRenderInfoPtr RenderInfo;
var Texture GradientTexture;
var transient Matrix GradientMatrix;
var transient Matrix Matrix;
var transient Vector OldLocation;
//#ifdef __L2 // zodiac
var float	ProjectorRadius;
//#endif

// Native interface.

native function AttachProjector();
native function DetachProjector(optional bool Force);
native function AbandonProjector(optional float Lifetime);

native function AttachActor( Actor A );
native function DetachActor( Actor A );

simulated event PostBeginPlay()
{
	AttachProjector();
	if( bLevelStatic )
	{
		AbandonProjector();
		Destroy();
	}
	if( bProjectActor )
		SetCollision(True, False, False);
}

simulated event Touch( Actor Other )
{
	if(Other==None)
		return;
	if( Other.bAcceptsProjectors && (ProjectTag=='' || Other.Tag==ProjectTag) && (bProjectStaticMesh || Other.StaticMesh == None) )
		AttachActor(Other);
}

simulated event Untouch( Actor Other )
{
	DetachActor(Other);
}

defaultproperties
{
     FrameBufferBlendingOp=PB_Modulate
     MaxTraceDistance=1000
     bProjectBSP=True
     bProjectTerrain=True
     bProjectStaticMesh=True
     bProjectParticles=True
     bProjectActor=True
     GradientTexture=Texture'Engine.GRADIENT_Fade'
     ProjectorRadius=-1.000000
     bStatic=True
     bHidden=True
     bCheckChangableLevel=True
     RemoteRole=ROLE_None
     Texture=Texture'Engine.Proj_Icon'
     bDirectional=True
}
