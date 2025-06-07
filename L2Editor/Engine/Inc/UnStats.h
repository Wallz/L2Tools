/*=============================================================================
	UnStats.h: Performance statistics utilities.
	Copyright 1997-2002 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Andrew Scheidecker
		* Rewritten multiple times by Andrew Scheidecker and Daniel Vogel
		* Last rewrite by Daniel Vogel for .csv support
=============================================================================*/

enum EStatsType
{
	STATSTYPE_Render		= 0,
	STATSTYPE_Game,
	STATSTYPE_Hardware,
	STATSTYPE_Anim,
	STATSTYPE_Matinee,
	STATSTYPE_Audio,
	STATSTYPE_Net,
	STATSTYPE_L2,
	STATSTYPE_L2_Unk00,
	STATSTYPE_L2_Unk01,
	STATSTYPE_L2_Unk02,
	STATSTYPE_L2_Unk03,
	STATSTYPE_L2_Unk04,
	STATSTYPE_MAX
};

enum EStatsDataType
{
	STATSDATATYPE_DWORD		= 0,
	STATSDATATYPE_FLOAT,
	STATSDATATYPE_STRING,
	STATSDATATYPE_MAX
};

enum EStatsUnit
{
	STATSUNIT_Default		= 0,
	STATSUNIT_Combined_Default_MSec,
	STATSUNIT_Combined_Default_Default,
	STATSUNIT_Byte,
	STATSUNIT_KByte,
	STATSUNIT_MByte,
	STATSUNIT_MSec
};

class ENGINE_API FStats
{
public:
	FLOAT	FPSAvg;
	BYTE	Pad00[16];

	// Init Data.
	UBOOL			Registered[STATSTYPE_MAX];

	// Stats Data.
	TArray<DWORD>	DWORDStats;
	TArray<DWORD>	DWORDOldStats;
	TArray<FString>	DWORDStatsDescriptions;
	TArray<FString>	DWORDStatsSectionDescriptions;

	TArray<FLOAT>	FLOATStats;
	TArray<FLOAT>	FLOATOldStats;
	TArray<FString>	FLOATStatsDescriptions;
	TArray<FString>	FLOATStatsSectionDescriptions;

	TArray<FString>	FSTRINGStats;
	TArray<FString> FSTRINGOldStats;
	TArray<FString>	FSTRINGStatsDescriptions;
	TArray<FString>	FSTRINGStatsSectionDescriptions;

	struct FSingleStatsInfo
	{
		INT				Index;
		EStatsDataType	DataType;
		EStatsUnit		StatsUnit;
	};

	TArray<FSingleStatsInfo> Stats[STATSTYPE_MAX];

	struct FMovingAverage
	{		
		INT CycIndex;
		INT CycBufferSize;
		INT Count;
		EStatsDataType	DataType;
		TArray<DWORD>   Samples;
	};

	TArray<FMovingAverage> Averages;

	// Interface.
	FStats();
	INT RegisterStats( EStatsType Type, EStatsDataType DataType, FString Description, FString SectionDescription, EStatsUnit StatsUnit );
	void Render(class UViewport* Viewport, class UEngine* Engine);
	void UpdateString( FString& Result, UBOOL Descriptions );
	void Clear();
	void CalcMovingAverage( INT StatIndex, DWORD CycleSize );
};


class ENGINE_API FEngineStats
{
public:
	INT		STATS_FirstEntry,
		STATS_Frame_TotalCycles,
		STATS_Frame_RenderCycles,
		STATS_Frame_RenderLevel,
		STATS_Frame_RenderActor,
		STATS_Karma_Collision,
		STATS_Karma_CollisionContactGen,
		STATS_Karma_TrilistGen,
		STATS_Karma_RagdollTrilist,
		STATS_Karma_Dynamics,
		STATS_Karma_physKarma,
		STATS_Karma_physKarma_Con,
		STATS_Karma_physKarmaRagDoll,
		STATS_Karma_Temp,
		STATS_BSP_RenderCycles,
		STATS_BSP_Sections,
		STATS_BSP_Nodes,
		STATS_BSP_Triangles,
		STATS_BSP_DynamicLightingCycles,
		STATS_BSP_DynamicLights,
		STATS_BSP_CollisionCycles,
		STATS_LightMap_Updates,
		STATS_LightMap_Cycles,
		STATS_Projector_RenderCycles,
		STATS_Projector_AttachCycles,
		STATS_Projector_Projectors,
		STATS_Projector_Triangles,
		STATS_Stencil_RenderCycles,
		STATS_Stencil_Nodes,
		STATS_Stencil_Triangles,
		STATS_Visibility_SetupCycles,
		STATS_Visibility_CombineLevel,
		STATS_Visibility_FilterActor,
		STATS_Visibility_ClearDynamicLightList,
		STATS_Visibility_MaskTests,
		STATS_Visibility_MaskRejects,
		STATS_Visibility_BoxTests,
		STATS_Visibility_BoxRejects,
		STATS_Visibility_TraverseCycles,
		STATS_Visibility_AntiPortal,
		STATS_Visibility_ProcessLeaf,
		STATS_Visibility_ProcessLeafCall,
		STATS_Visibility_ProcessNode,
		STATS_Visibility_ProcessNodeCall,
		STATS_Visibility_ScratchBytes,
		STATS_Visibility_MeshLightCycles,
		STATS_Terrain_RenderCycles,
		STATS_Terrain_CollisionCycles,
		STATS_Terrain_Sectors,
		STATS_Terrain_Triangles,
		STATS_Terrain_DrawPrimitives,
		STATS_DecoLayer_RenderCycles,
		STATS_DecoLayer_Triangles,
		STATS_DecoLayer_Decorations,
		STATS_Matinee_TickCycles,
		STATS_SkeletalMesh_CPUSkinning,
		STATS_SkeletalMesh_Result,
		STATS_SkeletalMesh_LOD,
		STATS_SkeletalMesh_Total,
		STATS_SkeletalMesh_Pose,
		STATS_SkeletalMesh_Rigid,
		STATS_SkeletalMesh_DrawCall,
		STATS_SkeletalMesh_SkinVertexNum,
		STATS_SkeletalMesh_SameAniMeshNum,
		STATS_SkeletalMesh_Smooth,
		STATS_SkeletalMesh_GPUBoneMatrix,
		STATS_SkeletalMesh_UntilGetFrame,
		STATS_SkeletalMesh_AfterGetFrame,
		STATS_SkeletalMesh_GPUSetMatrix,
		STATS_SkeletalMesh_GPUSetVector,
		STATS_SkeletalMesh_GPUSetBoneMatrix,
		STATS_SkeletalMesh_S_DrawSection_NPC,
		STATS_SkeletalMesh_S_Render_PC_DHair,
		STATS_SkeletalMesh_S_EX_Render,
		STATS_SkeletalMesh_GPUSetConstant,
		STATS_Particle_SpriteSetupCycles,
		STATS_Particle_Particles,
		STATS_Particle_ParticlesTicked,
		STATS_Particle_RenderCycles,
		STATS_StaticMesh_Batches,
		STATS_StaticMesh_BatchedSortedSections,
		STATS_StaticMesh_BatchedSortedTriangles,
		STATS_StaticMesh_BatchedSortCycles,
		STATS_StaticMesh_BatchedUnsortedSections,
		STATS_StaticMesh_BatchedUnsortedTriangles,
		STATS_StaticMesh_BatchedRenderCycles,
		STATS_StaticMesh_UnbatchedSortedSections,
		STATS_StaticMesh_UnbatchedSortedTriangles,
		STATS_StaticMesh_UnbatchedSortCycles,
		STATS_StaticMesh_UnbatchedUnsortedSections,
		STATS_StaticMesh_UnbatchedUnsortedTriangles,
		STATS_StaticMesh_UnbatchedRenderCycles,
		STATS_StaticMesh_Triangles,
		STATS_StaticMesh_Triangles2,
		STATS_StaticMesh_CollisionCycles,
		STATS_StaticMesh_CollisionCacheHits,
		STATS_StaticMesh_CollisionCacheMisses,
		STATS_StaticMesh_CollisionCacheFlushes,
		STATS_Unk_00,
		STATS_Unk_01,
		STATS_Unk_02,
		STATS_Unk_03,
		STATS_Unk_04,
		STATS_Batch_Batches,
		STATS_Batch_Primitives,
		STATS_Batch_RenderCycles,
		STATS_Stats_RenderCycles,
		STATS_Game_ScriptCycles,
		STATS_Game_ScriptTickCycles,
		STATS_Game_ActorTickCycles,
		STATS_Game_Pawn,
		STATS_Game_Controller,
		STATS_Game_Level,
		STATS_Game_L2Net,
		STATS_Game_FindPathCycles,
		STATS_Game_SeePlayerCycles,
		STATS_Game_SpawningCycles,
		STATS_Game_AudioTickCycles,
		STATS_Game_CleanupDestroyedCycles,
		STATS_Game_UnusedCycles,
		STATS_Game_NetTickCycles,
		STATS_Game_ParticleTickCycles,
		STATS_Game_CanvasCycles,
		STATS_Game_PhysicsCycles,
		STATS_Game_MoveCycles,
		STATS_Game_NumMoves,
		STATS_Game_MLCheckCycles,
		STATS_Game_MPCheckCycles,
		STATS_Game_UpdateRenderData,
		STATS_Game_HUDPostRender,
		STATS_Game_InteractionPreRender,
		STATS_Game_InteractionPostRender,
		STATS_Game_ScriptDebugTime,
		STATS_Game_ActorLightSLChecks,
		STATS_Fluid_SimulateCycles,
		STATS_Fluid_VertexGenCycles,
		STATS_Fluid_RenderCycles,
		STATS_Corona_CollisionCycles,
		STATS_Net_Ping,
		STATS_Net_Channels,
		STATS_Net_InUnordered,
		STATS_Net_OutUnordered,
		STATS_Net_InPacketLoss,
		STATS_Net_OutPacketLoss,
		STATS_Net_InPackets,
		STATS_Net_OutPackets,
		STATS_Net_InBunches,
		STATS_Net_OutBunches,
		STATS_Net_InBytes,
		STATS_Net_OutBytes,
		STATS_Net_Speed,
		STATS_Net_NumReps,
		STATS_Net_NumRPC,
		STATS_Net_NumPV,
		STATS_L2_X,
		STATS_L2_Y,
		STATS_L2_Z,
		STATS_L2_MapX,
		STATS_L2_MapY,
		STATS_L2_Speed,
		STATS_L2_CloakSimul,
		STATS_L2_HairSimul,
		STATS_L2_Time,
		STATS_L2_PawnResourceNum,
		STATS_L2_PawnResourceLoadCycles,
		STATS_L2_PawnResourceAddCycles,
		STATS_L2_XX,
		STATS_L2_YY,
		STATS_L2_ZZ,
		STATS_L2_Script1[10],
		STATS_L2_Script2[10],
		STATS_LastEntry;

	FEngineStats();
	void Init();
};
