class L2Float extends Actor
	placeable
	native;

enum TagState
{
	L2TAG_NONE,
	L2TAG_WAIT,
	L2TAG_BATTLE
};

var TagState	State;
var int			FishType;
var bool		Gut;
var bool		Fake;
var float		WaterEffectTimer;
var float		FakeEffectTimer;
var vector		OldEffectLoc;

var float		fEffectElapsedTime;
var int			EffectType;
var vector		OrgLocation;

var emitter		BrightEffect;

var name WaitAnimName;
var name BattleAnimName;
var name BattleWaitAnimName[6];

defaultproperties
{
     WaitAnimName="Wait"
     BattleAnimName="battle"
     BattleWaitAnimName(0)="battlewait01"
     BattleWaitAnimName(1)="battlewait03"
     BattleWaitAnimName(2)="battlewait05"
     BattleWaitAnimName(3)="battlewait02"
     BattleWaitAnimName(4)="battlewait04"
     BattleWaitAnimName(5)="battlewait06"
     DrawType=DT_Mesh
     bCheckChangableLevel=True
}
