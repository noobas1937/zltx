#ifndef __COCOS2D_EXT_H__
#define __COCOS2D_EXT_H__

#include "ExtensionMacros.h"


#include "GUI/CCControlExtension/CCControlExtensions.h"
#include "GUI/CCScrollView/CCScrollView.h"
#include "GUI/CCScrollView/CCTableView.h"

// Physics integration
#include "physics-nodes/CCPhysicsDebugNode.h"
#include "physics-nodes/CCPhysicsSprite.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT && _MSC_VER < 1900)
// Window 10 UWP does not yet support CURL
#include "assets-manager/AssetsManager.h"
#include "assets-manager/AssetsManagerEx.h"
#include "assets-manager/CCEventAssetsManagerEx.h"
#include "assets-manager/CCEventListenerAssetsManagerEx.h"
#include "assets-manager/Manifest.h"
#endif

#include "ExtensionDeprecated.h"

// Particle System, include Particle Universe Particle System
//lixu 20151224 no usage for 3d
//#include "Particle3D/CCParticleSystem3D.h"
//#include "Particle3D/PU/CCPUParticleSystem3D.h"

#endif /* __COCOS2D_EXT_H__ */
