//=============================================================================
// Emitter: An Unreal Emitter Actor.
//=============================================================================
class Emitter extends Actor
	native
	placeable;

#exec Texture Import File=Textures\S_Emitter.pcx  Name=S_Emitter Mips=Off MASKED=1


var()	export	editinline	array<ParticleEmitter>	Emitters;

var		(Global)	bool				AutoDestroy;
var		(Global)	bool				AutoReset;
var		(Global)	bool				DisableFogging;
var		(Global)	rangevector			GlobalOffsetRange;
var		(Global)	range				TimeTillResetRange;
//#ifdef __L2 //kurt
var		(Global)	bool				AutoReplay;//only editor
var		float							SpeedRate;//for skillspeed rate
var		(Global)	bool				bRotEmitter;
var		(Global)	rotator				RotPerSecond;
var		(Global)	bool				FixedBoundingBox;
var		(Global)	FLOAT				FixedBoundingBoxExpand;

var		(SpawnSound)	array<sound>	SpawnSound;
var		(SpawnSound)	float			SoundRadius;
var		(SpawnSound)	float			SoundVolume;
var		(SpawnSound)	bool			SoundLooping;
var		(SpawnSound)	float			SoundPitchMin;
var		(SpawnSound)	float			SoundPitchMax;
//#endif

var		transient	int					Initialized;
var		transient	box					BoundingBox;
var		transient	float				EmitterRadius;
var		transient	float				EmitterHeight;
var		transient	bool				ActorForcesEnabled;
var		transient	vector				GlobalOffset;
var		transient	float				TimeTillReset;
var		transient	bool				UseParticleProjectors;
var		transient	ParticleMaterial	ParticleMaterial;
var		transient	bool				DeleteParticleEmitters;

//nonblock
var		transient	float				FixedLifeTime;

//kurt
var		transient	bool				FirstSpawnParticle;
var		transient	vector				TrailerPrePivot;
//emitter light
var     (EmitterLight)		bool		bUseLight;
var		(EmitterLight)		byte		LightType, LightEffect;
var		(EmitterLight)		byte		LightBrightness;
var		(EmitterLight)		float		LightRadius;
var		(EmitterLight)		byte		LightHue, LightSaturation;
var		(EmitterLight)		byte		EmitterLightingType;
var		transient		emitterlight	pEmitterLight;
var		(EmitterLight)		float		EL_LifeSpan;
var		(EmitterLight)		float		EL_InitialDelay;

//emitter quake 
var     (EmitterQuake)		bool		bUseQuake;
var     (EmitterQuake)		byte		ShakeType;
var     (EmitterQuake)		float		ShakeIntensity;
var     (EmitterQuake)		vector		ShakeVector;
var     (EmitterQuake)		float		ShakeRange;
var     (EmitterQuake)		int			ShakeCount;
var     (EmitterQuake)		float		ShakeTime;
var     (EmitterQuake)		float		EQ_InitialDelay;

// nonblock
// render if the distance is within
var		(Global)			range			VisibleLimit;
var		(Global)			float			VisibilityInterpRange;

// flagoftiger
var		(Global)			bool			bSetSizeScale;

// shutdown the emitter and make it auto-destroy when the last active particle dies.
native function Kill();

// idearain
native final function SetSizeScale(float NewScale);

simulated function UpdatePrecacheMaterials()
{
	local int i;
	for( i=0; i<Emitters.Length; i++ )
	{
		if( Emitters[i] != None )
		{
			if( Emitters[i].Texture != None )
				Level.AddPrecacheMaterial(Emitters[i].Texture);
		}
	}
}

event Trigger( Actor Other, Pawn EventInstigator )
{
	local int i;
	for( i=0; i<Emitters.Length; i++ )
	{
		if( Emitters[i] != None )
			Emitters[i].Trigger();
	}
}

simulated function SetDisabled(bool dis)
{
	local int i;
	for( i=0; i<Emitters.Length; i++ )
	{
		if( Emitters[i] != None )
			Emitters[i].Disabled = dis;
	}
}

defaultproperties
{
     SpeedRate=1.000000
     SoundPitchMin=1.000000
     SoundPitchMax=1.000000
     VisibilityInterpRange=500.000000
     bSetSizeScale=True
     DrawType=DT_Particle
     bNeedCleanup=False
     bNoDelete=True
     bCheckChangableLevel=True
     RemoteRole=ROLE_None
     Texture=Texture'Engine.S_Emitter'
     Style=STY_Particle
     bUnlit=True
}
