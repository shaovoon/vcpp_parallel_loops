
// CPUBloomDoc.cpp : implementation of the CCPUBloomDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CPUBloom.h"
#endif

#include "CPUBloomDoc.h"
#include "PictureView.h"
#include "ControlView.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCPUBloomDoc

IMPLEMENT_DYNCREATE(CCPUBloomDoc, CDocument)

BEGIN_MESSAGE_MAP(CCPUBloomDoc, CDocument)
END_MESSAGE_MAP()


// CCPUBloomDoc construction/destruction

CCPUBloomDoc::CCPUBloomDoc()
{
	// TODO: add one-time construction code here

}

CCPUBloomDoc::~CCPUBloomDoc()
{
}

BOOL CCPUBloomDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CCPUBloomDoc serialization

void CCPUBloomDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CCPUBloomDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CCPUBloomDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CCPUBloomDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCPUBloomDoc diagnostics

#ifdef _DEBUG
void CCPUBloomDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCPUBloomDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCPUBloomDoc commands


BOOL CCPUBloomDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here

	return TRUE;
}

void CCPUBloomDoc::SetBloomSaturation(float val) 
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetBloomSaturation(val);
}

void CCPUBloomDoc::SetBloomIntensity(float val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetBloomIntensity(val);
}

void CCPUBloomDoc::SetBaseSaturation(float val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetBaseSaturation(val);
}

void CCPUBloomDoc::SetBaseIntensity(float val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetBaseIntensity(val);
}

void CCPUBloomDoc::SetPicZoomOut(float val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetPicZoomOut(val);
}

void CCPUBloomDoc::SetLoopMethod(int method)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->SetLoopMethod(method);
}

void CCPUBloomDoc::SetSerialBenResult(int val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));

	pCtrlView->SetSerialBenResult(val);
}

void CCPUBloomDoc::SetOpenMPBenResult(int val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));

	pCtrlView->SetOpenMPBenResult(val);
}

void CCPUBloomDoc::SetPPLBenResult(int val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));

	pCtrlView->SetPPLBenResult(val);
}

void CCPUBloomDoc::SetAutoPBenResult(int val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));

	pCtrlView->SetAutoPBenResult(val);
}

void CCPUBloomDoc::SetPForEachBenResult(int val)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));

	pCtrlView->SetPForEachBenResult(val);
}

void CCPUBloomDoc::BeginReset()
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->BeginReset();
}

void CCPUBloomDoc::EndReset()
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	pView->EndReset();

}



BOOL CCPUBloomDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	POSITION posView = GetFirstViewPosition();
	CControlView* pCtrlView = (CControlView*)(GetNextView(posView));
	CPictureView* pView = (CPictureView*)(GetNextView(posView));

	Gdiplus::Bitmap* bmp = pView->GetRenderedImage();

	CString str = PathFindExtension(lpszPathName);

	using namespace Gdiplus;

	str = str.MakeLower();
	if(str==".jpg"||str==".jpeg")
		SaveJpegFile(bmp, lpszPathName);
	else if(str==".png")
		SavePngFile(bmp, lpszPathName);
	else if(str==".bmp")
		SaveBmpFile(bmp, lpszPathName);
	else if(str==".gif")
		SaveGifFile(bmp, lpszPathName);
	else if(str==".tif"||str==".tiff")
		SaveTiffFile(bmp, lpszPathName);
	else
		SaveBmpFile(bmp, lpszPathName);

	//return CDocument::OnSaveDocument(lpszPathName);
	return TRUE;
}

bool CCPUBloomDoc::SavePngFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile)
{
	if(bmp)
	{
		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		Gdiplus::Status status = bmp->Save(pszFile, &pngClsid, NULL);
		return status == Gdiplus::Ok ? true : false;
	}

	return false;
}

bool CCPUBloomDoc::SaveJpegFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile)
{
	if(bmp)
	{
		CLSID jpegClsid;
		GetEncoderClsid(L"image/jpeg", &jpegClsid);
		using namespace Gdiplus;
		EncoderParameters params;
		params.Count = 1;
		params.Parameter[0].Guid = EncoderQuality;
		params.Parameter[0].Type = EncoderParameterValueTypeLong;
		params.Parameter[0].NumberOfValues = 1;
		ULONG quality = 80;
		params.Parameter[0].Value = &quality;
		Gdiplus::Status status = bmp->Save(pszFile, &jpegClsid, &params);
		bool bRet = status == Gdiplus::Ok ? true : false;
		return bRet;
	}

	return false;
}

bool CCPUBloomDoc::SaveBmpFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile)
{
	if(bmp)
	{
		CLSID bmpClsid;
		GetEncoderClsid(L"image/bmp", &bmpClsid);
		Gdiplus::Status status = bmp->Save(pszFile, &bmpClsid, NULL);
		return status == Gdiplus::Ok ? true : false;
	}

	return false;
}

bool CCPUBloomDoc::SaveGifFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile)
{
	if(bmp)
	{
		CLSID gifClsid;
		GetEncoderClsid(L"image/gif", &gifClsid);
		Gdiplus::Status status = bmp->Save(pszFile, &gifClsid, NULL);
		return status == Gdiplus::Ok ? true : false;
	}

	return false;
}

bool CCPUBloomDoc::SaveTiffFile(Gdiplus::Bitmap* bmp, const wchar_t* pszFile)
{
	if(bmp)
	{
		CLSID tiffClsid;
		GetEncoderClsid(L"image/tiff", &tiffClsid);
		Gdiplus::Status status = bmp->Save(pszFile, &tiffClsid, NULL);
		return status == Gdiplus::Ok ? true : false;
	}

	return false;
}

bool CCPUBloomDoc::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	using namespace Gdiplus;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return false;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return false;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return true;  // Success
		}    
	}

	free(pImageCodecInfo);
	return false;  // Failure
}
