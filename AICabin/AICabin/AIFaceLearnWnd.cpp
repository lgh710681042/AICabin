#include "stdafx.h"
#include "Singleton.h"
#include "CommonUtil.h"
#include "qFaceExpression.h"
#include "OpencvControl.h"
#include "AILearnBaseWnd.h"
#include "AIViewResultsWnd.h"
#include "AIFaceLearnWnd.h"
#include "AISpeakLearnWnd.h"
#include "AIActivityWnd.h"
#include "AICabinWnd.h"
#include "Application.h"
#include "SpeechSynthControl.h"

#define	AIFACE_LEARN_TIMERID			1001		//��ʱ����ÿ��1���ȡ�������ݣ��ж��Ƿ�ɹ������ɹ������
#define AIFACE_RECORD_TIMERID			2001		//��ʱ����5���ڽ��б���ʶ����5s��û�ɹ������ж�ʧ��

#define AIFACE_LEARN_TIMES				1000
#define AIFACE_RECORD_TIMES				8000

//����ʶ��ԲȦת����ʱ��
#define FACE_ANIMATE_TIMERID			3001	// ת����ʱ��
#define FACE_ANIMATE_TIMES		        80		// ÿ60s�л�ͼƬ

CAIFaceLearnWnd::CAIFaceLearnWnd()
{
}


CAIFaceLearnWnd::~CAIFaceLearnWnd()
{
}

HWND CAIFaceLearnWnd::CreateWnd(HWND hParent)
{
	SetLayerWindow(true);
	ModifyWndStyle(GWL_EXSTYLE, NULL, WS_EX_TOOLWINDOW);

	HWND hWnd = this->Create(hParent, _T("AIFaceLearnWnd"));
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

void CAIFaceLearnWnd::SetFaceQuestion(wstring strFaceQuestion, bool bSucc)
{
    m_bSuccess = bSucc;
	m_strFaceQuestion = strFaceQuestion;

	if (m_pBtnFaceCommonTitle
		&& m_pBtnFaceTitleTop)
	{
		WCHAR szBuf[1024];
		_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrFaceLearnCommonTitle")), m_strFaceQuestion.c_str());


        wstring strTitleFirst = _T("");
        if (m_bSuccess)
            strTitleFirst = I18NSTR(_T("#StrFaceLearnTitleTop"));
        else
            strTitleFirst = I18NSTR(_T("#StrFaceLearnTitleTop2"));

		m_pBtnFaceCommonTitle->SetText(szBuf);
		m_pBtnFaceTitleBottom->SetText(szBuf);
        m_pBtnFaceTitleTop->SetText(strTitleFirst.c_str());

        strTitleFirst += szBuf;
        CSpeechSynthControl::GetInstance()->ControlSpeechSynthCallFace(strTitleFirst, GetHWND());//�����ϳɲ���
	}
}

void CAIFaceLearnWnd::BeginFace()
{
	COpencvControl::GetInstance()->BeginFace(GetHWND());
}

void CAIFaceLearnWnd::Switch(UIFaceType uiFaceType)
{
    KillTimer(GetRoot(), FACE_ANIMATE_TIMERID);
    if (m_pFaceAnimateControl)
    {
        m_pFaceAnimateControl->SetAttribute(_T("bk.image"), I18NSTR(_T("#ai_face_animate_0")));
        m_pFaceAnimateControl->SetVisible(false);
    }

	switch (uiFaceType)
	{
	case FaceInitStype:
	{
		WCHAR szBuf[1024];
		_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrFaceLearnCommonTitle")), m_strFaceQuestion.c_str());
		if (m_pBtnFaceTitleBottom)
			m_pBtnFaceTitleBottom->SetText(szBuf);

		if (m_pFaceTitleTwo)
			m_pFaceTitleTwo->SetVisible(true);

		//if (m_pBtnFaceBegin)
		//	m_pBtnFaceBegin->SetVisible(true);

		if (m_pFaceRecongnition)
		{
			ImageStyle * pImageStyle = m_pFaceRecongnition->GetImageStyle(_T("bk"));
			if (pImageStyle)
			{
				pImageStyle->SetAttribute(_T("image"), _T("#ai_face_recongnition"));
			}
		}

		//false
		if (m_pBtnFaceCommonTitle)
			m_pBtnFaceCommonTitle->SetVisible(false);

		if (m_pBtnFaceViewResult)
			m_pBtnFaceViewResult->SetVisible(false);

		if (m_pFaceFailLayout)
			m_pFaceFailLayout->SetVisible(false);
	}
		break;
	case FaceFailRetry:
	{
		WCHAR szBuf[1024];
		_stprintf_s(szBuf, _countof(szBuf), I18NSTR(_T("#StrFaceLearnCommonTitle")), m_strFaceQuestion.c_str());
		if (m_pBtnFaceCommonTitle)
		{
			m_pBtnFaceCommonTitle->SetText(szBuf);
			m_pBtnFaceCommonTitle->SetVisible(true);
		}

		if (m_pBtnFaceTitleRetry)
		{
			wstring strLeftTimes = _T("");
			switch (m_nFaceTotalTimes - m_nFaceCurTimes)
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
			m_pBtnFaceTitleRetry->SetText(szBuf);
		}

		if (m_pFaceFailLayout)
			m_pFaceFailLayout->SetVisible(true);

		//false
		if (m_pFaceTitleTwo)
			m_pFaceTitleTwo->SetVisible(false);
			
		if (m_pBtnFaceBegin)
			m_pBtnFaceBegin->SetVisible(false);

		if (m_pBtnFaceViewResult)
			m_pBtnFaceViewResult->SetVisible(false);

		if (m_pFaceRecongnition)
		{
			ImageStyle * pImageStyle = m_pFaceRecongnition->GetImageStyle(_T("bk"));
			if (pImageStyle)
			{
				pImageStyle->SetAttribute(_T("image"), _T("#ai_face_recongnition_fail"));
			}
		}
	}
		break;
	case FaceFailViewResult:
	{
		if (m_pBtnFaceCommonTitle)
		{
			m_pBtnFaceCommonTitle->SetText(I18NSTR(_T("#StrFaceNotGoodViewResult")));
			m_pBtnFaceCommonTitle->SetVisible(true);
		}

		if (m_pBtnFaceViewResult)
			m_pBtnFaceViewResult->SetVisible(true);

		//false
		if (m_pFaceFailLayout)
			m_pFaceFailLayout->SetVisible(false);

		if (m_pFaceTitleTwo)
			m_pFaceTitleTwo->SetVisible(false);

		if (m_pFaceRecongnition)
		{
			ImageStyle * pImageStyle = m_pFaceRecongnition->GetImageStyle(_T("bk"));
			if (pImageStyle)
			{
				pImageStyle->SetAttribute(_T("image"), _T("#ai_face_recongnition_fail"));
			}
		}
	}
		break;
	case SuccessViewResult:
	{
		if (m_pBtnFaceCommonTitle)
		{
			m_pBtnFaceCommonTitle->SetText(I18NSTR(_T("#StrFaceSuccess")));
			m_pBtnFaceCommonTitle->SetVisible(true);
		}

		if (m_pBtnFaceViewResult)
			m_pBtnFaceViewResult->SetVisible(true);

		//false
		if (m_pFaceFailLayout)
			m_pFaceFailLayout->SetVisible(false);

		if (m_pFaceTitleTwo)
			m_pFaceTitleTwo->SetVisible(false);
	}
		break;
	default:
		break;
	}
}

void CAIFaceLearnWnd::OnCreate()
{
	__super::OnCreate();

	m_pBtnFaceCommonTitle = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_common_title")));
	m_pBtnFaceTitleTop = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_title_top")));
	m_pBtnFaceTitleBottom = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_title_bottom")));
	m_pFaceLayout = dynamic_cast<CLayoutUI*>(FindControl(_T("layout_face")));
	m_pFaceTitleTwo = dynamic_cast<CLayoutUI*>(FindControl(_T("face_title_two")));
	m_pBtnFaceBegin = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_begin")));
	m_pBtnFaceViewResult = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_view_result")));
	m_pFaceFailLayout = dynamic_cast<CLayoutUI*>(FindControl(_T("AIFaceFailLayout")));
	m_pBtnFaceTitleRetry = dynamic_cast<CButtonUI*> (FindControl(_T("btn_face_title_Retry")));
	m_pFaceRecongnition = dynamic_cast<CLayoutUI*>(FindControl(_T("layout_face_recongnition_id")));
    m_pFaceAnimateControl = this->FindControl(_T("AIFaceAnimateControl"));
    if (m_pFaceAnimateControl)
        m_pFaceAnimateControl->SetVisible(false);


	if (m_pAILeftBtnPanel
		&& m_pAILeftBtnPanelUnExpend)
	{
		m_pAILeftBtnPanel->SetVisible(true);
		m_pAILeftBtnPanelUnExpend->SetVisible(false);
	}

    CApplication::GetInstance()->m_pAIFaceLearnWnd = this;

	GetRoot()->OnEvent += MakeDelegate(this, &CAIFaceLearnWnd::OnTimer);
}

void CAIFaceLearnWnd::OnClose()
{
    CApplication::GetInstance()->m_pAIFaceLearnWnd = nullptr;

	COpencvControl::GetInstance()->EndFace();
}

LRESULT CAIFaceLearnWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_FACE_HBITMAP)
	{
		if (m_pFaceLayout)
		{
			//DibObj obj;
			//obj.Create(::GetDC(NULL), 960, 540);
			//BYTE* bByte = obj.GetBits();
			//COpencvControl::GetInstance()->CopyBitsData(bByte, COpencvControl::GetInstance()->m_Imgdecode);
			//ImageObj* imgObj = new ImageObj(obj.Detach());

			//HBITMAP hbmp = COpencvControl::GetInstance()->MatImage2CBitmap(::GetDC(NULL), COpencvControl::GetInstance()->m_Imgdecode);
			//ImageObj* imgObj = new ImageObj(hbmp);
		

			ImageStyle * pImageStyle = m_pFaceLayout->GetImageStyle(_T("bk"));
			if (pImageStyle)
			{
				//pImageStyle->SetImage(imgObj);
				pImageStyle->SetAttribute(_T("image"), CommonUtil::GetFaceImagePath().c_str());
			}

			m_pFaceLayout->Invalidate();
		}
	}
	else if (message == WM_FACE_AUDIO_CALL_END)
	{
		Switch(FaceInitStype);

		//����������ɣ�������ͷ
		BeginFace();

		Sleep(100);

		//COpencvControl::GetInstance()->BeginFaceExpRecognition();

		COpencvControl::GetInstance()->BeginFaceExpRecognition();

		SetTimer(GetRoot(), AIFACE_LEARN_TIMERID, AIFACE_LEARN_TIMES);
		SetTimer(GetRoot(), AIFACE_RECORD_TIMERID, AIFACE_RECORD_TIMES);

		HideBelowControl();
	}

	return __super::WindowProc(message, wParam, lParam);
}

bool CAIFaceLearnWnd::OnTimer(TEventUI& event)
{
	if (event.nType == UIEVENT_TIMER)
	{
		if (event.wParam == AIFACE_LEARN_TIMERID)
		{
			map<string, int> mapExp = COpencvControl::GetInstance()->GetExpress();

			string strQuestionTmp = CommonUtil::UnicodeToUtf8(m_strFaceQuestion.c_str());
			transform(strQuestionTmp.begin(), strQuestionTmp.end(), strQuestionTmp.begin(), ::tolower);

			auto ele = mapExp.find(strQuestionTmp);
			if (ele != mapExp.end())
			{
				COpencvControl::GetInstance()->EndFaceExpRecognition();

				// ʶ��ɹ�
				m_bFaceSuccess = true;
				KillTimer(GetRoot(), AIFACE_LEARN_TIMERID);
				KillTimer(GetRoot(), AIFACE_RECORD_TIMERID);

				//�鿴�ɼ�
				Switch(SuccessViewResult);
			}
		}
		else if (event.wParam == AIFACE_RECORD_TIMERID)
		{
			COpencvControl::GetInstance()->EndFaceExpRecognition();

			KillTimer(GetRoot(), AIFACE_LEARN_TIMERID);
			KillTimer(GetRoot(), AIFACE_RECORD_TIMERID);

			map<string, int> mapExp = COpencvControl::GetInstance()->GetExpress();

			string strQuestionTmp = CommonUtil::UnicodeToUtf8(m_strFaceQuestion.c_str());
			transform(strQuestionTmp.begin(), strQuestionTmp.end(), strQuestionTmp.begin(), ::tolower);

			auto ele = mapExp.find(strQuestionTmp);
			if (ele != mapExp.end())
			{
				//ʶ��ɹ����鿴�ɼ�
				m_bFaceSuccess = true;
				Switch(SuccessViewResult);
			}
			else
			{
				if (m_nFaceCurTimes >= m_nFaceTotalTimes)
				{
					//���û��ģ��ȿ������εĳɼ�
					m_bFaceSuccess = false;
					Switch(FaceFailViewResult);
					return true;
				}

				//��Ҫ����
				Switch(FaceFailRetry);
				m_nFaceCurTimes++;
			}
		}
        else if (event.wParam == FACE_ANIMATE_TIMERID)
        {
            if (m_pFaceAnimateControl == nullptr)
                return true;

            if (m_nCurFaceAnimateFrame > 74)
                m_nCurFaceAnimateFrame = 0;

            TCHAR szPath[MAX_PATH];
            _stprintf_s(szPath, _T("#ai_face_animate_%d"), m_nCurFaceAnimateFrame);
            m_pFaceAnimateControl->SetAttribute(_T("bk.image"), szPath);
            m_pFaceAnimateControl->GetParent()->Invalidate();

            m_nCurFaceAnimateFrame++;
        }
	}

	return true;
}

bool CAIFaceLearnWnd::OnEventReturn(TNotifyUI* pTNotify)
{
    //�ر��������鴰��
    CloseWindow();

    //�ر�ѧϰ��Ƭ����
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //���������
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->ShowWindowData();

    return true;
}

bool CAIFaceLearnWnd::OnEventLeave(TNotifyUI* pTNotify)
{
    //�ر��������鴰��
    CloseWindow();

    //�ر�ѧϰ��Ƭ����
    if (CApplication::GetInstance()->m_pAISpeakLearnWnd != nullptr)
        (CApplication::GetInstance()->m_pAISpeakLearnWnd)->CloseWindow();

    //�رջ�б���
    if (CApplication::GetInstance()->m_pAIActivityWnd != nullptr)
        (CApplication::GetInstance()->m_pAIActivityWnd)->CloseWindow();

    //������������
    if (CApplication::GetInstance()->m_pAICabinWnd != nullptr)
        (CApplication::GetInstance()->m_pAICabinWnd)->ShowEndLayout();

    return true;
}

//ʹ��pTNotify ��Ҫ�ж��Ƿ�Ϊ��
bool CAIFaceLearnWnd::OnBtnViewResult(TNotifyUI* pTNotify)
{
    //���뵽�ɼ��������

    CAIViewResultsWnd* pAIViewResultsWnd = new CAIViewResultsWnd;
    if (pAIViewResultsWnd)
    {
        int nScore = 0;
        if (m_bSuccess)//��Ƭ�ʶ��ɹ����
            nScore += 50;
        
        //��������ʶ��ɹ���� �����ӱ�������޸ģ�
		if (m_bFaceSuccess)//��������ʶ��ɹ����
			nScore += 50;
        
        time_t tNowTime = 0;
        time(&tNowTime);

		time_t tBeginTime = 0;
		if (CApplication::GetInstance()->m_pAISpeakLearnWnd)
			tBeginTime = (CApplication::GetInstance()->m_pAISpeakLearnWnd)->GetBeginTime();

		int nTimes = tNowTime - tBeginTime;
		pAIViewResultsWnd->SetPercentage(nScore, nTimes);
        pAIViewResultsWnd->CreateWnd(CApplication::GetInstance()->GetMainUIHwnd());
        pAIViewResultsWnd->ShowWindow();
    }

	COpencvControl::GetInstance()->EndFace();

    return true;
}

bool CAIFaceLearnWnd::OnBtnFaceBegin(TNotifyUI* pTNotify)
{
	COpencvControl::GetInstance()->BeginFaceExpRecognition();

	SetTimer(GetRoot(), AIFACE_LEARN_TIMERID,AIFACE_LEARN_TIMES);
	SetTimer(GetRoot(), AIFACE_RECORD_TIMERID, AIFACE_RECORD_TIMES);

    HideBelowControl();

	return true;
}

bool CAIFaceLearnWnd::OnBtnFaceNoRetry(TNotifyUI* pTNotify)
{
	m_bFaceSuccess = false;

	OnBtnViewResult(nullptr);

	return true;
}

void CAIFaceLearnWnd::HideBelowControl()
{
    if (m_pFaceAnimateControl)
        m_pFaceAnimateControl->SetVisible(true);

    m_nCurFaceAnimateFrame = 0;
    this->SetTimer(GetRoot(), FACE_ANIMATE_TIMERID, FACE_ANIMATE_TIMES);

    if (m_pBtnFaceBegin)
        m_pBtnFaceBegin->SetVisible(false);

    if (m_pFaceFailLayout)
        m_pFaceFailLayout->SetVisible(false);

    if (m_pBtnFaceViewResult)
        m_pBtnFaceViewResult->SetVisible(false);
}

bool CAIFaceLearnWnd::OnBtnFaceRetry(TNotifyUI* pTNotify)
{
	COpencvControl::GetInstance()->BeginFaceExpRecognition();

	SetTimer(GetRoot(), AIFACE_LEARN_TIMERID, AIFACE_LEARN_TIMES);
	SetTimer(GetRoot(), AIFACE_RECORD_TIMERID, AIFACE_RECORD_TIMES);

	if (m_pFaceRecongnition)
	{
		ImageStyle * pImageStyle = m_pFaceRecongnition->GetImageStyle(_T("bk"));
		if (pImageStyle)
		{
			pImageStyle->SetAttribute(_T("image"), _T("#ai_face_recongnition"));
		}
	}

    HideBelowControl();

	return true;
}
