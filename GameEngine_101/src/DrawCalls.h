#pragma once

class cGameObject;

// Draws cloth soft bodies
void ClothDraw(cGameObject* pTheGO);

// Draws a cGameObject*
void DrawObject(cGameObject* pTheGO);

//// Draws a debug cube at an AABB location with a specific size
//void DrawAABB(cGameObject* pTheGO, float size);
//
//// Draws an AABB for specific points
//void DrawAABBforPoints(std::vector<glm::vec3> vertices, float AABBSize);