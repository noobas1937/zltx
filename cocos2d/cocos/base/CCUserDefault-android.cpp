/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "base/CCUserDefault.h"
#include "platform/CCPlatformConfig.h"
#include "base/ccUtils.h"
#include "platform/CCCommon.h"
#include "base/base64.h"
#include "base/CCDirector.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

// root name of xml
#define USERDEFAULT_ROOT_NAME    "userDefaultRoot"

#define KEEP_COMPATABILITY

#define XML_FILE_NAME "UserDefault.xml"

#ifdef KEEP_COMPATABILITY
#include "platform/CCFileUtils.h"
#include "tinyxml2.h"
#endif

using namespace std;

NS_CC_BEGIN

/**
 * implements of UserDefault
 */

#define USE_CACHE 1

UserDefault* UserDefault::_userDefault = nullptr;
string UserDefault::_filePath = string("");
bool UserDefault::_isFilePathInitialized = false;
static std::unordered_map<std::string, Value> mCachesValue;

#ifdef KEEP_COMPATABILITY
static tinyxml2::XMLElement* getXMLNodeForKey(const char* pKey, tinyxml2::XMLDocument **doc)
{
    tinyxml2::XMLElement* curNode = nullptr;
    tinyxml2::XMLElement* rootNode = nullptr;
    
    if (! UserDefault::isXMLFileExist())
    {
        return nullptr;
    }
    
    // check the key value
    if (! pKey)
    {
        return nullptr;
    }
    
    do
    {
        tinyxml2::XMLDocument* xmlDoc = new tinyxml2::XMLDocument();
        *doc = xmlDoc;
        ssize_t size;
        
        std::string xmlBuffer = FileUtils::getInstance()->getStringFromFile(UserDefault::getInstance()->getXMLFilePath().c_str());

        if (xmlBuffer.empty())
        {
            CCLOG("can not read xml file");
            break;
        }
        xmlDoc->Parse(xmlBuffer.c_str());

        // get root node
        rootNode = xmlDoc->RootElement();
        if (nullptr == rootNode)
        {
            CCLOG("read root node error");
            break;
        }
        // find the node
        curNode = rootNode->FirstChildElement();
        if (!curNode)
        {
            // There is not xml node, delete xml file.
            remove(UserDefault::getInstance()->getXMLFilePath().c_str());
            
            return nullptr;
        }
        
        while (nullptr != curNode)
        {
            const char* nodeName = curNode->Value();
            if (!strcmp(nodeName, pKey))
            {
                // delete the node
                break;
            }
            
            curNode = curNode->NextSiblingElement();
        }
    } while (0);
    
    return curNode;
}

static void deleteNode(tinyxml2::XMLDocument* doc, tinyxml2::XMLElement* node)
{
    if (node)
    {
        doc->DeleteNode(node);
        doc->SaveFile(UserDefault::getInstance()->getXMLFilePath().c_str());
        delete doc;
    }
}

static void deleteNodeByKey(const char *pKey)
{
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    deleteNode(doc, node);
#ifdef USE_CACHE
    mCachesValue.erase(pKey);
#endif
}
#endif

UserDefault::~UserDefault()
{
}

UserDefault::UserDefault()
{
}

// FIXME:: deprecated
void UserDefault::purgeSharedUserDefault()
{
    UserDefault::destroyInstance();
}

void UserDefault::destroyInstance()
{
   CC_SAFE_DELETE(_userDefault);
}

bool UserDefault::getBoolForKey(const char* pKey)
{
    return getBoolForKey(pKey, false);
}

bool UserDefault::getBoolForKey(const char* pKey, bool defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef USE_CACHE
    auto valIndex = mCachesValue.find(pKey);
    if( valIndex != mCachesValue.end())
    {
        return valIndex->second.asBool();
    }
#endif
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            const char* value = (const char*)node->FirstChild()->Value();
            bool ret = (! strcmp(value, "true"));
            
            // set value in NSUserDefaults
            setBoolForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    bool ret = getBoolForKeyJNI(pKey, defaultValue);
#ifdef USE_CACHE
    mCachesValue[pKey] = ret;
#endif
    return ret;
}

int UserDefault::getIntegerForKey(const char* pKey)
{
    return getIntegerForKey(pKey, 0);
}

int UserDefault::getIntegerForKey(const char* pKey, int defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef USE_CACHE
    auto valIndex = mCachesValue.find(pKey);
    if( valIndex != mCachesValue.end())
    {
        return valIndex->second.asInt();
    }
#endif
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            int ret = atoi((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setIntegerForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif
    
	int ret = getIntegerForKeyJNI(pKey, defaultValue);
#ifdef USE_CACHE
    mCachesValue[pKey] = ret;
#endif
    return ret;
}

float UserDefault::getFloatForKey(const char* pKey)
{
    return getFloatForKey(pKey, 0.0f);
}

float UserDefault::getFloatForKey(const char* pKey, float defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef USE_CACHE
    auto valIndex = mCachesValue.find(pKey);
    if( valIndex != mCachesValue.end())
    {
        return valIndex->second.asFloat();
    }
#endif
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            float ret = utils::atof((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setFloatForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    float ret = getFloatForKeyJNI(pKey, defaultValue);
#ifdef USE_CACHE
    mCachesValue[pKey] = ret;
#endif
    return ret;
}

double  UserDefault::getDoubleForKey(const char* pKey)
{
    return getDoubleForKey(pKey, 0.0);
}

double UserDefault::getDoubleForKey(const char* pKey, double defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef USE_CACHE
    auto valIndex = mCachesValue.find(pKey);
    if( valIndex != mCachesValue.end())
    {
        return valIndex->second.asDouble();
    }
#endif
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            double ret = utils::atof((const char*)node->FirstChild()->Value());
            
            // set value in NSUserDefaults
            setDoubleForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

	double ret = getDoubleForKeyJNI(pKey, defaultValue);
#ifdef USE_CACHE
    mCachesValue[pKey] = ret;
#endif
    return ret;
}

std::string UserDefault::getStringForKey(const char* pKey)
{
    return getStringForKey(pKey, "");
}

string UserDefault::getStringForKey(const char* pKey, const std::string & defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef USE_CACHE
    auto valIndex = mCachesValue.find(pKey);
    if( valIndex != mCachesValue.end())
    {
        return valIndex->second.asString();
    }
#endif
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            string ret = (const char*)node->FirstChild()->Value();
            
            // set value in NSUserDefaults
            setStringForKey(pKey, ret);
            flush();
            
            // delete xmle node
            deleteNode(doc, node);
            
            return ret;
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif

    string ret = getStringForKeyJNI(pKey, defaultValue.c_str());
#ifdef USE_CACHE
    mCachesValue[pKey] = ret;
#endif
    return ret;
}

Data UserDefault::getDataForKey(const char* pKey)
{
    return getDataForKey(pKey, Data::Null);
}

Data UserDefault::getDataForKey(const char* pKey, const Data& defaultValue)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    tinyxml2::XMLDocument* doc = nullptr;
    tinyxml2::XMLElement* node = getXMLNodeForKey(pKey, &doc);
    if (node)
    {
        if (node->FirstChild())
        {
            const char * encodedData = node->FirstChild()->Value();
            
            unsigned char * decodedData;
            int decodedDataLen = base64Decode((unsigned char*)encodedData, (unsigned int)strlen(encodedData), &decodedData);
            
            if (decodedData) {
                Data ret;
                ret.fastSet(decodedData, decodedDataLen);
                
                // set value in NSUserDefaults
                setDataForKey(pKey, ret);
                
                flush();
                
                // delete xmle node
                deleteNode(doc, node);
                
                return ret;
            }
        }
        else
        {
            // delete xmle node
            deleteNode(doc, node);
        }
    }
#endif
    
    char * encodedDefaultData = NULL;
    unsigned int encodedDefaultDataLen = !defaultValue.isNull() ? base64Encode(defaultValue.getBytes(), defaultValue.getSize(), &encodedDefaultData) : 0;
    
    string encodedStr = getStringForKeyJNI(pKey, encodedDefaultData);

    if (encodedDefaultData)
        free(encodedDefaultData);

    CCLOG("ENCODED STRING: --%s--%d", encodedStr.c_str(), encodedStr.length());
      
    unsigned char * decodedData = NULL;
    int decodedDataLen = base64Decode((unsigned char*)encodedStr.c_str(), (unsigned int)encodedStr.length(), &decodedData);

    CCLOG("DECODED DATA: %s %d", decodedData, decodedDataLen);
    
    if (decodedData && decodedDataLen) {
        Data ret;
        ret.fastSet(decodedData, decodedDataLen);
        return ret;
    }

    return defaultValue;
}


void UserDefault::setBoolForKey(const char* pKey, bool value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
#ifdef USE_CACHE
    mCachesValue[pKey] = value;
#endif
    return setBoolForKeyJNI(pKey, value);
}

void UserDefault::setIntegerForKey(const char* pKey, int value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
#ifdef USE_CACHE
    mCachesValue[pKey] = value;
#endif
    return setIntegerForKeyJNI(pKey, value);
}

void UserDefault::setFloatForKey(const char* pKey, float value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
#ifdef USE_CACHE
    mCachesValue[pKey] = value;
#endif
    return setFloatForKeyJNI(pKey, value);
}

void UserDefault::setDoubleForKey(const char* pKey, double value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
#ifdef USE_CACHE
    mCachesValue[pKey] = value;
#endif
    return setDoubleForKeyJNI(pKey, value);
}

void UserDefault::setStringForKey(const char* pKey, const std::string & value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
#ifdef USE_CACHE
    mCachesValue[pKey] = value;
#endif
    return setStringForKeyJNI(pKey, value.c_str());
}

void UserDefault::setDataForKey(const char* pKey, const Data& value)
{
    Autolock lock(g_platmMutex);
    
#ifdef KEEP_COMPATABILITY
    deleteNodeByKey(pKey);
#endif
    
    CCLOG("SET DATA FOR KEY: --%s--%d", value.getBytes(), (int)(value.getSize()));
    char * encodedData = nullptr;
    unsigned int encodedDataLen = base64Encode(value.getBytes(), value.getSize(), &encodedData);

    CCLOG("SET DATA ENCODED: --%s", encodedData);
    
    setStringForKeyJNI(pKey, encodedData);
    
    if (encodedData)
        free(encodedData);
}

// FIXME:: deprecated
UserDefault* UserDefault::sharedUserDefault()
{
    return UserDefault::getInstance();
}

UserDefault* UserDefault::getInstance()
{    
    if (! _userDefault)
    {
#ifdef KEEP_COMPATABILITY
        initXMLFilePath();
#endif
        _userDefault = new (std::nothrow) UserDefault();
    }

    return _userDefault;
}

bool UserDefault::isXMLFileExist()
{
    return FileUtils::getInstance()->isFileExist(_filePath);
}

void UserDefault::initXMLFilePath()
{
#ifdef KEEP_COMPATABILITY
    if (! _isFilePathInitialized)
    {
        // UserDefault.xml is stored in /data/data/<package-path>/ before v2.1.2
        _filePath += "/data/data/" + getPackageNameJNI() + "/" + XML_FILE_NAME;
        _isFilePathInitialized = true;
    }
#endif
}

// create new xml file
bool UserDefault::createXMLFile()
{
    return false;
}

const string& UserDefault::getXMLFilePath()
{
    return _filePath;
}

void UserDefault::flush()
{
}

void UserDefault::resetUserDefault()
{
    
}

NS_CC_END

#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
