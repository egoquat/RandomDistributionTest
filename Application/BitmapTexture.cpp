#include ".\bitmaptexture.h"

BOOL CBitmapTexture::SaveBitmap(char* lpszFileName, BYTE *bmpColorBuffers, UINT uiWidth, int uiHeight)
{
	DWORD dwSizeImage;

	//int nColorTableEntries = 256;
	int nColorTableEntries = 0;

	// 1.bmp Info Header
	LPBITMAPINFOHEADER bmpInfoHeader;				// buffer containing the BITMAPINFOHEADER

	bmpInfoHeader = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries];
	bmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader->biWidth = uiWidth;		
	bmpInfoHeader->biHeight = uiHeight;		
	bmpInfoHeader->biPlanes = 1;				
	bmpInfoHeader->biBitCount = 24;					// 8bit (GRAY LAVEL 사용) 씩 3개의 bit
	bmpInfoHeader->biCompression = BI_RGB;			// RGB모드
	bmpInfoHeader->biSizeImage = uiWidth * uiHeight * 3; 
	bmpInfoHeader->biXPelsPerMeter = 0;				// 이미지의 가로 해상도
	bmpInfoHeader->biYPelsPerMeter = 0;				// 이미지의 세로 해상도
	bmpInfoHeader->biClrUsed = 0;					// 256 색상 사용
	bmpInfoHeader->biClrImportant = 0;				// 중요한 색상 개수

	dwSizeImage = bmpInfoHeader->biSizeImage;		// 이미지 사이즈 정보    

	// 2.bmp File Header
	BITMAPFILEHEADER bmpFileHeader;							
	bmpFileHeader.bfType = 0x4d42;							// 'BM' -> BMP 파일을 의미

	// size of Buffer + header
	int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries + dwSizeImage;
	bmpFileHeader.bfSize = nSize + sizeof(BITMAPFILEHEADER); 
	bmpFileHeader.bfReserved1 = bmpFileHeader.bfReserved2 = 0;  

	//	실제 저장 공간 주소까지의 오프셋 사이즈
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries; 

	FILE* pStream = NULL;
	int errorno = fopen_s(&pStream, lpszFileName,"wb");
	if (errorno != 0)
	{
		char buf[1024];
		sprintf(buf,"Cannot open file (%s)", lpszFileName);
		MessageBox(NULL, "Open File Error", buf ,MB_OK);
		return FALSE;
	}

	fwrite( &bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, pStream );
	fwrite( bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, pStream );
	fwrite( (LPBYTE) bmpColorBuffers, bmpInfoHeader->biSizeImage, 1, pStream );

	fclose(pStream);

	pStream = 0;

	delete [] bmpInfoHeader;   

	return TRUE;    
}

void CBitmapTexture::ConvertARGBToBitmap( const UINT *aruiSrcColor, UINT uiWidth, UINT uiHeight, BYTE *arbyDstColor )
{
	UINT uiCur = 0, uiByteIdx = 0;
	UINT SrcCo, a,r,g,b; SrcCo=a=r=g=b=0;

	FillupArgbToBitmap(aruiSrcColor, uiWidth, uiHeight, _arbyColor);
}

BOOL CBitmapTexture::Initialize( const char* szFileName, UINT* Colors, UINT uiWidth, UINT uiHeight )
{
	BYTE		a, r, g, b;		a=r=g=b=0;
	UINT		uiCur = 0, uiByteIdx = 0;
	UINT		SrcCo = 0;

	BOOL resolutionCheck = SetResolution(uiWidth, uiHeight);
	if (FALSE == resolutionCheck) { return FALSE; }

	FillupArgbToBitmap(Colors, uiWidth, uiHeight, _arbyColor);

	NamingBitmapFile(szFileName, _szBmpFileName);

	return TRUE;
}

BOOL CBitmapTexture::Initialize( const char* szFileName, UINT* Colors, UINT uiResolution )
{
	return Initialize(szFileName, Colors, uiResolution, uiResolution);
}

BOOL CBitmapTexture::Initialize( const char* szFileName, UINT uiWidth, UINT uiHeight )
{
	BOOL resolutionCheck = SetResolution(uiWidth, uiHeight);
	if (FALSE == resolutionCheck) { return FALSE; }

	_arbyColor = new BYTE[_uiBitmapBufferSize];
	

	_filledUpByteColor = TRUE;

	NamingBitmapFile(szFileName, _szBmpFileName);

	return TRUE;
}

BOOL CBitmapTexture::Initialize( const char* szFileName, UINT uiResolution )
{
	return Initialize(szFileName, uiResolution, uiResolution );
}

BOOL CBitmapTexture::SaveBitmap()
{
	BOOL validated = (TRUE == _filledUpByteColor || TRUE == _filledUpUintColor);
	assert(validated);

	if(FALSE == validated) { return FALSE;}

	BOOL convertArgbToBitmap = (TRUE==_filledUpUintColor) && (FALSE==_filledUpByteColor);

	if( TRUE == convertArgbToBitmap)
	{	
		ConvertARGBToBitmap( _aruiColor, _uiWidth, _uiHeight, _arbyColor );
	}

	if( FALSE == SaveBitmap( _szBmpFileName, _arbyColor, _uiWidth, _uiHeight ) )
	{
		SAFEDELETE( _arbyColor );
		return FALSE;
	}

	SAFEDELETE( _arbyColor );
	return TRUE;
}

BOOL CBitmapTexture::Release()
{
	SAFEDELETE_ARRAY(_arbyColor);
	SAFEDELETE_ARRAY(_aruiColor);

	_uiWidth = _uiHeight = _uiBufferSize = _uiBitmapBufferSize = 0;

	_filledUpByteColor = FALSE;
	_filledUpUintColor = FALSE;

	return TRUE;
}

CBitmapTexture::CBitmapTexture(void)
{
	_arbyColor = NULL;
	_aruiColor = NULL;
	_filledUpByteColor = FALSE;
	_filledUpUintColor = FALSE;
	_uiWidth = _uiHeight = _uiBufferSize = _uiBitmapBufferSize = 0;
}

CBitmapTexture::~CBitmapTexture(void)
{
	Release();
}
