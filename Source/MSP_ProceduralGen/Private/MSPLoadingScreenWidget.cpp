#include "MSPLoadingScreenWidget.h"

#include "MoviePlayer.h"
#include "Slate/SlateTextures.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Images/SThrobber.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"


void SMSPLoadingScreenWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SThrobber)
			.Visibility(this, &SMSPLoadingScreenWidget::GetLoadIndicatorVisibility)
		]
		+ SVerticalBox::Slot()
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(STextBlock)
			.Text(NSLOCTEXT("MoviePlayerTestLoadingScreen", "LoadingComplete", "Loading complete!"))
		.Visibility(this, &SMSPLoadingScreenWidget::GetMessageIndicatorVisibility)
		]
	];
}

EVisibility SMSPLoadingScreenWidget::GetLoadIndicatorVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Collapsed : EVisibility::Visible;
}

EVisibility SMSPLoadingScreenWidget::GetMessageIndicatorVisibility() const
{
	return GetMoviePlayer()->IsLoadingFinished() ? EVisibility::Visible : EVisibility::Collapsed;
}
