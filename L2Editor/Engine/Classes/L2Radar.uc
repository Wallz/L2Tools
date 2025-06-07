class L2Radar extends Actor
	placeable
	native
	nativereplication;

var int Type;

defaultproperties
{
     DrawType=DT_Mesh
     bNeedCleanup=False
     NoCheatCollision=True
     bOrientOnSlope=True
     bAlwaysRelevant=True
     NetUpdateFrequency=8.000000
     NetPriority=1.400000
     Texture=Texture'Engine.S_Inventory'
     CollisionRadius=0.100000
     CollisionHeight=0.100000
     bCollideActors=True
     bProjTarget=True
     bFixedRotationDir=True
}
