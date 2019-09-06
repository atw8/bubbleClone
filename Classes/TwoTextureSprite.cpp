#include "TwoTextureSprite.h"


#define SCHEDULE_UPDATE_T "SCHEDULE_UPDATE_T"

TwoTextureSprite::TwoTextureSprite(Texture2D *_texture1, Texture2D *_texture2, float _t):texture1(_texture1), texture2(_texture2), t(_t){

}

TwoTextureSprite *TwoTextureSprite::create(Texture2D *_texture1, Texture2D *_texture2, float _t){
	TwoTextureSprite *twoTextureSprite = new TwoTextureSprite(_texture1, _texture2, _t);
	if (twoTextureSprite && twoTextureSprite->initWithTexture(_texture1)){
		twoTextureSprite->autorelease();
	}
	else {
		delete twoTextureSprite;
		twoTextureSprite = nullptr;
	}

	return twoTextureSprite;
}


void TwoTextureSprite::onEnter(){
	Sprite::onEnter();

	texture1->retain();
	texture2->retain();



	std::string frag = "global/changeTexture.fsh";
	std::string fragmentFilePath = FileUtils::getInstance()->fullPathForFilename(frag);
	std::string fragSource = FileUtils::getInstance()->getStringFromFile(fragmentFilePath);


	GLProgram *glProgram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());

	if (glProgram != nullptr){
		glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
		glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_COLOR, GLProgram::VERTEX_ATTRIB_COLOR);
		glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORDS);
		glProgram->link();
		glProgram->updateUniforms();

		this->setGLProgram(glProgram);


		GLProgramState *glProgramState = this->getGLProgramState();
		glProgramState->setUniformFloat("t", t);
		glProgramState->setUniformTexture("texture1", texture1);
		glProgramState->setUniformTexture("texture2", texture2);

		glProgram->updateUniforms();
	}

}

void TwoTextureSprite::onExit(){
	Sprite::onExit();

	texture1->release();
	texture2->release();
}

void TwoTextureSprite::setT(float _t){
	t = std::max(std::min(_t, 1.0f), 0.0f);

	GLProgramState *glProgramState = this->getGLProgramState();
	glProgramState->setUniformFloat("t", t);
	
}
float TwoTextureSprite::getT(){
	return t;
}

void TwoTextureSprite::setTSpeed(float _tSpeed){
	tSpeed = _tSpeed;
	this->schedule(std::bind(&TwoTextureSprite::updateT, this, std::placeholders::_1), SCHEDULE_UPDATE_T);
}

float TwoTextureSprite::getTSpeed(){
	return tSpeed;
}

void TwoTextureSprite::updateT(float dt){
	
	float newt = t + dt*tSpeed;

	bool isUnshedule = false;
	if (newt >= 1.0f && tSpeed >= 0.0f){
		newt = 1.0f;
		isUnshedule = true;
	}
	else if (newt <= 0.0f && tSpeed <= 0.0f){
		newt = 0.0f;
		isUnshedule = true;
	}

	setT(newt);
	if (isUnshedule){
		this->unschedule(SCHEDULE_UPDATE_T);
	}
}