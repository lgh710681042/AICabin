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

    m_nTotalTimeWidth = 0;//�����ܳ���
    m_nEveryTimeWidth = 0;//ÿ����������
    m_nTatalTimes = 25;//�ܴ���
    m_nCurTimes = 0;//��ǰ����
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

	m_pBtnSpeakLearnRecording = dynamic_cast<CAnimationUI*> (FindControl(_T("btn_speak_learn_recording")));
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

	//��ȡvec�����
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
    //��ʼ��������������˷簴ť����ʾ����ͼƬ
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

	CSpeechRecordControl::GetInstance()->ControlSpeechRecoStart();


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
