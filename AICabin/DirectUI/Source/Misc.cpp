#include "stdafx.h"
#include "Misc.h"

namespace dui{
//////////////////////////////////////////////////////////////////////////
//CRectX
//////////////////////////////////////////////////////////////////////////
CRectX::CRectX()
{
	left = top = right = bottom = 0;
}

CRectX::CRectX(const RECT& src)
{
	::CopyRect(this, &src);
}

CRectX::CRectX(int _left, int _top, int _right, int _bottom)
{
	left = _left;
	top = _top;
	right = _right;
	bottom = _bottom;
}

CRectX::CRectX(LPCRECT lpSrcRect)
{ 
	::CopyRect(this, lpSrcRect);
}

int CRectX::GetWidth() const
{
	return right - left;
}

int CRectX::GetHeight() const
{
	return bottom - top;
}

void CRectX::Join(const RECT& rc)
{
	if( rc.left < left )
	{
		left = rc.left;
	}

	if( rc.top < top )
	{
		top = rc.top;
	}

	if( rc.right > right ) 
	{
		right = rc.right;
	}

	if( rc.bottom > bottom )
	{
		bottom = rc.bottom;
	}
}

CRectX::CRectX(POINT point, SIZE size)
{ 
	right = (left = point.x) + size.cx; bottom = (top = point.y) + size.cy; 
}
CRectX::CRectX(POINT topLeft, POINT bottomRight)
{ 
	left = topLeft.x; top = topLeft.y;
	right = bottomRight.x; bottom = bottomRight.y;
}

int CRectX::Width() const
{ return right - left; }

int CRectX::Height() const
{ return bottom - top; }

CSizeX CRectX::Size() const
{ return CSizeX(right - left, bottom - top); }

CPointX& CRectX::TopLeft()
{ return *((CPointX*)this); }

CPointX& CRectX::BottomRight()
{ return *((CPointX*)this+1); }

const CPointX& CRectX::TopLeft() const
{ return *((CPointX*)this); }

const CPointX& CRectX::BottomRight() const
{ return *((CPointX*)this+1); }

CPointX CRectX::CenterPoint() const
{ return CPointX((left+right)/2, (top+bottom)/2); }

void CRectX::SwapLeftRight()
{ SwapLeftRight(LPRECT(this)); }

void CRectX::SwapLeftRight(LPRECT lpRect)
{ LONG temp = lpRect->left; lpRect->left = lpRect->right; lpRect->right = temp; }

CRectX::operator LPRECT()
{ return this; }

CRectX::operator LPCRECT() const
{ return this; }

BOOL CRectX::IsRectEmpty() const
{ return ::IsRectEmpty(this); }

BOOL CRectX::IsRectNull() const
{ return (left == 0 && right == 0 && top == 0 && bottom == 0); }

BOOL CRectX::PtInRect(POINT point) const
{ return ::PtInRect(this, point); }

void CRectX::SetRect(int x1, int y1, int x2, int y2)
{ ::SetRect(this, x1, y1, x2, y2); }

void CRectX::SetRect(POINT topLeft, POINT bottomRight)
{ ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y); }

void CRectX::SetRectEmpty()
{ ::SetRectEmpty(this); }

void CRectX::CopyRect(LPCRECT lpSrcRect)
{ ::CopyRect(this, lpSrcRect); }

BOOL CRectX::EqualRect(LPCRECT lpRect) const
{ return ::EqualRect(this, lpRect); }


void CRectX::InflateRect(int x, int y)
{ ::InflateRect(this, x, y); }

void CRectX::InflateRect(SIZE size)
{ ::InflateRect(this, size.cx, size.cy); }

void CRectX::DeflateRect(int x, int y)
{ ::InflateRect(this, -x, -y); }

void CRectX::DeflateRect(SIZE size)
{ ::InflateRect(this, -size.cx, -size.cy); }

void CRectX::OffsetRect(int x, int y)
{ ::OffsetRect(this, x, y); }

void CRectX::OffsetRect(POINT point)
{ ::OffsetRect(this, point.x, point.y); }

void CRectX::OffsetRect(SIZE size)
{ ::OffsetRect(this, size.cx, size.cy); }

BOOL CRectX::IntersectRect(LPCRECT lpRect1, LPCRECT lpRect2)
{ return ::IntersectRect(this, lpRect1, lpRect2);}

BOOL CRectX::UnionRect(LPCRECT lpRect1, LPCRECT lpRect2)
{ return ::UnionRect(this, lpRect1, lpRect2); }

void CRectX::operator=(const RECT& srcRect)
{ ::CopyRect(this, &srcRect); }

BOOL CRectX::operator==(const RECT& rect) const
{ return ::EqualRect(this, &rect); }

BOOL CRectX::operator!=(const RECT& rect) const
{ return !::EqualRect(this, &rect); }

void CRectX::operator+=(POINT point)
{ ::OffsetRect(this, point.x, point.y); }

void CRectX::operator+=(SIZE size)
{ ::OffsetRect(this, size.cx, size.cy); }

void CRectX::operator+=(LPCRECT lpRect)
{ InflateRect(lpRect); }

void CRectX::operator-=(POINT point)
{ ::OffsetRect(this, -point.x, -point.y); }

void CRectX::operator-=(SIZE size)
{ ::OffsetRect(this, -size.cx, -size.cy); }

void CRectX::operator-=(LPCRECT lpRect)
{ DeflateRect(lpRect); }

void CRectX::operator&=(const RECT& rect)
{ ::IntersectRect(this, this, &rect); }

void CRectX::operator|=(const RECT& rect)
{ ::UnionRect(this, this, &rect); }

CRectX CRectX::operator+(POINT pt) const
{ CRectX rect(*this); ::OffsetRect(&rect, pt.x, pt.y); return rect; }

CRectX CRectX::operator-(POINT pt) const
{ CRectX rect(*this); ::OffsetRect(&rect, -pt.x, -pt.y); return rect; }

CRectX CRectX::operator+(SIZE size) const
{ CRectX rect(*this); ::OffsetRect(&rect, size.cx, size.cy); return rect; }

CRectX CRectX::operator-(SIZE size) const
{ CRectX rect(*this); ::OffsetRect(&rect, -size.cx, -size.cy); return rect; }

CRectX CRectX::operator+(LPCRECT lpRect) const
{ CRectX rect(this); rect.InflateRect(lpRect); return rect; }

CRectX CRectX::operator-(LPCRECT lpRect) const
{ CRectX rect(this); rect.DeflateRect(lpRect); return rect; }

CRectX CRectX::operator&(const RECT& rect2) const
{ CRectX rect; ::IntersectRect(&rect, this, &rect2);
return rect; }

CRectX CRectX::operator|(const RECT& rect2) const
{ CRectX rect; ::UnionRect(&rect, this, &rect2);
return rect; }

BOOL CRectX::SubtractRect(LPCRECT lpRectSrc1, LPCRECT lpRectSrc2)
{ return ::SubtractRect(this, lpRectSrc1, lpRectSrc2); }

//out-of-line 
void CRectX::NormalizeRect()
{
	int nTemp;
	if (left > right)
	{
		nTemp = left;
		left = right;
		right = nTemp;
	}
	if (top > bottom)
	{
		nTemp = top;
		top = bottom;
		bottom = nTemp;
	}
}

void CRectX::InflateRect(LPCRECT lpRect)
{
	left -= lpRect->left;
	top -= lpRect->top;
	right += lpRect->right;
	bottom += lpRect->bottom;
}

void CRectX::InflateRect(int l, int t, int r, int b)
{
	left -= l;
	top -= t;
	right += r;
	bottom += b;
}

void CRectX::DeflateRect(LPCRECT lpRect)
{
	left += lpRect->left;
	top += lpRect->top;
	right -= lpRect->right;
	bottom -= lpRect->bottom;
}

void CRectX::DeflateRect(int l, int t, int r, int b)
{
	left += l;
	top += t;
	right -= r;
	bottom -= b;
}

CRectX CRectX::MulDiv(int nMultiplier, int nDivisor) const
{
	return CRectX(
		::MulDiv(left, nMultiplier, nDivisor),
		::MulDiv(top, nMultiplier, nDivisor),
		::MulDiv(right, nMultiplier, nDivisor),
		::MulDiv(bottom, nMultiplier, nDivisor));
}


//////////////////////////////////////////////////////////////////////////
//CSizeX
//////////////////////////////////////////////////////////////////////////
CSizeX::CSizeX()
{
	cx = cy = 0;
}

CSizeX::CSizeX(const SIZE& src)
{
	cx = src.cx;
	cy = src.cy;
}

CSizeX::CSizeX(int _cx, int _cy)
{
	cx = _cx;
	cy = _cy;
}

CSizeX::CSizeX(POINT initPt)
{ 
	*(POINT*)this = initPt;
}

CSizeX::CSizeX(DWORD dwSize)
{
	cx = (short)LOWORD(dwSize);
	cy = (short)HIWORD(dwSize);
}

BOOL CSizeX::operator==(SIZE size) const
{ 
	return (cx == size.cx && cy == size.cy); 
}

BOOL CSizeX::operator!=(SIZE size) const
{ 
	return (cx != size.cx || cy != size.cy);
}

void CSizeX::operator+=(SIZE size)
{ 
	cx += size.cx; cy += size.cy;
}

void CSizeX::operator-=(SIZE size)
{ 
	cx -= size.cx; cy -= size.cy;
}
CSizeX CSizeX::operator+(SIZE size) const
{ 
	return CSizeX(cx + size.cx, cy + size.cy); 
}

CSizeX CSizeX::operator-(SIZE size) const
{ 
	return CSizeX(cx - size.cx, cy - size.cy); 
}

CSizeX CSizeX::operator-() const
{ 
	return CSizeX(-cx, -cy); 
}

CPointX CSizeX::operator+(POINT point) const
{ 
	return CPointX(cx + point.x, cy + point.y); 
}

CPointX CSizeX::operator-(POINT point) const
{ 
	return CPointX(cx - point.x, cy - point.y); 
}

CRectX CSizeX::operator+(const RECT* lpRect) const
{ 
	return CRectX(lpRect) + *this; 
}

CRectX CSizeX::operator-(const RECT* lpRect) const
{ 
	return CRectX(lpRect) - *this; 
}


//////////////////////////////////////////////////////////////////////////
//CPointX
//////////////////////////////////////////////////////////////////////////
CPointX::CPointX()
{ 
	x = y = 0;
}

CPointX::CPointX(int initX, int initY)
{ 
	x = initX; 
	y = initY; 
}

CPointX::CPointX(POINT initPt)
{ 
	*(POINT*)this = initPt; 
}

CPointX::CPointX(SIZE initSize)
{ 
	*(SIZE*)this = initSize; 
}

CPointX::CPointX(DWORD dwPoint)
{
	x = (short)LOWORD(dwPoint);
	y = (short)HIWORD(dwPoint);
}

void CPointX::Offset(int xOffset, int yOffset)
{ 
	x += xOffset; 
	y += yOffset; 
}

void CPointX::Offset(POINT point)
{ 
	x += point.x; 
	y += point.y; 
}

void CPointX::Offset(SIZE size)
{ 
	x += size.cx; 
	y += size.cy; 
}

BOOL CPointX::operator==(POINT point) const
{ 
	return (x == point.x && y == point.y); 
}

BOOL CPointX::operator!=(POINT point) const
{ 
	return (x != point.x || y != point.y); 
}

void CPointX::operator+=(SIZE size)
{ 
	x += size.cx; 
	y += size.cy; 
}

void CPointX::operator-=(SIZE size)
{ 
	x -= size.cx; 
	y -= size.cy; 
}

void CPointX::operator+=(POINT point)
{ 
	x += point.x; 
	y += point.y; 
}

void CPointX::operator-=(POINT point)
{ 
	x -= point.x; 
	y -= point.y; 
}

CPointX CPointX::operator+(SIZE size) const
{ 
	return CPointX(x + size.cx, y + size.cy); 
}

CPointX CPointX::operator-(SIZE size) const
{ 
	return CPointX(x - size.cx, y - size.cy); 
}

CPointX CPointX::operator-() const
{ 
	return CPointX(-x, -y); 
}

CPointX CPointX::operator+(POINT point) const
{ 
	return CPointX(x + point.x, y + point.y); 
}

CSizeX CPointX::operator-(POINT point) const
{ 
	return CSizeX(x - point.x, y - point.y); 
}

CRectX CPointX::operator+(const RECT* lpRect) const
{ 
	return CRectX(lpRect) + *this; 
}

CRectX CPointX::operator-(const RECT* lpRect) const
{ 
	return CRectX(lpRect) - *this; 
}


////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CStdPtrArray::CStdPtrArray(int iPreallocSize) : m_ppVoid(NULL), m_nCount(0), m_nAllocated(iPreallocSize)
{
	if( iPreallocSize > 0 ) m_ppVoid = static_cast<LPVOID*>(malloc(iPreallocSize * sizeof(LPVOID)));
}

CStdPtrArray::~CStdPtrArray()
{
	if( m_ppVoid != NULL ) 
	{
		free(m_ppVoid);
		m_ppVoid = NULL;
	}
}

void CStdPtrArray::Empty()
{
	if( m_ppVoid != NULL ) free(m_ppVoid);
	m_ppVoid = NULL;
	m_nCount = m_nAllocated = 0;
}

void CStdPtrArray::EmptyAndFree()
{
	for (int i = 0; i < GetSize(); i++)
		delete GetAt(i);
	Empty();
}
void CStdPtrArray::Resize(int iSize)
{
	Empty();
	m_ppVoid = static_cast<LPVOID*>(malloc(iSize * sizeof(LPVOID)));
	::ZeroMemory(m_ppVoid, iSize * sizeof(LPVOID));
	m_nAllocated = iSize;
	m_nCount = iSize;
}

bool CStdPtrArray::IsEmpty() const
{
	return m_nCount == 0;
}

bool CStdPtrArray::Add(LPVOID pData)
{
	if( ++m_nCount >= m_nAllocated) {
		m_nAllocated *= 2;
		if( m_nAllocated == 0 ) m_nAllocated = 11;
		m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
		if( m_ppVoid == NULL ) return false;
	}
	m_ppVoid[m_nCount - 1] = pData;
	return true;
}

bool CStdPtrArray::InsertAt(int iIndex, LPVOID pData)
{
	if( iIndex == m_nCount ) return Add(pData);
	if( iIndex < 0 || iIndex > m_nCount ) return false;
	if( ++m_nCount >= m_nAllocated) {
		m_nAllocated *= 2;
		if( m_nAllocated == 0 ) m_nAllocated = 11;
		m_ppVoid = static_cast<LPVOID*>(realloc(m_ppVoid, m_nAllocated * sizeof(LPVOID)));
		if( m_ppVoid == NULL ) return false;
	}
	memmove(&m_ppVoid[iIndex + 1], &m_ppVoid[iIndex], (m_nCount - iIndex - 1) * sizeof(LPVOID));
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CStdPtrArray::SetAt(int iIndex, LPVOID pData)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	m_ppVoid[iIndex] = pData;
	return true;
}

bool CStdPtrArray::Remove(int iIndex)
{
	if( iIndex < 0 || iIndex >= m_nCount ) return false;
	if( iIndex < --m_nCount ) ::CopyMemory(m_ppVoid + iIndex, m_ppVoid + iIndex + 1, (m_nCount - iIndex) * sizeof(LPVOID));
	return true;
}

bool  CStdPtrArray::Remove(LPVOID iIndex)
{
	int i = Find(iIndex);
	return Remove(i);
}

int CStdPtrArray::Find(LPVOID pData) const
{
	for( int i = 0; i < m_nCount; i++ ) if( m_ppVoid[i] == pData ) return i;
	return -1;
}

int CStdPtrArray::GetSize() const
{
	return m_nCount;
}

LPVOID* CStdPtrArray::GetData()
{
	return m_ppVoid;
}

LPVOID CStdPtrArray::GetAt(int iIndex) const
{
	if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
	return m_ppVoid[iIndex]; 
}

LPVOID CStdPtrArray::operator[] (int iIndex) const
{
	if( iIndex < 0 || iIndex >= m_nCount ) return NULL;
	return m_ppVoid[iIndex];
}

void  CStdPtrArray::Swap(int nStart, int nEnd)
{
	LPVOID  lptmp = m_ppVoid[nStart];
	m_ppVoid[nStart] = m_ppVoid[nEnd];
	m_ppVoid[nEnd] = lptmp;
}

void CStdPtrArray::Swap(int nStart1, int nEnd1, int nStart2, int nEnd2)
{
	if (nStart1 >= nEnd2)
	{
		int tmp = nStart1;
		nStart1 = nStart2;
		nStart2 = tmp;
		tmp = nEnd1;
		nEnd1 = nEnd2;
		nEnd2 = tmp;
	}
	if (nEnd1 >= nStart2)
		return;

	int nCount1 = nEnd1 - nStart1 + 1;
	int nCount2 = nEnd2 - nStart2 + 1;
	if (nCount1 >= nCount2)
	{
		LPVOID lpTmp = static_cast<LPVOID*>(malloc(nCount1 * sizeof(LPVOID)));
		::CopyMemory(lpTmp, m_ppVoid + nStart1, nCount1 * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nStart1 + nCount2, m_ppVoid + nEnd1 + 1, (nStart2 - nEnd1 - 1) * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nStart1, m_ppVoid + nStart2, nCount2 * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nEnd2 - nCount1 + 1, lpTmp, nCount1 * sizeof(LPVOID));
		free(lpTmp);
	}
	else
	{
		LPVOID lpTmp = static_cast<LPVOID*>(malloc(nCount2 * sizeof(LPVOID)));
		::CopyMemory(lpTmp, m_ppVoid + nStart2, nCount2 * sizeof(LPVOID));
		::CopyMemory( m_ppVoid + nEnd1 + 1 +  nCount2 -  nCount1, m_ppVoid + nEnd1 + 1, (nStart2 - nEnd1 - 1) * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nStart2 + nCount2 -  nCount1, m_ppVoid + nStart1, nCount1 * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nStart1, lpTmp, nCount2 * sizeof(LPVOID));
		free(lpTmp);
	}
}

void CStdPtrArray::Move(int nStart, int nEnd, int nTo)
{
	if (nTo > m_nCount)
		nTo = m_nCount;
	if (nTo < 0)
		nTo = 0;
	if (nTo >=nStart && nTo <= nEnd)
		return;
	if (nStart < 0 || nStart >= m_nCount)
		return;
	if (nEnd < 0 || nEnd >= m_nCount)
		return;
	int nCount = nEnd - nStart + 1;
	LPVOID lpTmp = static_cast<LPVOID*>(malloc(nCount * sizeof(LPVOID)));
	::CopyMemory(lpTmp, m_ppVoid + nStart, nCount * sizeof(LPVOID));
	if (nTo > nEnd)
	{
		::CopyMemory(m_ppVoid + nStart, m_ppVoid + nEnd + 1, (nTo - nEnd) * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nTo - nCount, lpTmp, nCount * sizeof(LPVOID));
	}
	else if (nTo < nStart)
	{
		::CopyMemory(m_ppVoid + nTo + nCount, m_ppVoid + nTo, (nStart - nTo) * sizeof(LPVOID));
		::CopyMemory(m_ppVoid + nTo, lpTmp, nCount * sizeof(LPVOID));

	}
	free(lpTmp);
}


/////////////////////////////////////////////////////////////////////////
void duGetAnchorPos(uint8 nAnchor, const RECT* lprcParent, RECT* lprcChild)
{
	switch (nAnchor)
	{
	case 4:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->top + lprcChild->bottom;
			return;
		}
	case 6:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->bottom - lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			return;
		}
	case 7:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->left + lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			return;
		}
	case 5:
		{
			lprcChild->left = lprcParent->right - lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			return;
		}
	case 8:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			break;
		}
	case 3:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->bottom - lprcChild->top;
			lprcChild->right = lprcParent->left + lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			return;
		}
	case 1:
		{
			lprcChild->left = lprcParent->right - lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->top + lprcChild->bottom;
			return;
		}
	case  2:
		{
			lprcChild->left=lprcParent->right-lprcChild->left;
			lprcChild->top=lprcParent->bottom-lprcChild->top;
			lprcChild->right = lprcParent->right - lprcChild->right;
			lprcChild->bottom = lprcParent->bottom - lprcChild->bottom;
			return;

		}
	case 0:
		{
			lprcChild->left = lprcParent->left + lprcChild->left;
			lprcChild->top = lprcParent->top + lprcChild->top;
			lprcChild->right = lprcParent->left + lprcChild->right;
			lprcChild->bottom = lprcParent->top + lprcChild->bottom;
			return;
		}
	case 9: //中间对齐
		{
			if (lprcChild->left < 0)
			{
				int offsetx = (lprcParent->right - lprcParent->left - lprcChild->right) / 2;
				lprcChild->left = lprcParent->left + offsetx;// + lprcChild->left;
				lprcChild->right = lprcChild->left + lprcChild->right;
			}
			else
			{
				if (lprcChild->right < 0)
				{
					lprcChild->left = lprcParent->right - lprcChild->left;
					lprcChild->right = lprcChild->left - lprcChild->right;
				}
				else
				{
					lprcChild->left = lprcParent->left + lprcChild->left;
					lprcChild->right = lprcChild->left + lprcChild->right;
				}	
			}
			
			if (lprcChild->top < 0)
			{
				int offsety = (lprcParent->bottom - lprcParent->top - lprcChild->bottom) / 2;
				lprcChild->top = lprcParent->top + offsety;// + lprcChild->top;
				lprcChild->bottom = lprcChild->top + lprcChild->bottom;
			}
			else
			{
				if (lprcChild->bottom < 0)
				{
					lprcChild->top = lprcParent->bottom - lprcChild->top;
					lprcChild->bottom = lprcChild->top - lprcChild->bottom;
				}
				else
				{
					lprcChild->top = lprcParent->top + lprcChild->top;
					lprcChild->bottom = lprcChild->top + lprcChild->bottom;
				}	
			}
			return;
		}
	}
}

tstring  GetRunDir()
{
	TCHAR rtfPath[MAX_PATH];
	GetModuleFileName(NULL, rtfPath, MAX_PATH);
	wstring filepath(rtfPath);
	size_t npos = filepath.rfind(_T('\\'));
	if (npos != wstring::npos)
		filepath.erase(npos+1);
	return filepath;
}
/////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
int CSSE::m_nSupportSSE = -1;
bool CSSE::IsSupportSSE()
{
	if(m_nSupportSSE == -1)
	{
		__try 
		{
			__asm 
			{
				xorpd xmm0, xmm0        // executing SSE instruction
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER) 
		{
			if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) 
			{
				m_nSupportSSE = 0;
			}
			m_nSupportSSE = 0;
		}
		if(m_nSupportSSE == -1)
		{
			m_nSupportSSE = 1;
		}
	}
	return m_nSupportSSE == 1;
}


} // dui命名空间
