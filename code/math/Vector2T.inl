template <typename T>
T& Vector2T<T>::operator[](unsigned int i) 
{ 
	#ifdef _DEBUG
	if(i > 1)
	{
		throw OutOfBoundsException("A two component vector can only be indexed between 0-1", "T& Vector2T<T>::operator[](unsigned int i) ", __FILE__, __LINE__);	
	}
	#endif
	return xy[i]; 
}

template <typename T>
const T& Vector2T<T>::operator[](unsigned int i) const 
{ 
	#ifdef _DEBUG
	if(i > 1)
	{
		throw OutOfBoundsException("A two component vector can only be indexed between 0-1", "const T& Vector2T<T>::operator[](unsigned int i) const", __FILE__, __LINE__);	
	}
	#endif
	return xy[i]; 
}