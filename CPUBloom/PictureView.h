#pragma once



// CPictureView view

class CPictureView : public CScrollView
{
	DECLARE_DYNCREATE(CPictureView)

protected:
	CPictureView();           // protected constructor used by dynamic creation
	virtual ~CPictureView();
	void SetTarget(CWnd* pWnd);
protected:
	CWnd* m_pWndTarget;
	Gdiplus::Bitmap* m_pBmp;
	Gdiplus::Bitmap* m_pBmpBloom;
	UINT     m_wTimerRes;
	float m_fBloomSaturation;
	float m_fBloomIntensity;
	float m_fBaseSaturation;
	float m_fBaseIntensity;
	float m_fPicZoomOut;
	float m_fPrevPicZoomOut;
	int m_nLoopMethod;
	int m_nSIMDMethod;
	bool m_bDirty;
	unsigned int m_nlogicalProcessorCount; 
	bool m_bSSE2;

public:
	void SetBloomSaturation(float val);
	void SetBloomIntensity(float val);
	void SetBaseSaturation(float val);
	void SetBaseIntensity(float val);
	void SetPicZoomOut(float val);
	void SetLoopMethod(int method);
	void SetSIMDMethod(int method);
	void SetSSE2(bool enable);


	bool BenchmarkSerial();
	bool BenchmarkOpenMP();
	bool BenchmarkPPL();
	bool BenchmarkAutoP();
	bool BenchmarkParallelForEach();

	void BeginReset();
	void EndReset();

	Gdiplus::Bitmap* GetRenderedImage() {return m_pBmpBloom;};

	bool GetImageFromResource(Gdiplus::Bitmap** ppImage, int nResID);


private:
	Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_gdiplusToken;
	bool m_bResetInProgress;

public:

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


