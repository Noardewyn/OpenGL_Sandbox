#ifndef __MESH_PROVIDER_H__
#define __MESH_PROVIDER_H__

#include "engine/Mesh.h"

namespace engine {

std::unique_ptr<Mesh> generateCubeMesh();
std::unique_ptr<Mesh> generateSphereMesh(int detalization);
std::unique_ptr<Mesh> generatePlaneMesh(bool tangent_space = false);
std::unique_ptr<Mesh> generateSkyBox();

} // namespace engine

#endif // __MESH_PROVIDER_H__