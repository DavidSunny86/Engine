#include "Human.h"
#include "Manager/ModelManager.h"
#include "RenderTree/Environment/Environment.h"

Human::Human(AbstractNode* parent) : AbstractNode(parent)
{
}


Human::~Human()
{
}

void Human::Update(double deltaT)
{
    rotation_[2] += 0.1 * deltaT;
}
