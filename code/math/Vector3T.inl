
template <typename T>
Vector3T<T>::Vector3T() 
{
	X(T());	
	Y(T());	
	Z(T());			
}

template <typename T>
Vector3T<T>::Vector3T(const T& x, const T& y, const T& z)
{
	xyz[0] = x;	
	xyz[1] = y;	
	xyz[2] = z;	
}

template <typename T>
T Vector3T<T>::DotProduct(const Vector3T& other) const
{
	return X()*other.X() + Y()*other.Y() + Z()*other.Z();
}

template <typename T>
T Vector3T<T>::DotProduct(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return a.DotProduct(b);
}

template <typename T>
Vector3T<T> Vector3T<T>::CrossProduct(const Vector3T<T>& other) const
{
	return Vector3T<T>(
		Y()*other.Z()-Z()*other.Y(), 
		Z()*other.X()-X()*other.Z(),
		X()*other.Y()-Y()*other.X());
}

template <typename T>
Vector3T<T> Vector3T<T>::CrossProduct(const Vector3T<T>& a, const Vector3T<T>& b)
{
	return a.CrossProduct(b);
}

template <typename T>
const Vector3T<T>& Vector3T<T>::Normalize() 
{ 
	T norm = GetNorm();
	xyz[0] /= norm;
	xyz[1] /= norm;
	xyz[2] /= norm;
	return *this;
}

template <typename T>
Vector3T<T> Vector3T<T>::GetNormalized() const 
{ 
	Vector3T<T> outVec = *this;
	return outVec.Normalize();
}

template <typename T>
T& Vector3T<T>::operator[](unsigned int i) 
{ 
	#ifdef _DEBUG
	if(i > 2)
	{
		throw OutOfBoundsException("A three component vector can only be indexed between 0-2", "T& Vector3T<T>::operator[](unsigned int i) ", __FILE__, __LINE__);	
	}
	#endif
	return xyz[i]; 
}

template <typename T>
const T& Vector3T<T>::operator[](unsigned int i) const 
{ 
	#ifdef _DEBUG
	if(i > 2)
	{
		throw OutOfBoundsException("A three component vector can only be indexed between 0-2", "const T& Vector3T<T>::operator[](unsigned int i) const", __FILE__, __LINE__);	
	}
	#endif
	return xyz[i]; 
}


