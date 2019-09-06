#include "RegularPolygon.h"


RegularPolygon::RegularPolygon(float _radius, int _numOfVertices, float _initialAngle) :radius(_radius), numOfVertices(_numOfVertices), initialAngle(_initialAngle){
}

RegularPolygon::~RegularPolygon(){

}

RegularPolygon *RegularPolygon::create(float _radius, int _numOfVertices, float _initialAngle){
	RegularPolygon *hexagonNode = new RegularPolygon(_radius, _numOfVertices, _initialAngle);
	if (hexagonNode && hexagonNode->init()){
		hexagonNode->autorelease();
	}
	else {
		delete hexagonNode;
		hexagonNode = nullptr;
	}

	return hexagonNode;
}


void RegularPolygon::onEnter(){
	DrawNode::onEnter();

	//Find the horizontal arrangement
	std::vector<float> zConstantAngles;
	for (int i = 0; i < numOfVertices; i++){
		float angle = initialAngle;
		angle += (float)i *2.0f*M_PI / (float)numOfVertices;

		zConstantAngles.push_back(angle);
	}

	std::vector<Vec2> zConstantVertices;
	for (float angle : zConstantAngles){
		Vec2 vertice = radius*Vec2(std::cos(angle), std::sin(angle));
		zConstantVertices.push_back(vertice);
	}

	this->drawPoly(&zConstantVertices[0], zConstantVertices.size(), true, Color4F::RED);

}
