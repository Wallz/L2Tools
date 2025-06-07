class LevelObject extends Object
	abstract
	native
	nativereplication;

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
var bool					bDeleteMe;
var bool					bSelected;
var(Advanced) bool			bHidden;
var(Advanced) bool			bHiddenEd;
var(Advanced) bool			bHiddenEdGroup;
var(Advanced) bool			bLockUndelete;
var(Advanced) bool			bLockLocation;
var	bool					bPendingDelete;
var transient const bool	bTempEditor;
var(Object) name			Group;
var(Movement) vector		Location;
var(Display) Material		Texture;
var(Display) float			DrawScale;
var(Advanced) const			PointRegion     Region;
var transient const Level	SWXLevel;
var transient const Level	XLevel;
var LevelObject				Deleted;

defaultproperties
{
     bHidden=True
     DrawScale=1.000000
}
