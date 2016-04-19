namespace sa
{
	///A class representing a 4x4 matrix.
	/**
	A class representing a 4x4 matrix.
	The layout of the matrix is as folows:
	0	3	6	
	1	4	7
	2	5	8	
	

	When using multiplication transformations are applied from right to left (pre-multiplied).

	*/
	template <typename T>
	class Matrix33T
	{
	public:
		Matrix33T();
		///Sets the matrix to identity.
		void LoadIdentity();
		///Transposes the matrix.
		void Transpose();
		///Creates a rotation about the X axis.
		void LoadRotateX(const T& amountRad);
		///Returns a rotation about the X axis.
		//static Matrix33T GetRotateX(const T& amountRad) { Matrix44T m; m.LoadRotateX(amountRad); return m; }
		///Creates a rotation about the Y axis.
		void LoadRotateY(const T& amountRad);
		///Creates a rotation about the Z axis.
		void LoadRotateZ(const T& amountRad);
		///Creates a rotation about the axis.
		void LoadRotate(const T& amountRad, const Vector3T<T>& axis);

		T GetDeterminant() const;

		///Returns the specified column.
		T* operator[](unsigned int col);
			
		///Returns the specified column.
		const T* operator[](unsigned int col) const;


	private:
		T E[3*3];
	};

#include "Matrix33T.inl"
}
