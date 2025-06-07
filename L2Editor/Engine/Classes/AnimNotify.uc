class AnimNotify extends Object
	native
	abstract
	editinlinenew
	hidecategories(Object)
	collapsecategories;

var transient int Revision;

// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)
// (cpptext)

cpptext
{
	// AnimNotify interface.
	virtual void Notify( UMeshInstance *Instance, AActor *Owner ) {};
	virtual FString ExportToScript();
	// UObject interface.
	virtual void PostEditChange();
}


defaultproperties
{
}
