// PictureView.cpp : implementation file
//

#include "stdafx.h"
#include "CPUBloom.h"
#include "PictureView.h"
#include "BloomEffect.h"
#include "BloomEffectSSE2.h"
#include "CPUBloomDoc.h"
#include "HyperThreading.h"
#include <ppl.h>
#include <algorithm>
#include <execution>

// CPictureView

IMPLEMENT_DYNCREATE(CPictureView, CScrollView)

CPictureView::CPictureView()
: m_pBmp(NULL)
, m_pBmpBloom(NULL)
, m_fBloomSaturation(1.0f)
, m_fBloomIntensity(1.0f)
, m_fBaseSaturation(1.0f)
, m_fBaseIntensity(1.0f)
, m_fPicZoomOut(0.0f)
, m_fPrevPicZoomOut(0.0f)
, m_nLoopMethod(0)
, m_bDirty(false)
, m_bResetInProgress(false)
, m_bSSE2(false)
{
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

	TIMECAPS tc;

	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) 
	{
		// Error; application can't continue.
	}

	m_wTimerRes = min(max(tc.wPeriodMin, TARGET_RESOLUTION), tc.wPeriodMax);
	timeBeginPeriod(m_wTimerRes); 

	//unsigned int processorPackageCount=0;
	//unsigned int processorCoreCount=0;
	//unsigned int logicalProcessorCount=0; 
	//unsigned int processorL1CacheSize=0;
	//unsigned int processorL1CacheLineSize=0;
	//unsigned int processorL2CacheSize=0;
	//unsigned int processorL2CacheLineSize=0;
	//unsigned int processorL3CacheSize=0;
	//unsigned int processorL3CacheLineSize=0;
	//bool bError = false;
	//std::wstring errMsg = L"";

	//bool bHT =  HyperThreading::GetHyperThreadingInfo (processorPackageCount, processorCoreCount, logicalProcessorCount, 
	//	processorL1CacheSize, processorL2CacheSize, processorL3CacheSize, 
	//	processorL1CacheLineSize, processorL2CacheLineSize, processorL3CacheLineSize,
	//	bError, errMsg);

	//if(bHT)
	//{
	//	m_nlogicalProcessorCount = logicalProcessorCount/2;
	//}
	//else
	//{
	//	m_nlogicalProcessorCount = logicalProcessorCount;
	//}

}

CPictureView::~CPictureView()
{
	if(m_pBmp!=NULL)
	{
		delete m_pBmp;
		m_pBmp = NULL;
	}
	if(m_pBmpBloom!=NULL)
	{
		delete m_pBmpBloom;
		m_pBmpBloom = NULL;
	}

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	timeEndPeriod(m_wTimerRes);
}


BEGIN_MESSAGE_MAP(CPictureView, CScrollView)
END_MESSAGE_MAP()


// CPictureView drawing

void CPictureView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	CDocument* pDoc = GetDocument();
	CString strFile = pDoc->GetPathName();

	if(strFile!=L"")
	{
		using namespace Gdiplus;
		if(m_pBmp!=NULL)
		{
			delete m_pBmp;
			m_pBmp = NULL;
		}
		if(m_pBmpBloom!=NULL)
		{
			delete m_pBmpBloom;
			m_pBmpBloom = NULL;
		}

		try
		{
			m_pBmp = new Bitmap(strFile);
		}
		catch (CMemoryException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}
		catch (CFileException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}
		catch (CException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}

		m_pBmpBloom = m_pBmp->Clone(0,0,m_pBmp->GetWidth(),m_pBmp->GetHeight(),PixelFormat32bppARGB);
		sizeTotal.cx = m_pBmpBloom->GetWidth();
		sizeTotal.cy = m_pBmpBloom->GetHeight();
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
	else // load from resource
	{
		using namespace Gdiplus;
		if(m_pBmp!=NULL)
		{
			delete m_pBmp;
			m_pBmp = NULL;
		}
		if(m_pBmpBloom!=NULL)
		{
			delete m_pBmpBloom;
			m_pBmpBloom = NULL;
		}

		try
		{
			GetImageFromResource(&m_pBmp, 312);
		}
		catch (CMemoryException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}
		catch (CFileException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}
		catch (CException* e)
		{
			MessageBox(L"Image file cannot be opened", L"Error");
		}

		m_pBmpBloom = m_pBmp->Clone(0,0,m_pBmp->GetWidth(),m_pBmp->GetHeight(),PixelFormat32bppARGB);
		sizeTotal.cx = m_pBmpBloom->GetWidth();
		sizeTotal.cy = m_pBmpBloom->GetHeight();
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
}

void CPictureView::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
	// TODO: add draw code here

	if(m_pBmp!=NULL&&m_pBmpBloom!=NULL)
	{
		using namespace Gdiplus;
		Graphics g(pDC->GetSafeHdc());
		//g.DrawImage(m_pBmpBloom, 0,0,m_pBmpBloom->GetWidth(),m_pBmpBloom->GetHeight());

		float zoom = m_fPicZoomOut + 1.0f;
		zoom = 1.0f/zoom;

		if(m_bDirty)
		{
			//if (m_bSSE2)
			//	OutputDebugStringA("SSE2=1");
			//else
			//	OutputDebugStringA("SSE2=0");

			if(m_nLoopMethod==BENCHMARK_SERIAL)
			{
				BenchmarkSerial();
			}
			else if(m_nLoopMethod==BENCHMARK_OPENMP)
			{
				BenchmarkOpenMP();
			}
			else if(m_nLoopMethod==BENCHMARK_PPL)
			{
				BenchmarkPPL();
			}
			else if(m_nLoopMethod==BENCHMARK_AUTOP)
			{
				BenchmarkAutoP();
			}
			else if (m_nLoopMethod == BENCHMARK_PARALLEL_FOREACH)
			{
				BenchmarkParallelForEach();
			}
		}

		m_bDirty = false;

		g.DrawImage(m_pBmpBloom, 
			Gdiplus::Rect(0,0,m_pBmpBloom->GetWidth() * zoom, m_pBmpBloom->GetHeight() * zoom),
			0,0,m_pBmpBloom->GetWidth(),m_pBmpBloom->GetHeight(), UnitPixel);
	}
}


// CPictureView diagnostics

#ifdef _DEBUG
void CPictureView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPictureView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPictureView message handlers
void CPictureView::SetTarget(CWnd* pWnd)
{
	m_pWndTarget = pWnd;
}


BOOL CPictureView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(m_pWndTarget)
	{           
		return m_pWndTarget->SendMessage(WM_COMMAND, wParam, lParam);
	}
	else
	{
		return CScrollView::OnCommand(wParam, lParam);
	}

	//return CScrollView::OnCommand(wParam, lParam);
}

void CPictureView::SetBloomSaturation(float val) 
{
	m_fBloomSaturation=val;
	m_bDirty = true;
	if(m_bResetInProgress == false)
		Invalidate(FALSE);
}

void CPictureView::SetBloomIntensity(float val) 
{
	m_fBloomIntensity=val;
	m_bDirty = true;
	if(m_bResetInProgress == false)
		Invalidate(FALSE);
}

void CPictureView::SetBaseSaturation(float val) 
{
	m_fBaseSaturation=val;
	m_bDirty = true;
	if(m_bResetInProgress == false)
		Invalidate(FALSE);
}

void CPictureView::SetBaseIntensity(float val) 
{
	m_fBaseIntensity=val;
	m_bDirty = true;
	if(m_bResetInProgress == false)
		Invalidate(FALSE);
}

void CPictureView::SetPicZoomOut(float val) 
{
	float fPrevPicZoomOut = m_fPicZoomOut;
	m_fPicZoomOut=val;

	float zoom = m_fPicZoomOut + 1.0f;
	zoom = 1.0f/zoom;

	if(m_pBmpBloom!=NULL)
	{
		CSize sizeTotal;
		sizeTotal.cx = m_pBmpBloom->GetWidth() * zoom;
		sizeTotal.cy = m_pBmpBloom->GetHeight() * zoom;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
	else
	{
		CSize sizeTotal;
		sizeTotal.cx = 100;
		sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	m_bDirty = true;

	if(m_bResetInProgress == false)
	{
		if(m_fPicZoomOut>fPrevPicZoomOut) // curr pic is smaller than prev pic
			Invalidate(TRUE);
		else
			Invalidate(FALSE);
	}
}

void CPictureView::SetLoopMethod(int method) 
{
	m_nLoopMethod=method;
	m_bDirty = true;

	if(m_bResetInProgress == false)
		Invalidate(FALSE);
}

void CPictureView::SetSSE2(bool enable)
{
	m_bSSE2 = enable;
	m_bDirty = true;

	if (m_bResetInProgress == false)
		Invalidate(FALSE);
}

bool CPictureView::BenchmarkSerial()
{
	if(m_pBmp==NULL||m_pBmpBloom==NULL)
		return false;

	UINT* pixelsSrc = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataSrc;
	BitmapData bitmapDataDest;
	Rect rectDest(0, 0, m_pBmpBloom->GetWidth(), m_pBmpBloom->GetHeight() );
	Rect rectSrc(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight() );

	m_pBmp->LockBits(
		&rectSrc,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataSrc );

	m_pBmpBloom->LockBits(
		&rectDest,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataDest );

	pixelsSrc = (UINT*)bitmapDataSrc.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if( !pixelsSrc || !pixelsDest )
		return false;

	int stride = bitmapDataDest.Stride >> 2;

	DWORD startTime = timeGetTime();

	if (m_bSSE2)
	{
		for (UINT row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffectSSE2 effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}
	else
	{
		for (UINT row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffect effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}

	DWORD endTime = timeGetTime();
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetSerialBenResult(endTime-startTime);

	m_pBmpBloom->UnlockBits(&bitmapDataDest);
	m_pBmp->UnlockBits(&bitmapDataSrc);

	return true;
}

bool CPictureView::BenchmarkOpenMP()
{
	if(m_pBmp==NULL||m_pBmpBloom==NULL)
		return false;

	UINT* pixelsSrc = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataSrc;
	BitmapData bitmapDataDest;
	Rect rectDest(0, 0, m_pBmpBloom->GetWidth(), m_pBmpBloom->GetHeight() );
	Rect rectSrc(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight() );

	m_pBmp->LockBits(
		&rectSrc,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataSrc );

	m_pBmpBloom->LockBits(
		&rectDest,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataDest );

	pixelsSrc = (UINT*)bitmapDataSrc.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if( !pixelsSrc || !pixelsDest )
		return false;

	int stride = bitmapDataDest.Stride >> 2;

	DWORD startTime = timeGetTime();

	if (m_bSSE2)
	{
#pragma omp parallel for
		for (int row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffectSSE2 effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}
	else
	{
#pragma omp parallel for
		for (int row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffect effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}
	DWORD endTime = timeGetTime();
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetOpenMPBenResult(endTime-startTime);

	m_pBmpBloom->UnlockBits(&bitmapDataDest);
	m_pBmp->UnlockBits(&bitmapDataSrc);

	return true;
}

bool CPictureView::BenchmarkPPL()
{
	if(m_pBmp==NULL||m_pBmpBloom==NULL)
		return false;

	UINT* pixelsSrc = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataSrc;
	BitmapData bitmapDataDest;
	Rect rectDest(0, 0, m_pBmpBloom->GetWidth(), m_pBmpBloom->GetHeight() );
	Rect rectSrc(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight() );

	m_pBmp->LockBits(
		&rectSrc,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataSrc );

	m_pBmpBloom->LockBits(
		&rectDest,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataDest );

	pixelsSrc = (UINT*)bitmapDataSrc.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if( !pixelsSrc || !pixelsDest )
		return false;

	int stride = bitmapDataDest.Stride >> 2;

	DWORD startTime = timeGetTime();

	using namespace Concurrency;
	if (m_bSSE2)
	{
		parallel_for((UINT)0, bitmapDataDest.Height, [&](UINT row)
			{
				BloomEffectSSE2 effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
				for (UINT col = 0; col < bitmapDataDest.Width; ++col)
				{
					UINT index = row * stride + col;

					pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
				}
			});
	}
	else
	{
		parallel_for((UINT)0, bitmapDataDest.Height, [&](UINT row)
			{
				BloomEffect effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
				for (UINT col = 0; col < bitmapDataDest.Width; ++col)
				{
					UINT index = row * stride + col;

					pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
				}
			});
	}
	DWORD endTime = timeGetTime();
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetPPLBenResult(endTime-startTime);

	m_pBmpBloom->UnlockBits(&bitmapDataDest);
	m_pBmp->UnlockBits(&bitmapDataSrc);

	return true;
}

bool CPictureView::BenchmarkAutoP()
{
	if(m_pBmp==NULL||m_pBmpBloom==NULL)
		return false;

	UINT* pixelsSrc = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataSrc;
	BitmapData bitmapDataDest;
	Rect rectDest(0, 0, m_pBmpBloom->GetWidth(), m_pBmpBloom->GetHeight() );
	Rect rectSrc(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight() );

	m_pBmp->LockBits(
		&rectSrc,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataSrc );

	m_pBmpBloom->LockBits(
		&rectDest,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataDest );

	pixelsSrc = (UINT*)bitmapDataSrc.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if( !pixelsSrc || !pixelsDest )
		return false;

	int stride = bitmapDataDest.Stride >> 2;

	DWORD startTime = timeGetTime();

	if (m_bSSE2)
	{
#pragma loop(hint_parallel(8))
		for (UINT row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffectSSE2 effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}
	else
	{
#pragma loop(hint_parallel(8))
		for (UINT row = 0; row < bitmapDataDest.Height; ++row)
		{
			BloomEffect effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
			for (UINT col = 0; col < bitmapDataDest.Width; ++col)
			{
				UINT index = row * stride + col;

				pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
			}
		}
	}
	DWORD endTime = timeGetTime();
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetAutoPBenResult(endTime-startTime);

	m_pBmpBloom->UnlockBits(&bitmapDataDest);
	m_pBmp->UnlockBits(&bitmapDataSrc);

	return true;
}

bool CPictureView::BenchmarkParallelForEach()
{
	if (m_pBmp == NULL || m_pBmpBloom == NULL)
		return false;

	UINT* pixelsSrc = NULL;
	UINT* pixelsDest = NULL;

	using namespace Gdiplus;

	BitmapData bitmapDataSrc;
	BitmapData bitmapDataDest;
	Rect rectDest(0, 0, m_pBmpBloom->GetWidth(), m_pBmpBloom->GetHeight());
	Rect rectSrc(0, 0, m_pBmp->GetWidth(), m_pBmp->GetHeight());

	m_pBmp->LockBits(
		&rectSrc,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataSrc);

	m_pBmpBloom->LockBits(
		&rectDest,
		ImageLockModeRead,
		PixelFormat32bppARGB,
		&bitmapDataDest);

	pixelsSrc = (UINT*)bitmapDataSrc.Scan0;
	pixelsDest = (UINT*)bitmapDataDest.Scan0;

	if (!pixelsSrc || !pixelsDest)
		return false;

	int stride = bitmapDataDest.Stride >> 2;

	std::vector<UINT> vec_cnt(bitmapDataDest.Height);
	std::iota(std::begin(vec_cnt), std::end(vec_cnt), 0);

	DWORD startTime = timeGetTime();

	if (m_bSSE2)
	{
		std::for_each(std::execution::par, std::begin(vec_cnt), std::end(vec_cnt), [&](UINT row)
			{
				BloomEffectSSE2 effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
				for (UINT col = 0; col < bitmapDataDest.Width; ++col)
				{
					UINT index = row * stride + col;

					pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
				}
			});
	}
	else
	{
		std::for_each(std::execution::par, std::begin(vec_cnt), std::end(vec_cnt), [&](UINT row)
			{
				BloomEffect effect(m_fBloomIntensity, m_fBloomSaturation, m_fBaseIntensity, m_fBaseSaturation);
				for (UINT col = 0; col < bitmapDataDest.Width; ++col)
				{
					UINT index = row * stride + col;

					pixelsDest[index] = effect.ComputeBloomInt(pixelsSrc[index]);
				}
			});
	}
	DWORD endTime = timeGetTime();
	CCPUBloomDoc* pDoc = (CCPUBloomDoc*)(GetDocument());
	pDoc->SetPForEachBenResult(endTime - startTime);

	m_pBmpBloom->UnlockBits(&bitmapDataDest);
	m_pBmp->UnlockBits(&bitmapDataSrc);

	return true;
}

void CPictureView::BeginReset()
{
	m_bResetInProgress = true;
}

void CPictureView::EndReset()
{
	m_bResetInProgress = false;
	Invalidate(TRUE);
}

bool CPictureView::GetImageFromResource(Gdiplus::Bitmap** ppImage, int nResID)
{
	HINSTANCE hResInstance = AfxGetResourceHandle( );

	HRSRC res = FindResource(hResInstance,
		MAKEINTRESOURCE(nResID),L"BINARY");
	if (res) 
	{
		HGLOBAL mem = LoadResource(hResInstance, res);
		void *data = LockResource(mem);
		size_t len = SizeofResource(hResInstance, res);

		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, len);
		LPVOID pvData = GlobalLock( hGlobal );
		memcpy(pvData,data,len);
		GlobalUnlock(hGlobal);

		LPSTREAM pStream = NULL;
		HRESULT hr = CreateStreamOnHGlobal( hGlobal, TRUE, &pStream );

		using namespace Gdiplus;
		*ppImage = Bitmap::FromStream(pStream,true);

		pStream->Release();

		return true;
	}
	return false;
}
