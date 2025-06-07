class ViewportWindowController extends Actor
	native;

enum ECALCSTEP
{
	STEP_TARGET_PLAYER,
	STEP_MOVE_BACK,
	STEP_ROTATE_FLOAT,
	STEP_TARGET_FLOAT,
	STEP_MOVE_FLOAT
};

var	SkeletalMeshInstance	TargetSMInst;
var	int						BoneIndex;

var vector					TargetLocation;
var	rotator					TargetRotation;
var array<vector>			CameraPath;
var	int						CalcStep;

// for skill effect
var vector					OrgLocation;
var int						EffectType;
var float					fEffectElapsedTime;

defaultproperties
{
     DrawType=DT_None
}
