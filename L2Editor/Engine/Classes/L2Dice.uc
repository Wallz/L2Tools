class L2Dice extends Actor
	placeable
	native
	nativereplication;
	
struct NActionPtr
{
	var	int		Ptr;
};

var rotator TargetRotation;
var rotator DeltaRotation;
var sound	DropSound;
var vector	CheckLocation;
var NActionPtr Action;	
var bool bActionOn;

defaultproperties
{
     DrawType=DT_Mesh
     bNeedCleanup=False
     NoCheatCollision=True
     bOrientOnSlope=True
     bAlwaysRelevant=True
     bCheckChangableLevel=True
     NetUpdateFrequency=8.000000
     NetPriority=1.400000
     Texture=Texture'Engine.S_Inventory'
     CollisionRadius=0.100000
     CollisionHeight=0.100000
     bCollideActors=True
     bProjTarget=True
     bFixedRotationDir=True
}
