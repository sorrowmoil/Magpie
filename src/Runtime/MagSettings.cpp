#include "pch.h"
#include "MagSettings.h"
#if __has_include("MagSettings.g.cpp")
#include "MagSettings.g.cpp"
#endif


namespace winrt::Magpie::Runtime::implementation {

void MagSettings::CopyFrom(Magpie::Runtime::MagSettings other) {
	MagSettings* otherImpl = get_self<MagSettings>(other.as<default_interface<MagSettings>>());

	_captureMode = otherImpl->_captureMode;
	_isBreakpointMode = otherImpl->_isBreakpointMode;
	_isDisableEffectCache = otherImpl->_isDisableEffectCache;
	_isSaveEffectSources = otherImpl->_isSaveEffectSources;
	_isWarningsAreErrors = otherImpl->_isWarningsAreErrors;
	_isSimulateExclusiveFullscreen = otherImpl->_isSimulateExclusiveFullscreen;
	_is3DGameMode = otherImpl->_is3DGameMode;
	_isShowFPS = otherImpl->_isShowFPS;
}

}