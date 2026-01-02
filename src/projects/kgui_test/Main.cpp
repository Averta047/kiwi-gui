//========= Copyright KiwiEngine, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#if 0
#pragma comment(linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")
#endif

#include <windows.h>

#include <KForm.h>
#include <KButton.h>
#include <KCheckBox.h>

class MainGui
{
public:
	MainGui();
	~MainGui();

public:
	bool Initialize();
	void Run();
	void Release();

public:
	void OnUpdate();

public:
	LRESULT OnClickEvent(KObject*, KEventParams_t);

private:
	KForm*		m_pMainWindow;
	KButton*	m_pButton;
	KCheckBox*	m_pCheckBox;
};

int main()
{
	MainGui app;

	if (app.Initialize()) {
		app.Run();
	}

	app.Release();

	return 0;
}

MainGui::MainGui()
{
	m_pMainWindow	= nullptr;
	m_pButton		= nullptr;
	m_pCheckBox		= nullptr;
}

MainGui::~MainGui()
{

}

bool MainGui::Initialize()
{
	//-----------------------------------------------------------------------------
	//	Window Creation
	//-----------------------------------------------------------------------------
	m_pMainWindow = new KForm();
	if (!m_pMainWindow)
	{
		return false;
	}

	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_pMainWindow->Text = TEXT("KGui - test application");
	m_pMainWindow->Width = 632;
	m_pMainWindow->Height = 431;
	m_pMainWindow->X = iScreenWidth / 2 - m_pMainWindow->Width / 2;
	m_pMainWindow->Y = iScreenHeight / 2 - m_pMainWindow->Height / 2;
	m_pMainWindow->OnDestroyEvent = [](KObject*, KEventParams_t)
		{
			constexpr const LRESULT quit_application = 1;
			return quit_application;
		};

	//-----------------------------------------------------------------------------
	//	Button Creation
	//-----------------------------------------------------------------------------
	m_pButton = new KButton(m_pMainWindow);
	if (!m_pButton)
	{
		return false;
	}

	m_pButton->Text = TEXT("Push Button");
	m_pButton->X = 10;
	m_pButton->Y = 10;
	m_pButton->Width = 100;
	m_pButton->Height = 26;
	m_pButton->OnClickEvent = std::bind(&MainGui::OnClickEvent, this, std::placeholders::_1, std::placeholders::_2);

	//-----------------------------------------------------------------------------
	//	CheckBox Creation
	//-----------------------------------------------------------------------------
	m_pCheckBox = new KCheckBox(m_pMainWindow);
	if (!m_pCheckBox)
		return false;

	m_pCheckBox->Text = TEXT("Check Box");
	m_pCheckBox->X = 10;
	m_pCheckBox->Y = (int)m_pButton->Y + (int)m_pButton->Height + 10;
	m_pCheckBox->Width = 100;

	m_pMainWindow->Show();

	return true;
}

LRESULT MainGui::OnClickEvent(KObject*, KEventParams_t)
{
	return 1;
}

void MainGui::Run()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		OnUpdate();
	}
}

void MainGui::Release()
{
	if (m_pCheckBox) {
		m_pCheckBox->Destroy();
		delete m_pCheckBox;
		m_pCheckBox = nullptr;
	}
	
	if (m_pButton) {
		m_pButton->Destroy();
		delete m_pButton;
		m_pButton = nullptr;
	}

	if (m_pMainWindow) {
		m_pMainWindow->Destroy();
		delete m_pMainWindow;
		m_pMainWindow = nullptr;
	}
}

void MainGui::OnUpdate()
{

}
