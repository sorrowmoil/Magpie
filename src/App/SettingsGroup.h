#pragma once

#include "winrt/Windows.UI.Xaml.h"
#include "winrt/Windows.UI.Xaml.Markup.h"
#include "winrt/Windows.UI.Xaml.Interop.h"
#include "winrt/Windows.UI.Xaml.Controls.Primitives.h"
#include "SettingsGroup.g.h"


namespace winrt::Magpie::implementation {

struct SettingsGroup : SettingsGroupT<SettingsGroup> {
	SettingsGroup();

	void Title(const hstring& value);

	hstring Title() const;

	void Description(Windows::Foundation::IInspectable value);

	Windows::Foundation::IInspectable Description() const;

	void SettingsGroup_IsEnabledChanged(Windows::Foundation::IInspectable const&, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const&);

	void SettingsGroup_Loading(Windows::UI::Xaml::FrameworkElement const&, Windows::Foundation::IInspectable const&);

	static const Windows::UI::Xaml::DependencyProperty TitleProperty;
	static const Windows::UI::Xaml::DependencyProperty DescriptionProperty;

private:
	static void _OnPropertyChanged(Windows::UI::Xaml::DependencyObject const& sender, Windows::UI::Xaml::DependencyPropertyChangedEventArgs const& args);

	void _Update();

	void _SetEnabledState();
};

}

namespace winrt::Magpie::factory_implementation {

struct SettingsGroup : SettingsGroupT<SettingsGroup, implementation::SettingsGroup> {
};

}