#include "streamedmeshentity.h"

namespace sa {
StreamedMeshEntity::~StreamedMeshEntity()
{
  unload();
}


StreamedMeshEntity::StreamedMeshEntity(const std::string& resourcePath, const std::string& resourceName, bool castShadow)
  : m_mesh(new MeshRenderable(resourcePath, resourceName))
  , m_castShadow(castShadow)
  , m_currentStorage(DataStorage::Disk)
{
  m_boundingBox = m_mesh->getBoundingBox();
}

StreamedMeshEntity::StreamedMeshEntity(MeshRenderablePtr mesh, bool castShadow)
  : m_mesh(mesh)
  , m_castShadow(castShadow)
  , m_currentStorage(DataStorage::Disk)
{
  m_boundingBox = m_mesh->getBoundingBox();
}

void StreamedMeshEntity::setPosition(const glm::bvec3& position) {
  m_position = position;
}

void StreamedMeshEntity::setPosition(float x, float y, float z) {
  if(m_position!=glm::vec3(x, y, z)) {
    m_position = glm::vec3(x, y, z);
    PropertyChangedEvent evt("position", this);
    m_propertyChanged.notify(evt);
  }
}

const glm::vec3& StreamedMeshEntity::getPosition() const {
  return m_position;
}

void StreamedMeshEntity::setHeading(float headingInRad) {
  if(m_heading != headingInRad) {
    m_heading = headingInRad;
    PropertyChangedEvent evt("heading", this);
    m_propertyChanged.notify(evt);
  }
}

float StreamedMeshEntity::getHeading() const {
  return m_heading;
}

AABBModel StreamedMeshEntity::getBoundingBox() const {
  AABBModel transformedBox = m_boundingBox;
  transformedBox.transform(Matrix44T<float>::GetTranslate(m_position.x, m_position.y, m_position.z));
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

void StreamedMeshEntity::toCPU(ImageCache& imageCache, const std::string& texturePath, const std::string& shaderPath) {
  m_mesh->toCPU(imageCache, texturePath, shaderPath);
  m_mutex.lock();
  m_currentStorage = DataStorage::CPU;
  m_mutex.unlock();
}

void StreamedMeshEntity::toGPU(const ConfigurationManager& config, unsigned int numberOfShadowCascades, TextureCache& textureCache, ShaderCache& shaderCache, RenderDevice* device, RenderContext* context) {
  m_mesh->toGPU(config, numberOfShadowCascades, textureCache, shaderCache, device, context);
  m_mutex.lock();
  m_currentStorage = DataStorage::GPU;
  m_mutex.unlock();
}

void StreamedMeshEntity::applyAnimations(float dt) {
  m_mesh->setAnimationFrame(m_skeletonAnimation, m_nodeAnimation, m_skeletonAnimationTime, m_nodeAnimationTime);
  m_skeletonAnimationTime+=dt;
  m_nodeAnimationTime+=dt;

  m_mesh->applyTransformations();
//  m_drawData = m_mesh->getDrawData();
}



std::deque<std::string> StreamedMeshEntity::getSkeletalAnimations() const {
  return m_mesh->getSkeletalAnimations();
}

void StreamedMeshEntity::playSkeletalAnimation(const std::string& animationName) {
  m_skeletonAnimation = animationName;
  m_skeletonAnimationTime = 0;
}

std::deque<std::string> StreamedMeshEntity::getNodeAnimations() const {
  return m_mesh->getNodeAnimations();
}

void StreamedMeshEntity::playNodeAnimation(const std::string& animationName) {
  m_nodeAnimation = animationName;
  m_nodeAnimationTime = 0;
}

void StreamedMeshEntity::unload()
{

  m_mutex.lock();
  m_currentStorage = DataStorage::Pending;

  m_mesh->unload();

  m_currentStorage = DataStorage::Disk;
  m_mutex.unlock();
}


DrawDataList StreamedMeshEntity::getDrawData(RenderPass pass) {
  DrawDataList toDraw;

  m_drawData = m_mesh->getDrawData(pass);
  if(m_currentStorage == DataStorage::GPU)
  {
    if(pass == RenderPass::Shadow && !m_castShadow) {

    }
    else {
      Matrix44T<float> movementTransformation = Matrix44T<float>::GetTranslate(m_position.x, m_position.y, m_position.z) * Matrix44T<float>::GetRotateY(m_heading);
      for(DrawData dd : m_drawData) {
        dd.Uniforms.Matrix4Uniforms["u_modelMatrix"] = movementTransformation * dd.Uniforms.Matrix4Uniforms["u_modelMatrix"];
        toDraw.push_back(dd);
      }
    }
    return toDraw;
  }
  return toDraw;
}

void StreamedMeshEntity::addPropertyChangedListener(const std::function<void (const StreamedMeshEntity::PropertyChangedEvent &)> &f)
{
  m_propertyChanged.registerObserver(f);
}

bool StreamedMeshEntity::getCastShadow() const
{
  return m_castShadow;
}
}
