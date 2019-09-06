#include "ShootBubble.h"

ShootBubble::ShootBubble(float _innerRadius, Bubble _bubble):BubbleView(_innerRadius, _bubble){

}

ShootBubble *ShootBubble::create(float _innerRadius, Bubble _bubble){
	ShootBubble *shootBubble = new ShootBubble(_innerRadius, _bubble);
	if (shootBubble && shootBubble->init()){
		shootBubble->autorelease();
	}
	else {
		delete shootBubble;
		shootBubble = nullptr;
	}

	return shootBubble;
}

void ShootBubble::onEnter(){
	BubbleView::onEnter();
}


void ShootBubble::addVelocity(Vec2 val){
	setVelocity(getVelocity() + val);
}

Vec2 ShootBubble::getVelocity(){
	return velocity;
}
void ShootBubble::setVelocity(Vec2 _velocity){
	velocity = _velocity;
}

void ShootBubble::updateVelocity(float dt){
	this->setPosition(getPosition() + velocity*dt);
}

Vec2 ShootBubble::getDirection(){
	return velocity.getNormalized();
}

float ShootBubble::getShootBubbleRadius(){
	return shootBubbleRadius;
}
void ShootBubble::setShootBubbleRadiusRelativeToInnerRadius(float ratio){
	setShootBubbleRadius(innerRadius*ratio);
}

void ShootBubble::setShootBubbleRadius(float _shootBubbleRadius){
	shootBubbleRadius = _shootBubbleRadius;
}