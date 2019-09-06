#include "GlobalBuy.h"

#include "Global.h"
#include "DDUserDefault.h"

#include "AppActivity.h"

#include "cocos2d.h"
USING_NS_CC;

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"

GlobalBuy::GlobalBuy(){

}

GlobalBuy *GlobalBuy::getInstance(){
	static GlobalBuy gInstance;

	return &gInstance;
}


void GlobalBuy::buy(ShopStruct shopStruct, std::function<void(ShopStruct, bool)> buyBack){
	buyBacks[shopStruct] = buyBack;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo methodInfo;
	bool ret = JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, "buy", "(Ljava/lang/String;)V");
	if (ret){
		std::string feeInfo = getFeeInfoForShopStruct(shopStruct);
		
		jstring jFeeInfo = methodInfo.env->NewStringUTF(feeInfo.c_str());
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jFeeInfo);
	}
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 or CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	callBuyBack(shopStruct, true);
#endif	
}

void GlobalBuy::callBuyBack(std::string feeInfo, bool isSuccess){
	ShopStruct shopStruct = getShopStructForFeeInfo(feeInfo);
	callBuyBack(shopStruct, isSuccess);
}
void GlobalBuy::callBuyBack(ShopStruct shopStruct, bool isSuccess){
	buyBacks[shopStruct](shopStruct, isSuccess);
}


std::string GlobalBuy::getFeeInfoForShopStruct(ShopStruct shopStruct){
	std::string ret = "price" + to_string(shopStruct.price) + "gold" + to_string(shopStruct.gold);

	return ret;
}

ShopStruct GlobalBuy::getShopStructForFeeInfo(std::string feeInfo){
	
	ShopStruct ret(0, 0);

	std::vector<ShopStruct> shopStructs = DDUserDefault::getInstance()->getShopStructs();
	for (ShopStruct shopStruct : shopStructs){
		if (feeInfo.compare(getFeeInfoForShopStruct(shopStruct)) == 0){
			ret = shopStruct;
			break;
		}
	}

	return ret;
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_buySuccess(JNIEnv * env, jobject, jstring str, jboolean bol){
	const char *nativeString = env->GetStringUTFChars(str, 0);
	GlobalBuy::getInstance()->callBuyBack(std::string(nativeString), (bool)bol);

	env->ReleaseStringUTFChars(str, nativeString);
}
#endif
