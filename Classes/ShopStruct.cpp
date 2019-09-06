#include "ShopStruct.h"

ShopStruct::ShopStruct(int _gold, int _price) :gold(_gold), price(_price){
}



bool ShopStruct::operator<(const ShopStruct& s) const{
	bool ret;

	if (this->price == s.price){
		ret = this->gold < s.gold;
	}
	else {
		ret = this->price < s.price;
	}


	return ret;
}
