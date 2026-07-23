#include "ScreenMessage.h"

#include "Components/TextBlock.h"

void UScreenMessage::SetMessageText(const FText& NewMessageText)
{
	MessageTextBlock->SetText(NewMessageText);
}
