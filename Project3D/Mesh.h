#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

/// <summary>
/// Mesh is a wrapper class used to manage the initialisation and OpenGL based drawing
/// of a mesh based objects. It holds member variables to track the tri count, vertex
/// array and buffer objects, as well as the index buffer object if this Mesh is
/// initialised with one. The class on initialise() will generate and bind the OpenGL
/// buffers required to draw this mesh, and will also enable the vertex data as OpenGL
/// attributes. On draw(), the class will call one of two OpenGL glDraw functions depending
/// on whether or not the mesh has been set up to draw using an index buffer.
/// </summary>
class Mesh
{
public:

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {} // Initialise members to 0, as they are properly initialised in the initialise() function
	virtual ~Mesh();

	/// <summary>
	/// The Vertex struct contains a standard structure of vertex information
	/// for storing the position, normal and UV coordinate of each mesh node.
	/// </summary>
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec2 texCoord;
	};

	// Initialisation functions, the first is for an arbitrary mesh, and the other two are for specific mesh shapes
	void initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount = 0, unsigned int* indices = nullptr);
	void initialiseQuad();
	void initialiseFullscreenQuad(); // Covers the screen for post processing

	virtual void draw();

protected:

	unsigned int triCount; // Calculated once vertices and primative indices have been passed
	unsigned int vao, vbo, ibo; // Vertex Array and Buffer Objects, and Index Buffer Object

};

