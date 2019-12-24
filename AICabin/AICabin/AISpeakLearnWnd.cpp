#include "stdafx.h"
#include "AISpeakLearnWnd.h"

#define	SPEAK_LEARN_TIMERID			1001


CAISpeakLearnWnd::CAISpeakLearnWnd()
{
    m_vecExample.push_back("Happy");
    m_vecExample.push_back("Excited");
    m_vecExample.push_back("Surprised");
    m_vecExample.push_back("Sad");
    m_vecExample.push_back("Angry");

    m_nTotalTimeWidth = 0;//涨条总长度
    m_nEveryTimeWidth = 0;//每次涨条长度
    m_nTatalTimes = 25;//总次数
    m_nCurTimes = 0;//当前次数
    m_rc = { 0, 0, 0, 0 };

    m_pBtnCardText = nullptr;
    m_pBtnSpeakLearnRecord = nullptr;
    m_pBtnSpeakLearnRecording = nullptr;
    m_pLayoutSpeakLearnTime = nullptr;

}


CAISpeakLearnWnd::~CAISpeakLearnWnd()
{
}

HWND CAISpeakLearnWnd::CreateWnd(HWND hParent)
{
	SetLayerWindow(true);
	ModifyWndStyle(GWL_EXSTYLE, NULL, WS_EX_TOOLWINDOW);

	HWND hWnd = this->Create(hParent, _T("AISpeakLearnWnd"));
	if (!hWnd)
		return hWnd;

	RECT rtParent = { 0 };

	if (::GetWindowRect(hParent, &rtParent))
	{
		this->SetWindowPos(NULL,
			rtParent.left,
			rtParent.top,
			rtParent.right - rtParent.left,
			rtParent.bottom - rtParent.top,
			SWP_NOZORDER);
	}

	return hWnd;
}

void CAISpeakLearnWnd::OnCreate()
{
    m_pBtnCardText = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("card_text_lebel")));
    m_pBtnSpeakLearnRecord = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_learn_record")));
    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(true);

    m_pBtnSpeakLearnRecording = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_learn_recording")));
    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    m_pLayoutSpeakLearnTime = dynamic_cast<CLayoutUI*> (FindControl(_T("SpeakLearnTimeLayout")));
    if (m_pLayoutSpeakLearnTime)
    {
        m_rc = m_pLayoutSpeakLearnTime->GetRect();
        m_nTotalTimeWidth = m_rc.right - m_rc.left;
        m_nEveryTimeWidth = m_nTotalTimeWidth / m_nTatalTimes;
        m_pLayoutSpeakLearnTime->SetVisible(false);
    }

    GetRoot()->OnEvent += MakeDelegate(this, &CAISpeakLearnWnd::OnCheckRecordTime);  

	__super::OnCreate();
}

void CAISpeakLearnWnd::OnClose()
{
    
}

bool CAISpeakLearnWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    if (m_pBtnCardText)
        m_pBtnCardText->SetText(_T("Sad"));

    return true;
}

bool CAISpeakLearnWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    if (m_pBtnCardText)
        m_pBtnCardText->SetText(_T("Surprised"));

    return true;
}

bool CAISpeakLearnWnd::OnEventSpeakLearnRecord(TNotifyUI* pTNotify)
{
    //开始发声，并隐藏麦克风按钮，显示声音图片
    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(false);

    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(true);

    if (m_pLayoutSpeakLearnTime)
        m_pLayoutSpeakLearnTime->SetVisible(true);

    if (m_pLayoutSpeakLearnTime)
    {
        RECT rc = m_rc;
        rc.right = rc.left;
        m_pLayoutSpeakLearnTime->SetRect(rc);
        m_pLayoutSpeakLearnTime->SetVisible(true);
    }
    m_nCurTimes = 0;
    SetTimer(GetRoot(), SPEAK_LEARN_TIMERID, 200);
    return true;
}

bool CAISpeakLearnWnd::OnCheckRecordTime(TEventUI &evt)
{
    if (UIEVENT_TIMER == evt.nType)
    {
        switch (evt.wParam)
        {
        case SPEAK_LEARN_TIMERID:
        {
            if (m_nCurTimes > m_nTatalTimes)
            {
                KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);
                if (m_pLayoutSpeakLearnTime)
                    m_pLayoutSpeakLearnTime->SetVisible(false);

                return true;
            }
            

            RECT rc = m_rc;
            rc.right = rc.left + m_nCurTimes * m_nEveryTimeWidth;
            if (rc.right > m_rc.right)
            {
                KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);
                if (m_pLayoutSpeakLearnTime)
                    m_pLayoutSpeakLearnTime->SetVisible(false);

                return true;
            }

            if (m_pLayoutSpeakLearnTime){
                m_pLayoutSpeakLearnTime->SetRect(rc);
                m_pLayoutSpeakLearnTime->OnlyResizeChild();
                m_pLayoutSpeakLearnTime->Invalidate();
            }

            m_nCurTimes++;
        }
        break;
        default:
            break;
        }
    }
    return true;
}
