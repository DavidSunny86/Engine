#include "IMousePositionHandler.h"
#include "Input/Manager/MousePositionManager.h"


IMousePositionHandler::IMousePositionHandler()
{
    MousePositionManager::Instance()->Subscribe(this);
}


IMousePositionHandler::~IMousePositionHandler()
{
    MousePositionManager::Instance()->Unsubscribe(this);
}
