class AnimNotify_Sound extends AnimNotify
	native;

var() sound Sound;
var() float Volume;
var() int Radius;
//#ifdef __L2 Hunter
var() int Random;
//#endif
//#ifdef __L2 zodiac
var sound DefaultWalkSound[3];
var sound DefaultRunSound[3];
var sound GrassWalkSound[3];
var sound GrassRunSound[3];
var sound WaterWalkSound[3];
var sound WaterRunSound[3];
var sound DefaultActorWalkSound[3];
var sound DefaultActorRunSound[3];
//#endif

enum L2PawnSoundType
{
	LPST_GRASS,
	LPST_LAND,
	LPST_WATER,
	LPST_ACTOR
};

// (cpptext)
// (cpptext)
// (cpptext)
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
	// #ifdef __L2 // zodiac
	// Load Sound
	virtual USound *LoadSound(TCHAR *Name);
	virtual USound *GetSound(PMoveType MoveType, L2PawnSoundType type);
	void PostLoad();
	// #endif
}


defaultproperties
{
     Volume=1.000000
     Random=100
}
