#ifndef _MEMDC_H_
#define _MEMDC_H_

//////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:?keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//젨젨젨젨젨젨젨젨젨 Added print support.
//
// This class implements a memory Device Context


// 이 MemDC를 사용하면, 소멸자에서 실제 Paint를 하므로,
// 화면 떨림현상이 없어진다.

class CMemDC : public CDC
{
private:
	CBitmap m_bitmap; // Offscreen bitmap
	CBitmap* m_oldBitmap; // bitmap originally found in CMemDC
	CDC* m_pDC; // Saves CDC passed in constructor
	CRect m_rect; // Rectangle of drawing area.
	BOOL m_bMemDC; // TRUE if CDC really is a Memory DC.

public:
	CMemDC(CDC* pDC) : CDC(), m_oldBitmap(NULL), m_pDC(pDC)
	{
		ASSERT(m_pDC != NULL); // If you asserted here, you passed in a NULL CDC.
		
		m_bMemDC = !pDC->IsPrinting();
		
		if(m_bMemDC)
		{
			// Create a Memory DC
			CreateCompatibleDC(pDC);
			pDC->GetClipBox(&m_rect);
			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			// 아래 함수는 원본 소스에는 있었는 데, WinCE에서는 지원하지 않는 함수라서...
			// 지워버렸다.
//			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else
		{
			// Make a copy of the relevent parts of the current DC for printing
			m_bPrinting = pDC->m_bPrinting;
			m_hDC = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
	
	~CMemDC()
	{
		if(m_bMemDC)
		{
			// Copy the offscreen bitmap onto the screen.
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
				this, m_rect.left, m_rect.top, SRCCOPY);
			//Swap back the original bitmap.
			SelectObject(m_oldBitmap);
		}
		else
		{
			// All we need to do is replace the DC with an illegal value,
			// this keeps us from accidently deleting the handles associated with
			// the CDC that was passed to the constructor.
			m_hDC = m_hAttribDC = NULL;
		}
	}
	
	// Allow usage as a pointer
	CMemDC* operator->() { return this; }
	
	// Allow usage as a pointer
	operator CMemDC*() { return this; }
};

#endif
