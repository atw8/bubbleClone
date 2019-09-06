#ifndef _TWO_TEXTURE_SPRITE_H_
#define _TWO_TEXTURE_SPRITE_H_

#include "cocos2d.h"
USING_NS_CC;

class TwoTextureSprite : public Sprite{
private:
	TwoTextureSprite(Texture2D *_texture1, Texture2D *_texture2, float _t);
public:
	static TwoTextureSprite *create(Texture2D *_texture1, Texture2D *_texture2, float _t);


	void onEnter() override;
	void onExit() override;


private:
	Texture2D *texture1;
	Texture2D *texture2;
	float t;
	float tSpeed;


public:
	void setT(float _t);
	float getT();

	void setTSpeed(float _tSpeed);
	float getTSpeed();
public:
	void updateT(float dt);
};


#endif //_TWO_TEXTURE_SPRITE_H_