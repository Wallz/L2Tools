////////////////////////////////////////////
// SkillVisualEffect.uc
//  
// revision history : created by nonblock (26th,Oct,2004)
////////////////////////////////////////////
// #ifdef __L2 // by nonblock
class SkillVisualEffect extends Object
	native
	hidecategories(Object);

/////////////////////////////////
// 1. General
/////////////////////////////////
var(General) name Desc;
/////////////////////////////////
// 2. Visual
/////////////////////////////////

//// !OBSOLTE
// [0]~[n] = Actions to be taken before casting (includes instant skills)
// [n+1]~[m] = Actions to be taken at the 1st Notify_AttackShot.
// .....
// the chain from 0 to n as well as n+1 to m, is connected among themselves by NextConcurrentAction of the array element
// and won't end up until NextConcurrentAction == -1
// var(Visual) array<SkillAction> NotifyAction;
//// !

struct native SkillActionInfo
{
	var() editinlinenotify SkillAction Action;
	var() int SpecificStage;
};

var(Visual) array<SkillActionInfo> CastingActions;
var(Visual) array<SkillActionInfo> ChannelingActions;
var(Visual) array<SkillActionInfo> PreshotActions;
var(Visual) array<SkillActionInfo> ShotActions;
var(Visual) array<SkillActionInfo> ExplosionActions;
var(Visual) float FlyingTime;

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

// #endif

cpptext
{
	// trigger the actions	
	void TriggerTransientEffect(class APawn *Owner, class AActor *Target, INT SkillID);
	void TriggerCasting(class APawn *Owner, class AActor *Target);
	void TriggerChanneling(class APawn *Owner, class AActor *Target);
	void TriggerPreshot(class APawn *Owner);
	void TriggerShot(class APawn* Owner);
	void TriggerExplosion(class ANSkillProjectile *Projectile);
}


defaultproperties
{
}
