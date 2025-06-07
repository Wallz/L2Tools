class L2Indicator extends Emitter
	placeable
	native
	nativereplication;

var int		Type;

defaultproperties
{
     DrawType=DT_Mesh
     NoCheatCollision=True
     bOrientOnSlope=True
     bAlwaysRelevant=True
     bCheckChangableLevel=False
     RemoteRole=ROLE_DumbProxy
     NetUpdateFrequency=8.000000
     NetPriority=1.400000
     Texture=Texture'Engine.S_Inventory'
     CollisionRadius=0.100000
     CollisionHeight=0.100000
     bCollideActors=True
     bProjTarget=True
     bFixedRotationDir=True
}
