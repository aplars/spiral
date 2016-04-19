
namespace sa
{
	template <typename T>
	Vector4T<T>::Vector4T() { }
		
	template <typename T>
	Vector4T<T>::Vector4T(T x, T y, T z, T w)
	{ 
		E[0] = x;
		E[1] = y;
		E[2] = z;
		E[3] = w;
	}
		
	template <typename T>
	T Vector4T<T>::DotProduct(const Vector4T<T>& other) const
	{
		return DotProduct(*this, other);
	}
		
	template <typename T>
	T Vector4T<T>::DotProduct(const Vector4T<T>& a, const Vector4T<T>& b)
	{
		return a.GetX()*b.GetX() + a.GetY()*b.GetY() + a.GetZ()*b.GetZ() + a.GetW()*b.GetW();
	}

	template <typename T>
	T Vector4T<T>::GetX() const
	{
		return E[0];
	}

	template <typename T>
	T Vector4T<T>::GetY() const
	{
		return E[1];
	}
		
	template <typename T>
	T Vector4T<T>::GetZ() const
	{
		return E[2];
	}

	template <typename T>
	T Vector4T<T>::GetW() const
	{
		return E[3];
	}

	template <typename T>
	T& Vector4T<T>::operator[](unsigned int i)
	{
		return E[i];
	}

	template <typename T>
	const T& Vector4T<T>::operator[](unsigned int i) const
	{
		return E[i];
	}
}
