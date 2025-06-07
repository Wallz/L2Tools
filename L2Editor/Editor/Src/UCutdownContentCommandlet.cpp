/*=============================================================================
	UCutdownContentCommandlet.cpp: Load the specified levels and save only the content
	                     packages which they reference.
	Copyright 2001 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Jack Porter
	* Rogered by Glen Miner

=============================================================================*/

#include "EditorPrivate.h"

/*-----------------------------------------------------------------------------
	UCutdownContentCommandlet
-----------------------------------------------------------------------------*/


#define OLDUT2DIR TEXT("..\\Maps\\")
#define OLDUTXDIR TEXT("..\\Textures\\")
#define OLDUSXDIR TEXT("..\\StaticMeshes\\")
#define OLDUAXDIR TEXT("..\\Sounds\\")

#define NEWUT2DIR TEXT("..\\CutdownMaps\\")
#define NEWUTXDIR TEXT("..\\CutdownTextures\\")
#define NEWUSXDIR TEXT("..\\CutdownStaticMeshes\\")
#define NEWUAXDIR TEXT("..\\CutdownSounds\\")

class UCutdownContentCommandlet : public UCommandlet
{
	DECLARE_CLASS(UCutdownContentCommandlet,UCommandlet,CLASS_Transient,Editor);
	void StaticConstructor()
	{
		guard(UCutdownContentCommandlet::StaticConstructor);

		LogToStdout     = 0;
		IsClient        = 1;
		IsEditor        = 1;
		IsServer        = 1;
		LazyLoad        = 0;
		ShowErrorCount  = 1;

		unguard;
	}
	INT Main( const TCHAR* Parms )
	{
		guard(UCutdownContentCommandlet::Main);
		
		UClass* EditorEngineClass = UObject::StaticLoadClass( UEditorEngine::StaticClass(), NULL, TEXT("ini:Engine.Engine.EditorEngine"), NULL, LOAD_NoFail | LOAD_DisallowFiles, NULL );
		GEditor  = ConstructObject<UEditorEngine>( EditorEngineClass );
		GEditor->UseSound = 0;
		GEditor->InitEditor();
		
		GLazyLoad = 0;

	    for( INT i=0; i < GSys->Paths.Num(); i++ )
	    {
		    FString PackageWildcard;
            TArray<FString> FilesInPath;
            FString PathPrefix;
            INT j;

		    PackageWildcard = FString(appBaseDir()) + GSys->Paths(i);

            if( PackageWildcard.Right (appStrlen(TEXT ("*.u"))) != TEXT ("*.u"))
                continue;

		    FilesInPath = GFileManager->FindFiles( *PackageWildcard, 1, 0 );

            if( !FilesInPath.Num() )
                continue;

            check( PackageWildcard.InStr(TEXT("*")) >= 0 );
            PathPrefix = PackageWildcard.Left( PackageWildcard.InStr( TEXT("*"), 1 ));

		    for( j = 0; j < FilesInPath.Num(); j++ )
            {
                const FString &File = FilesInPath(j);
                UObject* Package;

                GWarn->Logf (NAME_Log, TEXT("Loading %s"), *File);

			    Package = UObject::LoadPackage( NULL, *File, 0 );

                UObject::ResetLoaders( NULL, 0, 1 );

                if( !Package )
                {
                    GWarn->Logf (NAME_Log, TEXT("    Error loading %s!"), *File);
                    continue;
                }
            }
        }

		TArray<FString> UTXList = GFileManager->FindFiles( *FString::Printf(TEXT("%s%s"), OLDUTXDIR, TEXT("*.utx")), 1, 0 );
		TArray<FString> USXList = GFileManager->FindFiles( *FString::Printf(TEXT("%s%s"), OLDUSXDIR, TEXT("*.usx")), 1, 0 );
		TArray<FString> UAXList = GFileManager->FindFiles( *FString::Printf(TEXT("%s%s"), OLDUAXDIR, TEXT("*.uax")), 1, 0 );
	
		FString Wildcard;
		while( ParseToken( Parms, Wildcard, 0 ) )
		{
			TArray<FString> UT2List = GFileManager->FindFiles( *FString::Printf(TEXT("%s%s"), OLDUT2DIR, *Wildcard), 1, 0 );
			for( INT i=0;i<UT2List.Num();i++ )
			{	
				UObject::CollectGarbage(RF_Native | RF_Standalone);

				FString OldUT2Name = FString::Printf(TEXT("%s%s"), OLDUT2DIR, *UT2List(i));
				FString NewUT2Name = FString::Printf(TEXT("%s%s"), NEWUT2DIR, *UT2List(i));

				GWarn->Logf(TEXT("  Loading %s"), *OldUT2Name );
				UObject* Package = LoadPackage( NULL, *OldUT2Name, 0 );
				check(Package);
				ULevel* Level = FindObject<ULevel>( Package, TEXT("MyLevel") );
				check(Level);
			
				GWarn->Logf(TEXT("  Saving %s"), *NewUT2Name );
				SavePackage( Package, Level, 0, *NewUT2Name, GWarn );
			}
		}

		GWarn->Logf(TEXT("Saving Textures...."));
		for (INT i=0; i<UTXList.Num(); i++)
		{	
			UPackage* Package = FindObject<UPackage>( NULL, *(UTXList(i).Left(UTXList(i).Len()-4)) );
			if( Package )
			{
				FString NewUTXName = FString::Printf(TEXT("%s%s"), NEWUTXDIR, *UTXList(i));

                if( GFileManager->FileSize(*NewUTXName) > 0 )
                    continue;
                
				GWarn->Logf(TEXT("  Saving %s"), *NewUTXName );
				SavePackage( Package, NULL, RF_Standalone, *NewUTXName, NULL );
			}
		}

		GWarn->Logf(TEXT("Saving Static Meshes...."));
		for (INT i=0; i<USXList.Num(); i++)
		{
			UPackage* Package = FindObject<UPackage>( NULL, *(USXList(i).Left(USXList(i).Len()-4)) );
			if( Package )
			{
				FString NewUSXName = FString::Printf(TEXT("%s%s"), NEWUSXDIR, *USXList(i) );

                if( GFileManager->FileSize(*NewUSXName) > 0 )
                    continue;

				GWarn->Logf(TEXT("  Saving %s"), *NewUSXName );
				SavePackage( Package, NULL, RF_Standalone, *NewUSXName, NULL );
			}
		}

		GWarn->Logf(TEXT("Saving Sounds...."));
		for (INT i=0; i<UAXList.Num(); i++)
		{
			UPackage* Package = FindObject<UPackage>( NULL, *(UAXList(i).Left(UAXList(i).Len()-4)) );
			if( Package )
			{
				FString NewUAXName = FString::Printf(TEXT("%s%s"), NEWUAXDIR, *UAXList(i) );

                if( GFileManager->FileSize(*NewUAXName) > 0 )
                    continue;

				GWarn->Logf(TEXT("  Saving %s"), *NewUAXName );
				SavePackage( Package, NULL, RF_Standalone, *NewUAXName, NULL );
			}
		}

		GIsRequestingExit=1;
		return 0;
		unguard;
	}
};
IMPLEMENT_CLASS(UCutdownContentCommandlet)

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
