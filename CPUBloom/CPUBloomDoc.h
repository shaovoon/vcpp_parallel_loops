
// CPUBloomDoc.h : interface of the CCPUBloomDoc class
//


#pragma once


class CCPUBloomDoc : public CDocument
{
protected: // create from serialization only
	CCPUBloomDoc();
	DECLARE_DYNCREATE(CCPUBloomDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CCPUBloomDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void SetBloomSaturation(float val);
	void SetBloomIntensity(float val);
	void SetBaseSaturation(float val);
	void SetBaseIntensity(float val);
	void SetPicZoomOut(float val);
	void SetLoopMethod(int method);

	void SetSerialBenResult(int val);
	void SetOpenMPBenResult(int val);
	void SetPPLBenResult(int val);
	void SetAutoPBenResult(int val);

	void BeginReset();
	void EndReset();

	bool SavePngFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile);
	bool SaveJpegFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile);
	bool SaveBmpFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile);
	bool SaveGifFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile);
	bool SaveTiffFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile);
	bool GetEncoderClsid(const WCHAR* format, CLSID* pClsid);


// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};
