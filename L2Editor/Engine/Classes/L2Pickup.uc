class L2Pickup extends Actor
	placeable
	native
	nativereplication;
	
var rotator TargetRotation;
var rotator DeltaRotation;
var sound	DropSound;
//__L2 kurt
var bool	bPendingDrop;
var Emitter	DropEffectActor;
var bool	bDropEffectActor;
var vector	CheckLocation;


simulated function Timer()
{
	if(bPendingDrop)	
		bHidden=False;		
}

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
