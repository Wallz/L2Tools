/*=============================================================================
	UCC.cpp: Unreal command-line launcher.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

Revision history:
	* Created by Tim Sweeney.
=============================================================================*/

#if WIN32
	#include <windows.h>
	#include <process.h>
#else
	#include <errno.h>
	#include <sys/stat.h>
#endif
#include <malloc.h>
#include <stdio.h>

// Core and Engine
#include "Engine.h"

#if __STATIC_LINK

// Extra stuff for static links for Engine.
//#include "UnCon.h"
#include "UnRender.h"
#include "UnNet.h"

// NullNetDriver static stuff
//#include "NullNetDriver.h"

#ifdef __LINUX_X86__
// Fire static stuff.
//#include "UnFractal.h"

// IpDrv static stuff.
//#include "UnIpDrv.h"
//#include "UnTcpNetDriver.h"
//#include "UnIpDrvCommandlets.h"
//#include "UnIpDrvNative.h"

// UWeb static stuff
//#include "UWeb.h"
//#include "UWebNative.h"
#endif

#endif

INT GFilesOpen, GFilesOpened;

/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

// General.
#if _MSC_VER
	extern "C" {HINSTANCE hInstance;}
#endif
extern "C" {TCHAR GPackage[64]=TEXT("UCC");}

// Log.
#include "FOutputDeviceFile.h"
FOutputDeviceFile Log;

// gam --- Aux log.
FOutputDeviceFile AuxLog;
// --- gam

// Error.
#include "FOutputDeviceAnsiError.h"
FOutputDeviceAnsiError Error;

// Feedback.
#include "FFeedbackContextAnsi.h"
FFeedbackContextAnsi Warn;

// File manager.
#if WIN32
	#include "FFileManagerWindows.h"
	FFileManagerWindows FileManager;
	#include "FMallocAnsi.h"
	FMallocAnsi Malloc;
#elif __PSX2_EE__
	#include "FFileManagerPSX2.h"
	FFileManagerPSX2 FileManager;
	#include "FMallocAnsi.h"
	FMallocAnsi Malloc;
#elif __LINUX_X86__
	#include "FFileManagerLinux.h"
	FFileManagerLinux FileManager;
	#include "FMallocAnsi.h"
	FMallocAnsi Malloc;
#else
	#include "FFileManagerAnsi.h"
	FFileManagerAnsi FileManager;
	#include "FMallocAnsi.h"
	FMallocAnsi Malloc;
#endif

// Config.
#include "FConfigCacheIni.h"

/*-----------------------------------------------------------------------------
	Main.
-----------------------------------------------------------------------------*/

// Unreal command-line applet executor.
FString RightPad( FString In, INT Count )
{
	while( In.Len()<Count )
		In += TEXT(" ");
	return In;
}
static INT Compare( FString& A, FString& B ) // gam
{
	return appStricmp( *A, *B );
}
void ShowBanner( FOutputDevice& Warn )
{
	Warn.Logf( TEXT("=======================================") );
	Warn.Logf( TEXT("ucc.exe: UnrealOS execution environment") );
	Warn.Logf( TEXT("Copyright 2001 Epic Games Inc") );
	Warn.Logf( TEXT("=======================================") );
	Warn.Logf( TEXT("") );
}

#if __DO_LINUX_MCHECKING__
extern "C"
{
    int mcheck(void *feh);
}
#endif

int main( int argc, char* argv[] )
{
    #if __LINUX__
        setbuf(stdout, NULL);
        setbuf(stderr, NULL);
        #if __DO_LINUX_MCHECKING__
            mcheck(NULL);
        #endif
    #endif

	#if __STATIC_LINK
	// Clean lookups.
	for (INT k=0; k<ARRAY_COUNT(GNativeLookupFuncs); k++)
	{
		GNativeLookupFuncs[k] = NULL;
	}

	// Core lookups.
	GNativeLookupFuncs[0] = &FindCoreUObjectNative;
	GNativeLookupFuncs[1] = &FindCoreUCommandletNative;

	// Engine lookups.
	GNativeLookupFuncs[2] = &FindEngineAActorNative;
	GNativeLookupFuncs[3] = &FindEngineAPawnNative;
	GNativeLookupFuncs[4] = &FindEngineAPlayerPawnNative;
	GNativeLookupFuncs[5] = &FindEngineADecalNative;
	GNativeLookupFuncs[6] = &FindEngineAZoneInfoNative;
	GNativeLookupFuncs[7] = &FindEngineAWarpZoneInfoNative;
	GNativeLookupFuncs[8] = &FindEngineALevelInfoNative;
	GNativeLookupFuncs[9] = &FindEngineAGameInfoNative;
	GNativeLookupFuncs[10] = &FindEngineANavigationPointNative;
	GNativeLookupFuncs[11] = &FindEngineUCanvasNative;
	GNativeLookupFuncs[12] = &FindEngineUConsoleNative;

	#ifdef __LINUX_X86__
	// IpDrv lookups.
//	GNativeLookupFuncs[13] = &FindIpDrvAInternetLinkNative;
//	GNativeLookupFuncs[14] = &FindIpDrvAUdpLinkNative;
//	GNativeLookupFuncs[15] = &FindIpDrvATcpLinkNative;

	// UWeb lookups.
//	GNativeLookupFuncs[16] = &FindUWebUWebResponseNative;
//	GNativeLookupFuncs[17] = &FindUWebUWebRequestNative;
	#endif

	#endif

	INT ErrorLevel = 0;
	GIsStarted     = 1;
#ifndef _DEBUG
	try
#endif
	{
		GIsGuarded = 1;

		#if !_MSC_VER
		// Set module name.
		strcpy( GModule, "ucc" );
		//!!vogel: pathname might be too long
		//appStrcpy( GModule, argv[0] );
		#endif
		
		// Parse command line.
		TCHAR CmdLine[1024], *CmdLinePtr=CmdLine;
		*CmdLinePtr = 0;
		#if WIN32
			ANSICHAR* Ch = GetCommandLineA();
			while( *Ch && *Ch!=' ' )
				Ch++;
			while( *Ch==' ' )
				Ch++;
			while( *Ch )
				*CmdLinePtr++ = *Ch++;
			*CmdLinePtr++ = 0;
		#else
			for( INT i=2; i<argc; i++ )
			{
				if( i>2 )
					appStrcat( CmdLine, TEXT(" ") );
				appStrcat( CmdLine, ANSI_TO_TCHAR(argv[i]) );
			}
		#endif

		// Init engine core.
		appInit( TEXT("L2"), CmdLine, &Malloc, &Log, &Error, &Warn, &FileManager, FConfigCacheIni::Factory, 1 ); // gam

		// Launch the bug report monitor.
#if ((WIN32) && (!defined _DEBUG))
#if 0
		const TCHAR* Parameters[] = { TEXT("BugReport"), appItoa(GetCurrentProcessId()), NULL };
		_wspawnv(
			_P_NOWAIT,
			TEXT("BugReport"),
			Parameters
			);
#endif
#endif

		// Init static classes.
		#if __STATIC_LINK
			AUTO_INITIALIZE_REGISTRANTS_ENGINE;
			AUTO_INITIALIZE_REGISTRANTS_NULLNETDRIVER;
			#ifdef __LINUX_86__
		//	AUTO_INITIALIZE_REGISTRANTS_FIRE;
		//	AUTO_INITIALIZE_REGISTRANTS_IPDRV;
		//	AUTO_INITIALIZE_REGISTRANTS_UWEB;
			#endif
		#endif

		// Set up meta package readers.
//		for( i=0; i<GSys->MetaPackages.Num(); i++ )
//		{
//			printf("%s\n", *GSys->MetaPackages(i));
//			((FFileManagerPSX2*) GFileManager)->AddMetaArchive( *GSys->MetaPackages(i), &Error );
//		}

		// Get the ucc stuff going.	

	    // Language (gam)
	    TCHAR Temp[256];
	    if( GConfig->GetString( TEXT("Engine.Engine"), TEXT("Language"), Temp, ARRAY_COUNT(Temp) ) )
	    UObject::SetLanguage( Temp );

		FString Token = argc>1 ? appFromAnsi(argv[1]) : TEXT("");
		TArray<FRegistryObjectInfo> List;
		UObject::GetRegistryObjects( List, UClass::StaticClass(), UCommandlet::StaticClass(), 0 );
		GIsClient = GIsServer = GIsEditor = GIsScriptable = GIsUCC = 1;
		GLazyLoad = 0;
		UBOOL Help = 0;
		DWORD LoadFlags = LOAD_NoWarn | LOAD_Quiet;
		if( Token==TEXT("") )
		{
			ShowBanner( Warn );
			Warn.Logf( TEXT("Use \"ucc help\" for help") );
		}
		else if( Token==TEXT("HELP") )
		{
			ShowBanner( Warn );
			verify(UObject::StaticLoadClass( UCommandlet::StaticClass(), NULL, TEXT("Core.Commandlet"), NULL, LOAD_NoFail, NULL )==UCommandlet::StaticClass());
			const TCHAR* Tmp = appCmdLine();
			GIsEditor = 0; // To enable loading localized text.
			if( !ParseToken( Tmp, Token, 0 ) )
			{
				INT i;
				Warn.Logf( TEXT("Usage:") );
				Warn.Logf( TEXT("   ucc <command> <parameters>") );
				Warn.Logf( TEXT("") );
				Warn.Logf( TEXT("Commands for \"ucc\":") );
				TArray<FString> Items;
				for( i=0; i<List.Num(); i++ )
				{
					UClass* Class = UObject::StaticLoadClass( UCommandlet::StaticClass(), NULL, *List(i).Object, NULL, LoadFlags, NULL );
					if( Class )
					{
						UCommandlet* Default = (UCommandlet*)Class->GetDefaultObject();
						new(Items)FString( FString(TEXT("   ucc ")) + RightPad(Default->HelpCmd,21) + TEXT(" ") + Default->HelpOneLiner );
					}
				}
				new(Items)FString( TEXT("   ucc help <command>        Get help on a command") );
				Sort( &Items(0), Items.Num() );
				for( i=0; i<Items.Num(); i++ )
					Warn.Log( Items(i) );
			}
			else
			{
				Help = 1;
				goto Process;
			}
		}
		else
		{
			// Look it up.
			if( Token==TEXT("Make") )
				LoadFlags |= LOAD_DisallowFiles;
		Process:
			UClass* Class = UObject::StaticLoadClass( UCommandlet::StaticClass(), NULL, *Token, NULL, LoadFlags, NULL );
			if( !Class )
				Class = UObject::StaticLoadClass( UCommandlet::StaticClass(), NULL, *(Token+TEXT("Commandlet")), NULL, LoadFlags, NULL );
			if( !Class )
			{
				INT i;
				for( i=0; i<List.Num(); i++ )
				{
					FString Str = List(i).Object;
					while( Str.InStr(TEXT("."))>=0 )
						Str = Str.Mid(Str.InStr(TEXT("."))+1);
					if( Token==Str || Token+TEXT("Commandlet")==Str )
						break;
				}
				if( i<List.Num() )
					Class = UObject::StaticLoadClass( UCommandlet::StaticClass(), NULL, *List(i).Object, NULL, LoadFlags, NULL );
			}
			if( Class )
			{
				UCommandlet* Default = (UCommandlet*)Class->GetDefaultObject();
				if( Help )
				{
					// Get help on it.
					if( Default->HelpUsage!=TEXT("") )
					{
						Warn.Logf( TEXT("Usage:") );
						Warn.Logf( TEXT("   ucc %s"), *Default->HelpUsage );
					}
					if( Default->HelpParm[0]!=TEXT("") )
					{
						Warn.Logf( TEXT("") );
						Warn.Logf( TEXT("Parameters:") );
						for( INT i=0; i<ARRAY_COUNT(Default->HelpParm) && Default->HelpParm[i]!=TEXT(""); i++ )
							Warn.Logf( TEXT("   %s %s"), *RightPad(Default->HelpParm[i],16), *Default->HelpDesc[i] );
					}
					if( Default->HelpWebLink!=TEXT("") )
					{
						Warn.Logf( TEXT("") );
						Warn.Logf( TEXT("For more info, see") );
						Warn.Logf( TEXT("   %s"), *Default->HelpWebLink );
					}
				}
				else
				{
					// Run it.
					if( Default->LogToStdout )
					{
						Warn.AuxOut = GLog;
						GLog        = &Warn;
					}
                    else
                    {
                        // gam ---
                        appStrcpy( AuxLog.Filename, TEXT("StdOut.log") );
                        AuxLog.bLogEvent = 0;
                        Warn.AuxOut = &AuxLog;
                        // --- gam
                    }
					if( Default->ShowBanner )
					{
						ShowBanner( Warn );
					}
					debugf( TEXT("Executing %s"), Class->GetFullName() );
					GIsClient = Default->IsClient;
					GIsServer = Default->IsServer;
					GIsEditor = Default->IsEditor;
					GLazyLoad = Default->LazyLoad;
					UCommandlet* Commandlet = ConstructObject<UCommandlet>( Class );
					Commandlet->InitExecution();
					Commandlet->ParseParms( appCmdLine() );
					Commandlet->Main( appCmdLine() );
					if( Commandlet->ShowErrorCount )
                    {
                        // gam ---
                        if ( Warn.ErrorCount == 0)
						    GWarn->Logf( TEXT("Success - %d error(s), %d warning(s)"), Warn.ErrorCount, Warn.WarningCount );
                        else
                        {
						    GWarn->Logf( TEXT("Failure - %d error(s), %d warning(s)"), Warn.ErrorCount, Warn.WarningCount );
                            ErrorLevel = 1;
                        }
                        // --- gam
                    }
					if( Default->LogToStdout )
					{
						Warn.AuxOut = NULL;
						GLog        = &Log;
					}
				}
			}
			else
			{
				ShowBanner( Warn );
				Warn.Logf( TEXT("Commandlet %s not found"), *Token );
			}
		}
		appPreExit();
		GIsGuarded = 0;
	}
#ifndef _DEBUG
	catch( ... )
	{
		// Crashed.
		ErrorLevel = 1;
		GIsGuarded = 0;
		Error.HandleError();
	}
#endif
	appExit();
	GIsStarted = 0;
	return ErrorLevel;
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
