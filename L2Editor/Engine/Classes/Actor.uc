//=============================================================================
// Actor: The base class of all actors.
// Actor is the base class of all gameplay objects.  
// A large number of properties, behaviors and interfaces are implemented in Actor, including:
//
// -	Display 
// -	Animation
// -	Physics and world interaction
// -	Making sounds
// -	Networking properties
// -	Actor creation and destruction
// -	Triggering and timers
// -	Actor iterator functions
// -	Message broadcasting
//
// This is a built-in Unreal class and it shouldn't be modified.
//=============================================================================
class Actor extends Object
	abstract
	native
	nativereplication;

// Imported data (during full rebuild).
#exec Texture Import File=Textures\S_Actor.pcx Name=S_Actor Mips=Off MASKED=1
#exec Texture Import File=Textures\LockLocation.pcx Name=S_LockLocation Mips=Off MASKED=1
#exec Texture Import File=Textures\AutoAlignToTerrain.pcx Name=S_AutoAlignToTerrain Mips=Off MASKED=1

// Priority Parameters
// Actor's current physics mode.
var(Movement) const enum EPhysics
{
	PHYS_None,
	PHYS_Walking,
	PHYS_Falling,
	PHYS_Swimming,
	PHYS_Flying,
	PHYS_Rotating,
	PHYS_Projectile,
	PHYS_Interpolating,
	PHYS_MovingBrush,
	PHYS_Spider,
	PHYS_Trailer,
	PHYS_Ladder,
	PHYS_RootMotion,
    PHYS_Karma,
    PHYS_KarmaRagDoll,
//#ifdef __L2 //kurt
	PHYS_MovingTrailer,
	PHYS_EffectTrailer,
	PHYS_NProjectile,
	PHYS_NMover,
//#endif
//#ifdef __L2 // zodiac
	PHYS_L2Movement,
//#endif
} Physics;

// Drawing effect.
var(Display) const enum EDrawType
{
	DT_None,
	DT_Sprite,
	DT_Mesh,
	DT_Brush,
	DT_RopeSprite,
	DT_VerticalSprite,
	DT_Terraform,
	DT_SpriteAnimOnce,
	DT_StaticMesh,
	DT_DrawType,
	DT_Particle,
	DT_AntiPortal,
	DT_FluidSurface,
//#ifdef __L2	kurt 
	DT_Sun,
//#endif
//#ifdef __L2 // zodiac
	DT_MusicVolume,
	DT_Custom, // need collision detection even without its mesh
//#endif
//#ifdef __L2 // idearain
	DT_Native	// 해당 Actor의 Render함수를 호출한다.
//#endif
} DrawType;

var(Display) const StaticMesh StaticMesh;		// StaticMesh if DrawType=DT_StaticMesh

// Owner.
var const Actor	Owner;			// Owner actor.
var const Actor	Base;           // Actor we're standing on.

struct ActorRenderDataPtr { var int Ptr; };
struct LightRenderDataPtr { var int Ptr; };

var const native ActorRenderDataPtr	ActorRenderData;
var const native LightRenderDataPtr	LightRenderData;
var const native int				RenderRevision;

enum EFilterState
{
	FS_Maybe,
	FS_Yes,
	FS_No
};

var const native EFilterState	StaticFilterState;
//#ifdef __L2 // zodiac
var bool bDynamicActorFilterState;
//#endif

var(Display) const name	ForcedVisibilityZoneTag; // Makes the visibility code treat the actor as if it was in the zone with the given tag.

// Lighting.
var(Lighting) bool	     bSpecialLit;			// Only affects special-lit surfaces.
var(Lighting) bool	     bActorShadows;			// Light casts actor shadows.
var(Lighting) bool	     bCorona;			   // Light uses Skin as a corona.
var(Lighting) bool		 bLightingVisibility;	// Calculate lighting visibility for this actor with line checks.
var(Display) bool		 bUseDynamicLights;
var bool				 bLightChanged;			// Recalculate this light's lighting now.

//#ifdef __L2 // zodiac
var bool				bUpdateShadow;
var bool				bHideShadow;
var bool				bHideRightHandMesh;
var bool				bHideLeftHandMesh;
var bool				bNeedCleanup;
var bool				bShadowOnly;
//#endif

//	Detail mode enum.

enum EDetailMode
{
	DM_Low,
	DM_High,
	DM_SuperHigh
};

enum EL2EventCmd
{
	LEC_None,
	LEC_Show,
	LEC_Play
};

//#ifdef __L2 Hunter
struct L2Event
{
	var() int EventID;
	var() EL2EventCmd EventCmd;
};

struct NpcPrivate
{
	var() name Name;
	var() name ai;
	var() float Num;
};

struct WhenExtinctionCreate
{
	var() string respawn;
	var() string Name;
};

struct WayPoint
{
	var() vector point;
	var() string Delay;
};

var int  CreatureID;
var bool NoCheatCollision;
var bool CanIngnoreCollision;
var bool bDeleteNow;
var bool bAlwaysVisible;
//#endif

// Flags.
var(Advanced) const bool	bStatic;			// Does not move or change over time. Don't let L.D.s change this - screws up net play
var(Advanced)		bool	bHidden;			// Is hidden during gameplay.
var(Advanced) const bool	bNoDelete;			// Cannot be deleted during play.
var			  const	bool	bDeleteMe;			// About to be deleted.
var transient const bool	bTicked;			// Actor has been updated.
var(Lighting)		bool	bDynamicLight;		// This light is dynamic.
var					bool	bTimerLoop;			// Timer loops (else is one-shot).
var					bool    bOnlyOwnerSee;		// Only owner can see this actor.
var(Advanced)		bool    bHighDetail;		// Only show up in high or super high detail mode.
var(Advanced)		bool	bSuperHighDetail;	// Only show up in super high detail mode.
var					bool	bOnlyDrawIfAttached;	// don't draw this actor if not attached (useful for net clients where attached actors and their bases' replication may not be synched)
var(Advanced)		bool	bStasis;			// In StandAlone games, turn off if not in a recently rendered zone turned off if  bStasis  and physics = PHYS_None or PHYS_Rotating.
var					bool	bTrailerAllowRotation; // If PHYS_Trailer and want independent rotation control.
var					bool	bTrailerSameRotation; // If PHYS_Trailer and true, have same rotation as owner.
var					bool	bTrailerPrePivot;	// If PHYS_Trailer and true, offset from owner by PrePivot.
//#ifdef __L2 //kurt
var					bool	bTrailerNoOwnerDestroy;	// If PHYS_Trailer and true, offset from owner by PrePivot.
//#endif
//#ifdef __L2 //nonblock
var					bool	bRelativeTrail;		// If PHYS_Trailer and true, trails the owner from the distance of RelativeTrailOffset
var					vector	RelativeTrailOffset;
//var					float	RelativeTrailRadius;

var(Rotation)		bool	bSelfRotation;		// If PHYS_Trailer and true, rotates by itself


//#endif
var					bool	bWorldGeometry;		// Collision and Physics treats this actor as world geometry
var(Display)		bool    bAcceptsProjectors;	// Projectors can project onto this actor
var					bool	bOrientOnSlope;		// when landing, orient base on slope of floor
var			  const	bool	bOnlyAffectPawns;	// Optimisation - only test ovelap against pawns. Used for influences etc.
var(Display)		bool	bDisableSorting;	// Manual override for translucent material sorting.
var(Movement)		bool	bIgnoreEncroachers; // Ignore collisions between movers and 

var					bool    bShowOctreeNodes;
var					bool    bWasSNFiltered;      // Mainly for debugging - the way this actor was inserted into Octree.

// Networking flags
var			  const	bool	bNetTemporary;				// Tear-off simulation in network play.
var					bool	bOnlyRelevantToOwner;			// this actor is only relevant to its owner.
var transient const	bool	bNetDirty;					// set when any attribute is assigned a value in unrealscript, reset when the actor is replicated
var					bool	bAlwaysRelevant;			// Always relevant for network.
var					bool	bReplicateInstigator;		// Replicate instigator to client (used by bNetTemporary projectiles).
var					bool	bReplicateMovement;			// if true, replicate movement/location related properties
var					bool	bSkipActorPropertyReplication; // if true, don't replicate actor class variables for this actor
var					bool	bUpdateSimulatedPosition;	// if true, update velocity/location after initialization for simulated proxies
var					bool	bTearOff;					// if true, this actor is no longer replicated to new clients, and 
														// is "torn off" (becomes a ROLE_Authority) on clients to which it was being replicated.
var					bool	bOnlyDirtyReplication;		// if true, only replicate actor if bNetDirty is true - useful if no C++ changed attributes (such as physics) 
														// bOnlyDirtyReplication only used with bAlwaysRelevant actors
var					bool	bReplicateAnimations;		// Should replicate SimAnim
var const           bool    bNetInitialRotation;        // Should replicate initial rotation
var					bool	bCompressedPosition;		// used by networking code to flag compressed position replication
var					bool	bAlwaysZeroBoneOffset;		// if true, offset always zero when attached to skeletalmesh

//#ifdef __L2 // zodiac
var vector				RelativeLocInVehicle;
var int					VehicleID;
var bool				bVehicleTargetMove;
var bool				bVehicleCompensativeMove;
var bool				bHasActorTarget;

var bool				bL2DesiredRotated;
var rotator				L2DesriedRotator;
var bool				L2NeedTick;
var bool				bCheckChangableLevel;

// flagoftiger
var bool				bImmediatelyStop;
// flagoftiger	Spawn되면 true가 된다.
var bool				bNeedPostSpawnProcess;

enum EActorViewType
{
	EAVT_None,
	EAVT_InSight,
	EAVT_InSighting,
	EAVT_OutSight,
	EAVT_OutSighting,
	EAVT_Spawning,
	EAVT_Deleting,
	EAVT_Hiding,
	EAVT_Showing,
	EAVT_Controller,
	EAVT_HighLight,
	EAVT_Despawning,
};

var EActorViewType		L2ActorViewtype;
var float				L2ActorViewDuration;
var float				L2ActorViewElapsedTime;

enum ELodBlendViewType
{
	ELBT_None,
	ELBT_Normal,
	ELBT_Downing,
	ELBT_Upping,
};

var ELodBlendViewType		L2LodViewType;
var float					L2LodViewDuration;
var float					L2LodViewElapsedTime;
var int						L2CurrentLod;
var int						L2ServerObjectRealID;
var(L2ServerObject) int		L2ServerObjectID;
var(L2ServerObject) const enum EL2ObjectType
{
	EL2_NORMAL,
	EL2_DOOR,
	EL2_CHAIR
} L2ServerObjectType;
//#endif

// Net variables.
enum ENetRole
{
	ROLE_None,              // No role at all.
	ROLE_DumbProxy,			// Dumb proxy of this actor.
	ROLE_SimulatedProxy,	// Locally simulated proxy of this actor.
	ROLE_AutonomousProxy,	// Locally autonomous proxy of this actor.
	ROLE_Authority,			// Authoritative control over the actor.
};
var ENetRole RemoteRole, Role;
var const transient int		NetTag;
var float NetUpdateTime;	// time of last update
var float NetUpdateFrequency; // How many seconds between net updates.
var float NetPriority; // Higher priorities means update it more frequently.
var Pawn                  Instigator;    // Pawn responsible for damage caused by this actor.
var const name			AttachmentBone;		// name of bone to which actor is attached (if attached to center of base, =='')
//#ifdef __L2 // kurt
enum	EAttachType
{
	EATP_Relative,
	EATP_Absolute
};
var		  EAttachType		AttachType;
//#endif

var       const LevelInfo Level;         // Level this actor is on.
var transient const Level	XLevel;			// Level object.
var(Advanced)	float		LifeSpan;		// How old the object lives before dying, 0=forever.

//-----------------------------------------------------------------------------
// Structures.

// Identifies a unique convex volume in the world.
struct PointRegion
{
	var zoneinfo Zone;       // Zone.
	var int      iLeaf;      // Bsp leaf.
	var byte     ZoneNumber; // Zone number.
};

//-----------------------------------------------------------------------------
// Major actor properties.

// Scriptable.
var(Advanced) const PointRegion     Region;        // Region this actor is in.
//#ifdef __L2 Hunter
var(Advanced) const name	ForcedRegionTag;
var(Advanced) const int		ForcedRegion;
//#endif
var				float       TimerRate;		// Timer event, 0=no timer.

var(Display)  mesh		Mesh;			// Mesh if DrawType=DT_Mesh.

var transient float		LastRenderTime;	// last time this actor was rendered.
var(Events) name			Tag;			// Actor's tag name.
var transient array<int>	Leaves;		 // BSP leaves this actor is in.
var(Events) name			Event;         // The event this actor causes.
var(Events)	array<L2Event>	L2GameEvent;
var Inventory				Inventory;     // Inventory chain.
var		const	float       TimerCounter;	// Counts up until it reaches TimerRate.
var transient MeshInstance MeshInstance;	// Mesh instance.

// #ifdef __L2 // zodiac
var(Events) name			L2MoveEvent;
//#endif

// #ifdef __L2  // kurt
var(ZoneLight)		bool   bSunAffect;	// There is Sun effect in this zone.
// #endif

var enum ETargetSpineStatus
{
	TSR_None,
	TSR_START,
	TSR_END
} TargetSpineStatus;

var(Display) float		  LODBias;
var(Object) name InitialState;
var(Object) name Group;

// Internal.
//#ifdef	__L2	Hunter
var const array<Actor>    Child;		 // List of Child actors.
//#endif
var const array<Actor>    Touching;		 // List of touching actors.
var const transient array<int>  OctreeNodes;// Array of nodes of the octree Actor is currently in. Internal use only.
var const transient Box	  OctreeBox;     // Actor bounding box cached when added to Octree. Internal use only.
var const transient vector OctreeBoxCenter;
var const transient vector OctreeBoxRadii;
var const actor           Deleted;       // Next actor in just-deleted chain.
var const float           LatentFloat;   // Internal latent function use.

// Internal tags.
var const native int CollisionTag;
var const transient int JoinedTag;

// The actor's position and rotation.
var const	PhysicsVolume	PhysicsVolume;	// physics volume this actor is currently in
var(Movement) const vector	Location;		// Actor's location; use Move to set.
var(Movement) const rotator Rotation;		// Rotation.
var(Movement) vector		Velocity;		// Velocity.
var			  vector        Acceleration;	// Acceleration.

// Attachment related variables
var(Movement)	name	AttachTag;
var const array<Actor>  Attached;			// array of actors attached to this actor.
var const vector		RelativeLocation;	// location relative to base/bone (valid if base exists)
var const rotator		RelativeRotation;	// rotation relative to base/bone (valid if base exists)

var(Movement) const bool bHardAttach;       // Uses 'hard' attachment code. bBlockActor and bBlockPlayer must also be false.
											// This actor cannot then move relative to base (setlocation etc.).
											// Dont set while currently based on something!
											// 
var const     Matrix    HardRelMatrix;		// Transform of actor in base's ref frame. Doesn't change after SetBase.

// Projectors
struct ProjectorRenderInfoPtr { var int Ptr; };	// Hack to to fool C++ header generation...
struct StaticMeshProjectorRenderInfoPtr { var int Ptr; };
var const native array<ProjectorRenderInfoPtr> Projectors;// Projected textures on this actor
var const native array<StaticMeshProjectorRenderInfoPtr>	StaticMeshProjectors;

//-----------------------------------------------------------------------------
// Display properties.

var(Display) Material		Texture;			// Sprite texture.if DrawType=DT_Sprite
var StaticMeshInstance		StaticMeshInstance; // Contains per-instance static mesh data, like static lighting data.
var const export model		Brush;				// Brush if DrawType=DT_Brush.
var(Display) const float	DrawScale;			// Scaling factor, 1.0=normal size.
var(Display) const vector	DrawScale3D;		// Scaling vector, (1.0,1.0,1.0)=normal size.
var(Display) vector			PrePivot;			// Offset from box center for drawing.
var(Display) array<Material> Skins;				// Multiple skin support - not replicated.
var	Material				OverlayMaterial; 
var	float					OverlayTimer;     
var color					OverlayColor;

var			Material		RepSkin;			// replicated skin (sets Skins[0] if not none)
var(Display) byte			AmbientGlow;		// Ambient brightness, or 255=pulsing.
var(Display) byte           MaxLights;          // Limit to hardware lights active on this primitive.
var(Display) ConvexVolume	AntiPortal;			// Convex volume used for DT_AntiPortal
var(Display) float          CullDistance;
var(Display) float			ScaleGlow;

//----------------------------------------------------------------------------
// move __L2 kurt
struct NMoverPtr
{
	var int Ptr;
};
var NMoverPtr NMoverActor;

var L2NMover L2NMover;

// __L2 gigadeth
var transient const Level SWXLevel;        // Level object.

// __L2 gigadeth
var rotator SwayRotationOrig;

// DS_SHADER
var(Advanced) bool bDontBatch; // Prevent this actor from being batched 
// end DS_SHADER

// Style for rendering sprites, meshes.
var(Display) enum ERenderStyle
{
	STY_None,
	STY_Normal,
	STY_Masked,
	STY_Translucent,
	STY_Modulated,
	STY_Alpha,
	STY_Additive,
	STY_Subtractive,
	STY_Particle,
	STY_AlphaZ,
} Style;

// #ifdef __L2 Hunter
enum EPawnSubMeshStyle
{
	PMS_Boots,
	PMS_LowBody,
	PMS_UpBody,
    PMS_Glove,
	PMS_Hair2,
	PMS_Hair1,
	PMS_Helm,
	PMS_HairAcce1,
	PMS_HairAcce2,
	PMS_Face,
	PMS_RightHand,
	PMS_LeftHand,
	PMS_RightArm,
	PMS_LeftArm,
	PMS_Cloak,
	PMS_CloakCover
};

enum EL2GameAction
{
	LGA_SaintConsensusOpening,
	LGA_SaintConsensusOpened,
	LGA_SaintConsensusClosing,
	LGA_SaintConsensusClosed
};
//#endif

// Display.
var(Display)  bool      bUnlit;					// Lights don't affect actor.
var(Display)  bool      bShadowCast;			// Casts static shadows.
var(Display)  bool		bStaticLighting;		// Uses raytraced lighting.
var(Display)  bool		bUseLightingFromBase;	// Use Unlit/AmbientGlow from Base
// #ifdef __L2 kurt 2002/01/27
var(Display)  bool		bIgnoredRange;			// Always display
var(Display)  bool		bUnlitCheck;			// Light line check
var			  bool		bCulledSunlight;
// #endif

// Advanced.
var			  bool		bHurtEntry;				// keep HurtRadius from being reentrant
var(Advanced) bool		bGameRelevant;			// Always relevant for game
var(Advanced) bool		bCollideWhenPlacing;	// This actor collides with the world when placing.
var			  bool		bTravel;				// Actor is capable of travelling among servers.
var(Advanced) bool		bMovable;				// Actor can be moved.
var			  bool		bDestroyInPainVolume;	// destroy this actor if it enters a pain volume
var(Advanced) bool		bShouldBaseAtStartup;	// if true, find base for this actor at level startup, if collides with world and PHYS_None or PHYS_Rotating
var			  bool		bPendingDelete;			// set when actor is about to be deleted (since endstate and other functions called 
												// during deletion process before bDeleteMe is set).
var					bool	bAnimByOwner;		// Animation dictated by owner.
var 				bool	bOwnerNoSee;		// Everything but the owner can see this actor.
var(Advanced)		bool	bCanTeleport;		// This actor can be teleported.
var					bool	bClientAnim;		// Don't replicate any animations - animation done client-side
var					bool    bDisturbFluidSurface; // Cause ripples when in contact with FluidSurface.
var			  const	bool	bAlwaysTick;		// Update even when players-only.

//-----------------------------------------------------------------------------
// Sound.

// Sound slots for actors.
enum ESoundSlot
{
	SLOT_None,
	SLOT_Misc,
	SLOT_Pain,
	SLOT_Interact,
	SLOT_Ambient,
	SLOT_Talk,
	SLOT_Interface,
};

// Music transitions.
enum EMusicTransition
{
	MTRAN_None,
	MTRAN_Instant,
	MTRAN_Segue,
	MTRAN_Fade,
	MTRAN_FastFade,
	MTRAN_SlowFade,
};

// Regular sounds.
var(Sound) float TransientSoundVolume;	// default sound volume for regular sounds (can be overridden in playsound)
var(Sound) float TransientSoundRadius;	// default sound radius for regular sounds (can be overridden in playsound)

//-----------------------------------------------------------------------------
// Collision.

// Collision size.
var(Collision) const float CollisionRadius;		// Radius of collision cyllinder.
var(Collision) const float CollisionHeight;		// Half-height cyllinder.

// Collision flags.
var(Collision) const bool bCollideActors;		// Collides with other actors.
var(Collision) bool       bCollideWorld;		// Collides with the world.
var(Collision) bool       bBlockActors;			// Blocks other nonplayer actors.
var(Collision) bool       bBlockPlayers;		// Blocks other player actors.
var(Collision) bool       bProjTarget;			// Projectiles should potentially target this actor.
var(Collision) bool		  bBlockZeroExtentTraces; // block zero extent actors/traces
var(Collision) bool		  bBlockNonZeroExtentTraces;	// block non-zero extent actors/traces
var(Collision) bool       bAutoAlignToTerrain;  // Auto-align to terrain in the editor
var(Collision) bool		  bUseCylinderCollision;// Force axis aligned cylinder collision (useful for static mesh pickups, etc.)
var(Collision) const bool bBlockKarma;			// Block actors being simulated with Karma.
var       			bool    bNetNotify;                 // actor wishes to be notified of replication events

//-----------------------------------------------------------------------------
// Physics.

// Options.
var			  bool		  bIgnoreOutOfWorld; // Don't destroy if enters zone zero
var(Movement) bool        bBounce;           // Bounces when hits ground fast.
var(Movement) bool		  bFixedRotationDir; // Fixed direction of rotation.
var(Movement) bool		  bRotateToDesired;  // Rotate to DesiredRotation.
var           bool        bInterpolating;    // Performing interpolating.
var			  const bool  bJustTeleported;   // Used by engine physics - not valid for scripts.

// Physics properties.
var(Movement) float       Mass;				// Mass of this actor.
var(Movement) float       Buoyancy;			// Water buoyancy.
var(Movement) rotator	  RotationRate;		// Change in rotation per second.
//#ifdef	__L2 Hunter
var			  rotator	  KayboardRotationRate;		// Change in Kayboard rotation per second.
// flagoftiger
var			  int	      KeyboardRotationYawFromServer;	// 서버에서 주는 로테이션속도
var(Movement) rotator	  RotationLimit;	// Pitch, Roll Limit
//#endif
var(Movement) rotator     DesiredRotation;	// Physics will smoothly rotate actor to this rotation if bRotateToDesired.
var			  Actor		  PendingTouch;		// Actor touched during move which wants to add an effect after the movement completes 
var       const vector    ColLocation;		// Actor's old location one move ago. Only for debugging

//#ifdef	__L2 Hunter
const MAXSTEPHEIGHT = 10.0; // Maximum step height walkable by pawns
//#else
//const MAXSTEPHEIGHT = 35.0; // Maximum step height walkable by pawns
//#endif
const MINFLOORZ = 0.7; // minimum z value for floor normal (if less, not a walkable floor)
					   // 0.7 ~= 45 degree angle for floor
// ifdef WITH_KARMA

// Used to avoid compression
struct KRBVec
{
	var float	X, Y, Z;
};

struct KRigidBodyState
{
	var KRBVec	Position;
	var Quat	Quaternion;
	var KRBVec	LinVel;
	var KRBVec	AngVel;
};
					   
var(Karma) export editinline KarmaParamsCollision KParams; // Parameters for Karma Collision/Dynamics.
var const native int KStepTag;

// endif

//-----------------------------------------------------------------------------
// Animation replication (can be used to replicate channel 0 anims for dumb proxies)
struct AnimRep
{
	var name AnimSequence; 
	var bool bAnimLoop;	
	var byte AnimRate;		// note that with compression, max replicated animrate is 4.0
	var byte AnimFrame;
	var byte TweenRate;		// note that with compression, max replicated tweentime is 4 seconds
};
var transient AnimRep		  SimAnim;		   // only replicated if bReplicateAnimations is true

//-----------------------------------------------------------------------------
// Forces.

enum EForceType
{
	FT_None,
	FT_DragAlong,
};

var (Force) EForceType	ForceType;
var (Force)	float		ForceRadius;
var (Force) float		ForceScale;


//-----------------------------------------------------------------------------
// Networking.

// Symmetric network flags, valid during replication only.
var const bool bNetInitial;       // Initial network update.
var const bool bNetOwner;         // Player owns this actor.
var const bool bNetRelevant;      // Actor is currently relevant. Only valid server side, only when replicating variables.
var const bool bDemoRecording;	  // True we are currently demo recording
var const bool bClientDemoRecording;// True we are currently recording a client-side demo
var const bool bClientDemoNetFunc;// True if we're client-side demo recording and this call originated from the remote.
var bool	   bNoRepMesh;			// don't replicate mesh

//Editing flags
var(Advanced) bool        bHiddenEd;     // Is hidden during editing.
var(Advanced) bool        bHiddenEdGroup;// Is hidden by the group brower.
var(Advanced) bool        bDirectional;  // Actor shows direction arrow during editing.
var const bool            bSelected;     // Selected in UnrealEd.
var(Advanced) bool        bEdShouldSnap; // Snap to grid in editor.
var transient bool        bEdSnap;       // Should snap to grid in UnrealEd.
var transient const bool  bTempEditor;   // Internal UnrealEd.
var	bool				  bObsolete;	 // actor is obsolete - warn level designers to remove it
var(Collision) bool		  bPathColliding;// this actor should collide (if bWorldGeometry && bBlockActors is true) during path building (ignored if bStatic is true, as actor will always collide during path building)
var transient bool		  bPathTemp;	 // Internal/path building
var	bool				  bScriptInitialized; // set to prevent re-initializing of actors spawned during level startup
var(Advanced) bool        bLockLocation; // Prevent the actor from being moved in the editor.
//#ifdef __L2	Hunter
var(Advanced) bool        bLockUndelete;	// Prevent the actor from being deleted and being moved in the editor.
//#endif



var class<LocalMessage> MessageClass;

// #ifdef __L2 kurt
//-----------------------------------------------------------------------------
// NEffects.
enum ENMagicAniStatus
{
	NMAS_None,
	NMAS_RotationTo,
	NMAS_CastStart,              
	NMAS_Cast,
	NMAS_CastEnd,
	NMAS_ShotStart,
	NMAS_Shot,
	NMAS_ShotEnd
};

struct NMagicInfo
{
	var SkillVisualEffect Agent;
	var int		DummyPtr;	// 

	var int		MagicID; 
	var int 	LevelID;	
	var int		AniIndex;
	var int		FlexibleAniIndex;
	var int		StageShot;
	var int		StagePreshot;

	var float	SkillHitTime;	
	//var float 	ShotStartTime;	
	var float	ShotTime;		
	//var float	ProgressTime;	
	var	float	TweenTime;
	var float 	ActiveTime;		

	var Actor	TargetPawn;	
	var float	MagicSpeed;
	var int		MagicAniStatus;

	//var name	CastAnimName;
	//var name	ProgressAnimName;
	//var name	ShotAnimName;
	
	var int		MagicType;
	var bool	bTargetExcepted;
	var int		nSlice;

	var rotator NormalRotationRate;
	var rotator SkillRotationRate;

	var transient array<Actor>  AssociatedActor;
	var transient array<Actor>  EffectActor;
	var transient array<INT>    EffectID;
	var transient array<vector> LocLIst;

	var name Anis[3];
	var	float AniDues[3];
	var name LastShotName;
	var int		PendingNotify;				// = 0 | EPAN_Shot | EPAN_FinalShot
	var	int		PendingPreshotNotify;		// = 0 | EPAN_Shot
// #ifdef __L2 // by anima - for channeling skill effect
	var int		PendingChannelingNotify;	// = 0 | EPAN_Shot	
// #endif
};

var transient array<NSkillProjectile> NSkillProjectileActor;
var transient array<Actor>  SpelledNEffectActor;
var transient array<Actor>  NProjectileActor;



//-----------------------------------------------------------------------------
// AttackProcess.
enum ENAttackStatus
{
	NAST_None,
	NAST_AttackStart,
	NAST_AttackDelay,
	NAST_AttackShot,
	NAST_AttackDemage,
	NAST_AttackEnd
};
var ENAttackStatus NAttackStatus;
var Actor			EffectOwner;

var(Movement) enum ESpawnPos
{
	SPN_Normal,
	SPN_Top,
	SPN_Mid,
	SPN_Btm
} SpawnPos;

//#endif

//#ifdef __L2 // zodiac
struct TextureModifyinfo
{
	var bool bUseModify,
			bTwoSide,
			bAlphaBlend,
			bDummy;
	var color Color;
	var int AlphaOp,
			ColorOp;
};

var TextureModifyinfo TexModifyInfo;
//#endif

//-----------------------------------------------------------------------------
// Enums.

// Travelling from server to server.
enum ETravelType
{
	TRAVEL_Absolute,	// Absolute URL.
	TRAVEL_Partial,		// Partial (carry name, reset server).
	TRAVEL_Relative,	// Relative URL.
};


// double click move direction.
enum EDoubleClickDir
{
	DCLICK_None,
	DCLICK_Left,
	DCLICK_Right,
	DCLICK_Forward,
	DCLICK_Back,
	DCLICK_Active,
	DCLICK_Done
};

enum eKillZType
{
	KILLZ_None,
	KILLZ_Lava,
	KILLZ_Suicide
};

enum EActorEffectType
{
	EAST_None,
	EAST_Silhouette,
	EAST_CubemapSilhouette,
	EAST_WeaponSilhouette,
};

var EActorEffectType		L2ActorEffecttype;

//for normal_bump mapping
var transient bool bTangentLoad;
var	transient int nUseNormalmap;

//-----------------------------------------------------------------------------
// natives.

// Execute a console command in the context of the current level and game engine.
native function string ConsoleCommand( string Command );
// ifdef __L2 // zodiac
native function viewport GetViewport();
// endif

//=============================================================================
// Actor error handling.

// Handle an error and kill this one actor.
native(233) final function Error( coerce string S );

//=============================================================================
// General functions.

// Latent functions.
native(256) final latent function Sleep( float Seconds );

// Collision.
native(262) final function SetCollision( optional bool NewColActors, optional bool NewBlockActors, optional bool NewBlockPlayers );
native(283) final function bool SetCollisionSize( float NewRadius, float NewHeight );
native final function SetDrawScale(float NewScale);
native final function SetDrawScale3D(vector NewScale3D);
native final function SetStaticMesh(StaticMesh NewStaticMesh);
native final function SetDrawType(EDrawType NewDrawType);

// Movement.
native(266) final function bool Move( vector Delta );
native(267) final function bool SetLocation( vector NewLocation );
native(299) final function bool SetRotation( rotator NewRotation );

// SetRelativeRotation() sets the rotation relative to the actor's base
native final function bool SetRelativeRotation( rotator NewRotation );
native final function bool SetRelativeLocation( vector NewLocation );

native(3969) final function bool MoveSmooth( vector Delta );
native(3971) final function AutonomousPhysics(float DeltaSeconds);

// Relations.
native(298) final function SetBase( actor NewBase, optional vector NewFloor );
native(272) final function SetOwner( actor NewOwner );

//=============================================================================
// Animation.

native final function string GetMeshName();

// Animation functions.
native(259) final function PlayAnim( name Sequence, optional float Rate, optional float TweenTime, optional int Channel );
native(260) final function LoopAnim( name Sequence, optional float Rate, optional float TweenTime, optional int Channel );
native(294) final function TweenAnim( name Sequence, float Time, optional int Channel );
native(282) final function bool IsAnimating(optional int Channel);
native(261) final latent function FinishAnim(optional int Channel);
native(263) final function bool HasAnim( name Sequence );
native final function StopAnimating( optional bool ClearAllButBase );
//#ifdef __L2 //kurt
native final function bool HasBoneName( name bonename);
//#endif
native final function FreezeAnimAt( float Time, optional int Channel);
native final function SetAnimFrame( float Time, optional int Channel, optional int UnitFlag );

native final function bool IsTweening(int Channel);

//#ifdef	__L2	hunter
event L2GameActionNotify( EL2GameAction GameAction );
event ClearL2Game();
//#endif
// Animation notifications.
event AnimEnd( int Channel );
native final function EnableChannelNotify ( int Channel, int Switch );
native final function int GetNotifyChannel();

// Skeletal animation.
simulated native final function LinkSkelAnim( MeshAnimation Anim, optional mesh NewMesh );
simulated native final function LinkMesh( mesh NewMesh, optional bool bKeepAnim );
native final function BoneRefresh();

native final function AnimBlendParams( int Stage, optional float BlendAlpha, optional float InTime, optional float OutTime, optional name BoneName, optional bool bGlobalPose);
native final function AnimBlendToAlpha( int Stage, float TargetAlpha, float TimeInterval );

native final function coords  GetBoneCoords(   name BoneName );
native final function rotator GetBoneRotation( name BoneName, optional int Space );
//#ifdef __L2 //kurt
native final function coords  GetBoneCoordsWithBoneIndex(   int BoneIndex );
//#endif

native final function vector  GetRootLocation();
native final function rotator GetRootRotation();
native final function vector  GetRootLocationDelta();
native final function rotator GetRootRotationDelta();

native final function bool  AttachToBone( actor Attachment, name BoneName );
native final function bool  DetachFromBone( actor Attachment );
//#ifdef __L2 //kurt
native final function bool  AttachToBoneWithIndex( actor Attachment, int BoneIndex );
//#endif

native final function LockRootMotion( int Lock );
native final function SetBoneScale( int Slot, optional float BoneScale, optional name BoneName );

native final function SetBoneDirection( name BoneName, rotator BoneTurn, optional vector BoneTrans, optional float Alpha, optional int Space );
native final function SetBoneLocation( name BoneName, optional vector BoneTrans, optional float Alpha );
native final function SetBoneRotation( name BoneName, optional rotator BoneTurn, optional int Space, optional float Alpha );
native final function GetAnimParams( int Channel, out name OutSeqName, out float OutAnimFrame, out float OutAnimRate );
native final function bool AnimIsInGroup( int Channel, name GroupName );  

//=========================================================================
// Rendering.

native final function plane GetRenderBoundingSphere();
native final function DrawDebugLine( vector LineStart, vector LineEnd, byte R, byte G, byte B); // SLOW! Use for debugging only!

//=========================================================================
// Physics.

native final function DebugClock();
native final function DebugUnclock();

// Physics control.
native(301) final latent function FinishInterpolation();
native(3970) final function SetPhysics( EPhysics newPhysics );

native final function OnlyAffectPawns(bool B);

// ifdef WITH_KARMA
native final function quat KGetRBQuaternion();

native final function KGetRigidBodyState(out KRigidBodyState RBstate);
native final function KDrawRigidBodyState(KRigidBodyState RBState, bool AltColour); // SLOW! Use for debugging only!
native final function vector KRBVecToVector(KRBVec RBvec);
native final function KRBVec KRBVecFromVector(vector v);

native final function KSetMass( float mass );
native final function float KGetMass();

// Set inertia tensor assuming a mass of 1. Scaled by mass internally to calculate actual inertia tensor.
native final function KSetInertiaTensor( vector it1, vector it2 );
native final function KGetInertiaTensor( out vector it1, out vector it2 );

native final function KSetDampingProps( float lindamp, float angdamp );
native final function KGetDampingProps( out float lindamp, out float angdamp );

native final function KSetFriction( float friction );
native final function float KGetFriction();

native final function KSetRestitution( float rest );
native final function float KGetRestitution();

native final function KSetCOMOffset( vector offset );
native final function KGetCOMOffset( out vector offset );
native final function KGetCOMPosition( out vector pos ); // get actual position of actors COM in world space

native final function KSetImpactThreshold( float thresh );
native final function float KGetImpactThreshold();

native final function KWake();
native final function bool KIsAwake();
native final function KAddImpulse( vector Impulse, vector Position, optional name BoneName );

native final function KSetStayUpright( bool stayUpright, bool allowRotate );

native final function KSetBlockKarma( bool newBlock );

native final function KSetActorGravScale( float ActorGravScale );
native final function float KGetActorGravScale();

// Disable/Enable Karma contact generation between this actor, and another actor.
// Collision is on by default.
native final function KDisableCollision( actor Other );
native final function KEnableCollision( actor Other );

// Ragdoll-specific functions
native final function KSetSkelVel( vector Velocity, optional vector AngVelocity, optional bool AddToCurrent );
native final function float KGetSkelMass();
native final function KFreezeRagdoll();

// You MUST turn collision off (KSetBlockKarma) before using bone lifters!
native final function KAddBoneLifter( name BoneName, InterpCurve LiftVel, float LateralFriction, InterpCurve Softness ); 
native final function KRemoveLifterFromBone( name BoneName ); 
native final function KRemoveAllBoneLifters(); 

// Used for only allowing a fixed maximum number of ragdolls in action.
native final function KMakeRagdollAvailable();
native final function bool KIsRagdollAvailable();

// event called when Karmic actor hits with impact velocity over KImpactThreshold
event KImpact(actor other, vector pos, vector impactVel, vector impactNorm); 

// event called when karma actor's velocity drops below KVelDropBelowThreshold;
event KVelDropBelow();

// event called when a ragdoll convulses (see KarmaParamsSkel)
event KSkelConvulse();

// event called just before sim to allow user to 
// NOTE: you should ONLY put numbers into Force and Torque during this event!!!!
event KApplyForce(out vector Force, out vector Torque);

// This is called from inside C++ physKarma at the appropriate time to update state of Karma rigid body.
// If you return true, newState will be set into the rigid body. Return false and it will do nothing.
event bool KUpdateState(out KRigidBodyState newState);

// endif

// Timing
native final function Clock(out float time);
native final function UnClock(out float time);

//=========================================================================
// Music

native final function int PlayMusic( string Song, float FadeInTime );
native final function StopMusic( int SongHandle, float FadeOutTime );
native final function StopAllMusic( float FadeOutTime );

// ifdef __L2 kurt
native final function bool NDestroy();
// endif

//=========================================================================
// Engine notification functions.

//
// Major notifications.
//
event Destroyed();
event GainedChild( Actor Other );
event LostChild( Actor Other );
event Tick( float DeltaTime );
event PostNetReceive(); 

//
// Triggers.
//
event Trigger( Actor Other, Pawn EventInstigator );
event UnTrigger( Actor Other, Pawn EventInstigator );
event BeginEvent();
event EndEvent();

//
// Physics & world interaction.
//
event Timer();
event HitWall( vector HitNormal, actor HitWall );
event Falling();
event Landed( vector HitNormal );
event ZoneChange( ZoneInfo NewZone );
event PhysicsVolumeChange( PhysicsVolume NewVolume );
event Touch( Actor Other );
event PostTouch( Actor Other ); // called for PendingTouch actor after physics completes
event UnTouch( Actor Other );
event Bump( Actor Other );
event BaseChange();
event Attach( Actor Other );
event Detach( Actor Other );
event Actor SpecialHandling(Pawn Other);
event bool EncroachingOn( actor Other );
event EncroachedBy( actor Other );
event FinishedInterpolation()
{
	bInterpolating = false;
}

event EndedRotation();			// called when rotation completes
event UsedBy( Pawn user ); // called if this Actor was touching a Pawn who pressed Use

simulated event FellOutOfWorld(eKillZType KillType)
{
	SetPhysics(PHYS_None);
	Destroy();
}	

//
// Damage and kills.
//
event KilledBy( pawn EventInstigator );
event TakeDamage( int Damage, Pawn EventInstigator, vector HitLocation, vector Momentum, class<DamageType> DamageType);

//
// Trace a line and see what it collides with first.
// Takes this actor's collision properties into account.
// Returns first hit actor, Level if hit level, or None if hit nothing.
//
native(277) final function Actor Trace
(
	out vector      HitLocation,
	out vector      HitNormal,
	vector          TraceEnd,
	optional vector TraceStart,
	optional bool   bTraceActors,
	optional vector Extent,
	optional out material Material
);

// returns true if did not hit world geometry
native(548) final function bool FastTrace
(
	vector          TraceEnd,
	optional vector TraceStart
);

//
// Spawn an actor. Returns an actor of the specified class, not
// of class Actor (this is hardcoded in the compiler). Returns None
// if the actor could not be spawned (either the actor wouldn't fit in
// the specified location, or the actor list is full).
// Defaults to spawning at the spawner's location.
//
native(278) final function actor Spawn
(
	class<actor>      SpawnClass,
	optional actor	  SpawnOwner,
	optional name     SpawnTag,
	optional vector   SpawnLocation,
	optional rotator  SpawnRotation,
//#ifdef	__L2	Hunter
	optional bool	  bNoCollisionCheck
//#endif
);

//
// Destroy this actor. Returns true if destroyed, false if indestructable.
// Destruction is latent. It occurs at the end of the tick.
//
native(279) final function bool Destroy();

// Networking - called on client when actor is torn off (bTearOff==true)
event TornOff();

//=============================================================================
// Timing.

// Causes Timer() events every NewTimerRate seconds.
native(280) final function SetTimer( float NewTimerRate, bool bLoop );

//=============================================================================
// Sound functions.

/* Play a sound effect.
*/
native(264) final function PlaySound
(
	sound				Sound,
	optional ESoundSlot Slot,
	optional float		Volume,
	optional bool		bNoOverride,
	optional float		Radius,
	optional float		Pitch,
	optional bool		Attenuate
);

/* play a sound effect, but don't propagate to a remote owner
 (he is playing the sound clientside)
 */
native simulated final function PlayOwnedSound
(
	sound				Sound,
	optional ESoundSlot Slot,
	optional float		Volume,
	optional bool		bNoOverride,
	optional float		Radius,
	optional float		Pitch,
	optional bool		Attenuate
);

native simulated event DemoPlaySound
(
	sound				Sound,
	optional ESoundSlot Slot,
	optional float		Volume,
	optional bool		bNoOverride,
	optional float		Radius,
	optional float		Pitch,
	optional bool		Attenuate
);

//#ifdef __L2 // zodiac
// Vehicle에서의 무지향성 소리를 위해서 넣는다.
native function PlaySoundOnVehicle
(
	sound				Sound,
	optional ESoundSlot Slot,
	optional float		Volume,
	optional bool		bNoOverride,
	optional float		Radius,
	optional float		Pitch,
	optional bool		Attenuate
);
//#endif

/* Get a sound duration.
*/
native final function float GetSoundDuration( sound Sound );

//=============================================================================
// AI functions.

/* Inform other creatures that you've made a noise
 they might hear (they are sent a HearNoise message)
 Senders of MakeNoise should have an instigator if they are not pawns.
*/
native(512) final function MakeNoise( float Loudness );

/* PlayerCanSeeMe returns true if any player (server) or the local player (standalone
or client) has a line of sight to actor's location.
*/
native(532) final function bool PlayerCanSeeMe();

native final function vector SuggestFallVelocity(vector Destination, vector Start, float MaxZ, float MaxXYSpeed);
 
//=============================================================================
// Regular engine functions.

// Teleportation.
event bool PreTeleport( Teleporter InTeleporter );
event PostTeleport( Teleporter OutTeleporter );

// Level state.
event BeginPlay();

//========================================================================
// Disk access.

// Find files.
native(539) final function string GetMapName( string NameEnding, string MapName, int Dir );
native(545) final function GetNextSkin( string Prefix, string CurrentSkin, int Dir, out string SkinName, out string SkinDesc );
native(547) final function string GetURLMap();
native final function string GetNextInt( string ClassName, int Num );
native final function GetNextIntDesc( string ClassName, int Num, out string Entry, out string Description );
native final function bool GetCacheEntry( int Num, out string GUID, out string Filename );
native final function bool MoveCacheEntry( string GUID, optional string NewFilename );  

//=============================================================================
// Iterator functions.

// Iterator functions for dealing with sets of actors.

/* AllActors() - avoid using AllActors() too often as it iterates through the whole actor list and is therefore slow
*/
native(304) final iterator function AllActors     ( class<actor> BaseClass, out actor Actor, optional name MatchTag );

/* DynamicActors() only iterates through the non-static actors on the list (still relatively slow, bu
 much better than AllActors).  This should be used in most cases and replaces AllActors in most of 
 Epic's game code. 
*/
native(313) final iterator function DynamicActors     ( class<actor> BaseClass, out actor Actor, optional name MatchTag );

/* ChildActors() returns all actors owned by this actor.  Slow like AllActors()
*/
native(305) final iterator function ChildActors   ( class<actor> BaseClass, out actor Actor );

/* BasedActors() returns all actors based on the current actor (slow, like AllActors)
*/
native(306) final iterator function BasedActors   ( class<actor> BaseClass, out actor Actor );

/* TouchingActors() returns all actors touching the current actor (fast)
*/
native(307) final iterator function TouchingActors( class<actor> BaseClass, out actor Actor );

/* TraceActors() return all actors along a traced line.  Reasonably fast (like any trace)
*/
native(309) final iterator function TraceActors   ( class<actor> BaseClass, out actor Actor, out vector HitLoc, out vector HitNorm, vector End, optional vector Start, optional vector Extent );

/* RadiusActors() returns all actors within a give radius.  Slow like AllActors().  Use CollidingActors() or VisibleCollidingActors() instead if desired actor types are visible
(not bHidden) and in the collision hash (bCollideActors is true)
*/
native(310) final iterator function RadiusActors  ( class<actor> BaseClass, out actor Actor, float Radius, optional vector Loc );

/* VisibleActors() returns all visible actors within a radius.  Slow like AllActors().  Use VisibleCollidingActors() instead if desired actor types are 
in the collision hash (bCollideActors is true)
*/
native(311) final iterator function VisibleActors ( class<actor> BaseClass, out actor Actor, optional float Radius, optional vector Loc );

/* VisibleCollidingActors() returns visible (not bHidden) colliding (bCollideActors==true) actors within a certain radius.
Much faster than AllActors() since it uses the collision hash
*/
native(312) final iterator function VisibleCollidingActors ( class<actor> BaseClass, out actor Actor, float Radius, optional vector Loc, optional bool bIgnoreHidden );

/* CollidingActors() returns colliding (bCollideActors==true) actors within a certain radius.
Much faster than AllActors() for reasonably small radii since it uses the collision hash
*/
native(321) final iterator function CollidingActors ( class<actor> BaseClass, out actor Actor, float Radius, optional vector Loc );

//=============================================================================
// Color functions
native(549) static final operator(20) color -     ( color A, color B );
native(550) static final operator(16) color *     ( float A, color B );
native(551) static final operator(20) color +     ( color A, color B );
native(552) static final operator(16) color *     ( color A, float B );

//=============================================================================
// Scripted Actor functions.

/* RenderOverlays()
called by player's hud to request drawing of actor specific overlays onto canvas
*/
function RenderOverlays(Canvas Canvas);

// RenderTexture
event RenderTexture(ScriptedTexture Tex);

//
// Called immediately before gameplay begins.
//
event PreBeginPlay()
{
	// Handle autodestruction if desired.
	// Added NULL check - NeverDie
	if( !bGameRelevant
		&& (Level.NetMode != NM_Client)
		&& ( None != Level.Game && None != Level.Game.BaseMutator && !Level.Game.BaseMutator.CheckRelevance(Self) )
		)
		Destroy();
}

//
// Broadcast a localized message to all players.
// Most message deal with 0 to 2 related PRIs.
// The LocalMessage class defines how the PRI's and optional actor are used.
//
event BroadcastLocalizedMessage( class<LocalMessage> MessageClass, optional int Switch, optional PlayerReplicationInfo RelatedPRI_1, optional PlayerReplicationInfo RelatedPRI_2, optional Object OptionalObject )
{
	Level.Game.BroadcastLocalized( self, MessageClass, Switch, RelatedPRI_1, RelatedPRI_2, OptionalObject );
}

// Called immediately after gameplay begins.
//
event PostBeginPlay();

// Called after PostBeginPlay.
//
simulated event SetInitialState()
{
	bScriptInitialized = true;
	if( InitialState!='' )
		GotoState( InitialState );
	else
		GotoState( 'Auto' );
}

// called after PostBeginPlay.  On a net client, PostNetBeginPlay() is spawned after replicated variables have been initialized to
// their replicated values
event PostNetBeginPlay();

// NPC의 경우 Mesh 가 SetPawnResource 후에 생기는데
// 기존에 PostBeginPlay() 에서 이펙트를 본에 Attach를 시키면 Mesh가 없기 때문에 Pawn의 중심에 이펙트가 붙는 현상이 생겼다.
// 기획팀에서 uc 코드만으로 이펙트를 알아서 붙일 수 있게 하기 위해서 이 함수를 만들었다. - by idearain [2006/11/06]
event PostSetPawnResource();

simulated function UpdatePrecacheMaterials()
{
	local int i;
	
	for ( i=0; i<Skins.Length; i++ )
		Level.AddPrecacheMaterial(Skins[i]);
}

simulated function UpdatePrecacheStaticMeshes()
{
	if ( (DrawType == DT_StaticMesh) && !bStatic && !bNoDelete )
		Level.AddPrecacheStaticMesh(StaticMesh);
}

/* HurtRadius()
 Hurt locally authoritative actors within the radius.
*/
simulated final function HurtRadius( float DamageAmount, float DamageRadius, class<DamageType> DamageType, float Momentum, vector HitLocation )
{
	local actor Victims;
	local float damageScale, dist;
	local vector dir;
	
	if( bHurtEntry )
		return;

	bHurtEntry = true;
	foreach VisibleCollidingActors( class 'Actor', Victims, DamageRadius, HitLocation )
	{
		// don't let blast damage affect fluid - VisibleCollisingActors doesn't really work for them - jag
		if( (Victims != self) && (Victims.Role == ROLE_Authority) && (!Victims.IsA('FluidSurfaceInfo')) )
		{
			dir = Victims.Location - HitLocation;
			dist = FMax(1,VSize(dir));
			dir = dir/dist; 
			damageScale = 1 - FMax(0,(dist - Victims.CollisionRadius)/DamageRadius);
			Victims.TakeDamage
			(
				damageScale * DamageAmount,
				Instigator, 
				Victims.Location - 0.5 * (Victims.CollisionHeight + Victims.CollisionRadius) * dir,
				(damageScale * Momentum * dir),
				DamageType
			);
		} 
	}
	bHurtEntry = false;
}

// Called when carried onto a new level, before AcceptInventory.
//
event TravelPreAccept();

// Called when carried into a new level, after AcceptInventory.
//
event TravelPostAccept();

// Called by PlayerController when this actor becomes its ViewTarget.
//
function BecomeViewTarget();

// Returns the string representation of the name of an object without the package
// prefixes.
//
function String GetItemName( string FullName )
{
	local int pos;

	pos = InStr(FullName, ".");
	While ( pos != -1 )
	{
		FullName = Right(FullName, Len(FullName) - pos - 1);
		pos = InStr(FullName, ".");
	}

	return FullName;
}

// Returns the human readable string representation of an object.
//
simulated function String GetHumanReadableName()
{
	return GetItemName(string(class));
}

final function ReplaceText(out string Text, string Replace, string With)
{
	local int i;
	local string Input;
		
	Input = Text;
	Text = "";
	i = InStr(Input, Replace);
	while(i != -1)
	{	
		Text = Text $ Left(Input, i) $ With;
		Input = Mid(Input, i + Len(Replace));	
		i = InStr(Input, Replace);
	}
	Text = Text $ Input;
}

// Set the display properties of an actor.  By setting them through this function, it allows
// the actor to modify other components (such as a Pawn's weapon) or to adjust the result
// based on other factors (such as a Pawn's other inventory wanting to affect the result)
function SetDisplayProperties(ERenderStyle NewStyle, Material NewTexture, bool bLighting )
{
	Style = NewStyle;
	texture = NewTexture;
	bUnlit = bLighting;
}

function SetDefaultDisplayProperties()
{
	Style = Default.Style;
	texture = Default.Texture;
	bUnlit = Default.bUnlit;
}

// Get localized message string associated with this actor
static function string GetLocalString(
	optional int Switch,
	optional PlayerReplicationInfo RelatedPRI_1, 
	optional PlayerReplicationInfo RelatedPRI_2
	)
{
	return "";
}

function MatchStarting(); // called when gameplay actually starts
function SetGRI(GameReplicationInfo GRI);

function String GetDebugName()
{
	return GetItemName(string(self));
}

/* DisplayDebug()
list important actor variable on canvas.  HUD will call DisplayDebug() on the current ViewTarget when
the ShowDebug exec is used
*/
simulated function DisplayDebug(Canvas Canvas, out float YL, out float YPos)
{
	local string T;
	local float XL;
	local int i;
	local Actor A;
	local name anim;
	local float frame,rate;

	Canvas.Style = ERenderStyle.STY_Normal;
	Canvas.StrLen("TEST", XL, YL);
	YPos = YPos + YL;
	Canvas.SetPos(4,YPos);
	Canvas.SetDrawColor(255,0,0);
	T = GetDebugName();
	if ( bDeleteMe )
		T = T$" DELETED (bDeleteMe == true)";

	Canvas.DrawText(T, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	Canvas.SetDrawColor(255,255,255);

	if ( Level.NetMode != NM_Standalone )
	{
		// networking attributes
		T = "ROLE ";
		Switch(Role)
		{
			case ROLE_None: T=T$"None"; break;
			case ROLE_DumbProxy: T=T$"DumbProxy"; break;
			case ROLE_SimulatedProxy: T=T$"SimulatedProxy"; break;
			case ROLE_AutonomousProxy: T=T$"AutonomousProxy"; break;
			case ROLE_Authority: T=T$"Authority"; break;
		}
		T = T$" REMOTE ROLE ";
		Switch(RemoteRole)
		{
			case ROLE_None: T=T$"None"; break;
			case ROLE_DumbProxy: T=T$"DumbProxy"; break;
			case ROLE_SimulatedProxy: T=T$"SimulatedProxy"; break;
			case ROLE_AutonomousProxy: T=T$"AutonomousProxy"; break;
			case ROLE_Authority: T=T$"Authority"; break;
		}
		if ( bTearOff )
			T = T$" Tear Off";
		Canvas.DrawText(T, false);
		YPos += YL;
		Canvas.SetPos(4,YPos);
	}
	T = "Physics ";
	Switch(PHYSICS)
	{
		case PHYS_None: T=T$"None"; break;
		case PHYS_Walking: T=T$"Walking"; break;
		case PHYS_Falling: T=T$"Falling"; break;
		case PHYS_Swimming: T=T$"Swimming"; break;
		case PHYS_Flying: T=T$"Flying"; break;
		case PHYS_Rotating: T=T$"Rotating"; break;
		case PHYS_Projectile: T=T$"Projectile"; break;
		case PHYS_Interpolating: T=T$"Interpolating"; break;
		case PHYS_MovingBrush: T=T$"MovingBrush"; break;
		case PHYS_Spider: T=T$"Spider"; break;
		case PHYS_Trailer: T=T$"Trailer"; break;
		case PHYS_Ladder: T=T$"Ladder"; break;
	}
	T = T$" in physicsvolume "$GetItemName(string(PhysicsVolume))$" on base "$GetItemName(string(Base));
	if ( bBounce )
		T = T$" - will bounce";
	Canvas.DrawText(T, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	Canvas.DrawText("Location: "$Location$" Rotation "$Rotation, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	Canvas.DrawText("Velocity: "$Velocity$" Speed "$VSize(Velocity), false);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	Canvas.DrawText("Acceleration: "$Acceleration, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	
	Canvas.DrawColor.B = 0;
	Canvas.DrawText("Collision Radius "$CollisionRadius$" Height "$CollisionHeight);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	Canvas.DrawText("Collides with Actors "$bCollideActors$", world "$bCollideWorld$", proj. target "$bProjTarget);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	Canvas.DrawText("Blocks Actors "$bBlockActors$", players "$bBlockPlayers);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	T = "Touching ";
	ForEach TouchingActors(class'Actor', A)
		T = T$GetItemName(string(A))$" ";
	if ( T == "Touching ")
		T = "Touching nothing";
	Canvas.DrawText(T, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	Canvas.DrawColor.R = 0;
	T = "Rendered: ";
	Switch(Style)
	{
		case STY_None: T=T; break;
		case STY_Normal: T=T$"Normal"; break;
		case STY_Masked: T=T$"Masked"; break;
		case STY_Translucent: T=T$"Translucent"; break;
		case STY_Modulated: T=T$"Modulated"; break;
		case STY_Alpha: T=T$"Alpha"; break;
	}		

	Switch(DrawType)
	{
		case DT_None: T=T$" None"; break;
		case DT_Sprite: T=T$" Sprite "; break;
		case DT_Mesh: T=T$" Mesh "; break;
		case DT_Brush: T=T$" Brush "; break;
		case DT_RopeSprite: T=T$" RopeSprite "; break;
		case DT_VerticalSprite: T=T$" VerticalSprite "; break;
		case DT_Terraform: T=T$" Terraform "; break;
		case DT_SpriteAnimOnce: T=T$" SpriteAnimOnce "; break;
		case DT_StaticMesh: T=T$" StaticMesh "; break;
	}

	if ( DrawType == DT_Mesh )
	{
		T = T$GetItemName(string(Mesh));
		if ( Skins.length > 0 )
		{
			T = T$" skins: ";
			for ( i=0; i<Skins.length; i++ )
			{
				if ( skins[i] == None )
					break;
				else
					T =T$GetItemName(string(skins[i]))$", ";
			}
		}

		Canvas.DrawText(T, false);
		YPos += YL;
		Canvas.SetPos(4,YPos);
		
		// mesh animation
		GetAnimParams(0,Anim,frame,rate);
		T = "AnimSequence "$Anim$" Frame "$frame$" Rate "$rate;
		if ( bAnimByOwner )
			T= T$" Anim by Owner";
	}
	else if ( (DrawType == DT_Sprite) || (DrawType == DT_SpriteAnimOnce) )
		T = T$Texture;
	else if ( DrawType == DT_Brush )
		T = T$Brush;
		
	Canvas.DrawText(T, false);
	YPos += YL;
	Canvas.SetPos(4,YPos);
	
	Canvas.DrawColor.B = 255;	
	Canvas.DrawText("Tag: "$Tag$" Event: "$Event$" STATE: "$GetStateName(), false);
	YPos += YL;
	Canvas.SetPos(4,YPos);

	Canvas.DrawText("Instigator "$GetItemName(string(Instigator))$" Owner "$GetItemName(string(Owner)));
	YPos += YL;
	Canvas.SetPos(4,YPos);
}

// NearSpot() returns true is spot is within collision cylinder
simulated final function bool NearSpot(vector Spot)
{
	local vector Dir;

	Dir = Location - Spot;
	
	if ( abs(Dir.Z) > CollisionHeight )
		return false;

	Dir.Z = 0;
	return ( VSize(Dir) <= CollisionRadius );
}

simulated final function bool TouchingActor(Actor A)
{
	local vector Dir;

	Dir = Location - A.Location;
	
	if ( abs(Dir.Z) > CollisionHeight + A.CollisionHeight )
		return false;

	Dir.Z = 0;
	return ( VSize(Dir) <= CollisionRadius + A.CollisionRadius );
}

/* StartInterpolation()
when this function is called, the actor will start moving along an interpolation path
beginning at Dest
*/	
simulated function StartInterpolation()
{
	GotoState('');
	//#ifdef __L2 // zodiac
	//SetCollision(True,false,false);
	//#endif
	bCollideWorld = False;
	bInterpolating = true;
	SetPhysics(PHYS_None);
}

/* Reset() 
reset actor to initial state - used when restarting level without reloading.
*/
function Reset();

/* 
Trigger an event
*/
event TriggerEvent( Name EventName, Actor Other, Pawn EventInstigator )
{
	local Actor A;

	if ( EventName == '' )
		return;

	ForEach DynamicActors( class 'Actor', A, EventName )
		A.Trigger(Other, EventInstigator);
}

/*
Untrigger an event
*/
function UntriggerEvent( Name EventName, Actor Other, Pawn EventInstigator )
{
	local Actor A;

	if ( EventName == '' )
		return;

	ForEach DynamicActors( class 'Actor', A, EventName )
		A.Untrigger(Other, EventInstigator);
}

function bool IsInVolume(Volume aVolume)
{
	local Volume V;
	
	ForEach TouchingActors(class'Volume',V)
		if ( V == aVolume )
			return true;
	return false;
}
	 
function bool IsInPain()
{
	local PhysicsVolume V;

	ForEach TouchingActors(class'PhysicsVolume',V)
		if ( V.bPainCausing && (V.DamagePerSec > 0) )
			return true;
	return false;
}

function PlayTeleportEffect(bool bOut, bool bSound);

function bool CanSplash()
{
	return false;
}

function vector GetCollisionExtent()
{
	local vector Extent;

	Extent = CollisionRadius * vect(1,1,0);
	Extent.Z = CollisionHeight;
	return Extent;
}

simulated function bool EffectIsRelevant(vector SpawnLocation, bool bForceDedicated )
{
	local PlayerController P;
	local bool bResult;
	
	if ( Level.NetMode == NM_DedicatedServer )
		bResult = bForceDedicated;
	else if ( Level.NetMode == NM_Client )
		bResult = true;
	else if ( (Instigator != None) && Instigator.IsHumanControlled() )
		bResult =  true;
	else if ( SpawnLocation == Location )
		bResult = ( Level.TimeSeconds - LastRenderTime < 3 );
	else if ( (Instigator != None) && (Level.TimeSeconds - Instigator.LastRenderTime < 3) )
		bResult = true;
	else
	{	
		P = Level.GetLocalPlayerController();
		if ( P == None )
			bResult = false;
		else 
			bResult = ( (Vector(P.Rotation) Dot (SpawnLocation - P.ViewTarget.Location)) > 0.0 );
	}
	return bResult;
}
//__L2 kurt
function SetOverlayColor(byte R, byte G, byte B,  byte A)
{
	OverlayColor.R = R;
	OverlayColor.G = G;
	OverlayColor.B = B;
	OverlayColor.A = A;
}

event GetEffTargetLocation(out vector LocVector)
{
	LocVector = Location; // GetValidLocation();
}

//function vector GetValidLocation()
//{
	//return Location;
//}

defaultproperties
{
     DrawType=DT_Sprite
     bLightingVisibility=True
     bUseDynamicLights=True
     bUpdateShadow=True
     bNeedCleanup=True
     CreatureID=-1
     bAcceptsProjectors=True
     bReplicateMovement=True
     L2NeedTick=True
     bNeedPostSpawnProcess=True
     L2LodViewDuration=1.500000
     L2CurrentLod=-1
     L2ServerObjectRealID=-1
     L2ServerObjectID=-1
     RemoteRole=ROLE_DumbProxy
     Role=ROLE_Authority
     NetUpdateFrequency=100.000000
     NetPriority=1.000000
     LODBias=1.000000
     Texture=Texture'Engine.S_Actor'
     DrawScale=1.000000
     DrawScale3D=(X=1.000000,Y=1.000000,Z=1.000000)
     MaxLights=4
     ScaleGlow=1.000000
     Style=STY_Normal
     bMovable=True
     TransientSoundVolume=0.300000
     TransientSoundRadius=300.000000
     CollisionRadius=22.000000
     CollisionHeight=22.000000
     bBlockZeroExtentTraces=True
     bBlockNonZeroExtentTraces=True
     bJustTeleported=True
     Mass=100.000000
     RotationLimit=(Pitch=65535,Yaw=65535,Roll=65535)
     MessageClass=Class'Engine.LocalMessage'
}
