#include "Tpch.h"
#include "Renderer.h"

#include "Tiny/Renderer/RenderCommand.h"
#include "Tiny/Renderer/Renderer2D.h"

namespace Tiny {

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		RenderCommand::Shutdown();
		Renderer2D::Shutdown();
	}

	void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(x, y, width, height);
	}

}


























//void Renderer::Init(unsigned int& vao, unsigned int& vbo, std::vector<Vertex> vertices)
//{
//	////Enable depth test (Z Buffer)
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	//Wireframe
//	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//
//	//Enable blending
//	//Culling
//	/*glDisable(GL_CULL_FACE);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
//
//	//Create a Vertex Array Object 
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	//Create a Vertex Buffer Object
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//	glBufferData(GL_ARRAY_BUFFER,
//		sizeof(Vertex) * vertices.size(),
//		&vertices[0],
//		GL_STATIC_DRAW);
//
//	//Setup Vertex Specifications or Attributes 
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	/*glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::color));*/
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));
//
//
//	//Create an Element buffer Object
//	/*glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);*/
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//
//
//}
//
//void Renderer::Init(unsigned int& vao, unsigned int& vbo, unsigned int& ebo, std::vector<Vertex> vertices, std::vector<unsigned int> indices)
//{
//	////Enable depth test (Z Buffer)
//	glEnable(GL_DEPTH_TEST);
//	glDepthFunc(GL_LESS);
//
//	//Create a Vertex Array Object 
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//
//	//Create a Vertex Buffer Object
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//
//	glBufferData(GL_ARRAY_BUFFER,
//		sizeof(Vertex) * vertices.size(),
//		&vertices[0],
//		GL_STATIC_DRAW);
//
//	//Setup Vertex Specifications or Attributes 
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	/*glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::color));*/
//	/*glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));*/
//
//
//	//Create an Element buffer Object
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//}

//void Renderer::Draw(unsigned int ShaderProgram, unsigned int& vao, unsigned int& ebo,unsigned int count)
//{
//	//Shader Program Set up 
//	GLCall(glUseProgram(ShaderProgram));

//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.f);

//	glm::mat4 view = glm::lookAt(glm::vec3(0.0f,0.0f,2.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
//	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
//	rotation = glm::rotate(rotation, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::mat4 rotation = glm::mat4(1.0f);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

//	model = translation * rotation * scale * model;

//	glm::mat4 PVM = projection * view * model;

//	int location = glGetUniformLocation(ShaderProgram, "PVM");
//	T_CORE_ASSERT(location != -1, "Failed to get uniform location");
//	glUniformMatrix4fv(location, 1, GL_FALSE, &PVM[0][0]);

//	GLCall(glBindVertexArray(vao));
//	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

//	GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
//	

//}

//void Renderer::Draw(unsigned int ShaderProgram, unsigned int& vao, unsigned int& ebo, unsigned int count, glm::vec3 pos)
//{
//	//Shader Program Set up 
//	GLCall(glUseProgram(ShaderProgram));

//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//	glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
//	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.f);

//	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
//	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	rotation = glm::rotate(rotation, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::mat4 rotation = glm::mat4(1.0f);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));

//	model = translation * rotation * scale * model;

//	glm::mat4 PVM = projection * view * model;

//	int location = glGetUniformLocation(ShaderProgram, "PVM");
//	T_CORE_ASSERT(location != -1, "Failed to get uniform location");
//	glUniformMatrix4fv(location, 1, GL_FALSE, &PVM[0][0]);

//	GLCall(glBindVertexArray(vao));
//	//GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));

//	//GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
//	GLCall(glDrawArrays(GL_TRIANGLES, 0, 2880));

//}

//void Renderer::Draw(unsigned int ShaderProgram, unsigned int& vao, unsigned int count, glm::vec3 pos)
//{
//	//Shader Program Set up 
//	GLCall(glUseProgram(ShaderProgram));

//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.f);

//	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
//	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	rotation = glm::rotate(rotation, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::mat4 rotation = glm::mat4(1.0f);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));

//	model = translation * rotation * scale * model;

//	glm::mat4 PVM = projection * view * model;

//	int location = glGetUniformLocation(ShaderProgram, "PVM");
//	T_CORE_ASSERT(location != -1, "Failed to get uniform location");
//	glUniformMatrix4fv(location, 1, GL_FALSE, &PVM[0][0]);
//	
//	int texLocation = glGetUniformLocation(ShaderProgram, "myTextureSampler");
//	T_CORE_ASSERT(texLocation != -1, "Failed to get uniform location");
//	glUniform1i(texLocation, 0);

//	GLCall(glBindVertexArray(vao));

//	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));

//}

//void Renderer::DrawRotatingPrimitiveZ(Ref<Shader>& shader, unsigned int& vao, unsigned int count, glm::vec3 pos, float rotation)
//{

//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);
//	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 960.0f, 0.1f, 100.f);

//	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
//	glm::mat4 __rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	__rotation = glm::rotate(__rotation, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	__rotation = glm::rotate(__rotation, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::mat4 rotation = glm::mat4(1.0f);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));

//	model = translation * __rotation * scale * model;

//	glm::mat4 PVM = projection * view * model;


//	/*int location = glGetUniformLocation(ShaderProgram, "PVM");
//	T_CORE_ASSERT(location != -1, "Failed to get uniform location");
//	glUniformMatrix4fv(location, 1, GL_FALSE, &PVM[0][0]);*/
//	/*int texLocation = glGetUniformLocation(ShaderProgram, "myTextureSampler");
//	T_CORE_ASSERT(texLocation != -1, "Failed to get uniform location");
//	glUniform1i(texLocation, 0);*/

//	shader->Bind();
//	shader->SetMat4("PVM", PVM);
//	shader->SetInt("myTextureSampler", 0);

//	GLCall(glBindVertexArray(vao));

//	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
//}

//void Renderer::DrawPrimitiveCamera(Ref<Shader>& shader, unsigned int& vao, unsigned int count, glm::vec3 pos, Camera& camera)
//{

//	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

//	glm::mat4 translation = glm::translate(glm::mat4(1.0f), pos);
//	glm::mat4 __rotation = glm::rotate(glm::mat4(1.0f), glm::radians(25.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//	__rotation = glm::rotate(__rotation, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	__rotation = glm::rotate(__rotation, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	//glm::mat4 rotation = glm::mat4(1.0f);
//	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));

//	model = translation * __rotation * scale * model;

//	//glm::mat4 PVM = camera.GetProjectionViewMatrix() * model;
//	glm::mat4 PVM = m_SceneData->ProjectionViewMatrix * model;

//	/*int location = glGetUniformLocation(ShaderProgram, "PVM");
//	T_CORE_ASSERT(location != -1, "Failed to get uniform location");
//	glUniformMatrix4fv(location, 1, GL_FALSE, &PVM[0][0]);*/
//	/*int texLocation = glGetUniformLocation(ShaderProgram, "myTextureSampler");
//	T_CORE_ASSERT(texLocation != -1, "Failed to get uniform location");
//	glUniform1i(texLocation, 0);*/

//	shader->Bind();
//	shader->SetMat4("PVM", PVM);
//	shader->SetInt("myTextureSampler", 0);

//	GLCall(glBindVertexArray(vao));

//	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
//}