//=============================================================================
// AIMarker.
//=============================================================================
class AIMarker extends SmallNavigationPoint
	native;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

var AIScript markedScript;

cpptext
{
	virtual UBOOL IsIdentifiedAs(FName ActorName);
}


defaultproperties
{
     bCollideWhenPlacing=False
     bHiddenEd=True
}
