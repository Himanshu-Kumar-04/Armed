#include<armed.h>
#include"EditorLayer.h"

//---EntryPoint---------
#include "Armed/core/entryPoint.h"

namespace Arm {
	class Editor : public Application {
	public:
		Editor()
		: Application("Armed Editor") 
		{
			pushLayer(new EditorLayer());
		}
		~Editor() {

		}
	};

	Application* createApplication() {
		return new Editor();
	}
}