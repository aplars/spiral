#include "streamedmeshentity.h"
#include "meshrenderable.h"

namespace sa {
StreamedMeshEntity::~StreamedMeshEntity()
{
  unloadGPU();
  unloadCPU();
}


StreamedMeshEntity::StreamedMeshEntity(const std::string& resourcePath, const std::string& resourceName)
  : m_mesh(new MeshRenderable(resourcePath, resourceName))
  , m_currentStorage(DataStorage::Disk)
{ }

void StreamedMeshEntity::setPosition(const Vector3T<float>& position) {
  m_position = position;
}

void StreamedMeshEntity::setPosition(float x, float y, float z) {
  m_position.Set(x, y, z);
}

const Vector3T<float>& StreamedMeshEntity::getPosition() const {
  return m_position;
}

void StreamedMeshEntity::setHeading(float headingInRad) {
  m_heading = headingInRad;
}

float StreamedMeshEntity::getHeading() const {
  return m_heading;
}

AABBModel StreamedMeshEntity::getBoundingBox() const {
  AABBModel transformedBox = m_mesh->getBoundingBox();
  transformedBox.transform(Matrix44T<float>::GetTranslate(m_position));
  return transformedBox;
}

DataStorage StreamedMeshEntity::currentDataStorage() const {
  DataStorage local;

  m_mutex.lock();
  local = m_currentStorage;
  m_mutex.unlock();
  return local;
}

void StreamedMeshEntity::setPendingStorage() {
  m_mutex.lock();
  m_currentStorage = DataStorage::Pending;
  m_mutex.unlock();
}

void StreamedMeshEntity::setDiskStorage() {
  m_mutex.lock();
  m_currentStorage = DataStorage::Disk;
  m_mutex.unlock();
}

void StreamedMeshEntity::toCPU(ImageCache& imageCache) {
  m_mesh->toCPU(imageCache);
  m_mutex.lock();
  m_currentStorage = DataStorage::CPU;
  m_mutex.unlock();
}

void StreamedMeshEntity::toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, lru<std::string, TexturePtr>& textureCache, RenderDevice* device, RenderContext* context) {
  m_mesh->toGPU(config, numberOfShadowCascades, textureCache, device, context);
  m_mutex.lock();
  m_currentStorage = DataStorage::GPU;
  m_mutex.unlock();
}

void StreamedMeshEntity::applyAnimations(float dt) {
  m_mesh->applyAnimations(dt);
}

void StreamedMeshEntity::applyTransformations() {
  //Apply the local transformations.
  m_mesh->applyTransformations();
  //Apply the position
  Matrix44T<float> movementTransformation = Matrix44T<float>::GetTranslate(m_position) * Matrix44T<float>::GetRotateY(m_heading);
  for(DrawData& dd : m_mesh->getDrawData()) {
    int location = dd.SP->uniformLocation("u_modelMatrix");
    dd.Matrix4Uniforms[location] = movementTransformation * dd.Matrix4Uniforms[location];
  }
}

std::deque<std::string> StreamedMeshEntity::getSkeletalAnimations() const {
  return m_mesh->getSkeletalAnimations();
}

void StreamedMeshEntity::playSkeletalAnimation(const std::string& animationName) {
  m_mesh->playSkeletalAnimation(animationName);
}

std::deque<std::string> StreamedMeshEntity::getNodeAnimations() const {
  return m_mesh->getNodeAnimations();
}

void StreamedMeshEntity::playNodeAnimation(const std::string& animationName) {
  m_mesh->playNodeAnimation(animationName);
}

void StreamedMeshEntity::unloadGPU()
{
  m_mesh->unloadGPU();
}

void StreamedMeshEntity::unloadCPU()
{
  m_mesh->unloadCPU();
}

std::deque<DrawData>& StreamedMeshEntity::getDrawData() {
  return m_mesh->getDrawData();
}
}
