class AnimNotify_IdleSound extends AnimNotify
	native;

var() sound Sound;
var() float Volume;
var() int Radius;
var() int Random;
var sound IdleSound[3];

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

cpptext
{
	// AnimNotify interface.
	virtual void Notify( UMeshInstance *Instance, AActor *Owner );
	// Load Sound
	virtual USound* LoadSound( TCHAR* Name );
	virtual USound* GetSound( INT ItemClassID );
}


defaultproperties
{
     Volume=1.000000
     Random=100
}
