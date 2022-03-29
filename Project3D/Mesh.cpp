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

	// Enable the first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable the second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	// Enable the third element as textCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

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

	// Define 6 vertices and for 2 triangles (not using ibo), and set their corresponding UV coordinates
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 }; // bottom left 
	vertices[0].texCoord = { 0, 1 }; 
	vertices[0].normal = { 0, 1, 0, 0 }; 
	vertices[1].position = { 0.5f, 0, 0.5f, 1 }; // bottom right 
	vertices[1].texCoord = { 1, 1 }; 
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 }; // top left 
	vertices[2].texCoord = { 0, 0 }; 
	vertices[2].normal = { 0, 1, 0, 0 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 }; // top left
	vertices[3].texCoord = { 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 }; // bottom right 
	vertices[4].texCoord = { 1, 1 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 }; // top right 
	vertices[5].texCoord = { 1, 0 }; 
	vertices[5].normal = { 0, 1, 0, 0 };

	triCount = 2;

	// Fill the vbo with the vertices data
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 6, vertices, GL_STATIC_DRAW);

	// Enable the first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable the second element as normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)sizeof(glm::vec4));

	// Enable the third element as textCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// Unbind the buffers as we are done assigning to them
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initialiseFullscreenQuad()
{
	// Make sure the mesh is not already initialised
	assert(vao == 0);

	// Generate the buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the buffers to be ready for editing/drawing
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Define 6 vertices and for 2 triangles (not using ibo), and set their corresponding UV coordinates
	float vertices[] = {
		-1,1, // left top 
		-1,-1, // left bottom 
		1,1, // right top 
		-1,-1, // left bottom 
		1,-1, // right bottom 
		1, 1 // right top
	};
	triCount = 2;

	// Fill the vbo with the vertices data
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices, GL_STATIC_DRAW);

	// Enable the first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

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
