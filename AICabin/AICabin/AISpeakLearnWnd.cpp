#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechRecordControl.h"
#include "AILearnBaseWnd.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include <algorithm>

#define	SPEAK_LEARN_TIMERID			1001
#define SPEAK_RECORD_TIMERID		2001
#define SPEAK_RECORD_INTERVAL		5000		//语音识别5秒


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
	__super::OnCreate();

    m_pBtnCardText = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("card_text_lebel")));
    m_pBtnSpeakLearnRecord = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_learn_record")));
    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(true);

	m_pBtnSpeakLearnRecording = dynamic_cast<CAnimationUI*> (FindControl(_T("btn_speak_learn_recording")));
    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    m_pBtnSpeakJump = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_jump")));
    m_pBtnSpeakReadAgain = dynamic_cast<CButtonUI*> (FindControl(_T("btn_speak_read_again")));
    m_pBtnSpeakTimesTips = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("speak_times_tips")));

	SetSpeakFailLayoutVisible(false);

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
		m_strSpeakQuestion = m_vecExample[nIndex];
		m_pBtnCardText->SetText(m_strSpeakQuestion.c_str());
	}

	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(true);
		m_pAILeftBtnPanelUnExpend->SetVisible(false);
	}
}

void CAISpeakLearnWnd::OnClose()
{
    
}

LRESULT CAISpeakLearnWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SPEAK_RESULT )
	{
		do
		{
			SpeakResultStruct* pSpeakResult = (SpeakResultStruct*)lParam;
			if (pSpeakResult == nullptr
				|| pSpeakResult->strMsgUnicode.empty())
				break;

			wstring strMstUnicodeTmp = pSpeakResult->strMsgUnicode;
			wstring strSpeakQuestionTmp = m_strSpeakQuestion;

			transform(strMstUnicodeTmp.begin(), strMstUnicodeTmp.end(), strMstUnicodeTmp.begin(), ::tolower);
			transform(strSpeakQuestionTmp.begin(), strSpeakQuestionTmp.end(), strSpeakQuestionTmp.begin(), ::tolower);

			wstring::size_type nPos = strMstUnicodeTmp.find(strSpeakQuestionTmp);

			if (nPos == wstring::npos)
			{
				delete pSpeakResult;
				break;
			}

			//语音识别成功，人脸识别窗口显示
			CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
			if (pAIFaceLearnWnd)
			{
				pAIFaceLearnWnd->CreateWnd(GetParent(GetHWND()));
				pAIFaceLearnWnd->SetFaceQuestion(m_strSpeakQuestion);
				pAIFaceLearnWnd->ShowWindow();
			}

			delete pSpeakResult;

			return __super::WindowProc(message, wParam, lParam);

		} while (false);

		if (m_nSpeakCurTimes >= m_nSpeakTotalTimes)
		{
			//失败次数当于总次数,直接显示成绩界面
			//CAIViewResultsWnd

			return __super::WindowProc(message, wParam, lParam);
		}

		//识别失败
		ShowSpeakFailResult();
		m_nSpeakCurTimes++;
	}

	return __super::WindowProc(message, wParam, lParam);
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

	//begin record
	CSpeechRecordControl::GetInstance()->ControlSpeechRecoStart();
	CSpeechRecordControl::GetInstance()->SetMsgHwnd(GetHWND());
	SetTimer(GetRoot(), SPEAK_RECORD_TIMERID, SPEAK_RECORD_INTERVAL);

    return true;
}

bool CAISpeakLearnWnd::OnEventReadJump(TNotifyUI* pTNotify)
{
    //下一个
	SetSpeakFailLayoutVisible(false);

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
	SetSpeakFailLayoutVisible(false);

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
				if (m_pLayoutSpeakLearnTime)
				{
					m_pLayoutSpeakLearnTime->SetRect(m_rc);
					m_pLayoutSpeakLearnTime->OnlyResizeChild();
					m_pLayoutSpeakLearnTime->Invalidate();
				}

				KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);
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
		case SPEAK_RECORD_TIMERID:
		{
			CSpeechRecordControl::GetInstance()->ControlSpeechRecoStop();
			KillTimer(GetRoot(), SPEAK_RECORD_TIMERID);
			break;
		}
        default:
            break;
        }
    }
    return true;
}

//语音识别失败，UI展示
void CAISpeakLearnWnd::ShowSpeakFailResult()
{
    if (m_pBtnSpeakLearnRecord)
        m_pBtnSpeakLearnRecord->SetVisible(false);

    if (m_pBtnSpeakLearnRecording)
        m_pBtnSpeakLearnRecording->SetVisible(false);

    if (m_pLayoutSpeakLearnTime)
        m_pLayoutSpeakLearnTime->SetVisible(false);

	SetSpeakFailLayoutVisible(true);
}

//不要灰心，跳过按钮，重新朗读，显示隐藏
void CAISpeakLearnWnd::SetSpeakFailLayoutVisible(bool bVisible)
{
	if (m_pBtnSpeakTimesTips)
	{
		m_pBtnSpeakTimesTips->SetVisible(bVisible);
		if (bVisible)
		{
			wstring strLeftTimes = _T("");
			switch (m_nSpeakTotalTimes - m_nSpeakCurTimes)
			{
			case 2:
				strLeftTimes = I18NSTR(_T("#StrHanYuTwo"));
				break;
			case 1:
				strLeftTimes = I18NSTR(_T("#StrHanYuOne"));
				break;
			default:
				break;
			}

			WCHAR szBuf[1024];
			_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrSpeakNotGood")), strLeftTimes.c_str());

			m_pBtnSpeakTimesTips->SetText(szBuf);
		}
	}
		
	if (m_pBtnSpeakJump)
		m_pBtnSpeakJump->SetVisible(bVisible);

	if (m_pBtnSpeakReadAgain)
		m_pBtnSpeakReadAgain->SetVisible(bVisible);
}
