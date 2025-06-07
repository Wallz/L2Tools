//////////////////////////////
// emitter for Airvolume
//  - used periodical polling to prevent from mass spawning or despawning which could happen by constant camera movement.
//	created by nonblock(2004.11.15)
//////////////////////////////
class AirEmitter extends Emitter
	native;

var() bool  bAttachToPawn;		// If False - Attach onto Camera
var() bool	bByOpacity;			// Opacity-based or MaxParticle-based fading
var() int	NumSteps;			// actually transient if bByOpacity=False
var() range	FadeRange;
var() transient float	FadeAlpha;
var() transient	int		FadeAlphaByNum;

var() transient airvolume RecentVolume;
var() transient float	RecentFadeSeconds;
//var() transient float	RecentSizeScale;

// how fast to vanish myself if owner has entered different volume. (by modifying FadeAlpha difference)
var(Vanish) float	BoostVanishCoef;	

// vanishing speed compared to fading speed. (by modifying TimerRate)
var(Vanish) float	VanishCoef;	

var() int	IndexPE;	// which particleemitter 

function bool IsInAirVolume()
{
	return ( RecentVolume != None && RecentVolume.EffectName != 'None' );
}

simulated function Destroyed()
{
	Super.Destroyed();
	ClearL2Game();
}

event ClearL2Game()
{
	if(Owner != None && LineagePlayerController(Owner) != None && LineagePlayerController(Owner).AirEffect == self )
	{
		LineagePlayerController(Owner).AirEffect = None;
	}

	SetBase(None);
}

event ForceKill()
{
	if( GetStateName() != '_killpending' )
		GotoState('_killpending');
}

// inside-state function. forward decl.
function TouchVolume();
function UnTouchVolume();

event SetAirVolume(AirVolume NewVolume)
{
	RecentVolume = NewVolume;

	if( IsInAirVolume() )
		RecentFadeSeconds = RecentVolume.FullFadeSeconds;
}

function Timer()
{
	if( IsInAirVolume() )
		TouchVolume();
	else
		UnTouchVolume();
}

function ApplyFadeAlpha()
{	
	//local int i;

	if(bByOpacity)
	{
		Emitters[IndexPE].Opacity = FadeAlpha;

		//for( i=0; i<Emitters.Length; i++ )
		//{
			//if( Emitters[i] != None && Emitters[i].ForcedMaxParticles)
				//Emitters[i].Opacity = FadeAlpha;
		//}
	}
	else
	{
		//VanishFadeAlpha = int(FadeAlpha);
		Emitters[IndexPE].MaxActiveParticles = int(FadeAlpha);

		// !TODO : Check this method of vanishing works.
		//for( i=0; i<Emitters.Length; i++ )
		//{
			//if( Emitters[i] != None && Emitters[i].ForcedMaxParticles)
				 //Emitters[i].MaxActiveParticles = int(FadeAlpha);
		//}
	}
}

function PostBeginPlay()
{
	Super.PostBeginPlay();	

	FadeAlpha = 0.0;

	if(!bByOpacity)
		NumSteps = int(FadeRange.Max - FadeRange.Min);

	//if( LineagePlayerController(Owner) != None )
	//{
		//GotoState('active');
		// InitialState = GetStateName();
	//}
	//else
	//{
		// unreachable
	//}
}

state() dummyinitial
{	
ignores Timer, ApplyFadeAlpha;
	simulated function EndState()
	{
		Super.EndState();
		FadeAlpha = FadeRange.Min;
	}
Begin:
	Sleep(1.0);
	if( IsInAirVolume() )
		GotoState('active');
}

// inside valid airvolume.
state() active
{
	simulated function BeginState()
	{	
		Super.BeginState();
		SetTimer(RecentFadeSeconds / NumSteps, true);
		ApplyFadeAlpha();
	}

	function TouchVolume()
	{
		FadeAlpha = FClamp( (FadeRange.Max-FadeRange.Min) / NumSteps + FadeAlpha, FadeRange.Min, FadeRange.Max);
		ApplyFadeAlpha();
	}

	function UnTouchVolume()
	{
		GotoState('vanishing');
	}
}

state() vanishing
{
	simulated function BeginState()
	{
		// Note : doesn't work cuz context is still inside the GotoState call  :)
		// UnTouchVolume();

		SetTimer(RecentFadeSeconds / (VanishCoef * NumSteps), true);
	}

	function TouchVolume()
	{		
		// local int i;

		GotoState('active');

		// reentering.
		//if( IsA( RecentVolume.EffectName) )
		//{
			////RelativeTrailOffset.X = RecentVolume.RelativeOffset;
			//for( i=0; i< Emitters.Length; i++)
				//Emitters[i].StartLocationOffset.X = RecentVolume.RelativeOffset;

			//GotoState('active');
		//}

		// otherwise, vanish myself to let playercontroller spawn another one.
		//else			
			//UnTouchVolume();

	}

	function UnTouchVolume()
	{
		if(IsInAirVolume())
			FadeAlpha = FClamp( FadeAlpha - BoostVanishCoef*(FadeRange.Max - FadeRange.Min ) / NumSteps, FadeRange.Min, FadeRange.Max );
		else
			FadeAlpha = FClamp( FadeAlpha - (FadeRange.Max - FadeRange.Min ) / NumSteps, FadeRange.Min, FadeRange.Max );

		ApplyFadeAlpha();

		if(FadeAlpha == FadeRange.Min)
		{
			GotoState('_killpending');
		}
	}
}

state() _killpending
{
	ignores SetAirVolume, Timer, TouchVolume, UnTouchVolume;

	simulated function BeginState()
	{
		Super.BeginState();

		SetTimer(0.0, false);

		if( LineagePlayerController(Owner) != None && LineagePlayerController(Owner).AirEffect == self)
			LineagePlayerController(Owner).AirEffect = None;
		Kill();
	}
}

defaultproperties
{
     NumSteps=1
     FadeRange=(Max=1.000000)
     BoostVanishCoef=1.500000
     VanishCoef=3.000000
     AutoDestroy=True
     InitialState="dummyinitial"
}
