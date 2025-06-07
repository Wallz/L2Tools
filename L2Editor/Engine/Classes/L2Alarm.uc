class L2Alarm extends Actor
	placeable
	native
	nativereplication;
	
var sound	ClickSound;

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
