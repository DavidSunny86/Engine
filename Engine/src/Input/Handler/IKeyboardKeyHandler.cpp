#include "IKeyboardKeyHandler.h"
#include "Input/Manager/KeyboardKeyManager.h"


IKeyboardKeyHandler::IKeyboardKeyHandler()
{
    KeyboardKeyManager::Instance()->Subscribe(this);
}


IKeyboardKeyHandler::~IKeyboardKeyHandler()
{
    KeyboardKeyManager::Instance()->Unsubscribe(this);
}
