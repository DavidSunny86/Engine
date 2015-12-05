#include "IMouseButtonHandler.h"
#include "Input/Manager/MouseButtonManager.h"


IMouseButtonHandler::IMouseButtonHandler()
{
    MouseButtonManager::Instance()->Subscribe(this);
}


IMouseButtonHandler::~IMouseButtonHandler()
{
    MouseButtonManager::Instance()->Unsubscribe(this);
}
