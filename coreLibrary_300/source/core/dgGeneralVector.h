/* Copyright (c) <2003-2011> <Julio Jerez, Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef __dgGeneralVector__
#define __dgGeneralVector__

#include "dgStdafx.h"
#include "dgDebug.h"

#define DG_COUNT_FLOAT_OPS

template<class T> class dgSPDMatrix;
template<class T> class dgGeneralMatrix;

template<class T>
class dgGeneralVector
{
	friend class dgSPDMatrix<T>;
	friend class dgGeneralMatrix<T>;

	public:
	dgGeneralVector (dgInt32 size);
	dgGeneralVector (dgInt32 size, T* const mem);
	dgGeneralVector (const dgGeneralVector<T> &src);
	dgGeneralVector (const dgGeneralVector<T> &src, T* const mem);

	~dgGeneralVector ();

	T& operator[] (dgInt32 i);
	const T& operator[] (dgInt32 i) const;

	dgInt32 GetRowCount() const;

	void Clear (T val);
	void Copy (const dgGeneralVector<T> &src);
	T DotProduct (const dgGeneralVector &b) const;
	T Norm () const;
	T Norm2 () const;

	void operator += (const dgGeneralVector &A);
	void operator -= (const dgGeneralVector &A); 

	void Scale (T scale);
	void LinearCombine (T scale, const dgGeneralVector &A, const dgGeneralVector &B);

	static dgInt32 GetFloatOps();
	static dgInt32 GetMemWrites();

	static void SetFloatOps(dgInt32 initialCount = 0);
	static void SetMemWrites(dgInt32 initialCount = 0);

	void Trace () const;


	protected:
	bool m_ownMemory;
	dgInt32 m_colCount;
	T* m_columns;

	static dgInt32 m_floatsOp;
	static dgInt32 m_memoryWrite;


	dgGeneralVector();
};


// ***********************************************************************************************
//
//	vector
//
// ***********************************************************************************************
template<class T>
dgGeneralVector<T>::dgGeneralVector() 
{
	m_ownMemory = false;
	m_colCount = 0;
	m_columns = NULL;
}

template<class T>
dgGeneralVector<T>::dgGeneralVector (dgInt32 size)
{
	dgAssert  (size > 0);
	m_ownMemory = true;
	m_colCount = size;
	m_columns = new T[size];
	dgAssert ((((dgUnsigned32) m_columns) & 0x0f) == 0);
}

template<class T>
dgGeneralVector<T>::dgGeneralVector (dgInt32 size, T *mem)
{
	m_ownMemory = false;
	m_colCount = size;
	m_columns = mem;
	dgAssert ((((dgUnsigned32) m_columns) & 0x0f) == 0);
}

template<class T>
dgGeneralVector<T>::dgGeneralVector (const dgGeneralVector<T> &src)
{
	m_ownMemory = true;
	m_colCount = src.m_colCount;
	m_columns = new T[m_colCount];
	dgAssert ((((dgUnsigned32) m_columns) & 0x0f) == 0);

	Copy (src);
}

template<class T>
dgGeneralVector<T>::dgGeneralVector (const dgGeneralVector<T> &src,  T* const mem)
{
	m_ownMemory = false;
	m_colCount = src.m_colCount;
	m_columns = mem;
	dgAssert ((((dgUnsigned32) m_columns) & 0x0f) == 0);

	Copy (src);
}


template<class T>
dgInt32 dgGeneralVector<T>::GetFloatOps()
{
	return m_floatsOp;
}

template<class T>
void dgGeneralVector<T>::SetFloatOps(dgInt32 initialCount)
{
	m_floatsOp = initialCount;
}


template<class T>
dgInt32 dgGeneralVector<T>::GetMemWrites()
{
	return m_memoryWrite;
}


template<class T>
void dgGeneralVector<T>::SetMemWrites(dgInt32 initialCount)
{
	m_memoryWrite = initialCount;
}



template<class T>
dgGeneralVector<T>::~dgGeneralVector ()
{
	if (m_ownMemory) {
		delete[] m_columns;
	}
}

template<class T>
void dgGeneralVector<T>::Trace () const
{
	for (dgInt32 i = 0; i < m_colCount; i ++) {
		dgTrace (("%f ", m_columns[i]));
		
	}
	dgTrace (("\n"));
}

template<class T>
T& dgGeneralVector<T>::operator[] (dgInt32 i)
{
	dgAssert (i < m_colCount);
	dgAssert (i >= 0);
	return m_columns[i];
}	

template<class T>
const T& dgGeneralVector<T>::operator[] (dgInt32 i) const
{
	dgAssert (i < m_colCount);
	dgAssert (i >= 0);
	return m_columns[i];
}

template<class T>
dgInt32 dgGeneralVector<T>::GetRowCount() const
{
	return m_colCount;
}


// return dot product
template<class T>
T dgGeneralVector<T>::DotProduct (const dgGeneralVector<T> &A) const 
{
	dgAssert (m_colCount == A.m_colCount);
	T val (0.0);
	for (dgInt32 i = 0; i < m_colCount; i ++) {
		val = val + m_columns[i] * A.m_columns[i];
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += 2 * m_colCount;
	m_memoryWrite += m_colCount;
	#endif

	return val;
}




template<class T>
void dgGeneralVector<T>::Clear (T val)
{
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] = val;
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_memoryWrite += m_colCount;
	#endif
}

template<class T>
void dgGeneralVector<T>::Copy (const dgGeneralVector<T> &src)
{
	dgAssert (m_colCount == src.m_colCount);
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] = src.m_columns[i];
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_memoryWrite += m_colCount;
	#endif
}

template<class T>
void dgGeneralVector<T>::Scale (T scale)
{
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] = m_columns[i] * scale;
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += m_colCount;
	m_memoryWrite += m_colCount;
	#endif
}


template<class T>
T dgGeneralVector<T>::Norm2 () const
{
	T norm (0);
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		 norm = dgMax (m_columns[i] * m_columns[i], norm);
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += m_colCount;
	m_memoryWrite += m_colCount;
	#endif

	return norm;
}

template<class T>
T dgGeneralVector<T>::Norm () const
{
	return T (sqrt (Norm2 ()));
}


template<class T>
void dgGeneralVector<T>::LinearCombine (T scale, const dgGeneralVector<T> &A, const dgGeneralVector<T> &B)
{
	dgAssert (A.m_colCount == m_colCount);
	dgAssert (B.m_colCount == m_colCount);
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] = A.m_columns[i] * scale + B.m_columns[i];
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += 3 * m_colCount;
	m_memoryWrite += m_colCount;
	#endif
}


template<class T>
void dgGeneralVector<T>::operator+= (const dgGeneralVector<T> &A)
{
	dgAssert (A.m_colCount == m_colCount);
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] += A.m_columns[i];
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += m_colCount;
	m_memoryWrite += m_colCount;
	#endif
}

template<class T>
void dgGeneralVector<T>::operator-= (const dgGeneralVector<T> &A) 
{
	dgAssert (A.m_colCount == m_colCount);
	for (dgInt32 i = 0; i < m_colCount; i ++ ) {
		m_columns[i] -= A.m_columns[i];
	}

	#ifdef DG_COUNT_FLOAT_OPS
	m_floatsOp += m_colCount;
	m_memoryWrite += m_colCount;
	#endif
}


template<class T> dgInt32 dgGeneralVector <T>::m_floatsOp = 0;
template<class T> dgInt32 dgGeneralVector <T>::m_memoryWrite = 0;


#endif

