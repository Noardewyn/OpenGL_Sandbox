#ifndef __MESH_PROVIDER_H__
#define __MESH_PROVIDER_H__

#include "engine/Mesh.h"

namespace engine {

std::unique_ptr<Mesh> generateCubeMesh();
std::unique_ptr<Mesh> generateMatrixMesh(float width = 1.0f, float height = 1.0f, uint32_t segments_x = 1, uint32_t segments_y = 1);
std::unique_ptr<Mesh> generateQuadMesh(float width = 1.0f, float height = 1.0f);
std::unique_ptr<Mesh> generateSphereMesh(int detalization);
std::unique_ptr<Mesh> generatePlaneMesh(bool tangent_space = false);
std::unique_ptr<Mesh> generateSkyBox();

} // namespace engine

#endif // __MESH_PROVIDER_H__