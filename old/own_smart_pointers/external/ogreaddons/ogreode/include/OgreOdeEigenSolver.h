#ifndef _OGREODEEIGENSOLVER_H_
#define _OGREODEEIGENSOLVER_H_

#include "OgreOdePreReqs.h"

namespace OgreOde 
{
	class _OgreOdeExport EigenSolver
	{
	public:
		EigenSolver(int iSize);
		~EigenSolver();

		// set the matrix for eigensolving
		Ogre::Real& operator() (int iRow, int iCol);

		// Get the eigenresults (eigenvectors are columns of eigenmatrix).  The
		// GetEigenvector calls involving Vector2, Ogre::Vector3, and Vector4 should
		// only be called if you know that the eigenmatrix is of the appropriate
		// size.
	    void GetEigenvector (int i, Ogre::Vector3& rkV) const;
		Ogre::Real GetEigenvalue (int i) const;

		// solve eigensystem, use decreasing sort on eigenvalues
	    void DecrSortEigenStuff3 ();
		void IncrSortEigenStuff3 ();

		static void GenerateOrthonormalBasis (Ogre::Vector3& rkU, Ogre::Vector3& rkV, Ogre::Vector3& rkW, bool bUnitLengthW);
		static void orthogonalLineFit(unsigned int vertex_count, const Ogre::Vector3* vertices, Ogre::Vector3& origin, Ogre::Vector3& direction);
		static Ogre::Real SqrDistance(const Ogre::Vector3& rkPoint,const Ogre::Vector3& origin,const Ogre::Vector3& direction);
		static void GaussPointsFit(unsigned int iQuantity,const Ogre::Vector3* akPoint, Ogre::Vector3 &rkCenter, Ogre::Vector3 akAxis[3], Ogre::Real afExtent[3]);

	protected:
		int m_iSize;
		Ogre::Matrix3 m_kMat;
		Ogre::Real* m_afDiag;
		Ogre::Real* m_afSubd;

		// For odd size matrices, the Householder reduction involves an odd
		// number of reflections.  The product of these is a reflection.  The
		// QL algorithm uses rotations for further reductions.  The final
		// orthogonal matrix whose columns are the eigenvectors is a reflection,
		// so its determinant is -1.  For even size matrices, the Householder
		// reduction involves an even number of reflections whose product is a
		// rotation.  The final orthogonal matrix has determinant +1.  Many
		// algorithms that need an eigendecomposition want a rotation matrix.
		// We want to guarantee this is the case, so m_bRotation keeps track of
		// this.  The DecrSort and IncrSort further complicate the issue since
		// they swap columns of the orthogonal matrix, causing the matrix to
		// toggle between rotation and reflection.  The value m_bRotation must
		// be toggled accordingly.
		bool m_bIsRotation;
		void GuaranteeRotation ();

		// Householder reduction to tridiagonal form
	    void Tridiagonal3 ();
  
		// QL algorithm with implicit shifting, applies to tridiagonal matrices
		bool QLAlgorithm ();

		// sort eigenvalues from largest to smallest
		void DecreasingSort ();

		void IncreasingSort ();
	};
}

#endif
