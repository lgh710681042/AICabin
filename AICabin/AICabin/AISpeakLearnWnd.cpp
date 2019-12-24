#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechRecordControl.h"
#include "AISpeakLearnWnd.h"

#define	SPEAK_LEARN_TIMERID			1001
#define SPEAK_RECORD_TIMERID		2001


CAISpeakLearnWnd::CAISpeakLearnWnd()
{
    m_vecExample.push_back(_T("Happy"));
    m_vecExample.push_back(_T("Excited"));
    m_vecExample.push_back(_T("Surprised"));
    m_vecExample.push_back(_T("Sad"));
    m_vecExample.push_back(_T("Angry"));

    m_nTotalTimeWidth = 0;//涨条总长度
    m_nEveryTimeWidth = 0;//每次涨条长度
    m_nTatalTimes = 50;//总次数
    m_nCurTimes = 0;//当前次数
    m_rc = { 0, 0, 0, 0 };

    m_pBtnCardText = nullptr;
    m_pBtnSpeakLearnRecord = nullptr;
    m_pBtnSpeakLearnRecording = nullptr;
    m_pBtnSpeakJump = nullptr;
    m_pBtnSpeakReadAgain = nullptr;
    m_pBtnSpeakTimesTips = nullptr;
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

	m_pBtnSpeakLearnRecording = dynamic_cast<CAnimationUI*> (FindControl(_T("btn_speak_learn_recording")));
    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    m_pBtnSpeakJump = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_jump")));
    if (m_pBtnSpeakJump)
        m_pBtnSpeakJump->SetVisible(false);

    m_pBtnSpeakReadAgain = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_read_again")));
    if (m_pBtnSpeakReadAgain)
        m_pBtnSpeakReadAgain->SetVisible(false);
    
    m_pBtnSpeakTimesTips = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("speak_times_tips")));
    if (m_pBtnSpeakTimesTips)
        m_pBtnSpeakTimesTips->SetVisible(false);

    m_pLayoutSpeakLearnTime = dynamic_cast<CLayoutUI*> (FindControl(_T("SpeakLearnTimeLayout")));
    if (m_pLayoutSpeakLearnTime)
    {
        m_rc = m_pLayoutSpeakLearnTime->GetRect();
        m_nTotalTimeWidth = m_rc.right - m_rc.left;
        m_nEveryTimeWidth = m_nTotalTimeWidth / m_nTatalTimes;
        m_pLayoutSpeakLearnTime->SetVisible(false);
    }

    GetRoot()->OnEvent += MakeDelegate(this, &CAISpeakLearnWnd::OnCheckRecordTime);

	//获取vec随机数
	int nIndex = CommonUtil::ToolRandInt(0, m_vecExample.size() - 1);
	if (nIndex < m_vecExample.size())
	{
		wstring strText = m_vecExample[nIndex];
		m_pBtnCardText->SetText(strText.c_str());
	}

	__super::OnCreate();
}

void CAISpeakLearnWnd::OnClose()
{
    
}

bool CAISpeakLearnWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    return true;
}

bool CAISpeakLearnWnd::OnEventReturn(TNotifyUI* pTNotify)
{
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
    SetTimer(GetRoot(), SPEAK_LEARN_TIMERID, 100);

	CSpeechRecordControl::GetInstance()->ControlSpeechRecoStart();


    return true;
}

bool CAISpeakLearnWnd::OnEventReadJump(TNotifyUI* pTNotify)
{
    //下一个
    if (m_pBtnSpeakTimesTips)
        m_pBtnSpeakTimesTips->SetVisible(false);

    if (m_pBtnSpeakJump)
        m_pBtnSpeakJump->SetVisible(false);

    if (m_pBtnSpeakReadAgain)
        m_pBtnSpeakReadAgain->SetVisible(false);

    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    if (m_pLayoutSpeakLearnTime)
        m_pLayoutSpeakLearnTime->SetVisible(false);

    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(true);

    return true;
}

bool CAISpeakLearnWnd::OnEventReadAgain(TNotifyUI* pTNotify)
{
    if (m_pBtnSpeakTimesTips)
        m_pBtnSpeakTimesTips->SetVisible(false);

    if (m_pBtnSpeakJump)
        m_pBtnSpeakJump->SetVisible(false);

    if (m_pBtnSpeakReadAgain)
        m_pBtnSpeakReadAgain->SetVisible(false);

    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    if (m_pLayoutSpeakLearnTime)
        m_pLayoutSpeakLearnTime->SetVisible(false);

    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(true);

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
            RECT rc = m_rc;
            rc.right = rc.left + m_nCurTimes * m_nEveryTimeWidth;
            if (rc.right > m_rc.right || m_nCurTimes > m_nTatalTimes)
            {
                ShowSpeakResult();
                return true;
            }

            if (m_pLayoutSpeakLearnTime)
            {
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

void CAISpeakLearnWnd::ShowSpeakResult()
{
    KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);

    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(false);

    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    if (m_pLayoutSpeakLearnTime)
        m_pLayoutSpeakLearnTime->SetVisible(false);

    if (m_pBtnSpeakTimesTips)
        m_pBtnSpeakTimesTips->SetVisible(true);

    if (m_pBtnSpeakJump)
        m_pBtnSpeakJump->SetVisible(true);

    if (m_pBtnSpeakReadAgain)
        m_pBtnSpeakReadAgain->SetVisible(true);
}