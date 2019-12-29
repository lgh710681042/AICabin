#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "SpeechRecordControl.h"
#include "AILearnBaseWnd.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include <algorithm>
#include "AIActivityWnd.h"
#include "AICabinWnd.h"
#include "Application.h"
#include "SpeechSynthControl.h"

#define SPEAK_RECORD_TIMERID		2001
#define SPEAK_RECORD_INTERVAL		3000		//语音识别4秒

#define	SPEAK_LEARN_TIMERID			2002 //涨条定时器
#define SPEAK_EVERY_TIME            100 //定时器每次间隔
#define SPEAK_TOTAL_TIMES           25  //总次数（与上面字段相乘等于总时间）

CAISpeakLearnWnd::CAISpeakLearnWnd()
{
    m_vecExample.push_back(_T("Happy"));
    m_vecExample.push_back(_T("Surprise"));
    m_vecExample.push_back(_T("Sad"));
    m_vecExample.push_back(_T("Angry"));

    m_rc = { 0, 0, 0, 0 };
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
    m_pCtrlSpreadOne = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_error_spread_one")));
    m_pCtrlSpreadTwo = dynamic_cast<CControlUI*> (FindControl(_T("ctrl_error_spread_two")));
    m_pLabelSpeakTitle = dynamic_cast<CAutoSizeButtonUI*> (FindControl(_T("LabelSpeakTitle")));  
    m_pLayoutSpeakLearnCard = dynamic_cast<CLayoutUI*> (FindControl(_T("SpeakLearnCardLayout")));

	SetSpeakFailLayoutVisible(false);

    m_pLayoutSpeakLearnTime = dynamic_cast<CLayoutUI*> (FindControl(_T("SpeakLearnTimeLayout")));
    if (m_pLayoutSpeakLearnTime)
    {
        m_rc = m_pLayoutSpeakLearnTime->GetRect();
        m_nTotalTimeWidth = m_rc.right - m_rc.left;
        m_nEveryTimeWidth = m_nTotalTimeWidth / SPEAK_TOTAL_TIMES;
        m_pLayoutSpeakLearnTime->SetVisible(false);
    }

    GetRoot()->OnEvent += MakeDelegate(this, &CAISpeakLearnWnd::OnCheckRecordTime);

    //清空已读记录
    m_mapHaveRead.clear();
    time(&m_tBeginTime);

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

    CApplication::GetInstance()->m_pAISpeakLearnWnd = this;

    SpeechTitle();
}

void CAISpeakLearnWnd::OnClose()
{
    CApplication::GetInstance()->m_pAISpeakLearnWnd = nullptr;
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

            if (!CheckSpeakQuestionAccRate(strMstUnicodeTmp, strSpeakQuestionTmp))
            {
				delete pSpeakResult;
				break;
			}

            SaveHaveReadData();
            if (m_pLayoutSpeakLearnTime)
                m_pLayoutSpeakLearnTime->SetVisible(false);

			//语音识别成功，人脸识别窗口显示
			CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
			if (pAIFaceLearnWnd)
			{
				pAIFaceLearnWnd->CreateWnd(GetParent(GetHWND()));
				pAIFaceLearnWnd->SetFaceQuestion(m_strSpeakQuestion, true);
				//pAIFaceLearnWnd->BeginFace();
				pAIFaceLearnWnd->ShowWindow();
			}

			delete pSpeakResult;

			return __super::WindowProc(message, wParam, lParam);

		} while (false);

		if (m_nSpeakCurTimes >= m_nSpeakTotalTimes)
		{
			//失败次数当于总次数,直接显示成绩界面
            SaveHaveReadData();
            if (m_pLayoutSpeakLearnTime)
                m_pLayoutSpeakLearnTime->SetVisible(false);

            //失败3次，也是进入人脸识别窗口
            CAIFaceLearnWnd* pAIFaceLearnWnd = new CAIFaceLearnWnd;
            if (pAIFaceLearnWnd)
            {
                pAIFaceLearnWnd->CreateWnd(GetParent(GetHWND()));
                pAIFaceLearnWnd->SetFaceQuestion(m_strSpeakQuestion,false);
				//pAIFaceLearnWnd->BeginFace();
                pAIFaceLearnWnd->ShowWindow();
            }

			return __super::WindowProc(message, wParam, lParam);
		}

		//识别失败
		ShowSpeakFailResult();
		m_nSpeakCurTimes++;
	}

	return __super::WindowProc(message, wParam, lParam);
}

void CAISpeakLearnWnd::StartNewRecord()
{
    SpeechTitle();

    KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);
    m_nSpeakCurTimes = 1;
    OnEventReadJump(nullptr);
}

bool CAISpeakLearnWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    //关闭学习卡片窗口
    CloseWindow();

    //关闭活动列表窗口
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->CloseWindow();

    //跳到结束界面
    if (CApplication::GetInstance()->m_pAICabinWnd != nullptr)
        (CApplication::GetInstance()->m_pAICabinWnd)->ShowEndLayout();

    return true;
}

bool CAISpeakLearnWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    //关闭学习卡片窗口
    CloseWindow();

    //跳到活动界面
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->ShowWindowData();

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
    { 
        m_pLayoutSpeakLearnTime->SetRect(m_rc);
        m_pLayoutSpeakLearnTime->SetVisible(true);
    }

	//begin record
	CSpeechRecordControl::GetInstance()->ControlSpeechRecoStart();
	CSpeechRecordControl::GetInstance()->SetMsgHwnd(GetHWND());

    m_nCurTimes = 0;
    SetTimer(GetRoot(), SPEAK_LEARN_TIMERID, SPEAK_EVERY_TIME);
	SetTimer(GetRoot(), SPEAK_RECORD_TIMERID, SPEAK_RECORD_INTERVAL);

    return true;
}

bool CAISpeakLearnWnd::OnEventReadJump(TNotifyUI* pTNotify)
{
    //下一个
	SetSpeakFailLayoutVisible(false);

    SaveHaveReadData();
    GetNewSpeakQuestion();

    if (m_pBtnCardText)
    {
        m_pBtnCardText->SetText(m_strSpeakQuestion.c_str());
        m_pBtnCardText->Invalidate();
    }


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
            rc.right -= m_nCurTimes * m_nEveryTimeWidth;
            if (rc.right < rc.left || m_nCurTimes > SPEAK_TOTAL_TIMES)
            {
				if (m_pLayoutSpeakLearnTime)
					m_pLayoutSpeakLearnTime->SetVisible(false);

				KillTimer(GetRoot(), SPEAK_LEARN_TIMERID);
                return true;
            }

            if (m_pLayoutSpeakLearnTime)
            {
                m_pLayoutSpeakLearnTime->SetRect(rc);
                m_pLayoutSpeakLearnTime->OnlyResizeChild();
                m_pLayoutSpeakLearnTime->GetParent()->Invalidate();
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

    if (m_pCtrlSpreadOne)
        m_pCtrlSpreadOne->SetVisible(bVisible);

    if (m_pCtrlSpreadTwo)
        m_pCtrlSpreadTwo->SetVisible(bVisible);

    if (m_pLabelSpeakTitle)
    {
        if (bVisible)
            m_pLabelSpeakTitle->SetText(I18NSTR(_T("#StrSpeakTitleError")));
        else 
            m_pLabelSpeakTitle->SetText(I18NSTR(_T("#StrSpeakLearnTitle")));
    } 

    if (m_pLayoutSpeakLearnCard)
    {
        if (bVisible)
            m_pLayoutSpeakLearnCard->SetAttribute(_T("bk.image"), _T("#ai_speak_learn_card_error"));
        else
            m_pLayoutSpeakLearnCard->SetAttribute(_T("bk.image"), _T("#ai_speak_learn_card"));

        m_pLayoutSpeakLearnCard->GetParent()->Invalidate();
    }
}

void CAISpeakLearnWnd::SaveHaveReadData()
{
    if (m_strSpeakQuestion.empty())
        return;

    map<std::wstring, int>::iterator iter = m_mapHaveRead.find(m_strSpeakQuestion);
    if (iter == m_mapHaveRead.end())
    {
        m_mapHaveRead.insert(pair<std::wstring, int>(m_strSpeakQuestion, 1));
        return;
    }
}

void CAISpeakLearnWnd::GetNewSpeakQuestion()
{
    time(&m_tBeginTime);

    if (m_mapHaveRead.size() >= m_vecExample.size())
    {
        m_mapHaveRead.clear();

        //获取vec随机数
        int nIndex = CommonUtil::ToolRandInt(0, m_vecExample.size() - 1);
        if (nIndex < m_vecExample.size())
            m_strSpeakQuestion = m_vecExample[nIndex];

        return;
    }      

    for (int i = 0; i < m_vecExample.size(); i++)
    {
        map<std::wstring, int>::iterator iter = m_mapHaveRead.find(m_vecExample[i]);
        if (iter == m_mapHaveRead.end())
        {
            m_strSpeakQuestion = m_vecExample[i];     
            return;
        }
    }
}

bool CAISpeakLearnWnd::CheckSpeakQuestionAccRate(const wstring& strResult, const wstring& strSrc)
{
    wstring::size_type nPos = strResult.find(strSrc);
    if (nPos != wstring::npos)
        return true;

    map<wchar_t, int> mapChar;//不重复字母集合
    map<wchar_t, int>::iterator iter;
    for (size_t i = 0; i < strSrc.length(); i++)
    {
        iter = mapChar.find(strSrc.at(i));
        if (iter == mapChar.end())
            mapChar.insert(pair<wchar_t, int>(strSrc.at(i), 1));
    }
    
    int nCounts = 0;
    iter = mapChar.begin();
    while (iter != mapChar.end())
    {
        wstring::size_type nPos = strResult.find(iter->first);
        if (nPos != wstring::npos)
            nCounts++;

        iter++;
    }

    float fHalf = (int)mapChar.size() / 2.0f;
    if (nCounts - fHalf > -0.0001)
        return true;

    return false;
}

void CAISpeakLearnWnd::SpeechTitle()
{
    wstring strEnd = I18NSTR(_T("#StrSpeakLearnTitle"));
    CSpeechSynthControl::GetInstance()->ControlSpeechSynthCall(strEnd);//语音合成播放
}
