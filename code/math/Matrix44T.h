#pragma once
#include "Vector4T.h"
#include "ScalarT.h"
#include "Matrix33T.h"
#include "PlaneT.h"
#include "RectangleT.h"
#include <iostream>
#include <array>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace sa
{
	template <typename T>
	class RectangleT;
	///A class representing a 4x4 matrix.
	/**
	A class representing a 4x4 matrix.
	The layout of the matrix is as folows:
	0	4	8	12
	1	5	9	13
	2	6	10	14
	3	7	11	15

	When using multiplication transformations are applied from right to left (pre-multiplied).

	*/
	template <typename T>
	class Matrix44T
	{
	public:
		enum ClipPlane
		{
			Left=0,
			Right,
			Top,
			Bottom,
			Near,
			Far
		};


		Matrix44T();
		Matrix44T(T arr[16]);
		Matrix44T(	const T& e00, const T& e10, const T& e20, const T& e30, 
					const T& e01, const T& e11, const T& e21, const T& e31, 
					const T& e02, const T& e12, const T& e22, const T& e32, 
					const T& e03, const T& e13, const T& e23, const T& e33);

		bool IsIdentity() const;
		///Sets the matrix to identity.
		/**
		Sets the elements to I.
		1	0	0	0
		0	1	0	0
		0	0	1	0
		0	0	0	1
		*/
		void LoadIdentity();
		static Matrix44T GetIdentity() { Matrix44T mat; return mat; }
		///Transposes the matrix.
		void Transpose();
		///Creates a rotation about the X axis.
		void LoadRotateX(const T& amountRad);
		///Returns a rotation about the X axis.
		static Matrix44T GetRotateX(const T& amountRad) { Matrix44T m; m.LoadRotateX(amountRad); return m; }
		///Creates a rotation about the Y axis.
		void LoadRotateY(const T& amountRad);
		///Returns a rotation about the Y axis.
		static Matrix44T GetRotateY(const T& amountRad) { Matrix44T m; m.LoadRotateY(amountRad); return m; }		
		///Creates a rotation about the Z axis.
		void LoadRotateZ(const T& amountRad);
		///Returns a rotation about the Z axis.
		static Matrix44T GetRotateZ(const T& amountRad) { Matrix44T m; m.LoadRotateZ(amountRad); return m; }
		///Creates a rotation about the axis.
    void LoadRotate(const T& amountRad, const glm::vec3& axis);


		///Creates a translation.
    void LoadTranslate(const glm::vec3& t);

		///Returns a translation.
    static Matrix44T GetTranslate(const T& x, const T& y, const T& z) { Matrix44T m; m.LoadTranslate(glm::vec3(x, y, z)); return m; }
		///
    void LoadScale(const glm::vec3& s);
		///
    static Matrix44T GetScale(const glm::vec3& s);
		

		///Creates a perspectiv projection.
		/**
		Creates a perspectiv projection.
		The projection will create a right handed coordinate system.
		*/
		void LoadPerspectiveProjection(const T& fovyRad, const T& aspect, const T& zNear, const T& zFar);

		///Creates a ortographic projection
		void LoadOrthographicProjection(const RectangleT<T>& viewport, const T& zNear, const T& zFar);

		///Creates a viewport transformation.
		void LoadViewport(const RectangleT<unsigned int>& viewport, const T& nearDepthRange, const T& farDepthRange);
		

		///Returns a perspective projection.
		/**
		Creates a perspectiv projection.
		The projection will create a right handed coordinate system.
		*/
		static Matrix44T GetPerspectiveProjection(const T& fovyRad, const T& aspect, const T& zNear, const T& zFar) { Matrix44T m; m.LoadPerspectiveProjection(fovyRad, aspect, zNear, zFar); return m; }

		///Creates a ortographic projection
		static Matrix44T GetOrthographicProjection(const RectangleT<T>& viewport, const T& zNear, const T& zFar) { Matrix44T m; m.LoadOrthographicProjection(viewport, zNear, zFar); return m; }
    static Matrix44T GetOrthographicProjection(const T& left, const T& right, const T& bottom, const T& top, const T& zNear, const T& zFar)
    {
      Matrix44T m;
      m.LoadOrthographicProjection(RectangleT<T>(left, bottom, right-left, top-bottom), zNear, zFar);
      return m;
    }

		///Creates a viewport transformation.
		static Matrix44T GetViewport(const RectangleT<unsigned int>& viewport, const T& nearDepthRange, const T& farDepthRange) { Matrix44T m; m.LoadViewport(viewport, nearDepthRange, farDepthRange); return m; }

		PlaneT<T> GetClipPlane(ClipPlane plane) const;


    std::array<PlaneT<T>, 6> GetFrustum() const;

		void InversePrimitive();

		Matrix44T<T> GetInversePrimitive();

		T GetDeterminant() const;
		Matrix44T<T> GetAdjoint() const;

		bool GetInverse(Matrix44T<T>& out) const;

		const Matrix44T& Inverse()
		{
			GetInverse(*this);
			return *this;
		}


		const Matrix44T operator*(const T& s) const;

		const Vector4T<T> operator*(const Vector4T<T>& other) const { return Mult(other); }

		const Matrix44T operator*(const Matrix44T& other) const;
		
		Matrix44T& operator*=(const Matrix44T& other);

		Vector4T<T> Mult(const Vector4T<T>& v) const;

    glm::vec4 Vec3Transform(const glm::vec3& in) const
		{
      Vector4T<T> v = Mult(Vector4T<T>(in[0], in[1], in[2], 1.0));
      return glm::vec4(v[0], v[1], v[2], v[3]);
    }

//    glm::vec4 Vec3Transform(const glm::vec3& in) const {
//      Vector4T<float> v = Mult(Vector4T<T>(in[0], in[1], in[2], 1.0));
//      return glm::vec4(v[0], v[1], v[2], v[3]);
//    }


		Vector4T<T> Vec3Transform(const T& x, const T& y, const T& z) const
		{
			return Mult(Vector4T<T>(x, y, z, 1.0));
		}

    glm::vec3 Vec3TransformH(const glm::vec3& in) const {
      Vector4T<float> v = Mult(Vector4T<T>(in[0], in[1], in[2], 1.0));
      return glm::vec3(v[0]/v[3], v[1]/v[3], v[2]/v[3]);
    }




		const Matrix44T& InverseTranspose()
		{ 
			Inverse();
			Transpose();			
			return *this;
		}

		Matrix44T GetInverseTranspose() const
		{ 
			Matrix44T tmp = *this;

			tmp.InverseTranspose();
			return tmp;
		}

		///Returns the specified column.
		T* operator[](unsigned int col);
			
		///Returns the specified column.
		const T* operator[](unsigned int col) const;

		const T* Column(unsigned int col) const { return &m_E[col*4]; }
		T* Column(unsigned int col) { return &m_E[col*4]; }

		void Set(unsigned int i, const T& v) { m_E[i] = v; }
		//const T& Index(unsigned int i) const {return E[i]; }
		//T& Index(unsigned int i) {return E[i]; }

		const T* GetConstPtr() const 
		{ 
			return &m_E[0]; 
		}	

		template <typename U>
		operator Matrix44T<U>() const
		{
			Matrix44T<U> e;
			for(unsigned int i = 0; i < 16; ++i)
			{
				e.Set(i, static_cast<U>(m_E[i]));
			}
			return e;
		}
		
		T m_E[16];
	private:
		Matrix33T<T> GetSubMatrix33(unsigned int col, unsigned int row) const;


	};

		


	#include "Matrix44T.inl"
}
