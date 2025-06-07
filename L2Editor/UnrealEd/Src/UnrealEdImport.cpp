/*=============================================================================
	UnrealEdImport.cpp: 
	Implementation of functions in UnrealEdDLL.dll interface
	Generally taken from other parts of the unreal engine
	( support for the direct maya-to-editor brush and skeletal 
	mesh art path.) 
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.	

	Revision history:
		* Created by Michael Arnold,  Secret Level.
=============================================================================*/


#include "UnrealEd.h"
#include "UnrealEdImport.h"


// NOTE: This stuff is not used
struct VBitmapOrigin
{
	char RawBitmapName[MAX_PATH];
	char RawBitmapPath[MAX_PATH];
};

struct VSkin
{
	TArray <VMaterial>			Materials; // Materials
	TArray <VPoint>				Points;    // 3D Points
	TArray <VVertex>			Wedges;  // Wedges
	TArray <VTriangle>			Faces;       // Faces
	TArray <VBone>				RefBones;   // reference skeleton
	TArray <void*>				RawMaterials; // alongside Materials - to access extra data //Mtl*
	TArray <VRawBoneInfluence>	RawWeights;  // Raw bone weights..
	TArray <VBitmapOrigin>      RawBMPaths;  // Full bitmap Paths for logging

	// Brushes: UV must embody the material size...
	TArray <INT>				MaterialUSize;
	TArray <INT>				MaterialVSize;
	
	int NumBones; // Explicit number of bones (?)
};


struct VAnimation
{
	AnimInfoBinary        AnimInfo;   //  Generic animation info as saved for the engine
	TArray <VQuatAnimKey> KeyTrack;   //  Animation track - keys * bones
	VAnimation()
	{}
};

struct VAnimationList
{
	explicit VAnimationList(TArray<VAnimation> * list = NULL)
		: AnimList(list)
	{}

	TArray<VAnimation> *	AnimList;
};

// #Skel - message("TODO:Find replacement for \"toTCHAR(const char*)\" somewhere in the engine")
TCHAR *	toTCHAR(const char * str) {
	static TCHAR local[256];

	if (str == 0) {
		local[0] = 0;
		return local;
	}

	unsigned int i = 0;
	while (i < 255 && str[i]) {
		local[i] = str[i];
		++i;
	}
	local[i] = 0;
	return local;
}

//--------------------------------------------------------------------------

void localLoadSkin(UPackage * Pkg, FString const& skinName, struct VSkin * skin, double scale)
{

	USkeletalMesh*	Mesh = new( Pkg, *skinName, RF_Public|RF_Standalone )USkeletalMesh();;

	USkelImport RawData; // local struct
	// transfer data to RawData
	RawData.Materials = skin->Materials;
	RawData.Wedges = skin->Wedges;
	RawData.Faces = skin->Faces;
	RawData.RefBonesBinary = skin->RefBones;
	RawData.Influences = skin->RawWeights;

	//RawData.Points = skin->Points;
	RawData.Points = TArray<FVector>(skin->Points.Num());
	for (int i = 0; i < skin->Points.Num(); ++i) {
		RawData.Points(i) = skin->Points(i).Point;
	}

	
	// Y-flip quaternions and translations from Max/Maya/etc space into Unreal space.
	for( INT b=0; b<RawData.RefBonesBinary.Num(); b++)
	{
		FQuat Bone = RawData.RefBonesBinary(b).BonePos.Orientation;
		Bone.W = - Bone.W;
		Bone.Y = - Bone.Y;
		RawData.RefBonesBinary(b).BonePos.Orientation = Bone;

		FVector Pos = RawData.RefBonesBinary(b).BonePos.Position;
		Pos.Y = - Pos.Y;
		RawData.RefBonesBinary(b).BonePos.Position = Pos;
	}

	// Y-flip skin, and adjust handedness.
	for( INT p=0; p<RawData.Points.Num(); p++ )
	{
		RawData.Points(p).Y =-RawData.Points(p).Y;
	}

	/*
	for( INT f=0; f<RawData.Faces.Num(); f++)
	{
		INT WIdx = RawData.Faces(f).WedgeIndex[2];
		RawData.Faces(f).WedgeIndex[2] = RawData.Faces(f).WedgeIndex[1];
		RawData.Faces(f).WedgeIndex[1] = WIdx;
	}
	*/
	
	// Initialize mesh defaults. TODO: - move into general LOD mesh initialization?
	// Default LOD settings.
	Mesh->LODMinVerts	  = 10;		// Minimum number of vertices with which to draw a model. (Minimum for a cube = 8...)
	Mesh->LODStrength	  = 1.00f;	// Scales the (not necessarily linear) falloff of vertices with distance.
	Mesh->LODMorph        = 0.30f;	// Morphing range. 0.0f = no morphing.
	Mesh->LODZDisplace    = 0.00f;    // Z-displacement (in world units) for falloff function tweaking.
	Mesh->LODHysteresis	  = 0.00f;	// Controls LOD-level change delay/morphing. (unused)

	// Init textures.
	for(INT i=0; i<Mesh->Materials.Num(); i++ ) 
	{
		Mesh->Materials(i) = NULL;//FindObject<UMaterial>( ANY_PACKAGE, *NewStr );
	}

	
	// Pad the texture pointers.. TODO: clean all this up when using materials.
	while( RawData.Materials.Num() > Mesh->Materials.Num() ) {
			Mesh->Materials.AddItem( NULL );
	}

	// set textures according to input
	for (INT j=0; j < RawData.Materials.Num(); ++j) {
		FString name_j(toTCHAR( RawData.Materials(j).MaterialName));
		UMaterial * mat_j = FindObject<UMaterial>( ANY_PACKAGE, *name_j );
		if (mat_j) {
			Mesh->Materials(j) = mat_j;
		}
	}

		// display summary info
		debugf(NAME_Log,TEXT(" * Skeletal skin VPoints: %i "),RawData.Points.Num());
		debugf(NAME_Log,TEXT(" * Skeletal skin VVertices: %i "),RawData.Wedges.Num());
		debugf(NAME_Log,TEXT(" * Skeletal skin VTriangles: %i "),RawData.Faces.Num());
		debugf(NAME_Log,TEXT(" * Skeletal skin VMaterials: %i "),RawData.Materials.Num());
		debugf(NAME_Log,TEXT(" * Skeletal skin VBones: %i "),RawData.RefBonesBinary.Num());
		debugf(NAME_Log,TEXT(" * Skeletal skin VRawBoneInfluences: %i "),RawData.Influences.Num());

	FLODProcessInfo LODInfo;
	LODInfo.LevelOfDetail = true; 
	LODInfo.ApplySmoothingGroups = false;
	LODInfo.Style = 0;				
	LODInfo.SampleFrame = 0;
	LODInfo.NoUVData = false;
	LODInfo.Specify = 0;

	// Setup mesh and process automatic LOD.
	GEditor->meshSkelLODProcess( Mesh, &LODInfo, &RawData );  
	// Compute per-frame bounding volumes plus overall bounding volume.
	Mesh->MeshGetInstance(NULL)->MeshBuildBounds(); 

		debugf(NAME_Log,TEXT(" * Total materials: %i "),((USkeletalMesh*)Mesh)->MeshMaterials.Num());
		
		// display summary info.	
		debugf(NAME_Log,TEXT(" * Skeletal skin Points: %i size %i "),Mesh->RawVerts.Num(), sizeof(FVector) );
		debugf(NAME_Log,TEXT(" * Skeletal skin Wedges: %i size %i "),Mesh->RawWedges.Num(), sizeof(FMeshWedge) );
		debugf(NAME_Log,TEXT(" * Skeletal skin Triangles: %i size %i "),Mesh->RawFaces.Num(), sizeof(VTriangle) );
		debugf(NAME_Log,TEXT(" * Skeletal skin Influences: %i size %i "),Mesh->RawInfluences.Num(), sizeof(FVertInfluence) );
		debugf(NAME_Log,TEXT(" * Skeletal skin Bones: %i size %i "),Mesh->RefSkeleton.Num(), sizeof(FMeshBone) );

	// 
	FVector ScaleVector(scale,scale,scale);				
	Mesh->MeshGetInstance(NULL)->SetScale( ScaleVector );	

	// Quick high-level maya 2 unreal turn-upright trick.
	Mesh->RotOrigin.Yaw = -16384;
	Mesh->RotOrigin.Pitch = 0;
	Mesh->RotOrigin.Roll = 16384;	

	Pkg->bDirty = 1;
}

void localLoadAnims(UPackage * Pkg, FString const& skinName, FString const& animName, TArray <VBone> * bonelist, TArray<VAnimation> * animlist)
{
	if (bonelist == 0 || animlist == 0) {
		return;
	}
	
	if (bonelist->Num() == 0 || animlist->Num() == 0) {
		return;
	}

	UMeshAnimation * NewAnimation = (UMeshAnimation*) UObject::StaticFindObject( UMeshAnimation::StaticClass(), ANY_PACKAGE, *animName);
	// Existing mesh ? Delete it first.
	if( NewAnimation ) 
	{					
		delete( NewAnimation );
		Pkg->bDirty = 1;
	}
				

	// Allocate skeletal mesh object.
	NewAnimation = new( Pkg, *animName, RF_Public|RF_Standalone )UMeshAnimation();

	// Allocate/initialize temp import helper structure.
	NewAnimation->InitForDigestion();

	// Loading Animation Stuff

	// Translate the raw data from the bones to Animation->RefBones FNames
	NewAnimation->RefBones.Add(bonelist->Num());
	for( INT n=0; n<bonelist->Num(); n++)
	{
		VBone& bone_n = (*bonelist)(n);
		appTrimSpaces(&bone_n.Name[0]);
		NewAnimation->RefBones(n).Name  = FName(appFromAnsi(&bone_n.Name[0]));
		NewAnimation->RefBones(n).Flags = bone_n.Flags;
		NewAnimation->RefBones(n).ParentIndex = bone_n.ParentIndex;
	}

	NewAnimation->DigestHelper->RawAnimSeqInfo.Add( animlist->Num() );
	for ( INT k = 0; k < animlist->Num(); k++) {

		VAnimation& anim_k = (*animlist)(k);

		INT raw_keys = NewAnimation->DigestHelper->RawAnimKeys.Num();
		INT raw_frames = raw_keys / bonelist->Num();

		NewAnimation->DigestHelper->RawAnimSeqInfo(k) = anim_k.AnimInfo;
		NewAnimation->DigestHelper->RawAnimSeqInfo(k).FirstRawFrame += raw_frames;

		// transfer the key track data
		if (anim_k.KeyTrack.Num() > 0) {
			NewAnimation->DigestHelper->RawAnimKeys.Add( anim_k.KeyTrack.Num() );

			VQuatAnimKey * dst = &NewAnimation->DigestHelper->RawAnimKeys(raw_keys);
			VQuatAnimKey * sce = &anim_k.KeyTrack(0);
			memcpy(dst,sce, anim_k.KeyTrack.Num() * sizeof(VQuatAnimKey));
		}
	}

	NewAnimation->DigestHelper->CompFactor = 1.0;

	// Y-flip quaternions and translations from Max/Maya/etc space into Unreal space.
	for( INT i=0; i<NewAnimation->DigestHelper->RawAnimKeys.Num(); i++)
	{
		FQuat Bone = NewAnimation->DigestHelper->RawAnimKeys(i).Orientation;
		Bone.W = - Bone.W;
		Bone.Y = - Bone.Y;
		NewAnimation->DigestHelper->RawAnimKeys(i).Orientation = Bone;

		FVector Pos = NewAnimation->DigestHelper->RawAnimKeys(i).Position;
		Pos.Y = - Pos.Y;
		NewAnimation->DigestHelper->RawAnimKeys(i).Position = Pos;
	}

	// Digest and compress the movements.					
	GEditor->digestMovementRepertoire(NewAnimation);

	// Erase the raw data.
	NewAnimation->DigestHelper->RawAnimKeys.Empty();
	NewAnimation->DigestHelper->MovesInfo.Empty();

	USkeletalMesh* NewMesh = (USkeletalMesh*) UObject::StaticFindObject( USkeletalMesh::StaticClass(), ANY_PACKAGE, *skinName);
	if (NewMesh) {
		NewMesh->DefaultAnim = NewAnimation;
	}

}

UNREALED_API void LoadSkin(const char * package, const char * name, struct VSkin * skin, double scale)
{
	FString pkgName(toTCHAR(package)), skinName(toTCHAR(name));

	// find the package
	UPackage * Pkg = (UPackage*) UObject::StaticFindObject( UPackage::StaticClass(), ANY_PACKAGE, *pkgName);
	if (Pkg == 0) {
		// create the package
		Pkg = UObject::CreatePackage(NULL,*pkgName);		
	}

	if (skin) {
		localLoadSkin( Pkg, skinName, skin, scale );
	}
}

UNREALED_API void LoadAnimations(const char * package, const char * name, struct VSkin * skin, struct VAnimationList * anims)
{
	FString pkgName(toTCHAR(package)), skinName(toTCHAR(name));

	// find the package
	UPackage * Pkg = (UPackage*) UObject::StaticFindObject( UPackage::StaticClass(), ANY_PACKAGE, *pkgName);
	if (Pkg == 0) {
		// create the package
		Pkg = UObject::CreatePackage(NULL,*pkgName);		
	}

	if (skin && anims) {
		localLoadAnims( Pkg, skinName, skinName, &skin->RefBones, anims->AnimList );
	}
}


UNREALED_API void LoadMesh(const char * package, const char * group, const char * name, const UnEditor::FMesh * mesh)
{
	FString pkgName(toTCHAR(package)), groupName(toTCHAR(group)), meshName(toTCHAR(name));

	// find the package
	UPackage * Pkg = (UPackage*) UObject::StaticFindObject( UPackage::StaticClass(), ANY_PACKAGE, *pkgName);
	if (Pkg == 0) {
		// create the package
		Pkg = UObject::CreatePackage(NULL,*pkgName);		
	}

	UPackage * Grp = 0;
	if (group) {
		Grp = (UPackage*) UObject::StaticFindObject( UPackage::StaticClass(), Pkg, *groupName);
		if (Grp == 0) {
			// create the package
			Grp = UObject::CreatePackage(Pkg,*groupName);		
		}
	}


	if (mesh) 
	{
		// set stuff up
		unsigned int num_triangles = mesh->Faces.Num();
		TArray<FStaticMeshTriangle> triangles( num_triangles );
		for (unsigned int i = 0; i < num_triangles; ++i) { // triangle[i]
			
			UnEditor::FTriangle const& face_i = mesh->Faces(i);
			FStaticMeshTriangle& triangle_i = triangles(i);

			triangle_i.Vertices[0] = mesh->Points( face_i.points[0] );
			triangle_i.Vertices[1] = mesh->Points( face_i.points[1] );
			triangle_i.Vertices[2] = mesh->Points( face_i.points[2] );

			// Y-flip skin to adjust handedness & conform with Unreal coordinates..
			triangle_i.Vertices[0].Y = -triangle_i.Vertices[0].Y;
			triangle_i.Vertices[1].Y = -triangle_i.Vertices[1].Y;
			triangle_i.Vertices[2].Y = -triangle_i.Vertices[2].Y;

			triangle_i.UVs[0][0].U = mesh->TPoints( face_i.t_points[0] ).U;
			triangle_i.UVs[0][0].V = mesh->TPoints( face_i.t_points[0] ).V;
			triangle_i.UVs[1][0].U = mesh->TPoints( face_i.t_points[1] ).U;
			triangle_i.UVs[1][0].V = mesh->TPoints( face_i.t_points[1] ).V;
			triangle_i.UVs[2][0].U = mesh->TPoints( face_i.t_points[2] ).U;
			triangle_i.UVs[2][0].V = mesh->TPoints( face_i.t_points[2] ).V;

			triangle_i.Colors[0] = FColor(255,255,255,255);
			triangle_i.Colors[1] = FColor(255,255,255,255);
			triangle_i.Colors[2] = FColor(255,255,255,255);

			triangle_i.MaterialIndex = face_i.MatIndex;
			triangle_i.SmoothingMask = 1;
			triangle_i.NumUVs = 1;
			triangle_i.LegacyMaterial = 0;
			triangle_i.LegacyPolyFlags = 1;

		} // triangle[i]

		// set materials up
		unsigned int num_materials = mesh->Materials.Num();
		TArray<FStaticMeshMaterial> materials( num_materials );
		for (unsigned int j = 0; j < num_materials; ++j) { // material[j]

			FString name_j(toTCHAR( mesh->Materials(j).MaterialName));
			materials(j) = FindObject<UMaterial>( ANY_PACKAGE, *name_j );

		} // material[j]

		FName new_mesh_name( *meshName );
		UStaticMesh * new_mesh = CreateStaticMesh( triangles, materials, (Grp) ? Grp : Pkg , new_mesh_name );

		Pkg->bDirty = 1;

	}
}
