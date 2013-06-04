#include ".\bitmaptexture.h"

BOOL CBitmapTexture::SaveBitmap(char* lpszFileName, BYTE *bitmapColors, UINT uiWidth, int uiHeight)
{
	DWORD dwSizeImage;

	//int nColorTableEntries = 256;
	int nColorTableEntries = 0;

	// bmp Info Header
	LPBITMAPINFOHEADER bitmapBuffer;				// buffer containing the BITMAPINFOHEADER

	bitmapBuffer = (LPBITMAPINFOHEADER) new char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries];
	bitmapBuffer->biSize = sizeof(BITMAPINFOHEADER);
	bitmapBuffer->biWidth = uiWidth;		
	bitmapBuffer->biHeight = uiHeight;		
	bitmapBuffer->biPlanes = 1;				
	bitmapBuffer->biBitCount = 24;					// 8bit (GRAY LAVEL ���) �� 3���� bit
	bitmapBuffer->biCompression = BI_RGB;			// RGB���
	bitmapBuffer->biSizeImage = uiWidth*uiHeight*3; 
	bitmapBuffer->biXPelsPerMeter = 0;				// �̹����� ���� �ػ�
	bitmapBuffer->biYPelsPerMeter = 0;				// �̹����� ���� �ػ�
	bitmapBuffer->biClrUsed = 0;					// 256 ���� ���
	bitmapBuffer->biClrImportant = 0;				// �߿��� ���� ����

	dwSizeImage = bitmapBuffer->biSizeImage;		// �̹��� ������ ����    

	BITMAPFILEHEADER bmfh;							// Header Bitmap

	bmfh.bfType = 0x4d42;							// 'BM' -> BMP ������ �ǹ�

	// size of Buffer + header
	int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries + dwSizeImage;
	bmfh.bfSize = nSize + sizeof(BITMAPFILEHEADER); 
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;  

	//	���� ���� ���� �ּұ����� ������ ������
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * nColorTableEntries; 

	FILE* pStream = NULL;
	int errorno = fopen_s(&pStream, lpszFileName,"wb");
	if (errorno != 0)
	{
		char buf[1024];
		sprintf(buf,"Cannot open file (%s)", lpszFileName);
		MessageBox(NULL, "Open File Error", buf ,MB_OK);
		return FALSE;
	}

	fwrite( &bmfh, sizeof(BITMAPFILEHEADER), 1, pStream );
	fwrite( bitmapBuffer, sizeof(BITMAPINFOHEADER), 1, pStream );
	fwrite( (LPBYTE) bitmapColors, bitmapBuffer->biSizeImage, 1, pStream );

	fclose(pStream);

	pStream = 0;

	delete [] bitmapBuffer;   

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
