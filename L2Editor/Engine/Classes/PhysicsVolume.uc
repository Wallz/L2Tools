//=============================================================================
// PhysicsVolume:  a bounding volume which affects actor physics
// Each Actor is affected at any time by one PhysicsVolume
// This is a built-in Unreal class and it shouldn't be modified.
//=============================================================================
class PhysicsVolume extends Volume
	native
	nativereplication;

var()		bool		bPainCausing;	 // Zone causes pain.
var()		vector		ZoneVelocity;
var()		vector		Gravity;
var()		float		GroundFriction;
var()		float		TerminalVelocity;
var()		float		DamagePerSec;
var() class<DamageType>	DamageType;
var()		int			Priority;	// determines which PhysicsVolume takes precedence if they overlap
var() sound  EntrySound;	//only if waterzone
var() sound  ExitSound;		// only if waterzone
var() class<actor> EntryActor;	// e.g. a splash (only if water zone)
var() class<actor> ExitActor;	// e.g. a splash (only if water zone)
var() float  FluidFriction;
var() vector ViewFlash, ViewFog;

var()		bool	bDestructive; // Destroys most actors which enter it.
var()		bool	bNoInventory;
var()		bool	bMoveProjectiles;// this velocity zone should impart velocity to projectiles and effects
var()		bool	bBounceVelocity;	// this velocity zone should bounce actors that land in it
var()		bool	bNeutralZone; // Players can't take damage in this zone.
var			bool	bWaterVolume;
var	Info PainTimer;

// Distance Fog
var(VolumeFog) bool   bDistanceFog;	// There is distance fog in this physicsvolume.
var(VolumeFog) color DistanceFogColor;
var(VolumeFog) float DistanceFogStart;
var(VolumeFog) float DistanceFogEnd;
// #ifdef __L2 // zodiac
var(VolumeFog) bool	bUseDistanceFogColor;
var(VolumeFog) bool bUseCellophane;
var(VolumeFog) color CellophaneColor;
// #endif


// Karma
var(Karma)	   float KExtraLinearDamping; // Extra damping applied to Karma actors in this volume.
var(Karma)	   float KExtraAngularDamping;
var(Karma)	   float KBuoyancy;			  // How buoyant Karma things are in this volume (if bWaterVolume true). Multiplied by Actors KarmaParams->KBuoyancy.

var PhysicsVolume NextPhysicsVolume;

//#ifdef __L2 // zodiac
var() bool bL2WaterVolume;
var() bool bL2StepVolume;

var(StepSound) int StepSoundID;
//#endif


replication
{
	// Things the server should send to the client.
	reliable if( bNetDirty && (Role==ROLE_Authority) )
		Gravity;
}

simulated function PostBeginPlay()
{
	Super.PostBeginPlay();

	if ( Role < ROLE_Authority )
		return;
	if ( bPainCausing )
		PainTimer = Spawn(class'VolumeTimer', self);
}

/* Called when an actor in this PhysicsVolume changes its physics mode
*/
event PhysicsChangedFor(Actor Other);

event ActorEnteredVolume(Actor Other);
event ActorLeavingVolume(Actor Other);

event PawnEnteredVolume(Pawn Other)
{
	if ( Other.IsPlayerPawn() )
		TriggerEvent(Event,self, Other);
}

event PawnLeavingVolume(Pawn Other)
{
	if ( Other.IsPlayerPawn() )
		UntriggerEvent(Event,self, Other);
}

/*
TimerPop
damage touched actors if pain causing.
since PhysicsVolume is static, this function is actually called by a volumetimer
*/
function TimerPop(VolumeTimer T)
{
	local actor A;

	if ( T == PainTimer )
	{
		if ( !bPainCausing )
			return;

		ForEach TouchingActors(class'Actor', A)
			CausePainTo(A);
	}
}

function Trigger( actor Other, pawn EventInstigator )
{
	// turn zone damage on and off
	if (DamagePerSec != 0)
	{
		bPainCausing = !bPainCausing;
		if ( bPainCausing && (PainTimer == None) )
			PainTimer = spawn(class'VolumeTimer', self);
	}
}

event touch(Actor Other)
{
	Super.Touch(Other);
	if ( Other == None )
		return;
	if ( bNoInventory && Other.IsA('Inventory') && (Other.Owner == None) )
	{
		Other.LifeSpan = 1.5;
		return;
	}
	if ( bMoveProjectiles && (ZoneVelocity != vect(0,0,0)) )
	{
		if ( Other.Physics == PHYS_Projectile )
			Other.Velocity += ZoneVelocity;
		else if ( Other.IsA('Effects') && (Other.Physics == PHYS_None) )
		{
			Other.SetPhysics(PHYS_Projectile);
			Other.Velocity += ZoneVelocity;
		}
	}
	if ( bPainCausing )
	{
		if ( Other.bDestroyInPainVolume )
		{
			Other.Destroy();
			return;
		}
		CausePainTo(Other);
	}
//#ifdef __L2 // zodiac
	if ( bL2WaterVolume && Other.CanSplash() )
		PlayEntrySplash(Other);
//#else
//	if ( bWaterVolume && Other.CanSplash() )
//		PlayEntrySplash(Other);
//#endif
}

function PlayEntrySplash(Actor Other)
{
	local float SplashSize;
	local actor splash;

	splashSize = FClamp(0.00003 * Other.Mass * (250 - 0.5 * FMax(-600,Other.Velocity.Z)), 0.1, 1.0 );
	if( EntrySound != None )
	{
		PlaySound(EntrySound, SLOT_Interact, splashSize);
		if ( Other.Instigator != None )
			MakeNoise(SplashSize);
	}
	if( EntryActor != None )
	{
		splash = Spawn(EntryActor); 
		if ( splash != None )
			splash.SetDrawScale(splashSize);
	}
}

event untouch(Actor Other)
{
//#ifdef __L2 // zodiac
	if ( bL2WaterVolume && Other.CanSplash() )
		PlayExitSplash(Other);
//#else
//	if ( bWaterVolume && Other.CanSplash() )
//		PlayExitSplash(Other);
//#endif
}

function PlayExitSplash(Actor Other)
{
	local float SplashSize;
	local actor splash;

	splashSize = FClamp(0.003 * Other.Mass, 0.1, 1.0 );
	if( ExitSound != None )
		PlaySound(ExitSound, SLOT_Interact, splashSize);
	if( ExitActor != None )
	{
		splash = Spawn(ExitActor); 
		if ( splash != None )
			splash.SetDrawScale(splashSize);
	}
}

function CausePainTo(Actor Other)
{
	local float depth;
	local Pawn P;

	// FIXMEZONE figure out depth of actor, and base pain on that!!!
	depth = 1;
	P = Pawn(Other);

	if ( DamagePerSec > 0 )
	{
		if ( Region.Zone.bSoftKillZ && (Other.Physics != PHYS_Walking) )
			return;
		Other.TakeDamage(int(DamagePerSec * depth), None, Location, vect(0,0,0), DamageType); 
		if ( (P != None) && (P.Controller != None) )
			P.Controller.PawnIsInPain(self);
	}	
	else
	{
		if ( (P != None) && (P.Health < P.Default.Health) )
		P.Health = Min(P.Default.Health, P.Health - depth * DamagePerSec);
	}
}

event	GetHitEffectName(out name a, out name b);

defaultproperties
{
     Gravity=(Z=-1500.000000)
     GroundFriction=8.000000
     TerminalVelocity=2500.000000
     FluidFriction=0.300000
     KBuoyancy=1.000000
     StepSoundID=-1
     bAlwaysRelevant=True
}
