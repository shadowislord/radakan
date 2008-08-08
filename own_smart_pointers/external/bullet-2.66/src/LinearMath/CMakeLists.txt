
INCLUDE_DIRECTORIES(
${RADAKAN_SOURCE_DIR}/external/bullet-2.66/src }
)

SET(SOURCE_FILES 
		btAlignedObjectArray.h
		btList.h
		btPoolAllocator.h
		btRandom.h
		btVector3.h
		btDefaultMotionState.h
		btMatrix3x3.h
		btQuadWord.h
		btScalar.h
		btAabbUtil2.h
		btGeometryUtil.cpp
		btMinMax.h
		btQuaternion.h
		btStackAlloc.h
		btGeometryUtil.h
		btMotionState.h
		btQuickprof.cpp
		btTransform.h
		btAlignedAllocator.h
		btIDebugDraw.h
		btPoint3.h
		btQuickprof.h
		btTransformUtil.h
		btQuickprof.cpp
		btGeometryUtil.cpp
		btAlignedAllocator.cpp
)

if(WIN32)
	ADD_LIBRARY(LibLinearMath ${SOURCE_FILES})
else(WIN32)
	ADD_LIBRARY(LibLinearMath SHARED ${SOURCE_FILES})
endif(WIN32)


