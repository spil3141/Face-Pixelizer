#include "Tiny.h"
#include "Tiny/Core/EntryPoint.h"

#include "EditorLayer.h"

class PixelizerAPP : public Tiny::Application
{
public:
	PixelizerAPP()
		:Application("Face-Pixelizer Editor", 1280, 720)
	{
		PushLayer(new EditorLayer());
	}
	~PixelizerAPP() = default;

};

Tiny::Application* Tiny::CreateApplication()
{
	return new PixelizerAPP();
}
