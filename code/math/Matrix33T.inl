template <typename T>
Matrix33T<T>::Matrix33T()
{ 
	LoadIdentity();
}

template <typename T> 
void Matrix33T<T>::LoadIdentity()
{
	E[0] = (T)1.0;	E[3] = (T)0.0;	E[6] = (T)0.0;
	E[1] = (T)0.0;	E[4] = (T)1.0;	E[7] = (T)0.0;
	E[2] = (T)0.0;	E[5] = (T)0.0;	E[8] = (T)1.0;
}

template <typename T> 
void Matrix33T<T>::Transpose()
{
	Matrix33T<T> EE;

	EE[0] = E[0];	EE[3] = E[1];	EE[6] = E[2];
	EE[1] = E[3];	EE[4] = E[4];	EE[7] = E[5];
	EE[2] = E[6];	EE[5] = E[7];	EE[8] = E[8];

	*this = EE;
}

template <typename T>
void Matrix33T<T>::LoadRotateX(const T& amountRad)
{
	E[ 0] = (T)1.0;		E[ 3] = (T)0.0;				E[ 6] = (T)0.0;				
	E[ 1] = (T)0.0;		E[ 4] = Cos(amountRad);		E[ 7] = -Sin(amountRad);	
	E[ 2] = (T)0.0;		E[ 5] = Sin(amountRad);		E[ 8] =	Cos(amountRad);			
}
		
template <typename T>
void Matrix33T<T>::LoadRotateY(const T& amountRad)
{
	E[ 0] = Cos(amountRad);		E[ 3] = (T)0.0;		E[ 6] = Sin(amountRad);		
	E[ 1] = (T)0.0;				E[ 4] = (T)1.0;		E[ 7] = (T)0.0;				
	E[ 2] = -Sin(amountRad);	E[ 5] = (T)0.0;		E[ 8] = Cos(amountRad);		
			
}
		
template <typename T>
void Matrix33T<T>::LoadRotateZ(const T& amountRad)
{
	E[ 0] = Cos(amountRad);		E[ 3] = -Sin(amountRad);	E[ 6] = (T)0.0;		
	E[ 1] = Sin(amountRad);		E[ 4] = Cos(amountRad);		E[ 7] = (T)0.0;		
	E[ 2] = (T)0.0;				E[ 5] = (T)0.0;				E[ 8] = (T)1.0;		
	
}

template <typename T>
void Matrix33T<T>::LoadRotate(const T& amountRad, const Vector3T<T>& axis)
{
	T c = Cos(amountRad);
	T s = Sin(amountRad);
	T t = static_cast<T>(1.0)-c;
	T X = axis[0];
	T Y = axis[1];
	T Z = axis[2];

	E[ 0]=t*X*X+c;		E[ 3]=t*X*Y-s*Z;	E[ 6]=t*X*Z+s*Y;		
	E[ 1]=t*X*Y+s*Z;	E[ 4]=t*Y*Y+c;		E[ 7]=t*Y*Z-s*X;		
	E[ 2]=t*X*Z-s*Y;	E[ 5]=t*Y*Z+s*X;	E[ 8]=t*Z*Z+c;											
}

template <typename T>
T Matrix33T<T>::GetDeterminant() const
{
	return 
		E[0]*(E[4]*E[8] - E[7]*E[5]) - 
		E[3]*(E[1]*E[8] - E[7]*E[2]) +
		E[6]*(E[1]*E[5] - E[4]*E[2]);
}


template <typename T>
T* Matrix33T<T>::operator[](unsigned int col)
{
	return &E[col*3];
}

template <typename T>
const T* Matrix33T<T>::operator[](unsigned int col) const
{
	return &E[col*3];
}
