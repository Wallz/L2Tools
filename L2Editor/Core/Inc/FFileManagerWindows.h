/*=============================================================================
    FFileManagerWindows.h: Unreal Windows OS based file manager.
    Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

    Revision history:
        * Created by Tim Sweeney
=============================================================================*/

#include "FFileManagerGeneric.h"

#include <sys/types.h>
#include <sys/stat.h>

/*-----------------------------------------------------------------------------
    File Manager.
-----------------------------------------------------------------------------*/

// File manager.
class FArchiveFileReader : public FArchive
{
public:
    FArchiveFileReader( HANDLE InHandle, FOutputDevice* InError, INT InSize )
    :   Handle          ( InHandle )
    ,   Error           ( InError )
    ,   Size            ( InSize )
    ,   Pos             ( 0 )
    ,   BufferBase      ( 0 )
    ,   BufferCount     ( 0 )
    {
        guard(FArchiveFileReader::FArchiveFileReader);
        ArIsLoading = ArIsPersistent = 1;
        unguard;
    }
    ~FArchiveFileReader()
    {
        guard(FArchiveFileReader::~FArchiveFileReader);
        if( Handle )
            Close();
        unguard;
    }
    void Precache( INT HintCount )
    {
        guardSlow(FArchiveFileReader::Precache);
        checkSlow(Pos==BufferBase+BufferCount);
        BufferBase = Pos;
        BufferCount = Min( Min( HintCount, (INT)(ARRAY_COUNT(Buffer) - (Pos&(ARRAY_COUNT(Buffer)-1))) ), Size-Pos );
        INT Count=0;
        ReadFile( Handle, Buffer, BufferCount, (DWORD*)&Count, NULL );
        if( Count!=BufferCount )
        {
            ArIsError = 1;
            Error->Logf( TEXT("ReadFile failed: Count=%i BufferCount=%i Error=%s"), Count, BufferCount, appGetSystemErrorMessage() );
        }
        unguardSlow;
    }
    void Seek( INT InPos )
    {
        guard(FArchiveFileReader::Seek);
        check(InPos>=0);
        check(InPos<=Size);
        if( SetFilePointer( Handle, InPos, 0, FILE_BEGIN )==0xFFFFFFFF )
        {
            ArIsError = 1;
            Error->Logf( TEXT("SetFilePointer Failed %i/%i: %i %s"), InPos, Size, Pos, appGetSystemErrorMessage() );
        }
        Pos         = InPos;
        BufferBase  = Pos;
        BufferCount = 0;
        unguard;
    }
    INT Tell()
    {
        return Pos;
    }
    INT TotalSize()
    {
        return Size;
    }
    UBOOL Close()
    {
        guardSlow(FArchiveFileReader::Close);
        if( Handle )
            CloseHandle( Handle );
        Handle = NULL;
        return !ArIsError;
        unguardSlow;
    }
    void Serialize( void* V, INT Length )
    {
        guardSlow(FArchiveFileReader::Serialize);
        while( Length>0 )
        {
            INT Copy = Min( Length, BufferBase+BufferCount-Pos );
            if( Copy==0 )
            {
                if( Length >= ARRAY_COUNT(Buffer) )
                {
                    INT Count=0;
                    ReadFile( Handle, V, Length, (DWORD*)&Count, NULL );
                    if( Count!=Length )
                    {
                        ArIsError = 1;
                        Error->Logf( TEXT("ReadFile failed: Count=%i Length=%i Error=%s"), Count, Length, appGetSystemErrorMessage() );
                    }
                    Pos += Length;
                    BufferBase += Length;
                    return;
                }
                Precache( MAXINT );
                Copy = Min( Length, BufferBase+BufferCount-Pos );
                if( Copy<=0 )
                {
                    ArIsError = 1;
                    Error->Logf( TEXT("ReadFile beyond EOF %i+%i/%i"), Pos, Length, Size );
                }
                if( ArIsError )
                    return;
            }
            appMemcpy( V, Buffer+Pos-BufferBase, Copy );
            Pos       += Copy;
            Length    -= Copy;
            V          = (BYTE*)V + Copy;
        }
        unguardSlow;
    }
protected:
    HANDLE          Handle;
    FOutputDevice*  Error;
    INT             Size;
    INT             Pos;
    INT             BufferBase;
    INT             BufferCount;
    BYTE            Buffer[1024];
};

static const DWORD kLineageHeaderSize = 28;
static const BYTE kLineageVer111[kLineageHeaderSize] = { 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x32, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x31, 0x00, 0x31, 0x00, 0x31};
static const BYTE kLineageVer121[kLineageHeaderSize] = { 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x32, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x31, 0x00, 0x32, 0x00, 0x31 };
static const BYTE kLineageVer413[kLineageHeaderSize] = { 0x4c, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x65, 0x00, 0x61, 0x00, 0x67, 0x00, 0x65, 0x00, 0x32, 0x00, 0x56, 0x00, 0x65, 0x00, 0x72, 0x00, 0x34, 0x00, 0x31, 0x00, 0x33 };

static int GetLinagePackageVersion(HANDLE Handle) {
	static char HeaderBuffer[kLineageHeaderSize] = { 0 };

	int result = -1;
	DWORD readCount = 0;
	if (ReadFile(Handle, HeaderBuffer, 28, &readCount, NULL))
	{
		if (memcmp(HeaderBuffer, kLineageVer111, kLineageHeaderSize) == 0)
			result = 111;
		else if (memcmp(HeaderBuffer, kLineageVer121, kLineageHeaderSize) == 0)
			result = 121;
		else if (memcmp(HeaderBuffer, kLineageVer413, kLineageHeaderSize) == 0)
			result = 413;
	}

	SetFilePointer(Handle, 0, NULL, FILE_BEGIN);

	return result;
}

class FLineageArchive111FileReader : public FArchiveFileReader
{
public:
	FLineageArchive111FileReader(HANDLE InHandle, FOutputDevice* InError, INT InSize) : FArchiveFileReader(InHandle, InError, InSize - kLineageHeaderSize) {
		SetFilePointer(Handle, kLineageHeaderSize, NULL, FILE_BEGIN);
	}

	void Serialize(void* V, INT Length) {
		FArchiveFileReader::Serialize(V, Length);
		BYTE* byteBuffer = (BYTE*)V;
		for (int currentIndex = 0; currentIndex < Length; ++currentIndex) {
			byteBuffer[currentIndex] ^= 0xAC;
		}
	}

	void Seek(INT InPos) {
		FArchiveFileReader::Seek(InPos);
		SetFilePointer(Handle, InPos + kLineageHeaderSize, 0, FILE_BEGIN);
	}
};

class FLineageArchive121FileReader : public FArchiveFileReader
{
	static INT GenerateXorKey(const wchar_t* Filename) {
		int amountOfFileName = 0;
		int currentIndex = 0;
		while (Filename[currentIndex]) {
			amountOfFileName += towlower(Filename[currentIndex]);
			++currentIndex;
		}

		return amountOfFileName & 0xff;
	}

	static const wchar_t* GetLastPathComponent(const wchar_t* Filename) {
		INT filenameLength = wcsnlen(Filename, 1024);

		const wchar_t* lastValidPathString = NULL;
		const wchar_t* currentPathSeparator = L"\\";
		const wchar_t* currentPathString = wcsstr(Filename, currentPathSeparator);
		if (currentPathString == NULL) {
			currentPathSeparator = L"/";
			currentPathString = wcsstr(Filename, currentPathSeparator);
		}

		while (currentPathString != NULL && currentPathString < currentPathString + filenameLength) {
			lastValidPathString = currentPathString;
			currentPathString = wcsstr(++currentPathString, currentPathSeparator);
		}

		return ++lastValidPathString;
	}
private:
	INT Key;

public:
	FLineageArchive121FileReader(const wchar_t* Filename, HANDLE InHandle, FOutputDevice* InError, INT InSize) : FArchiveFileReader(InHandle, InError, InSize - kLineageHeaderSize) {
		const wchar_t* lastPathComponent = GetLastPathComponent(Filename);
		if (lastPathComponent == NULL) {
			appErrorf(TEXT("Can't find last path component"));
		}

		Key = GenerateXorKey(lastPathComponent);
		SetFilePointer(Handle, kLineageHeaderSize, NULL, FILE_BEGIN);
	}

	void Serialize(void* V, INT Length) {
		FArchiveFileReader::Serialize(V, Length);
		BYTE* byteBuffer = (BYTE*)V;
		for (int currentIndex = 0; currentIndex < Length; ++currentIndex) {
			byteBuffer[currentIndex] ^= Key;
		}
	}

	void Seek(INT InPos) {
		FArchiveFileReader::Seek(InPos);
		SetFilePointer(Handle, InPos + kLineageHeaderSize, 0, FILE_BEGIN);
	}
};

namespace L2EncDec
{
	namespace rsa_key
	{
		const char* e = "0000001d";
		const char* N = "75b4d6de5c016544068a1acf125869f43d2e09fc55b8b1e289556daf9b8757635593446288b3653da1ce91c87bb1a5c18f16323495c55d7d72c0890a83f69bfd1fd9434eb1c02f3e4679edfa43309319070129c267c85604d87bb65bae205de3707af1d2108881abb567c3b3d069ae67c3a4c6a3aa93d26413d4c66094ae2039";
		const char* d = "";
	};
};

namespace L2Off
{
	namespace rsa_key
	{
		const char* e = "00000035";
		const char* N = "97df398472ddf737ef0a0cd17e8d172f0fef1661a38a8ae1d6e829bc1c6e4c3cfc19292dda9ef90175e46e7394a18850b6417d03be6eea274d3ed1dde5b5d7bde72cc0a0b71d03608655633881793a02c9a67d9ef2b45eb7c08d4be329083ce450e68f7867b6749314d40511d09bc5744551baa86a89dc38123dc1668fd72d83";
		const char* d = "";
	}
};

#include <tomcrypt.h>
#include <tomcrypt_lib.h>
class FLineageArchive413FileReader : public FArchiveFileReader
{
public:
	void reverse(BYTE* in, size_t len)
	{
		BYTE* out = in;

		for (size_t i = 0; i < len; i++)
		{
			size_t j = (len - 1) - i;
			if (i > j)
				break;

			BYTE b = in[i];
			in[i] = out[j];
			out[j] = b;
		}
	}

	FLineageArchive413FileReader(const wchar_t* Filename, HANDLE InHandle, FOutputDevice* InError, INT InSize) 
		: FArchiveFileReader(InHandle, InError, InSize - kLineageHeaderSize) 
	{
		/* register a math library */
		ltc_mp = ltm_desc;

		/* initialize RSA key */
		enum {
			pk_d,
			pk_e,
			pk_N,
			pk_max
		};

		unsigned char key_parts[pk_max][128] = {0};
		unsigned long key_lens[pk_max];
		for (int i = 0; i < pk_max; i++) 
			key_lens[i] = sizeof(key_parts[i]);

		int result = 0;
		result = radix_to_bin(L2EncDec::rsa_key::d, 16, key_parts[pk_d], &key_lens[pk_d]);
		result = radix_to_bin(L2EncDec::rsa_key::e, 16, key_parts[pk_e], &key_lens[pk_e]);
		result = radix_to_bin(L2EncDec::rsa_key::N, 16, key_parts[pk_N], &key_lens[pk_N]);

		rsa_key key;
		rsa_set_key(key_parts[pk_N], key_lens[pk_N], key_parts[pk_e], key_lens[pk_e], NULL, 0, &key);
		
		/* read encrypted file */
		int raw_size = InSize - kLineageHeaderSize;
		TArray<BYTE> raw(raw_size);

		SetFilePointer(Handle, kLineageHeaderSize, NULL, FILE_BEGIN);
		FArchiveFileReader::Serialize(&raw(0), raw_size);

		/* decrypt with public RSA key */
		TArray<BYTE> decoded;
		const int block_size = 128;
		int blocks = raw_size / block_size;
		for (int i = 0; i < blocks; i++)
		{
			BYTE out[block_size]; memset(out, 0, sizeof(out));
			int out_length = sizeof(out);

			result = rsa_exptmod(&raw(0) + i * block_size, block_size, out, (unsigned long*)&out_length, PK_PUBLIC, &key);
			//GWarn->Log(NAME_Log, FString::Printf(TEXT("%s: %d stat:%d result:%d (%S)"), TEXT("FLineageArchive413FileReader::Serialize"), 2, stat, result, error_to_string(result)));

			int total_bytes = decoded.Num();
			unsigned char add_bytes = *(unsigned char*)(out + 3);

			decoded.Add(add_bytes);
			memcpy_s(&decoded(total_bytes), add_bytes, out + ((block_size - add_bytes) & 0x7C), add_bytes);
		}

		/* RSA routines completed*/
		rsa_free(&key);

		/* Decompress data */
		int uncompressed_size = *(int*)&decoded(0);
		uncompressed_ = TArray<BYTE>(uncompressed_size);
		result = uncompress(&uncompressed_(0), (uLongf *)&uncompressed_size, &decoded(4), decoded.Num() - 4);

		/* Set new Size for GetTotal() */
		Size = uncompressed_size;
	}

	~FLineageArchive413FileReader()
	{}

	void Serialize(void* V, INT Length) 
	{
		/* read from buffer */
		memcpy_s(V, Length, &uncompressed_(0), min(Length, uncompressed_.Num()));
	}

	void Seek(INT InPos) 
	{
		FArchiveFileReader::Seek(InPos);
		SetFilePointer(Handle, InPos + kLineageHeaderSize, 0, FILE_BEGIN);
	}

private:
	TArray<BYTE> uncompressed_;
};

class FArchiveFileWriter : public FArchive
{
public:
    FArchiveFileWriter( HANDLE InHandle, FOutputDevice* InError, INT InPos )
    :   Handle      ( InHandle )
    ,   Error       ( InError )
    ,   Pos         ( InPos )
    ,   BufferCount ( 0 )
    {
        ArIsSaving = ArIsPersistent = 1;
    }
    ~FArchiveFileWriter()
    {
        guard(FArchiveFileWriter::~FArchiveFileWriter);
        if( Handle )
            Close();
        Handle = NULL;
        unguard;
    }
    void Seek( INT InPos )
    {
        Flush();
        if( SetFilePointer( Handle, InPos, 0, FILE_BEGIN )==0xFFFFFFFF )
        {
            ArIsError = 1;
            Error->Logf( LocalizeError("SeekFailed",TEXT("Core")) );
        }
        Pos = InPos;
    }
    INT Tell()
    {
        return Pos;
    }
    UBOOL Close()
    {
        guardSlow(FArchiveFileWriter::Close);
        Flush();
        if( Handle && !CloseHandle(Handle) )
        {
            ArIsError = 1;
            Error->Logf( LocalizeError("WriteFailed",TEXT("Core")) );
        }
		Handle = NULL;
        return !ArIsError;
        unguardSlow;
    }
    void Serialize( void* V, INT Length )
    {
        Pos += Length;
        INT Copy;
        while( Length > (Copy=ARRAY_COUNT(Buffer)-BufferCount) )
        {
            appMemcpy( Buffer+BufferCount, V, Copy );
            BufferCount += Copy;
            Length      -= Copy;
            V            = (BYTE*)V + Copy;
            Flush();
        }
        if( Length )
        {
            appMemcpy( Buffer+BufferCount, V, Length );
            BufferCount += Length;
        }
    }
    void Flush()
    {
        if( BufferCount )
        {
            INT Result=0;
            if( !WriteFile( Handle, Buffer, BufferCount, (DWORD*)&Result, NULL ) )
            {
                ArIsError = 1;
                Error->Logf( LocalizeError("WriteFailed",TEXT("Core")) );
            }
        }
        BufferCount = 0;
    }
protected:
    HANDLE          Handle;
    FOutputDevice*  Error;
    INT             Pos;
    INT             BufferCount;
    BYTE            Buffer[4096];
};
class FFileManagerWindows : public FFileManagerGeneric
{
public:
	FArchive* CreateMemReader(INT, INT) {
		return NULL;
	}

    FArchive* CreateFileReader( const TCHAR* Filename, DWORD Flags, FOutputDevice* Error )
    {
        guard(FFileManagerWindows::CreateFileReader);
        DWORD  Access    = GENERIC_READ;
        DWORD  WinFlags  = FILE_SHARE_READ;
        DWORD  Create    = OPEN_EXISTING;
        HANDLE Handle    = TCHAR_CALL_OS( CreateFileW( Filename, Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL ), CreateFileA( TCHAR_TO_ANSI(Filename), Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL ) );
        if( Handle==INVALID_HANDLE_VALUE )
        {
            if( Flags & FILEREAD_NoFail )
                appErrorf( TEXT("Failed to read file: %s"), Filename );
            return NULL;
        }
		
		FArchive* result = NULL;
		int cryptVersion = GetLinagePackageVersion(Handle);

		if (cryptVersion == 111) {
			result = new(TEXT("Lineage2111WindowsFileReader"))FLineageArchive111FileReader(Handle, Error, GetFileSize(Handle, NULL));
		}
		else if (cryptVersion == 121) {
			result = new(TEXT("Lineage2121WindowsFileReader"))FLineageArchive121FileReader(Filename, Handle, Error, GetFileSize(Handle, NULL));
		}
		else if (cryptVersion == 413) {
			result = new(TEXT("Lineage2413WindowsFileReader"))FLineageArchive413FileReader(Filename, Handle, Error, GetFileSize(Handle, NULL));
		}
		else {
			result = new(TEXT("WindowsFileReader"))FArchiveFileReader(Handle, Error, GetFileSize(Handle, NULL));
		}
		
		return result;
        unguard;
    }
    FArchive* CreateFileWriter( const TCHAR* Filename, DWORD Flags, FOutputDevice* Error )
    {
        guard(FFileManagerWindows::CreateFileWriter);
        if( (GFileManager->FileSize (Filename) >= 0) && (Flags & FILEWRITE_EvenIfReadOnly) ) // gam
            TCHAR_CALL_OS(SetFileAttributesW(Filename, 0),SetFileAttributesA(TCHAR_TO_ANSI(Filename), 0));
        DWORD  Access    = GENERIC_WRITE;
        DWORD  WinFlags  = (Flags & FILEWRITE_AllowRead) ? FILE_SHARE_READ : 0;
        DWORD  Create    = (Flags & FILEWRITE_Append) ? OPEN_ALWAYS : (Flags & FILEWRITE_NoReplaceExisting) ? CREATE_NEW : CREATE_ALWAYS;
        HANDLE Handle    = TCHAR_CALL_OS( CreateFileW( Filename, Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL ), CreateFileA( TCHAR_TO_ANSI(Filename), Access, WinFlags, NULL, Create, FILE_ATTRIBUTE_NORMAL, NULL ) );
        INT    Pos       = 0;
        if( Handle==INVALID_HANDLE_VALUE )
        {
            if( Flags & FILEWRITE_NoFail )
                appErrorf( TEXT("Failed to create file: %s"), Filename );
            return NULL;
        }
        if( Flags & FILEWRITE_Append )
            Pos = SetFilePointer( Handle, 0, 0, FILE_END );
		return new(TEXT("WindowsFileWriter"))FArchiveFileWriter(Handle,Error,Pos);
        unguard;
    }
    INT FileSize( const TCHAR* Filename )
    {
        guard(FFileManagerWindows::FileSize);
        HANDLE Handle = TCHAR_CALL_OS( CreateFileW( Filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ), CreateFileA( TCHAR_TO_ANSI(Filename), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL ) );
        if( Handle==INVALID_HANDLE_VALUE )
            return -1;
        DWORD Result = GetFileSize( Handle, NULL );
        CloseHandle( Handle );
        return Result;
        unguard;
    }
    DWORD Copy( const TCHAR* DestFile, const TCHAR* SrcFile, UBOOL ReplaceExisting, UBOOL EvenIfReadOnly, UBOOL Attributes, DWORD Compress, FCopyProgress* Progress )
    {
        if( EvenIfReadOnly )
            TCHAR_CALL_OS(SetFileAttributesW(DestFile, 0),SetFileAttributesA(TCHAR_TO_ANSI(DestFile), 0));
        DWORD Result;
        if( Progress || Compress != FILECOPY_Normal )
			Result = FFileManagerGeneric::Copy( DestFile, SrcFile, ReplaceExisting, EvenIfReadOnly, Attributes, Compress, Progress );
		else
		{
			if( TCHAR_CALL_OS(CopyFileW(SrcFile, DestFile, !ReplaceExisting),CopyFileA(TCHAR_TO_ANSI(SrcFile), TCHAR_TO_ANSI(DestFile), !ReplaceExisting)) != 0)
				Result = COPY_OK;
			else
				Result = COPY_MiscFail;
		}
        if( Result==COPY_OK && !Attributes )
            TCHAR_CALL_OS(SetFileAttributesW(DestFile, 0),SetFileAttributesA(TCHAR_TO_ANSI(DestFile), 0));
        return Result;
    }
    UBOOL Delete( const TCHAR* Filename, UBOOL RequireExists=0, UBOOL EvenReadOnly=0 )
    {
        guard(FFileManagerWindows::Delete);
        if( EvenReadOnly )
            TCHAR_CALL_OS(SetFileAttributesW(Filename,FILE_ATTRIBUTE_NORMAL),SetFileAttributesA(TCHAR_TO_ANSI(Filename),FILE_ATTRIBUTE_NORMAL));
        // gam ---
        INT Result = TCHAR_CALL_OS(DeleteFile(Filename),DeleteFileA(TCHAR_TO_ANSI(Filename)))!=0 || (!RequireExists && GetLastError()==ERROR_FILE_NOT_FOUND);
        if( !Result )
        {
            // gam ---
            DWORD error = GetLastError();
            debugf( NAME_Warning, TEXT("Error deleting file '%s' (0x%d)"), Filename, error );
            // --- gam
        }
        return Result!=0;
        // --- gam
        unguard;
    }
    UBOOL Move( const TCHAR* Dest, const TCHAR* Src, UBOOL Replace=1, UBOOL EvenIfReadOnly=0, UBOOL Attributes=0 )
    {
        guard(FFileManagerWindows::Move);
        //warning: MoveFileEx is broken on Windows 95 (Microsoft bug).
        Delete( Dest, 0, EvenIfReadOnly );
        INT Result = TCHAR_CALL_OS( MoveFileW(Src,Dest), MoveFileA(TCHAR_TO_ANSI(Src),TCHAR_TO_ANSI(Dest)) );
        if( !Result )
        {
            // gam ---
            DWORD error = GetLastError();
            debugf( NAME_Warning, TEXT("Error moving file '%s' to '%s' (%d)"), Src, Dest, error );
            // --- gam
        }
        return Result!=0;
        unguard;
    }
    SQWORD GetGlobalTime( const TCHAR* Filename )
    {
        //return grenwich mean time as expressed in nanoseconds since the creation of the universe.
        //time is expressed in meters, so divide by the speed of light to obtain seconds.
        //assumes the speed of light in a vacuum is constant.
        //the file specified by Filename is assumed to be in your reference frame, otherwise you
        //must transform the result by the path integral of the minkowski metric tensor in order to
        //obtain the correct result.

		WIN32_FILE_ATTRIBUTE_DATA	FileAttributes;

		if(GetFileAttributesEx(Filename,GetFileExInfoStandard,&FileAttributes))
			return *(SQWORD*)&FileAttributes.ftLastWriteTime;
		else
	        return 0;
    }
    UBOOL SetGlobalTime( const TCHAR* Filename )
    {
        return 0;//!!
    }
    UBOOL MakeDirectory( const TCHAR* Path, UBOOL Tree=0 )
    {
        guard(FFileManagerWindows::MakeDirectory);
        if( Tree )
			return FFileManagerGeneric::MakeDirectory( Path, Tree );
		return TCHAR_CALL_OS( CreateDirectoryW(Path,NULL), CreateDirectoryA(TCHAR_TO_ANSI(Path),NULL) )!=0 || GetLastError()==ERROR_ALREADY_EXISTS;
        unguard;
    }
    UBOOL DeleteDirectory( const TCHAR* Path, UBOOL RequireExists=0, UBOOL Tree=0 )
    {
        guard(FFileManagerWindows::DeleteDirectory);
        if( Tree )
            return FFileManagerGeneric::DeleteDirectory( Path, RequireExists, Tree );
        return TCHAR_CALL_OS( RemoveDirectoryW(Path), RemoveDirectoryA(TCHAR_TO_ANSI(Path)) )!=0 || (!RequireExists && GetLastError()==ERROR_FILE_NOT_FOUND);
        unguard;
    }
    TArray<FString> FindFiles( const TCHAR* Filename, UBOOL Files, UBOOL Directories )
    {
        guard(FFileManagerWindows::FindFiles);
        TArray<FString> Result;
        HANDLE Handle=NULL;
#if UNICODE
        if( GUnicodeOS )
        {
            WIN32_FIND_DATAW Data;
            Handle=FindFirstFileW(Filename,&Data);
            if( Handle!=INVALID_HANDLE_VALUE )
                do
                    if
                    (   appStricmp(Data.cFileName,TEXT("."))
                    &&  appStricmp(Data.cFileName,TEXT(".."))
                    &&  ((Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?Directories:Files) )
                        new(Result)FString(Data.cFileName);
                while( FindNextFileW(Handle,&Data) );
        }
        else
#endif
        {
            WIN32_FIND_DATAA Data;
            Handle=FindFirstFileA(TCHAR_TO_ANSI(Filename),&Data);
            if( Handle!=INVALID_HANDLE_VALUE )
            {
                // gam ---
                do
                {
                    TCHAR Name[MAX_PATH];

                    winToUNICODE( Name, Data.cFileName, Min<INT>( ARRAY_COUNT(Name), winGetSizeUNICODE(Data.cFileName) ) );
                
					if
					(	appStricmp(Name,TEXT("."))
					&&	appStricmp(Name,TEXT(".."))
					&&	((Data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)?Directories:Files) )
						new(Result)FString(Name);
                }
                while( FindNextFileA(Handle,&Data) );
                // --- gam
            }
        }
        if( Handle!=INVALID_HANDLE_VALUE )
            FindClose( Handle );
        return Result;
        unguard;
    }
    UBOOL SetDefaultDirectory( const TCHAR* Filename )
    {
        guard(FFileManagerWindows::SetDefaultDirectory);
        return TCHAR_CALL_OS(SetCurrentDirectoryW(Filename),SetCurrentDirectoryA(TCHAR_TO_ANSI(Filename)))!=0;
        unguard;
    }
    FString GetDefaultDirectory()
    {
        guard(FFileManagerWindows::GetDefaultDirectory);
#if UNICODE
        if( GUnicodeOS )
        {
            TCHAR Buffer[1024]=TEXT("");
            GetCurrentDirectoryW(ARRAY_COUNT(Buffer),Buffer);
            return FString(Buffer);
        }
        else
#endif
        {
            ANSICHAR Buffer[1024]="";
            GetCurrentDirectoryA(ARRAY_COUNT(Buffer),Buffer);
            return appFromAnsi( Buffer );
        }
        unguard;
    }
};

/*-----------------------------------------------------------------------------
    The End.
-----------------------------------------------------------------------------*/

