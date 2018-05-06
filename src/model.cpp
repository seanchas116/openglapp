#include "model.hpp"
#include "hash.hpp"
#include <iostream>
#include <unordered_map>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

using namespace glm;

using PackedVertex = std::tuple<vec3, vec2, vec3>;

void indexVertices(const std::vector<PackedVertex>& inVertices,
                   std::vector<PackedVertex>& outVertices,
                   std::vector<uint32_t>& outIndices) {
    outVertices.clear();
    outIndices.clear();
    std::unordered_map<PackedVertex, uint32_t> vertexIndices;

    for (auto vertex : inVertices) {
        auto it = vertexIndices.find(vertex);
        if (it != vertexIndices.end()) {
            // same index found
            outIndices.push_back(it->second);
        } else {
            uint32_t index = outVertices.size();
            outVertices.push_back(vertex);
            vertexIndices[vertex] = index;
            outIndices.push_back(index);
        }
    }
}

Model::Model(const char *objPath) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, objPath);

    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
        throw std::runtime_error(err);
    }


    std::vector<PackedVertex> nonIndexedVertices;

    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        int fv = shapes[s].mesh.num_face_vertices[f];

        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
          tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
          tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
          tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
          tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
          tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
          tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
          tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];

          nonIndexedVertices.push_back(std::make_tuple(vec3(vx, vy, vz), vec2(tx, ty), vec3(nx, ny, nz)));
        }
        index_offset += fv;

        // per-face material
        // shapes[s].mesh.material_ids[f];
      }
    }

    std::vector<PackedVertex> vertices;
    std::vector<uint32_t> indices;
    indexVertices(nonIndexedVertices, vertices, indices);

    // vertex array

    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // vertex buffer

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PackedVertex), vertices.data(), GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    count = indices.size();
}

void Model::draw() {
    glBindVertexArray(vertexArrayID);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
