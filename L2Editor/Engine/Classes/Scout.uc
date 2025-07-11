//=============================================================================
// Scout used for path generation.
//=============================================================================
class Scout extends Pawn
	native
	notplaceable;

var const float MaxLandingVelocity;

function PreBeginPlay()
{
	Destroy(); //scouts shouldn't exist during play
}

defaultproperties
{
     AccelRate=1.000000
     CollisionRadius=11.000000
     CollisionHeight=28.000000
     bCollideActors=False
     bCollideWorld=False
     bBlockActors=False
     bBlockPlayers=False
     bProjTarget=False
     bPathColliding=True
}
