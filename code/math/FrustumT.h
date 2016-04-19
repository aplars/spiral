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


		FrustumT GetExtruded(const T& e) const 
		{
			std::deque<PlaneT<T> > extPlanes;
			typename std::deque<PlaneT<T> >::const_iterator it = m_planes.begin();

			for(; it != m_planes.end(); ++it)
			{
				extPlanes.push_back(PlaneT<T>(it->GetNormal(), it->GetD() - e));
			}	
			return FrustumT(extPlanes);
		}

		bool IsInFrustum(const Vector3T<T>& point) const
		{
			typename std::deque<PlaneT<T> >::const_iterator it = m_planes.begin();
			for(; it != m_planes.end(); ++it)
			{
				if(it->GetSide(point) == PlaneT<T>::Back_Side)
				{
					return false;
				}
			}
			return true;
		}
	private:
		std::deque<PlaneT<T> > m_planes;
	};
}
