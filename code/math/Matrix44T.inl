	template <typename T>
	Matrix44T<T>::Matrix44T()
	{ 
		LoadIdentity();
	}

	template <typename T>
	Matrix44T<T>::Matrix44T(T arr[16])
	{
		memcpy(m_E, arr, sizeof(T)*16);
	}

	template <typename T>
	Matrix44T<T>::Matrix44T(	const T& e00, const T& e10, const T& e20, const T& e30, 
								const T& e01, const T& e11, const T& e21, const T& e31, 
								const T& e02, const T& e12, const T& e22, const T& e32, 
								const T& e03, const T& e13, const T& e23, const T& e33)
	{
		m_E[ 0] = e00;	m_E[ 4] = e10;		m_E[ 8] = e20;		m_E[12] = e30;
		m_E[ 1] = e01;	m_E[ 5] = e11;		m_E[ 9] = e21;		m_E[13] = e31;
		m_E[ 2] = e02;	m_E[ 6] = e12;		m_E[10] = e22;		m_E[14] = e32;
		m_E[ 3] = e03;	m_E[ 7] = e13;		m_E[11] = e23;		m_E[15] = e33;
	}


	template <typename T>
	bool Matrix44T<T>::IsIdentity() const
	{
		if(	!Equals<float>(Column(0)[0], 1.0) |
			!Equals<float>(Column(1)[1], 1.0) |
			!Equals<float>(Column(2)[2], 1.0) |
			!Equals<float>(Column(3)[3], 1.0))
		{
			return false;
		}

		for(unsigned int i=0; i < 4; i++)
		{
			for(unsigned int j=0; j < 4; j++)
			{
				if(i!=j)
				{
					if(!IsZero(Column(i)[j]))
					{
						return false;
					}
				}
			}
		}
		return true;
		
	}

	template <typename T>
	void Matrix44T<T>::LoadIdentity()
	{
		m_E[ 0] = (T)1.0;	m_E[ 4] = (T)0.0;	m_E[ 8] = (T)0.0;	m_E[12] = (T)0.0; 
		m_E[ 1] = (T)0.0;	m_E[ 5] = (T)1.0;	m_E[ 9] = (T)0.0;	m_E[13] = (T)0.0; 
		m_E[ 2] = (T)0.0;	m_E[ 6] = (T)0.0;	m_E[10] = (T)1.0;	m_E[14] = (T)0.0; 
		m_E[ 3] = (T)0.0;	m_E[ 7] = (T)0.0;	m_E[11] = (T)0.0;	m_E[15] = (T)1.0; 
	}

	template <typename T>
	void Matrix44T<T>::Transpose()
	{
		Matrix44T transposed;
		for(unsigned int i = 0; i < 4; ++i)
		{
			for(unsigned int j = 0; j < 4; ++j)
			{
				transposed[j][i] = this->operator[](i)[j];
			}
		}
		*this = transposed;
	}

	template <typename T>
	void Matrix44T<T>::LoadRotateX(const T& amountRad)
	{
		m_E[ 0] = (T)1.0;		m_E[ 4] = (T)0.0;				m_E[ 8] = (T)0.0;				m_E[12] = (T)0.0;
		m_E[ 1] = (T)0.0;		m_E[ 5] = Cos(amountRad);		m_E[ 9] = -Sin(amountRad);	m_E[13] = (T)0.0;
		m_E[ 2] = (T)0.0;		m_E[ 6] = Sin(amountRad);		m_E[10] =	Cos(amountRad);		m_E[14] = (T)0.0;
		m_E[ 3] = (T)0.0;		m_E[ 7] = (T)0.0;				m_E[11] = (T)0.0;				m_E[15] = (T)1.0;
	}
		
	template <typename T>
	void Matrix44T<T>::LoadRotateY(const T& amountRad)
	{
		m_E[ 0] = Cos(amountRad);		m_E[ 4] = (T)0.0;		m_E[ 8] = Sin(amountRad);		m_E[12] = (T)0.0; 
		m_E[ 1] = (T)0.0;				m_E[ 5] = (T)1.0;		m_E[ 9] = (T)0.0;				m_E[13] = (T)0.0; 
		m_E[ 2] = -Sin(amountRad);	m_E[ 6] = (T)0.0;		m_E[10] = Cos(amountRad);		m_E[14] = (T)0.0; 
		m_E[ 3] = (T)0.0;				m_E[ 7] = (T)0.0;		m_E[11] = (T)0.0;				m_E[15] = (T)1.0; 
	}
		
	template <typename T>
	void Matrix44T<T>::LoadRotateZ(const T& amountRad)
	{
		m_E[ 0] = Cos(amountRad);		m_E[ 4] = -Sin(amountRad);	m_E[ 8] = (T)0.0;		m_E[12] = (T)0.0; 
		m_E[ 1] = Sin(amountRad);		m_E[ 5] = Cos(amountRad);		m_E[ 9] = (T)0.0;		m_E[13] = (T)0.0; 
		m_E[ 2] = (T)0.0;				m_E[ 6] = (T)0.0;				m_E[10] = (T)1.0;		m_E[14] = (T)0.0; 
		m_E[ 3] = (T)0.0;				m_E[ 7] = (T)0.0;				m_E[11] = (T)0.0;		m_E[15] = (T)1.0; 
	}

	template <typename T>
  void Matrix44T<T>::LoadRotate(const T& amountRad, const glm::vec3& axis)
	{
		T c = Cos(amountRad);
		T s = Sin(amountRad);
		T t = static_cast<T>(1.0)-c;
		T X = axis[0];
		T Y = axis[1];
		T Z = axis[2];

		m_E[ 0]=t*X*X+c;		m_E[ 4]=t*X*Y-s*Z;	m_E[ 8]=t*X*Z+s*Y;		m_E[12]=0.0f;
		m_E[ 1]=t*X*Y+s*Z;	m_E[ 5]=t*Y*Y+c;		m_E[ 9]=t*Y*Z-s*X;		m_E[13]=0.0f;
		m_E[ 2]=t*X*Z-s*Y;	m_E[ 6]=t*Y*Z+s*X;	m_E[10]=t*Z*Z+c;			m_E[14]=0.0f;
		m_E[ 3]=0.0f;			m_E[ 7]=0.0;			m_E[11]=0.0f;				m_E[15]=1.0f;						
	}

	template <typename T>
  void Matrix44T<T>::LoadTranslate(const glm::vec3& t)
	{
		m_E[ 0] = (T)1.0;	m_E[ 4] = (T)0.0;	m_E[ 8] = (T)0.0;	m_E[12] = t[0]; 
		m_E[ 1] = (T)0.0;	m_E[ 5] = (T)1.0;	m_E[ 9] = (T)0.0;	m_E[13] = t[1]; 
		m_E[ 2] = (T)0.0;	m_E[ 6] = (T)0.0;	m_E[10] = (T)1.0;	m_E[14] = t[2]; 
		m_E[ 3] = (T)0.0;	m_E[ 7] = (T)0.0;	m_E[11] = (T)0.0;	m_E[15] = (T)1.0; 
	}

	template <typename T>
  void Matrix44T<T>::LoadScale(const glm::vec3& s)
	{
		m_E[ 0] = s[0];	m_E[ 4] = (T)0.0;	m_E[ 8] = (T)0.0;	m_E[12] = (T)0.0; 
		m_E[ 1] = (T)0.0;	m_E[ 5] = s[1];	m_E[ 9] = (T)0.0;	m_E[13] = (T)0.0; 
		m_E[ 2] = (T)0.0;	m_E[ 6] = (T)0.0;	m_E[10] = s[2];	m_E[14] = (T)0.0; 
		m_E[ 3] = (T)0.0;	m_E[ 7] = (T)0.0;	m_E[11] = (T)0.0;	m_E[15] = (T)1.0; 	
	}

	template <typename T>
  Matrix44T<T> Matrix44T<T>::GetScale(const glm::vec3& s)
	{
		Matrix44T<T> m;
		m.LoadScale(s);
		return m;
	}

	template <typename T>
	void Matrix44T<T>::LoadPerspectiveProjection(const T& fovyRad, const T& aspect, const T& zNear, const T& zFar)
	{
		T zero = static_cast<T>(0.0);
		T one = static_cast<T>(1.0);
		T two = static_cast<T>(2.0);

		T f=static_cast<T>(one)/static_cast<T>(tan(fovyRad/two));
			
		T e10=(zFar+zNear)/(zNear-zFar);
		T e14=(2*zFar*zNear)/(zNear-zFar);

		m_E[ 0] = f/aspect;	m_E[ 4] = zero;	m_E[ 8] = zero;		m_E[12] = zero;
		m_E[ 1] = zero;		m_E[ 5] = f;		m_E[ 9] = zero;		m_E[13] = zero;
		m_E[ 2] = zero;		m_E[ 6] = zero;	m_E[10] = e10;		m_E[14] = e14;
		m_E[ 3] = zero;		m_E[ 7] = zero;	m_E[11] = -one;		m_E[15] = zero;
	}

	template <typename T>
	void Matrix44T<T>::LoadOrthographicProjection(const RectangleT<T>& viewport, const T& zNear, const T& zFar)
	{
		T zero = static_cast<T>(0.0);
		T one = static_cast<T>(1.0);
		T two = static_cast<T>(2.0);

		T right = viewport.GetRight();
		T left = viewport.GetLeft();
		T top = viewport.GetTop();
		T bottom = viewport.GetBottom();


		T a = one / (right - left);
		T b = one / (top - bottom);
		T c = one / (zFar - zNear);

		T tx = -(right + left) * a;
		T ty = -(top + bottom) * b;
		T tz = -(zFar + zNear) * c;

		m_E[ 0] = two * a;	m_E[ 4] = zero;		m_E[ 8] = zero;			m_E[12] = tx;
		m_E[ 1] = zero;		m_E[ 5] = two * b;	m_E[ 9] = zero;			m_E[13] = ty;
		m_E[ 2] = zero;		m_E[ 6] = zero;		m_E[10] = -two * c;		m_E[14] = tz;
		m_E[ 3] = zero;		m_E[ 7] = zero;		m_E[11] = zero;			m_E[15] = one;

	}

	template <typename T>
	void Matrix44T<T>::LoadViewport(const RectangleT<unsigned int>& viewport, const T& nearDepthRange, const T& farDepthRange)
	{
        T halfWidth = viewport.GetSize()[0] * static_cast<T>(0.5);
        T halfHeight = viewport.GetSize()[1] * static_cast<T>(0.5);
        T halfDepth = (farDepthRange - nearDepthRange) * static_cast<T>(0.5);

		T zero = static_cast<T>(0.0);
		T one = static_cast<T>(1.0);

		m_E[ 0] = halfWidth;	m_E[ 4] = zero;			m_E[ 8] = zero;			m_E[12] = viewport.GetLeft() + halfWidth;
		m_E[ 1] = zero;		m_E[ 5] = halfHeight;		m_E[ 9] = zero;			m_E[13] = viewport.GetTop() + halfHeight;
		m_E[ 2] = zero;		m_E[ 6] = zero;			m_E[10] = halfDepth;		m_E[14] = nearDepthRange + halfDepth;
		m_E[ 3] = zero;		m_E[ 7] = zero;			m_E[11] = zero;			m_E[15] = one;
	}


	template <typename T>
	PlaneT<T> Matrix44T<T>::GetClipPlane(typename Matrix44T<T>::ClipPlane plane) const
	{
		T m = 0.0;
		short cola;
		switch(plane)
		{
		case Left:
			m = 1.0;
			cola = 0;
			break;
		case Right:
			m = -1.0;
			cola = 0;
			break;
		case Top:
			m = -1.0;
			cola = 1;
			break;
		case Bottom:
			m = 1.0;
			cola = 1;
			break;
		case Near:
			m = 1.0;
			cola = 2;
			break;
		case Far:
			m = -1.0;
			cola = 2;
			break;

		}
		T a = Column(0)[3] + m * Column(0)[cola];
		T b = Column(1)[3] + m * Column(1)[cola];
		T c = Column(2)[3] + m * Column(2)[cola];
		T d = Column(3)[3] + m * Column(3)[cola];
		//Normalize the plane.
		T t = sqrt(a*a+b*b+c*c);

    return PlaneT<T>(a/t, b/t, c/t, d/t);
	}


	template <typename T>
  std::array<PlaneT<T>, 6> Matrix44T<T>::GetFrustum() const
	{
    std::array<PlaneT<T>, 6> frustPlanes;
		
		for(unsigned int i = 0; i < 6; ++i)
		{
      frustPlanes[i] = (GetClipPlane((ClipPlane)i));
		}
    return frustPlanes;
	}


	template <typename T>
	void Matrix44T<T>::InversePrimitive()
	{
		*this = GetInversePrimitive();
	}

	template <typename T>
	Matrix44T<T> Matrix44T<T>::GetInversePrimitive()
	{
		Matrix44T<T> inverted;
		inverted.m_E[0 ] = m_E[0];
		inverted.m_E[1 ] = m_E[4];
		inverted.m_E[2 ] = m_E[8];
		inverted.m_E[3 ] = 0.0f;

		inverted.m_E[4 ] = m_E[1];
		inverted.m_E[5 ] = m_E[5];
		inverted.m_E[6 ] = m_E[9];
		inverted.m_E[7 ] = 0.0f;

		inverted.m_E[8 ] = m_E[2];
		inverted.m_E[9 ] = m_E[6];
		inverted.m_E[10] = m_E[10];
		inverted.m_E[11] = 0.0f;

		inverted.m_E[12] = 1.0f - (m_E[12] * m_E[ 0] + m_E[13] * m_E[ 1] + m_E[14] * m_E[ 2]);
		inverted.m_E[13] = 1.0f - (m_E[12] * m_E[ 4] + m_E[13] * m_E[ 5] + m_E[14] * m_E[ 6]);
		inverted.m_E[14] = 1.0f - (m_E[12] * m_E[ 8] + m_E[13] * m_E[ 9] + m_E[14] * m_E[10]);
		inverted.m_E[15] = 1.0f;	

		return inverted;
	}

	template <typename T>
	T Matrix44T<T>::GetDeterminant() const
	{
		int i=0;
		Matrix33T<T> m1=GetSubMatrix33(i, 0);
		T subdet1=Column(i)[0]*Pow<T>((-1.0), (i+1)+1)*m1.GetDeterminant();

		Matrix33T<T> m2=GetSubMatrix33(i, 1);
		T subdet2=Column(i)[1]*Pow<T>((-1.0), (i+1)+2)*m2.GetDeterminant();

		Matrix33T<T> m3=GetSubMatrix33(i, 2);
		T subdet3=Column(i)[2]*Pow<T>((-1.0), (i+1)+3)*m3.GetDeterminant();

		Matrix33T<T> m4=GetSubMatrix33(i, 3);
		T subdet4=Column(i)[3]*Pow<T>((-1.0), (i+1)+4)*m4.GetDeterminant();
		
		T tot=subdet1+subdet2+subdet3+subdet4;

		return tot;
		
	}

	template <typename T>
	Matrix44T<T> Matrix44T<T>::GetAdjoint() const
	{
		Matrix44T<T> adj;

		for(int i=0; i < 4; i++)
		{
			for(int j=0; j < 4; j++)
			{
				Matrix33T<T> mat3x3=GetSubMatrix33(j,i);
				T det=mat3x3.GetDeterminant();
				adj.Column(i)[j] = Pow(-static_cast<T>(1.0), i+j)*det;
			}
		}
		return adj;
	}

	template <typename T>
	bool Matrix44T<T>::GetInverse(Matrix44T<T>& out) const
	{
		if(IsIdentity())
		{
			out=*this;
			return true;
		}
		T det = GetDeterminant();
		if(IsZero(det))
		{
			return false;
		}
		out=GetAdjoint() * (static_cast<T>(1.0)/det);

		return true;
		
	}

	template <typename T>
	const Matrix44T<T> Matrix44T<T>::operator*(const T& s) const
	{
		Matrix44T<T> m;
		for(unsigned int row=0; row < 4; row++)
		{
			for(unsigned int col=0; col < 4; col++)
			{
				m[col][row] = Column(col)[row] * s;
			}
		}
		return m;
	}

	template <typename T>
	const Matrix44T<T> Matrix44T<T>::operator*(const Matrix44T<T>& other) const
	{
		T sum;
		Matrix44T<T> result;

		for(unsigned int row=0; row < 4; row++)
		{
			for(unsigned int col=0; col < 4; col++)
			{
				sum=0;
				for(unsigned int k=0; k < 4; k++)
				{
					sum+=Column(k)[row]*other[col][k];
				}
				result[col][row] = sum;
			}	

		}
		return result;
	}

	template <typename T>
	Matrix44T<T>& Matrix44T<T>::operator*=(const Matrix44T<T>& other) 
	{
		Matrix44T<T> result;
		
		*this = *this * other;

		return *this;
	}


	template <typename T>
	Vector4T<T> Matrix44T<T>::Mult(const Vector4T<T>& v) const
	{
		Vector4T<T> vOut(	
			m_E[ 0]*v[0] + m_E[ 4]*v[1] + m_E[ 8]*v[2]+ m_E[12],
			m_E[ 1]*v[0] + m_E[ 5]*v[1] + m_E[ 9]*v[2]+ m_E[13],
			m_E[ 2]*v[0] + m_E[ 6]*v[1] + m_E[10]*v[2]+ m_E[14],
			m_E[ 3]*v[0] + m_E[ 7]*v[1] + m_E[11]*v[2]+ m_E[15]);

		return vOut;
	}


	template <typename T>
	T* Matrix44T<T>::operator[](unsigned int col)
	{
		return &m_E[col*4];
	}

	template <typename T>
	const T* Matrix44T<T>::operator[](unsigned int col) const
	{
		return &m_E[col*4];
	}

	template <typename T>
	Matrix33T<T> Matrix44T<T>::GetSubMatrix33(unsigned int col, unsigned int row) const
	{
		Matrix33T<T> out;
		unsigned int currentR=Repeat<int>(0, 3, row+1);
		unsigned int currentC=Repeat<int>(0, 3, col+1);

		for(unsigned int i=0; i < 3; i++) //foreach column
		{
			for(unsigned int j=0; j < 3; j++) //foreach row
			{
				out[i][j] = Column(currentC)[currentR];	
				currentR=Repeat<int>(0, 3, currentR+1);
			}		
					
			currentR=Repeat<int>(0, 3, row+1); //restart the row.
			currentC=Repeat<int>(0, 3, currentC+1); //continue to next column.
		}
			
		return out;
	}




