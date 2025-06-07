////////////////////////////////////////////
// SkillAction.uc
//  
// revision history : created by nonblock (26th,Oct,2004)
////////////////////////////////////////////
class SkillAction extends Object
	native
	abstract
	editinlinenew	
	hidecategories(Object)
	collapsecategories;

var() class<Emitter> EffectClass;
var() bool bOnMultiTarget; // if true, DestActor is one of MagicInfo.AssociatedActor, otherwise it is MagicInfo.TargetPawn.

// BaseActor is either the caster or the projectile
// DestActor depends on this.bOnMultiTarget
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

cpptext
{
	virtual AEmitter* Notify( AActor *BaseActor, AActor* DestActor );
	// UObject interface.
	virtual void PostEditChange();
}


defaultproperties
{
}
