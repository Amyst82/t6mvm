#pragma once
#include <StdInclude.h>
#include "wintoastlib.h"
using namespace WinToastLib;
namespace WinToastHelper
{
	class CustomHandler : public IWinToastHandler 
	{
	public:
		void toastActivated() const 
		{
			std::wcout << L"The user clicked in this toast" << std::endl;
			exit(0);
		}

		void toastActivated(int actionIndex) const {
			std::wcout << L"The user clicked on action #" << actionIndex << std::endl;
			exit(16 + actionIndex);
		}

		void toastDismissed(WinToastDismissalReason state) const {
			switch (state) {
			case UserCanceled:
				std::wcout << L"The user dismissed this toast" << std::endl;
				break;
			case TimedOut:
				std::wcout << L"The toast has timed out" << std::endl;
				break;
			case ApplicationHidden:
				std::wcout << L"The application hid the toast using ToastNotifier.hide()" << std::endl;
				break;
			default:
				std::wcout << L"Toast not activated" << std::endl;
				break;
			}
		}

		void toastFailed() const {
			std::wcout << L"Error showing current toast" << std::endl;
			exit(5);
		}
	};
	static void send_toast_notification(const std::wstring& title, const std::wstring& text) 
	{
//		TCHAR NPath[MAX_PATH];
//		GetCurrentDirectory(MAX_PATH, NPath);
//
//#ifdef UNICODE
//		std::wstring ws(NPath);
//#else
//		int size_needed = MultiByteToWideChar(CP_ACP, 0, NPath, -1, NULL, 0);
//		std::wstring ws(size_needed, 0);
//		MultiByteToWideChar(CP_ACP, 0, NPath, -1, &ws[0], size_needed);
//		ws.resize(size_needed - 1);
//#endif
//
//		ws = ws + L"\\icon.ico";

		//Initialise WinToast
		if (!WinToast::isCompatible()) 
		{
			T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "T6MVM TOAST", "Your system in not supported!\r\n");
		}
		else 
		{
			const auto aumi = WinToast::configureAUMI(L"Team A", L"T6MVM", L"T6MVM_Recording", L"1.0.1");
			WinToast::instance()->setAppUserModelId(aumi);
			WinToast::instance()->setAppName(L"T6MVM");

			if (!WinToast::instance()->initialize()) 
			{
				T6SDK::ConsoleLog::LogTagged(T6SDK::ConsoleLog::C_ERROR, false, "T6MVM TOAST", "Your system in not compatible!\r\n");
			}
		}

		WinToastTemplate templ = WinToastTemplate(WinToastTemplate::ImageAndText02);

		templ.setTextField(title, WinToastTemplate::FirstLine);
		templ.setTextField(text, WinToastTemplate::SecondLine);
		/*templ.setImagePath(ws);*/
		WinToast::instance()->showToast(templ, new CustomHandler());
	}
}