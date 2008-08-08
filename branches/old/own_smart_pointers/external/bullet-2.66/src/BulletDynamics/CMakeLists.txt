INCLUDE_DIRECTORIES(
${RADAKAN_SOURCE_DIR}/external/bullet-2.66/src }
)

SET(SOURCE_FILES 
	ConstraintSolver/btContactConstraint.cpp
	ConstraintSolver/btContactConstraint.h
	ConstraintSolver/btConeTwistConstraint.cpp
	ConstraintSolver/btConeTwistConstraint.h
	ConstraintSolver/btGeneric6DofConstraint.cpp
	ConstraintSolver/btGeneric6DofConstraint.h
	ConstraintSolver/btHingeConstraint.cpp
	ConstraintSolver/btHingeConstraint.h
	ConstraintSolver/btPoint2PointConstraint.cpp
	ConstraintSolver/btPoint2PointConstraint.h
	ConstraintSolver/btSequentialImpulseConstraintSolver.cpp
	ConstraintSolver/btSequentialImpulseConstraintSolver.h
	ConstraintSolver/btSolve2LinearConstraint.cpp
	ConstraintSolver/btSolve2LinearConstraint.h
	ConstraintSolver/btTypedConstraint.cpp
	ConstraintSolver/btTypedConstraint.h
	Dynamics/Bullet-C-API.cpp
	Dynamics/btDiscreteDynamicsWorld.cpp
	Dynamics/btDiscreteDynamicsWorld.h
	Dynamics/btSimpleDynamicsWorld.cpp
	Dynamics/btSimpleDynamicsWorld.h
	Dynamics/btRigidBody.cpp
	Dynamics/btRigidBody.h
	Vehicle/btRaycastVehicle.cpp
	Vehicle/btRaycastVehicle.h
	Vehicle/btWheelInfo.cpp
	Vehicle/btWheelInfo.h
)

if(WIN32)
	ADD_LIBRARY(LibBulletDynamics ${SOURCE_FILES})
else(WIN32)
	ADD_LIBRARY(LibBulletDynamics SHARED ${SOURCE_FILES})
endif(WIN32)
