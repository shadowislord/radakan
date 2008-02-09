if(WIN32)
	find_path(Bullet_INCLUDE_DIR "btBulletCollisionCommon.h"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/src")

	find_path(Bullet_LIB_RELEASE "bullet.lib"
		"${RADAKAN_SOURCE_DIR}/lib"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/lib")

	find_path(Bullet_LIB_DEBUG "bullet.lib"
		"${RADAKAN_SOURCE_DIR}/lib"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/lib")

else(WIN32)
	find_path(Bullet_INCLUDE_DIR "btBulletCollisionCommon.h"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/src")

	find_path(Bullet_LIB_RELEASE "libLibBulletCollision.a"
		"${RADAKAN_SOURCE_DIR}/lib"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/lib")

	find_path(Bullet_LIB_DEBUG "libLibBulletCollision.a"
		"${RADAKAN_SOURCE_DIR}/lib"
		"${RADAKAN_SOURCE_DIR}/external/bullet-2.66/lib")
endif(WIN32)

set(Bullet_FOUND 0)

if(Bullet_INCLUDE_DIR AND Bullet_LIB_DEBUG AND Bullet_LIB_RELEASE)
	set(Bullet_FOUND 1)
endif(Bullet_INCLUDE_DIR AND Bullet_LIB_DEBUG AND Bullet_LIB_RELEASE)

if (Bullet_FOUND)
	if (NOT Bullet_FIND_QUIETLY)
		message(STATUS "Found Bullet: ${Bullet_INCLUDE_DIR}, ${Bullet_LIB_DEBUG}, ${Bullet_LIB_RELEASE}")
	endif (NOT Bullet_FIND_QUIETLY)
else (Bullet_FOUND)
	if (Bullet_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Bullet")
	endif (Bullet_FIND_REQUIRED)
endif (Bullet_FOUND)
