#include "pch.h"
#include "ThemeHelper.h"
#include "Win32Utils.h"

namespace Magpie {

enum class PreferredAppMode {
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};

using fnSetPreferredAppMode = PreferredAppMode (WINAPI*)(PreferredAppMode appMode);
using fnAllowDarkModeForWindow = bool (WINAPI*)(HWND hWnd, bool allow);
using fnRefreshImmersiveColorPolicyState = void (WINAPI*)();
using fnFlushMenuThemes = void (WINAPI*)();

static fnSetPreferredAppMode SetPreferredAppMode = nullptr;
static fnAllowDarkModeForWindow AllowDarkModeForWindow = nullptr;
static fnRefreshImmersiveColorPolicyState RefreshImmersiveColorPolicyState = nullptr;
static fnFlushMenuThemes FlushMenuThemes = nullptr;

static void ImportFuncs() noexcept {
	if (SetPreferredAppMode) {
		return;
	}

	HMODULE hUxtheme = LoadLibrary(L"uxtheme.dll");
	assert(hUxtheme);

	SetPreferredAppMode = (fnSetPreferredAppMode)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(135));
	AllowDarkModeForWindow = (fnAllowDarkModeForWindow)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(133));
	RefreshImmersiveColorPolicyState = (fnRefreshImmersiveColorPolicyState)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(104));
	FlushMenuThemes = (fnFlushMenuThemes)GetProcAddress(hUxtheme, MAKEINTRESOURCEA(136));
}

void ThemeHelper::Initialize() noexcept {
	ImportFuncs();

	SetPreferredAppMode(PreferredAppMode::AllowDark);
	RefreshImmersiveColorPolicyState();
}

void ThemeHelper::SetTheme(HWND hWnd, bool isDark) noexcept {
	ImportFuncs();

	SetPreferredAppMode(isDark ? PreferredAppMode::ForceDark : PreferredAppMode::Default);
	AllowDarkModeForWindow(hWnd, isDark);

	// 使标题栏适应黑暗模式
	// build 18985 之前 DWMWA_USE_IMMERSIVE_DARK_MODE 的值不同
	// https://github.com/MicrosoftDocs/sdk-api/pull/966/files
	constexpr const DWORD DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1 = 19;
	BOOL value = isDark;
	DwmSetWindowAttribute(
		hWnd,
		Win32Utils::GetOSVersion().Is20H1OrNewer() ? DWMWA_USE_IMMERSIVE_DARK_MODE : DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1,
		&value,
		sizeof(value)
	);

	RefreshImmersiveColorPolicyState();
	FlushMenuThemes();

	LONG_PTR style = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	if (!Win32Utils::GetOSVersion().IsWin11()) {
		// 在 Win10 上需要更多 hack
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, style | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, 254, LWA_ALPHA);
	}
	SetWindowLongPtr(hWnd, GWL_EXSTYLE, style);
}

}
