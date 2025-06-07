////////////////////////////////////////////
// SkillAction_LocateEffect.uc
//   agenda : this skillaction can put one emitter to the specified location.
//            most general one of all SkillActions.
// revision history : created by nonblock (26th,Oct,2004)
////////////////////////////////////////////
class SkillAction_LocateEffect extends SkillAction
	native;
	// native collapsecategories editinlinenew;

var(attach) enum EAttachMethod
{	
	EAM_None,				// don't attach
	EAM_RH,					// attach to GetRHandBoneName();
	EAM_LH,		
	EAM_BoneSpecified,		// attach to this.AttachBoneName
	EAM_AliasSpecified,		// attach to TagAlias(AttachBoneName);
	EAM_Trail,				// don't attach, trail the targetactor( assume physics of the emitter is PHYS_Trailer )
	EAM_RF,
	EAM_LF	
} AttachOn;
var(attach) name AttachBoneName;
var(attach) bool bAbsolute;	// EATP_Absolute or not.

// SpawnDelay = 0 : default
// SpawnDelay > 0 : SetDelayed(SpawnDelay)
// SpawnDelay < 0 : SetDelayed(SkillHitTime - abs(SpawnDelay))
var(time)	float SpawnDelay;

var(positioning) bool bUseCharacterRotation;

//#endif

///////////////////////////////////////////////
// !TODO : Consider 'Rotation'
//
// if true, let the emitter's rotation to be 'BaseActor => TargetPawn'
// would be mostly true.
// var(positioning) bool bUseDefaultDirection;
//
// direction of the emitter
// EAM_RH, EAM_LH , EAM_BoneSpecified : RelativeRotation of attachment.
// var(positioning) rotator Direction;
///////////////////////////////////////////////


// EAM_RH, EAM_LH, EAM_BoneSpecified : RelativeLocation of attachment.
// 
// resides in the coord system where Coords.X points out 'spellcaster-to-target' direction and Coords.Y upward
//
//        Y                               Y
//        ^                               ^ 
//        |                               |
//    (Caster) ----->X                 (Target) -----> X
var(positioning) vector Offset; // disposition of the emitter

var(positioning) bool bRelativeToCylinder; // If true, this.Offset will be scaled by X*=CollisionRadius, Y*=CollisionHeight, Z*=1

var(positioning) bool bSpawnOnTarget;
var(positioning) bool bSizeScale;
// var(positioning) float ScalingFactor;
// var(visual) emitter EffectEmitter;  // the emitter itself

// !TODO
// var(visual) bool bLifetimeSync;		// If true, emitter lifetime should be BaseActor.MagicInfo.ShotTime. true on nearly all casting effects.

// !TODO : if(fDelaySec > 0 ) SetDelayed(fDelaySec)
// var(visual) float fDelayingSec; 


// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

cpptext
{
	virtual AEmitter* Notify( AActor *BaseActor, AActor *DestActor);
	virtual void PostEditChange();
}


defaultproperties
{
     bRelativeToCylinder=True
}
