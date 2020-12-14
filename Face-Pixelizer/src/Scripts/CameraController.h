#pragma once 
#include <Tiny.h>

using namespace Tiny;
class CameraController : public ScriptableEntity
{
public:
	bool EnableControls = false;
	CameraComponent* cam = nullptr;
	float Zoom = 1.0f;

protected:
	void OnCreate() override
	{
		cam = &GetComponent<CameraComponent>();
	}


	void OnDestroy() override
	{
		throw std::logic_error("The method or operation is not implemented.");
	}


	void OnUpdate(Timestep ts) override
	{
		if (EnableControls && cam != nullptr)
		{
			cam->Camera.SetOrthographicSize(Zoom);
		}
	}

public:
	float GetOrthoSize() const
	{
		if (cam != nullptr)
			return cam->Camera.GetOrthographicSize();
		else
			return 0.0f;
	}

};