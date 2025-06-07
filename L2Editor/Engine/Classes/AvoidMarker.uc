//=============================================================================
// AvoidMarker.
// Creatures will tend to back away when near this spot
//=============================================================================
class AvoidMarker extends Triggers
	native
	notPlaceable;

function Touch( actor Other )
{
	if ( (Pawn(Other) != None) && (Pawn(Other).Controller != None) )
		Pawn(Other).Controller.FearThisSpot(self);
}

function StartleBots()
{
	local Pawn P;
	
	ForEach CollidingActors(class'Pawn', P, CollisionRadius)
	{
		if ( AIController(P.Controller) != None )
			AIController(P.Controller).Startle(self);
	}
}

defaultproperties
{
     RemoteRole=ROLE_None
     CollisionRadius=100.000000
}
