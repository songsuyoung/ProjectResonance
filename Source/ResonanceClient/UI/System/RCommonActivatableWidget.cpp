#include "UI/System/RCommonActivatableWidget.h"

TOptional<FUIInputConfig> URCommonActivatableWidget::GetDesiredInputConfig() const
{
	return InputConfig;
}
