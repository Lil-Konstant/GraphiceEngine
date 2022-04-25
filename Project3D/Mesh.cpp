#include "Mesh.h"
#include <gl_core_4_4.h>

/// <summary>
/// The deconstructor for Mesh simply uses the appropriate OpenGL calls to delete the VAO, VBO and IBO variable
/// data.
/// </summary>
Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

/// <summary>
/// initialise() takes an input of the number of vertices to initialise for this mesh, a pointer to said
/// vertices, the number of indices to use for this mesh (if any), and a pointer to said indices (if any),
/// and will then generate, bind and fill the vertex array and buffer objects for this mesh to be used
/// when drawing, as well as the index buffer object if any indices have been passed to the function. The 
/// function finishes by filling the VBO with said vertices, and then enables the position, normal and 
/// texCoord variables as OpenGL attributes to be passed correctly to whichever shader is bound.
/// </summary>
/// <param name="vertexCount"></param>
/// <param name="vertices"></param>
/// <param name="indexCount"></param>
/// <param name="indices"></param>
void Mesh::initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	// Make sure the mesh is not already initialised
	assert(vao == 0);

	// Generate the buffer and array object
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);
	
	// Bind the buffers to be ready for manipulating
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

	// If an index buffer was sent, then we need to generate the IBO, bind and fill it
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

/// <summary>
/// initialiseQuad() is a utility function used to initialise this mesh object as a flat quad made up of two
/// tris (therefore 6 vertices as it doesn't make use of an index buffer). The quad created is 1 by 1 units
/// across in model space. The function initially follows the standard initialisation pattern of generating
/// and binding the VAO and VBO member variables, and will then set up an array of 6 vertices representing the
/// 2 tris that make up the quad. For each vertex, the function initialises the vertex's corresponding position,
/// normal, and texCoord variables. The function finishes by filling the VBO with said vertices, and then enables
/// the position, normal and texCoord variables as OpenGL attributes to be passed correctly to whichever 
/// shader is bound.
/// </summary>
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
	// Tri 1
	vertices[0].position = { -0.5f, 0, 0.5f, 1 }; // bottom left 
	vertices[0].texCoord = { 0, 1 }; 
	vertices[0].normal = { 0, 1, 0, 0 }; 
	vertices[1].position = { 0.5f, 0, 0.5f, 1 }; // bottom right 
	vertices[1].texCoord = { 1, 1 }; 
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 }; // top left 
	vertices[2].texCoord = { 0, 0 }; 
	vertices[2].normal = { 0, 1, 0, 0 };
	// Tri 2
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

	// Enable the third element as texCoord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));

	// Unbind the buffers as we are done assigning to them
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/// <summary>
/// initialiseFullscreenQuad() is a function used to initialise this mesh with 6 vertices that are pre-positioned
/// in the four corners of screen space (6 vertices are used as the fullscreen quad mesh doesn't use an index buffer).
/// The function follows the standard initialisation sequence of generating and binding the VAO and VBO member 
/// variables using the appropriate OpenGL calls, but then fills the VBO with 6 2D vertices ranging from 
/// -1, -1 to 1, 1 for the bottom left and top right corners of the screen respectively.
/// </summary>
void Mesh::initialiseFullscreenQuad()
{
	// Make sure the mesh is not already initialised
	assert(vao == 0);

	// Generate the buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// Bind the buffers to be ready for manipulating
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Define 6 vertices and 2 triangles (not using an ibo), and set their corresponding screen-space coordinates
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

/// <summary>
/// draw() is used to trigger the OpenGL draw sequence for this Mesh object.
/// The function first binds the Vertex Array Object (vao) of this mesh using
/// the appropriate OpenGL call, and will then either draw the mesh using the
/// OpenGL function glDrawElements or glDrawArrays, depending on whether 
/// or not this mesh was set up to use an index buffer.
/// </summary>
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
