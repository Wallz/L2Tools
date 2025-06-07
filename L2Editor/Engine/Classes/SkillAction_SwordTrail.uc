////////////////////////////////////////////
// SkillAction_SwordTrail.uc
// 
// revision history : created by nonblock (8th,Nov,2004)
////////////////////////////////////////////
class SkillAction_SwordTrail extends SkillAction
	native;
	// native collapsecategories editinlinenew;

var() float	DurationRatio; // = duration / shotTime
var() bool bRightHand;

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
     DurationRatio=0.800000
     bRightHand=True
}
