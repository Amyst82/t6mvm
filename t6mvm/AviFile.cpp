#include "pch.h"
#include "AviFile.h"
#define _CRT_SECURE_NO_WARNINGS

#ifndef __countof
#define __countof(x)	((sizeof(x)/sizeof(x[0])))
#endif

CAviFile::CAviFile(const char* lpszFileName /* ="Output.avi") */,
	DWORD dwCodec /* = mmioFOURCC('M','P','G','4') */,
	DWORD dwFrameRate /* = 1 */)
{

	//	AVIFileInit();

	m_hHeap = NULL;
	m_hAviDC = NULL;
	m_lpBits = NULL;
	m_lSample = NULL;
	m_pAviFile = NULL;
	m_pAviStream = NULL;
	m_pAviCompressedStream = NULL;

	m_dwFCCHandler = dwCodec;
	m_dwFrameRate = dwFrameRate;

	strcpy_s(m_szFileName, lpszFileName);
	strcpy_s(m_szErrMsg, "Method Succeeded");
	m_szErrMsg[__countof(m_szErrMsg) - 1] = '\0';

	pAppendFrame[0] = &CAviFile::AppendDummy;			// VC8 requires & for Function Pointer; Remove it if your compiler complains;
	pAppendFrame[1] = &CAviFile::AppendFrameFirstTime;
	pAppendFrame[2] = &CAviFile::AppendFrameUsual;

	pAppendFrameBits[0] = &CAviFile::AppendDummy;
	pAppendFrameBits[1] = &CAviFile::AppendFrameFirstTime;
	pAppendFrameBits[2] = &CAviFile::AppendFrameUsual;

	m_nAppendFuncSelector = 1;		//0=Dummy	1=FirstTime	2=Usual
}

CAviFile::~CAviFile(void)
{
	ReleaseMemory();

	AVIFileExit();
}

void CAviFile::ReleaseMemory()
{
	m_nAppendFuncSelector = 0;		//Point to DummyFunction

	if (m_hAviDC)
	{
		DeleteDC(m_hAviDC);
		m_hAviDC = NULL;
	}
	if (m_pAviCompressedStream)
	{
		AVIStreamRelease(m_pAviCompressedStream);
		m_pAviCompressedStream = NULL;
	}
	if (m_pAviStream)
	{
		AVIStreamRelease(m_pAviStream);
		m_pAviStream = NULL;
	}
	if (m_pAviFile)
	{
		AVIFileRelease(m_pAviFile);
		m_pAviFile = NULL;
	}
	if (m_lpBits)
	{
		HeapFree(m_hHeap, HEAP_NO_SERIALIZE, m_lpBits);
		m_lpBits = NULL;
	}
	if (m_hHeap)
	{
		HeapDestroy(m_hHeap);
		m_hHeap = NULL;
	}
}

void CAviFile::SetErrorMessage(LPCTSTR lpszErrorMessage)
{
	strcpy_s(m_szErrMsg, __countof(m_szErrMsg) - 1, lpszErrorMessage);
}

HRESULT CAviFile::InitMovieCreation(int nFrameWidth, int nFrameHeight, int nBitsPerPixel)
{
	AVIFileInit();

	int	nMaxWidth = GetSystemMetrics(SM_CXSCREEN), nMaxHeight = GetSystemMetrics(SM_CYSCREEN);

	m_hAviDC = CreateCompatibleDC(NULL);
	if (m_hAviDC == NULL)
	{
		SetErrorMessage("Unable to Create Compatible DC");
		return E_FAIL;
	}

	if (nFrameWidth > nMaxWidth)	nMaxWidth = nFrameWidth;
	if (nFrameHeight > nMaxHeight)	nMaxHeight = nFrameHeight;

	m_hHeap = HeapCreate(HEAP_NO_SERIALIZE, nMaxWidth * nMaxHeight * 4, 0);
	if (m_hHeap == NULL)
	{
		SetErrorMessage("Unable to Create Heap");
		return E_FAIL;
	}

	m_lpBits = HeapAlloc(m_hHeap, HEAP_ZERO_MEMORY | HEAP_NO_SERIALIZE, nMaxWidth * nMaxHeight * 4);
	if (m_lpBits == NULL)
	{
		SetErrorMessage("Unable to Allocate Memory on Heap");
		return E_FAIL;
	}
	HRESULT result = AVIFileOpen(&m_pAviFile, m_szFileName, OF_CREATE | OF_WRITE, NULL);
	if (FAILED(result))
	{
		SetErrorMessage("Unable to Create the Movie File");
		switch (result) {
		case AVIERR_BADFORMAT: SetErrorMessage("Bad file format"); break;
		case REGDB_E_CLASSNOTREG: SetErrorMessage("Avi file type not registered"); break;
		case AVIERR_MEMORY: SetErrorMessage("Insufficient memory"); break;
		case AVIERR_FILEOPEN: SetErrorMessage("File already opened"); break;
		case AVIERR_FILEREAD: SetErrorMessage("File read problem"); break;
		default: SetErrorMessage("Unable to Create the Movie File");
		}
		return E_FAIL;
	}

	ZeroMemory(&m_AviStreamInfo, sizeof(AVISTREAMINFO));
	m_AviStreamInfo.fccType = streamtypeVIDEO;
	m_AviStreamInfo.fccHandler = m_dwFCCHandler;
	m_AviStreamInfo.dwScale = 1;
	m_AviStreamInfo.dwRate = m_dwFrameRate;	// Frames Per Second;
	m_AviStreamInfo.dwQuality = -1;				// Default Quality
	m_AviStreamInfo.dwSuggestedBufferSize = nMaxWidth * nMaxHeight * 4;
	SetRect(&m_AviStreamInfo.rcFrame, 0, 0, nFrameWidth, nFrameHeight);
	strcpy_s(m_AviStreamInfo.szName, "Video Stream");

	if (FAILED(AVIFileCreateStream(m_pAviFile, &m_pAviStream, &m_AviStreamInfo)))
	{
		SetErrorMessage("Unable to Create Video Stream in the Movie File");
		return E_FAIL;
	}

	ZeroMemory(&m_AviCompressOptions, sizeof(AVICOMPRESSOPTIONS));
	m_AviCompressOptions.fccType = streamtypeVIDEO;
	m_AviCompressOptions.fccHandler = m_AviStreamInfo.fccHandler;
	m_AviCompressOptions.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
	m_AviCompressOptions.dwKeyFrameEvery = 1;
	//m_AviCompressOptions.dwBytesPerSecond=1000/8;
	//m_AviCompressOptions.dwQuality=100;

	if (FAILED(AVIMakeCompressedStream(&m_pAviCompressedStream, m_pAviStream, &m_AviCompressOptions, NULL)))
	{
		// One reason this error might occur is if you are using a Codec that is not 
		// available on your system. Check the mmioFOURCC() code you are using and make
		// sure you have that codec installed properly on your machine.
		SetErrorMessage("Unable to Create Compressed Stream: Check your CODEC options");
		return E_FAIL;
	}

	BITMAPINFO bmpInfo;
	ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biWidth = nFrameWidth;
	bmpInfo.bmiHeader.biHeight = nFrameHeight;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biBitCount = nBitsPerPixel;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight * bmpInfo.bmiHeader.biBitCount / 8;

	if (FAILED(AVIStreamSetFormat(m_pAviCompressedStream, 0, (LPVOID)&bmpInfo, bmpInfo.bmiHeader.biSize)))
	{
		// One reason this error might occur is if your bitmap does not meet the Codec requirements.
		// For example, 
		//   your bitmap is 32bpp while the Codec supports only 16 or 24 bpp; Or
		//   your bitmap is 274 * 258 size, while the Codec supports only sizes that are powers of 2; etc...
		// Possible solution to avoid this is: make your bitmap suit the codec requirements,
		// or Choose a codec that is suitable for your bitmap.
		SetErrorMessage("Unable to Set Video Stream Format");
		return E_FAIL;
	}

	return S_OK;	// Everything went Fine
}


HRESULT	CAviFile::AppendFrameFirstTime(HBITMAP	hBitmap)
{
	BITMAP Bitmap;

	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

	if (SUCCEEDED(InitMovieCreation(Bitmap.bmWidth,
		Bitmap.bmHeight,
		Bitmap.bmBitsPixel)))
	{
		m_nAppendFuncSelector = 2;		//Point to the UsualAppend Function

		return AppendFrameUsual(hBitmap);
	}

	ReleaseMemory();

	return E_FAIL;
}

HRESULT CAviFile::AppendFrameUsual(HBITMAP hBitmap)
{
	BITMAPINFO	bmpInfo;

	bmpInfo.bmiHeader.biBitCount = 0;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	GetDIBits(m_hAviDC, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

	bmpInfo.bmiHeader.biCompression = BI_RGB;

	GetDIBits(m_hAviDC, hBitmap, 0, bmpInfo.bmiHeader.biHeight, m_lpBits, &bmpInfo, DIB_RGB_COLORS);

	if (FAILED(AVIStreamWrite(m_pAviCompressedStream, m_lSample++, 1, m_lpBits, bmpInfo.bmiHeader.biSizeImage, 0, NULL, NULL)))
	{
		SetErrorMessage("Unable to Write Video Stream to the output Movie File");

		ReleaseMemory();

		return E_FAIL;
	}

	return S_OK;
}

HRESULT CAviFile::AppendDummy(HBITMAP)
{
	return E_FAIL;
}

HRESULT CAviFile::AppendNewFrame(HBITMAP hBitmap)
{
	return (this->*pAppendFrame[m_nAppendFuncSelector])((HBITMAP)hBitmap);
}

HRESULT	CAviFile::AppendNewFrame(int nWidth, int nHeight, LPVOID pBits, int nBitsPerPixel)
{
	return (this->*pAppendFrameBits[m_nAppendFuncSelector])(nWidth, nHeight, pBits, nBitsPerPixel);
}

HRESULT	CAviFile::AppendFrameFirstTime(int nWidth, int nHeight, LPVOID pBits, int nBitsPerPixel)
{
	if (SUCCEEDED(InitMovieCreation(nWidth, nHeight, nBitsPerPixel)))
	{
		m_nAppendFuncSelector = 2;		//Point to the UsualAppend Function

		return AppendFrameUsual(nWidth, nHeight, pBits, nBitsPerPixel);
	}

	ReleaseMemory();

	return E_FAIL;
}

HRESULT	CAviFile::AppendFrameUsual(int nWidth, int nHeight, LPVOID pBits, int nBitsPerPixel)
{
	DWORD	dwSize = nWidth * nHeight * nBitsPerPixel / 8;

	if (FAILED(AVIStreamWrite(m_pAviCompressedStream, m_lSample++, 1, pBits, dwSize, 0, NULL, NULL)))
	{
		SetErrorMessage("Unable to Write Video Stream to the output Movie File");

		ReleaseMemory();

		return E_FAIL;
	}

	return S_OK;
}

HRESULT	CAviFile::AppendDummy(int nWidth, int nHeight, LPVOID pBits, int nBitsPerPixel)
{
	return E_FAIL;
}