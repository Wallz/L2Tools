//=============================================================================
// StaticMeshActor.
// An actor that is drawn using a static mesh(a mesh that never changes, and
// can be cached in video memory, resulting in a speed boost).
//=============================================================================

class StaticMeshActor extends Actor
	native
	placeable;

//#if __L2 // gigadeth
struct native AccessoryType
{
	var() int Depth;
	var() StaticMesh Mesh;
};
//#endif

//#ifdef __L2 // zodiac agit관련 변수
var(Agit) bool bAgitDefaultStaticMesh;
var(Agit) int AgitID;
// Accessroy는 0보다 커야 한다. 0은 wallpaper이기 때문이다.
var(Agit) int AccessoryIndex;
var(Agit) int AgitStatus;
var(Agit) transient int CurrAccessoryType;
var(Agit) array<AccessoryType> AccessoryTypeList;
//#endif

//#if __L2 // gigadeth
var(TimeReactor) bool bTimeReactor;
var(TimeReactor) float ShowTime;
var(TimeReactor) float HideTime;
//#endif

//#ifdef __L2 // zodiac
var(Sound) sound		StepSound_1;
var(Sound) sound		StepSound_2;
var(Sound) sound		StepSound_3;
//#endif


var() bool bExactProjectileCollision;		// nonzero extent projectiles should shrink to zero when hitting this actor

defaultproperties
{
     bExactProjectileCollision=True
     DrawType=DT_StaticMesh
     bStatic=True
     bWorldGeometry=True
     bShadowCast=True
     bStaticLighting=True
     CollisionRadius=1.000000
     CollisionHeight=1.000000
     bCollideActors=True
     bBlockActors=True
     bBlockPlayers=True
     bBlockKarma=True
     bEdShouldSnap=True
}
