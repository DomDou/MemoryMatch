#include <RGF/Game.h>
#include "MemoryScene.h"

int main()
{
	rgf::Game memory("res/configs/MemoryMatchSettings.xml");
	memory.pushScene(rgf::Scene::Ptr(new MemoryScene(memory)));
	memory.start();
}