#include "pch.h"
#include "HotkeySettings.h"
#if __has_include("HotkeySettings.g.cpp")
#include "HotkeySettings.g.cpp"
#endif

#include "Win32Utils.h"


namespace winrt::Magpie::App::implementation {

void HotkeySettings::CopyFrom(const Magpie::App::HotkeySettings& other) {
	HotkeySettings* otherImpl = get_self<HotkeySettings>(other.as<default_interface<HotkeySettings>>());
	_win = otherImpl->_win;
	_ctrl = otherImpl->_ctrl;
	_alt = otherImpl->_alt;
	_shift = otherImpl->_shift;
	_code = otherImpl->_code;
}

bool HotkeySettings::IsEmpty() const {
	return !_win && !_ctrl && !_alt && !_shift && _code == 0;
}

bool HotkeySettings::Equals(const Magpie::App::HotkeySettings& other) const {
	HotkeySettings* otherImpl = get_self<HotkeySettings>(other.as<default_interface<HotkeySettings>>());
	return _win == otherImpl->_win
		&& _ctrl == otherImpl->_ctrl
		&& _alt == otherImpl->_alt
		&& _shift == otherImpl->_shift
		&& _code == otherImpl->_code;
}

IVector<IInspectable> HotkeySettings::GetKeyList() const {
	std::vector<IInspectable> shortcutList;
	if (_win) {
		// Windows 键
		shortcutList.push_back(box_value(92));
	}

	if (_ctrl) {
		shortcutList.push_back(box_value(L"Ctrl"));
	}

	if (_alt) {
		shortcutList.push_back(box_value(L"Alt"));
	}

	if (_shift) {
		shortcutList.push_back(box_value(L"Shift"));
	}

	if (_code > 0) {
		switch (_code) {
		   // https://docs.microsoft.com/en-us/uwp/api/windows.system.virtualkey?view=winrt-20348
		case 38: // The Up Arrow key or button.
		case 40: // The Down Arrow key or button.
		case 37: // The Left Arrow key or button.
		case 39: // The Right Arrow key or button.
		// case 8: // The Back key or button.
		// case 13: // The Enter key or button.
			shortcutList.push_back(box_value(_code));
			break;
		default:
			std::wstring localKey = Win32Utils::GetKeyName(_code);
			shortcutList.push_back(box_value(localKey));
			break;
		}
	}

	return single_threaded_vector(std::move(shortcutList));
}

bool HotkeySettings::Check() const {
	if (!_win && !_ctrl && !_alt && !_shift) {
		return false;
	}

	UINT modifiers = MOD_NOREPEAT;
	if (_win) {
		modifiers |= MOD_WIN;
	}
	if (_ctrl) {
		modifiers |= MOD_CONTROL;
	}
	if (_alt) {
		modifiers |= MOD_ALT;
	}
	if (_shift) {
		modifiers |= MOD_SHIFT;
	}
	if (!RegisterHotKey(NULL, 0, modifiers, _code)) {
		return false;
	}
	
	UnregisterHotKey(NULL, 0);
	return true;
}

}