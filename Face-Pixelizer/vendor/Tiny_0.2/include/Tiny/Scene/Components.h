#pragma once

#include "glm/glm.hpp"
#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include "Tiny/Renderer/Texture.h"

namespace Tiny {

	struct TransformComponent
	{
		glm::mat4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& l_transform) // l for local parameter
			: transform(l_transform){}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () { return transform; }
	};

	struct TagComponent {

		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string l_tag)
			: tag(l_tag) {}

		operator std::string& () { return tag; }
		operator const std::string& () { return tag; }
		inline bool operator==(const std::string& other) { return strcmp(this->tag.c_str(), other.c_str()) == 0; }

	};

	struct SpriteRendererComponent
	{
		Ref<Texture2D> Texture;
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4 & color)
			: Color(color) {}
		SpriteRendererComponent(Ref<Texture2D> texture)
			: Texture(texture) {}
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}

