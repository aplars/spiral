#ifndef KDTREE_H
#define KDTREE_H
#include <memory>
#include <math/PlaneT.h>

namespace sa {


class KdNode {
public:

private:
  PlaneT<float> m_splitter;
  //KdNodePtr m_front;
  //KdNodePtr m_back;
};
typedef std::shared_ptr<KdNode> KdNodePtr;
class KdTree
{

public:
  KdTree();
};
}
#endif // KDTREE_H
