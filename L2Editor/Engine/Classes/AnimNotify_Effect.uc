class AnimNotify_Effect extends AnimNotify
	native;

var() class<Actor> EffectClass;
var() name Bone;
var() vector OffsetLocation;
var() rotator OffsetRotation;
var() bool Attach;
var() name Tag;
var() float DrawScale;
var() vector DrawScale3D;
//kurt
var() bool TrailCamera;
var() bool IndependentRotation;
var() float EffectScale;

var private transient Actor LastSpawnedEffect;	// Valid only in the editor.

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

#if defined(__L2)	// 2006/07/26 NeverDie
	virtual AActor* GetLastSpawnedEffect();
	virtual void SetLastSpawnedEffect( AActor* a_pLastSpawnedEffect );
#endif
}


defaultproperties
{
     DrawScale=1.000000
     DrawScale3D=(X=1.000000,Y=1.000000,Z=1.000000)
     EffectScale=1.000000
}
