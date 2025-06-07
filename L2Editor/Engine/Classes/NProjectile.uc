//__L2 kurt
class NProjectile extends Emitter
	native;

// Motion information.
var		float   Speed;               // Initial speed of projectile.
var		float   AccSpeed;            // Limit on speed of projectile (0 means no limit)

var		Actor	TargetActor;
var		vector LastTargetLocation;
var		rotator	LastTargetRotation;	// by nonblock
var     Actor	TraceActor;

var		bool	 bTrackingCamera;
var		bool	 bPreDestroy;

//#ifdef __L2 by nonblock
var(interpolation)		bool	 bHermiteInterpolation;
var(interpolation)		vector	 VelInitial;
var(interpolation)		vector	 VelFinal;
var(interpolation)		vector	 LocInitial;
var(interpolation)		float	 Duration;
var(interpolation)		transient	float	CurTime;
var(interpolation)		float	 Disp;
//var(interpolation)		range	DurationRange;
//var(interpolation)		float	DurationCoef;
var transient NMagicInfo     MagicInfo;


//#endif

simulated event	ShotNotify();

// __L2 by nonblock
simulated event PreshotNotify(Pawn Attacker);
// #endif

defaultproperties
{
     Speed=10.000000
     AccSpeed=10.000000
     bNoDelete=False
     LifeSpan=100.000000
     CollisionRadius=5.000000
     CollisionHeight=5.000000
}
