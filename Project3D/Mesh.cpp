#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	// Make sure the mesh is not already initialised
	assert(vao == 0);

	// Generate the buffer and array object
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
	// Bind the buffers to be ready for editing/drawing
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Fill the buffer with the vertices array
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexCount, vertices, GL_STATIC_DRAW);

	// Enable the first element as position to tell the vao how to arrange the vertex data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// If an index buffer was sent, then we need to generate the IBO and bind it
	if (indexCount > 0)
	{
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		// Fill the buffer with the vertices array
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);

		// The number of tri's will depend on the number of indexes used
		triCount = indexCount / 3;
	}
	// If no index buffer was sent, just set triCount using vertexCount
	else
	{
		triCount = vertexCount / 3;
	}
	
	// Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseQuad()
{
	// Make sure the mesh is not already initialised
	assert(vao == 0);

	// Generate the buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the buffers to be ready for editing/drawing
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Define 6 vertices and for 2 triangles (not using ibo)
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	triCount = 2;

	// Fill the vbo with the vertices data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertices, GL_STATIC_DRAW);

	// Enable the first element as position to tell the vao how to arrange the vertex data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Unbind the buffers as we are done assigning to them
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::draw()
{
	glBindVertexArray(vao);

	// Check if we are drawing using an index buffer or just vertices
	if (ibo != 0)
	{
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, 3 * triCount);
	}
}
