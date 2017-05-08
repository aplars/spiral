
template <typename T>
typename PlaneT<T>::Side PlaneT<T>::side(glm::vec3& point, const double& tolerance) const
{
  T dist = distance(point);
	if(IsZero(dist, tolerance))
	{
		return No_Side;	
	}
	else if(dist < 0)
	{
		return Back_Side;	
	}
	return Front_Side;
}

template <typename T>
T& PlaneT<T>::operator[](unsigned int i)
{
	if(i < 3)
	{
		return m_normal[i];	
	}
	return m_d;
}

template <typename T>
const T& PlaneT<T>::operator[](unsigned int i) const
{	
	if(i < 3)
	{
		return m_normal[i];	
	}
	return m_d;
}
