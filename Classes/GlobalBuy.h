#ifndef _GLOBAL_BUY_H_
#define _GLOBAL_BUY_H_

#include "ShopStruct.h"
#include <functional>
#include <map>

class GlobalBuy {
private:
	GlobalBuy();
public:
	static GlobalBuy* getInstance();

	void buy(ShopStruct shopStruct, std::function<void(ShopStruct, bool)> buyBack);

	void callBuyBack(std::string feeInfo, bool isSuccess);
	void callBuyBack(ShopStruct shopStruct, bool isSuccess);

private:
	std::map< ShopStruct, std::function<void(ShopStruct, bool)> > buyBacks;

	std::string getFeeInfoForShopStruct(ShopStruct shopStruct);
	ShopStruct getShopStructForFeeInfo(std::string feeInfo);
};


#endif //_GLOBAL_BUY_H_