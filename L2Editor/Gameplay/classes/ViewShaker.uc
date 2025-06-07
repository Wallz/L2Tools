//=============================================================================
// ViewShaker:  Shakes view of any playercontrollers - amb - rewrote this
// within the ShakeRadius
//=============================================================================
class ViewShaker extends Triggers;

//-----------------------------------------------------------------------------
// Variables.

var() float  ShakeRadius;		// radius within which to shake player views
var() vector RotMag;			// how far to rot view
var() vector RotRate;			// how fast to rot view
var() float  RotTime;			// how much time to rot the instigator's view
var() vector OffsetMag;		    // max view offset vertically
var() vector OffsetRate;		// how fast to offset view vertically
var() float  OffsetTime;	    // how much time to offset view

//-----------------------------------------------------------------------------
// Functions.

function Trigger( actor Other, pawn EventInstigator )
{
	local Controller C;

	for ( C=Level.ControllerList; C!=None; C=C.NextController )
		if ( (PlayerController(C) != None) && (VSize(Location - PlayerController(C).ViewTarget.Location) < ShakeRadius) )		
			C.ShakeView(RotMag, RotRate, RotTime, OffsetMag, OffsetRate, OffsetTime);
}

defaultproperties
{
	ShakeRadius=+2000.0
	OffsetMag=(X=10,Y=10,Z=10)
	OffsetRate=(X=1000,Y=1000,Z=1000)
	OffsetTime=3.0
}

