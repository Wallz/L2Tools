//
//	ShadowProjector
//

class ShadowProjector extends Projector;

var() Actor					ShadowActor;
var() vector				LightDirection;
var() float					LightDistance;
var() bool					RootMotion;
var() bool					bBlobShadow;
var() bool					bShadowActive;
var ShadowBitmapMaterial	ShadowTexture;
//#ifdef __L2 //v927->v2110 mark by giga
var() bool					L2Hidden;
var() bool					L2DefaultShadow;
//#endif

// ifdef __L2
native final function bool UpdateLightInfo(); // zodiac
native final function bool CheckVisible(); // goldbat
// endif

//
//	PostBeginPlay
//

event PostBeginPlay()
{
	Super(Actor).PostBeginPlay();
}

//
//	Destroyed
//

event Destroyed()
{
	if(ShadowTexture != None)
	{
		ShadowTexture.ShadowActor = None;
		
		if(!ShadowTexture.Invalid)
			Level.ObjectPool.FreeObject(ShadowTexture);

		ShadowTexture = None;
		ProjTexture = None;
//#ifdef __L2 // zodiac
		ShadowActor = None;
//#endif
	}

	Super.Destroyed();
}
//#ifdef __L2 // zodiac
function bool IsUnderWaterCamera()
{
	local viewport Viewport;

	Viewport = GetViewport();
	if ( Viewport != none ) {
		if ( Viewport.Actor.PhysicsVolume.bL2WaterVolume ) return true;
	}
	return false;
}
//#endif

//
//	InitShadow
//

function InitShadow()
{
	local Plane		BoundingSphere;

	if(ShadowActor != None)
	{
		BoundingSphere = ShadowActor.GetRenderBoundingSphere();
		FOV = Atan(BoundingSphere.W * 2 + 160, LightDistance) * 180 / PI;

		ShadowTexture = ShadowBitmapMaterial(Level.ObjectPool.AllocateObject(class'ShadowBitmapMaterial'));
		ProjTexture = ShadowTexture;

		if(ShadowTexture != None)
		{
			SetDrawScale(LightDistance * tan(0.5 * FOV * PI / 180) / (0.5 * ShadowTexture.USize));

			ShadowTexture.Invalid = False;
			ShadowTexture.bBlobShadow = bBlobShadow;
			ShadowTexture.ShadowActor = ShadowActor;
			ShadowTexture.LightDirection = Normal(LightDirection);
			ShadowTexture.LightDistance = LightDistance;
			ShadowTexture.LightFOV = FOV;
            ShadowTexture.CullDistance = CullDistance; 

			Enable('Tick');
			UpdateShadow();
		}
		else
			Log(Name$".InitShadow: Failed to allocate texture");
	}
	else
		Log(Name$".InitShadow: No actor");
}

//
//	UpdateShadow
//

function UpdateShadow()
{
//#ifdef __L2
	local vector	ShadowLocation;
	local Plane		BoundingSphere;

	DetachProjector(true);
	//SetCollision(false,false,false);

	//	if(ShadowActor != None && !ShadowActor.bHidden)
	if(!L2Hidden && ShadowActor != None && !ShadowActor.bHidden) // for L2 by goldbat
	{
		UpdateLightInfo();
		if ( L2DefaultShadow ) {
			SetLocation(ShadowActor.Location);
			SetRotation(Rotator(-vect(0,0,10)));
			SetDrawScale(0.05);
			//SetDrawScale(0.20);
			MaterialBlendingOp=PB_None;
			FrameBufferBlendingOp=PB_AlphaBlend;
			
			
			//MaterialBlendingOp=PB_None;
			//FrameBufferBlendingOp=PB_Modulate;
			ShadowTexture.bDefaultShadow = true;
		}
		else {
			//UpdateLightInfo();
			BoundingSphere = ShadowActor.GetRenderBoundingSphere();
			FOV = Atan(BoundingSphere.W * 2 +160, LightDistance) * 180 / PI + 5;

			if(ShadowActor.DrawType == DT_Mesh && ShadowActor.Mesh != None)
				ShadowLocation = ShadowActor.GetBoneCoords('').Origin;
			else
				ShadowLocation = ShadowActor.Location;

			SetLocation(ShadowLocation);
			SetRotation(Rotator(Normal(-LightDirection)));
			SetDrawScale((LightDistance * tan(0.5 * FOV * PI / 180) / (0.5*ShadowTexture.USize)));

			ShadowTexture.ShadowActor = ShadowActor;
			ShadowTexture.LightDirection = Normal(LightDirection);
			ShadowTexture.LightDistance = LightDistance;
			ShadowTexture.LightFOV = FOV;
			ShadowTexture.Dirty = true;
			//MaterialBlendingOp=PB_None;
			//FrameBufferBlendingOp=PB_Modulate;
			//#ifdef __L2 // zodiac
			MaterialBlendingOp=PB_None;
			FrameBufferBlendingOp=PB_AlphaBlend;
			//#endif
			ShadowTexture.bDefaultShadow = false;
		}

		AttachProjector();
		//SetCollision(true,false,false);
	}
//#else
/*
	local coords	C;

	DetachProjector(true);

	if(ShadowActor != None && !ShadowActor.bHidden && ShadowTexture != None && bShadowActive)
	{
		if(ShadowTexture.Invalid)
			Destroy();
		else
		{
			if(RootMotion && ShadowActor.DrawType == DT_Mesh && ShadowActor.Mesh != None)
			{
				C = ShadowActor.GetBoneCoords('');
				SetLocation(C.Origin);
			}
			else
				SetLocation(ShadowActor.Location + vect(0,0,5));

			SetRotation(Rotator(Normal(-LightDirection)));

			ShadowTexture.Dirty = true;
            ShadowTexture.CullDistance = CullDistance; 
            
            AttachProjector();
		}
	}
*/
//#endif
}

//
//	Tick
//

function Tick(float DeltaTime)
{
	super.Tick(DeltaTime);
	CheckVisible(); // for L2 by goldbat
	UpdateShadow();
}

//
//	Default properties
//

defaultproperties
{
     bShadowActive=True
     bProjectActor=False
     bClipBSP=True
     bProjectOnParallelBSP=True
     bDynamicAttach=True
     bNeedCleanup=False
     bStatic=False
     L2NeedTick=False
     CullDistance=1200.000000
     bOwnerNoSee=True
}
