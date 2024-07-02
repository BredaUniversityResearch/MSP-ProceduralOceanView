#pragma once

#include "Widgets/SCompoundWidget.h"

class SMSPLoadingScreenWidget : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SMSPLoadingScreenWidget) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);
	
private:

	EVisibility GetLoadIndicatorVisibility() const;

	EVisibility GetMessageIndicatorVisibility() const;
	
};
