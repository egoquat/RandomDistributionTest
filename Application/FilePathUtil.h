#pragma once
#include "Common.h"
#include "Singleton.h"
#include <string.h>

class CFilePathUtil : public CSingleton< CFilePathUtil >
{
public:
	char buf[DEFAULT_FILEPATH];
	char fullpath_buf[DEFAULT_FILEPATH];

	// <- use these members directly
	char drive[10];
	char dir[DEFAULT_FILEPATH];
	char fname[256];
	char ext[32];

	void SplitPath(const char* path);
	const char* JoinPath();

	char* FullPath(const char* path);
	void FullPath( const char* szPath, char* szReturnValue );

	void AddName(const char* adding);

	const char* CutCWD(const char* path);

	void PullStr( char* szBuffer, UINT uiStartPos );

	BOOL FindStr( const char *szSrcBuffer, const char *szFindBuffer );

	int ExtractDirPath( const char* szFullFileName, 
		char* szReturnDirPath,
		UINT uiSkipPathDegree = 0 );

	BOOL ExtractFileName_( const char* szFullFileName, 
		char* szReturnFileName  );

	BOOL ExtractFileEx_( const char* szFullFileName,
		char* szReturnBuffer );

	BOOL ExtractFileName(const char* szFullFileName, 
		char* szReturnBuffer );

	BOOL ExtractFileExceptEx_( const char* szFullFileName,
		char* szReturnBuffer );

	BOOL ExtractSpotLeftBuffer_Descending( const char *szFullFileName,
		char *szReturnBuffer,
		char **szFindValues,
		const int iValueCnt,
		const int iSkipDegree,
		BOOL bExceptionSpot = TRUE );

	BOOL ExtractSpotRightBuffer_Descending( const char *szFullFileName,
		char *szReturnBuffer,
		char **szFindValues,
		const int iValueCnt,
		BOOL bExceptionSpot = TRUE );

	BOOL ConvertToDoubleQuotationPath( const char* szFileName,
		char* szReturnBuffer );

	BOOL ConvertToSingleQuotationPath( const char* szFileName,
		char* szReturnBuffer );

private:
	BOOL ConvertValues( const char* szOriginalBuffer,
		char* szReturnBuffer,
		const char* szOriginValue,
		UINT iOriginLen,
		const char* szConvertValue,
		UINT iConvertLen );

public:
	CFilePathUtil();
	virtual ~CFilePathUtil();
};
