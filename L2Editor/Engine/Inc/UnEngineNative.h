/*=============================================================================
	UnEngineNative.h: Native function lookup table for static libraries.
	Copyright 2000 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Brandon Reinhart
=============================================================================*/

#ifndef UNENGINENATIVE_H
#define UNENGINENATIVE_H

/*
DECLARE_NATIVE_TYPE(Engine,AActor);
DECLARE_NATIVE_TYPE(Engine,APawn);
DECLARE_NATIVE_TYPE(Engine,AZoneInfo);
DECLARE_NATIVE_TYPE(Engine,AWarpZoneInfo);
DECLARE_NATIVE_TYPE(Engine,ALevelInfo);
DECLARE_NATIVE_TYPE(Engine,AGameInfo);
DECLARE_NATIVE_TYPE(Engine,ANavigationPoint);
DECLARE_NATIVE_TYPE(Engine,ASmallNavigationPoint);
DECLARE_NATIVE_TYPE(Engine,ADamageType);
DECLARE_NATIVE_TYPE(Engine,AController);
DECLARE_NATIVE_TYPE(Engine,APlayerController);
DECLARE_NATIVE_TYPE(Engine,AVolume);
DECLARE_NATIVE_TYPE(Engine,AProjector);
DECLARE_NATIVE_TYPE(Engine,AHUD);
DECLARE_NATIVE_TYPE(Engine,AKConstraint);
DECLARE_NATIVE_TYPE(Engine,ULogEntry); // sjs
DECLARE_NATIVE_TYPE(Engine,AxPickUpBase); // amb
DECLARE_NATIVE_TYPE(Engine,AxEmitter); // amb
DECLARE_NATIVE_TYPE(Engine,UParticleEmitter);
DECLARE_NATIVE_TYPE(Engine,UCanvas);

#define AUTO_INITIALIZE_REGISTRANTS_ENGINE \
AGameStats::StaticClass(); \
UAdminBase::StaticClass(); \
AKTire::StaticClass(); \
AKVehicle::StaticClass(); \
AKConstraint::StaticClass(); \
AKBSJoint::StaticClass(); \
AKHinge::StaticClass(); \
AKConeLimit::StaticClass(); \
AKCarWheelJoint::StaticClass(); \
UKMeshProps::StaticClass(); \
UKarmaParams::StaticClass(); \
UKarmaParamsRBFull::StaticClass(); \
UKarmaParamsSkel::StaticClass(); \
AKActor::StaticClass(); \
AActor::StaticClass(); \
ALight::StaticClass(); \
AClipMarker::StaticClass(); \
APolyMarker::StaticClass(); \
AWeapon::StaticClass(); \
ANote::StaticClass(); \
ALevelInfo::StaticClass(); \
AGameInfo::StaticClass(); \
ACamera::StaticClass(); \
AZoneInfo::StaticClass(); \
ASkyZoneInfo::StaticClass(); \
UReachSpec::StaticClass(); \
APathNode::StaticClass(); \
ANavigationPoint::StaticClass(); \
AScout::StaticClass(); \
AInterpolationPoint::StaticClass(); \
ADecoration::StaticClass(); \
AProjectile::StaticClass(); \
AWarpZoneInfo::StaticClass(); \
ATeleporter::StaticClass(); \
APlayerStart::StaticClass(); \
APlayerStats::StaticClass(); \
AKeypoint::StaticClass(); \
AInventory::StaticClass(); \
AInventorySpot::StaticClass(); \
ATriggers::StaticClass(); \
ATrigger::StaticClass(); \
AWarpZoneMarker::StaticClass(); \
AHUD::StaticClass(); \
ASavedMove::StaticClass(); \
ALiftCenter::StaticClass(); \
ALiftExit::StaticClass(); \
AInfo::StaticClass(); \
AReplicationInfo::StaticClass(); \
APlayerReplicationInfo::StaticClass(); \
AInternetInfo::StaticClass(); \
AGameReplicationInfo::StaticClass(); \
ULevelSummary::StaticClass(); \
AAmmo::StaticClass(); \
APickup::StaticClass(); \
APowerups::StaticClass(); \
AAmmunition::StaticClass(); \
AController::StaticClass(); \
AAIController::StaticClass(); \
APlayerController::StaticClass(); \
AVehicle::StaticClass(); \
AVehiclePart::StaticClass(); \
ALadder::StaticClass(); \
ADoor::StaticClass(); \
ADamageType::StaticClass(); \
ABrush::StaticClass(); \
AAIScript::StaticClass(); \
ALineOfSightTrigger::StaticClass(); \
AVolume::StaticClass(); \
APhysicsVolume::StaticClass(); \
ADefaultPhysicsVolume::StaticClass(); \
ALadderVolume::StaticClass(); \
AAutoLadder::StaticClass(); \
ATeamInfo::StaticClass(); \
ACoopInfo::StaticClass(); \
AInventoryAttachment::StaticClass(); \
AWeaponAttachment::StaticClass(); \
USound::StaticClass(); \
UAudioSubsystem::StaticClass(); \
UBeamEmitter::StaticClass(); \
UClient::StaticClass(); \
UViewport::StaticClass(); \
UCanvas::StaticClass(); \
UChannel::StaticClass(); \
UControlChannel::StaticClass(); \
UActorChannel::StaticClass(); \
UFileChannel::StaticClass(); \
UNetConnection::StaticClass(); \
UCheatManager::StaticClass(); \
UPlayerInput::StaticClass(); \
UDemoPlayPendingLevel::StaticClass(); \
UDemoRecConnection::StaticClass(); \
UDemoRecDriver::StaticClass(); \
UDownload::StaticClass(); \
UChannelDownload::StaticClass(); \
UEngine::StaticClass(); \
URenderDevice::StaticClass(); \
UServerCommandlet::StaticClass(); \
UGlobalTempObjects::StaticClass(); \
UPolys::StaticClass(); \
UFont::StaticClass(); \
UGameEngine::StaticClass(); \
UInput::StaticClass(); \
UInteraction::StaticClass(); \
UConsole::StaticClass(); \
UInteractions::StaticClass(); \
UInteractionMaster::StaticClass(); \
ULevelBase::StaticClass(); \
ULevel::StaticClass(); \
ULodMeshInstance::StaticClass(); \
ULodMesh::StaticClass(); \
UMaterial::StaticClass(); \
UBitmapMaterial::StaticClass(); \
UProxyBitmapMaterial::StaticClass(); \
URenderedMaterial::StaticClass(); \
UTexCoordMaterial::StaticClass(); \
UColorModifier::StaticClass(); \
UShader::StaticClass(); \
UCombiner::StaticClass(); \
UModifier::StaticClass(); \
UTexModifier::StaticClass(); \
UTexPanner::StaticClass(); \
UTexScaler::StaticClass(); \
UTexRotator::StaticClass(); \
UTexOscillator::StaticClass(); \
UTexEnvMap::StaticClass(); \
UTexMatrix::StaticClass(); \
UFinalBlend::StaticClass(); \
UMeshInstance::StaticClass(); \
UMesh::StaticClass(); \
UMeshEmitter::StaticClass(); \
UModel::StaticClass(); \
AMover::StaticClass(); \
UPackageMapLevel::StaticClass(); \
UNetDriver::StaticClass(); \
UParticleEmitter::StaticClass(); \
AEmitter::StaticClass(); \
UParticleMaterial::StaticClass(); \
APawn::StaticClass(); \
UPendingLevel::StaticClass(); \
UNetPendingLevel::StaticClass(); \
UPlayer::StaticClass(); \
UPrimitive::StaticClass(); \
UProjectorPrimitive::StaticClass(); \
AProjector::StaticClass(); \
URenderResource::StaticClass(); \
UVertexStreamBase::StaticClass(); \
UVertexStreamVECTOR::StaticClass(); \
UVertexStreamCOLOR::StaticClass(); \
UVertexStreamUV::StaticClass(); \
UVertexStreamPosNormTex::StaticClass(); \
UVertexBuffer::StaticClass(); \
UIndexBuffer::StaticClass(); \
USkinVertexBuffer::StaticClass(); \
ASceneManager::StaticClass(); \
UMatObject::StaticClass(); \
UMatAction::StaticClass(); \
UActionMoveCamera::StaticClass(); \
UActionPause::StaticClass(); \
UMatSubAction::StaticClass(); \
USubActionFade::StaticClass(); \
USubActionTrigger::StaticClass(); \
USubActionFOV::StaticClass(); \
USubActionOrientation::StaticClass(); \
USubActionGameSpeed::StaticClass(); \
USubActionSceneSpeed::StaticClass(); \
ALookTarget::StaticClass(); \
UMeshAnimation::StaticClass(); \
UAnimation::StaticClass(); \
USkeletalMesh::StaticClass(); \
USkeletalMeshInstance::StaticClass(); \
USparkEmitter::StaticClass(); \
USpriteEmitter::StaticClass(); \
UStaticMesh::StaticClass(); \
UStaticMeshInstance::StaticClass(); \
AStaticMeshActor::StaticClass(); \
UTerrainSector::StaticClass(); \
UTerrainPrimitive::StaticClass(); \
ATerrainInfo::StaticClass(); \
UTexture::StaticClass(); \
UPalette::StaticClass(); \
UCubemap::StaticClass(); \
UVertMesh::StaticClass(); \
UConvexVolume::StaticClass(); \
ADecorationList::StaticClass(); \
APotentialClimbWatcher::StaticClass(); \
AAIMarker::StaticClass(); \
UCameraEffect::StaticClass(); \
UVertMeshInstance::StaticClass(); \
ABlockingVolume::StaticClass(); \
AStationaryWeapons::StaticClass(); \
UAnimNotify::StaticClass(); \
UAnimNotify_Sound::StaticClass(); \
UAnimNotify_Effect::StaticClass(); \
UAnimNotify_Script::StaticClass(); \
UAnimNotify_Scripted::StaticClass(); \
AFluidSurfaceInfo::StaticClass(); \
UConstantMaterial::StaticClass(); \
UConstantColor::StaticClass(); \
UShadowBitmapMaterial::StaticClass(); \
USubActionCameraEffect::StaticClass(); \
UMotionBlur::StaticClass(); \
UConsole::StaticClass(); \
UTerrainPrimitive::StaticClass(); \
AMenu::StaticClass(); \
ULogEntry::StaticClass(); \
AxPickUpBase::StaticClass(); \
AxEmitter::StaticClass(); \
AxVehicle::StaticClass(); \
AxProcMesh::StaticClass(); \
AxWeatherEffect::StaticClass(); \
UManifest::StaticClass(); \
APotentialClimbWatcher::StaticClass(); \
UProceduralSound::StaticClass(); \
USoundGroup::StaticClass(); \
UConstantMaterial::StaticClass(); \
UConstantColor::StaticClass(); \
AAIMarker::StaticClass(); \
USpline::StaticClass(); \
AAmbientSound::StaticClass(); \
AWeaponFire::StaticClass(); \
UI3DL2Listener::StaticClass(); \
AWeaponStat::StaticClass(); \
APlayerStats::StaticClass(); \
AJumpDest::StaticClass(); \
AJumpPad::StaticClass(); \
ASecurity::StaticClass(); \
UScriptedTexture::StaticClass(); \
AFluidSurfaceOscillator::StaticClass(); \
AAntiPortalActor::StaticClass();\
UPackageCheckInfo::StaticClass(); \
UFluidSurfacePrimitive::StaticClass(); \
UFadeColor::StaticClass(); \
AAvoidMarker::StaticClass(); \
APrecacheHack::StaticClass(); \
UPlayInfo::StaticClass();
*/
#endif

