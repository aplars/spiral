#pragma once
#include "PlaneT.h"

#include <deque>


namespace sa
{

	template <typename T>
	class FrustumT
	{
	public:
		FrustumT() {}
		FrustumT(const std::deque<PlaneT<T> >& planes)
			: m_planes(planes) {}


    FrustumT getExtruded(const T& e) const
		{
			std::deque<PlaneT<T> > extPlanes;
			typename std::deque<PlaneT<T> >::const_iterator it = m_planes.begin();

			for(; it != m_planes.end(); ++it)
			{
				extPlanes.push_back(PlaneT<T>(it->GetNormal(), it->GetD() - e));
			}	
			return FrustumT(extPlanes);
		}

	private:
		std::deque<PlaneT<T> > m_planes;
	};
}
