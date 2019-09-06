#ifndef _SHOP_STRUCT_H_
#define _SHOP_STRUCT_H_


class ShopStruct {
public:
	ShopStruct(int _gold, int _price);

	int gold;
	int price;
	
	bool operator<(const ShopStruct& s) const;
}; 



#endif //_SHOP_STRUCT_H_