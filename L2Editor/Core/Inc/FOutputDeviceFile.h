/*=============================================================================
	FOutputDeviceFile.h: ANSI file output device.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

//
// ANSI file output device.
//
class FOutputDeviceFile : public FOutputDevice
{
public:
	FOutputDeviceFile()
	: LogAr( NULL )
	, Opened( 0 )
	, Dead( 0 )
    , bLogEvent( 1 ) // gam
	{
		Filename[0]=0;
	}
	~FOutputDeviceFile()
	{
		if( LogAr )
		{
			Logf( NAME_Log, TEXT("Log file closed, %s"), appTimestamp() );
			delete LogAr;
			LogAr = NULL;
		}
	}
	void Flush()
	{
		if( LogAr )
			LogAr->Flush();
	}
	void Serialize( const TCHAR* Data, enum EName Event )
	{
		static UBOOL Entry=0;
		if( !GIsCriticalError || Entry )
		{
			if( !FName::SafeSuppressed(Event) )
			{
				if( !LogAr && !Dead )
				{
					// Make log filename.
					if( !Filename[0] )
					{
                        #if WIN32
						appStrcpy( Filename, appBaseDir() );
                        #endif

						if
						(	!Parse(appCmdLine(), TEXT("LOG="), Filename+appStrlen(Filename), ARRAY_COUNT(Filename)-appStrlen(Filename) )
						&&	!Parse(appCmdLine(), TEXT("ABSLOG="), Filename, ARRAY_COUNT(Filename) ) )
						{
						    // gam ---
                            if( ParseParam( appCmdLine(), TEXT("SOAKING") ) )
						    {
						        TCHAR *InsertPoint;
						        const INT MaxTries = 9999;
						        INT i;
						        
						        TCHAR ComputerName[256];
						        
						        appStrcpy( ComputerName, appComputerName() );
						        
						        for( i = 0; ComputerName[i] != '\0'; i++ )
						            ComputerName[i] = appToUpper( ComputerName[i] );

	                            InsertPoint = Filename + appStrlen(Filename);

	                            for( i = 0; i < MaxTries; i++ )
	                            {
	                                appSprintf( InsertPoint, TEXT("%s_SOAK_%04d.log"), ComputerName, i );

		                            if( GFileManager->FileSize( Filename ) < 0 )
		                                break;
	                            }
	                            
	                            if( i >= MaxTries )
	                            {
							        appStrcpy( InsertPoint, appPackage() );
							        appStrcat( InsertPoint, TEXT(".log") );
	                            }
						    }
						    // --- gam
						    else
						    {
							    appStrcat( Filename, appPackage() );
							    appStrcat( Filename, TEXT(".log") );
							}
						}
					}

					// Open log file.
					LogAr = GFileManager->CreateFileWriter( Filename, FILEWRITE_AllowRead|FILEWRITE_Unbuffered|(Opened?FILEWRITE_Append:0));
					if( LogAr )
					{
						Opened = 1;
#if UNICODE && !FORCE_ANSI_LOG
                        #if __LINUX__
                        if (GUnicodeOS) {
                        #endif
						_WORD UnicodeBOM = UNICODE_BOM;
						LogAr->Serialize( &UnicodeBOM, 2 );
                        #if __LINUX__
                        }
                        #endif
#endif
						Logf( NAME_Log, TEXT("Log file open, %s"), appTimestamp() );
					}
					else Dead = 1;
				}
				if( LogAr && Event!=NAME_Title )
				{
#if ((FORCE_ANSI_LOG && UNICODE) || (__LINUX__))
                    #if __LINUX__
                    if (!GUnicodeOS) {
                    #endif

					TCHAR Ch[1024];
					ANSICHAR ACh[1024];

                    // gam ---
                    if( bLogEvent )
					    appSprintf( Ch, TEXT("%s: %s%s"), FName::SafeString(Event), Data, LINE_TERMINATOR );
                    else
					    appSprintf( Ch, TEXT("%s%s"), Data, LINE_TERMINATOR );
                    // --- gam

					INT i;
					for( i=0; Ch[i]; i++ )
						ACh[i] = ToAnsi(Ch[i] );
					ACh[i] = 0;
					LogAr->Serialize( ACh, i );
                    //LogAr->Flush(); //amb

                    #if __LINUX__
                    } else {
					WriteRaw( FName::SafeString(Event) );
					WriteRaw( TEXT(": ") );
					WriteRaw( Data );
					WriteRaw( LINE_TERMINATOR );
                    }
                    #endif
#else
                    // gam ---
                    if( bLogEvent )
                    {
					    WriteRaw( FName::SafeString(Event) );
					    WriteRaw( TEXT(": ") );
                    }
                    // --- gam
					WriteRaw( Data );
					WriteRaw( LINE_TERMINATOR );
#endif
#ifdef _DEBUG
#if _MSC_VER
					TCHAR Temp[1024];

                    // gam ---
                    if( bLogEvent )
					    appSprintf( Temp, TEXT("%s: %s%s"), FName::SafeString(Event), Data, LINE_TERMINATOR );
                    else
    					appSprintf( Temp, TEXT("%s%s"), Data, LINE_TERMINATOR );
                    // --- gam

					OutputDebugString(Temp);
#endif
#endif
				}
				if( GLogHook )
					GLogHook->Serialize( Data, Event );
			}
		}
		else
		{
			Entry=1;
			try
			{
				// Ignore errors to prevent infinite-recursive exception reporting.
				Serialize( Data, Event );
			}
			catch( ... )
			{}
			Entry=0;
		}
	}
	FArchive* LogAr;
	TCHAR Filename[1024];
    UBOOL bLogEvent; // gam
private:
	UBOOL Opened, Dead;
	void WriteRaw( const TCHAR* C )
	{
		LogAr->Serialize( const_cast<TCHAR*>(C), appStrlen(C)*sizeof(TCHAR) );
        //LogAr->Flush(); //amb
	}
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/

