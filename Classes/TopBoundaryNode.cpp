#include "TopBoundaryNode.h"

#include "Global.h"

TopBoundaryNode::TopBoundaryNode(std::string _sprStr, int _total) :sprStr(_sprStr), total(_total){

}

TopBoundaryNode* TopBoundaryNode::create(std::string _sprStr, int _total){
	TopBoundaryNode *topBoundaryNode = new TopBoundaryNode(_sprStr, _total);

	if (topBoundaryNode && topBoundaryNode->init()){
		topBoundaryNode->autorelease();
	}
	else {
		topBoundaryNode = nullptr;
	}

	return topBoundaryNode;
}


void TopBoundaryNode::onEnter() {
	Node::onEnter();

	slash = Sprite::create("taskBar_slash.png");
	this->addChild(slash);

	Sprite *charSpr = Sprite::create("numbers/numbers_taskLayer.png");

	currentLabel = Label::createWithCharMap(charSpr->getTexture(), 11, 16, '0');
	currentLabel->setString(to_string(total));
	currentLabel->setAnchorPoint(Vec2(1.0f, 0.5f));
	currentLabel->setPosition(Vec2(-10.0f, 0.0f));
	this->addChild(currentLabel);

	totalLabel = Label::createWithCharMap(charSpr->getTexture(), 11, 16, '0');
	totalLabel->setString(to_string(total));
	totalLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	totalLabel->setPosition(Vec2(5.0f, 0.0f));
	this->addChild(totalLabel);

	spr = Sprite::create(sprStr);
	spr->setAnchorPoint(Vec2(1.0f, 0.5f));
	spr->setPosition(currentLabel->getPosition() + Vec2(-getContWidth(currentLabel), 0.0f));
	this->addChild(spr);
	
	currentLabel->setString(to_string(0));


	Rect boundBox = getBoundingBox();

	DrawNode *drawNode = DrawNode::create();
	//std::vector<Vec2> ellipseCoords = getSuperEllipseCoords(1.1f*boundBox.size.width*0.5f, 1.1f*boundBox.size.height*0.5f, 20.0f, 2.0f, 30);
	std::vector<Vec2> ellipseCoords = getSuperEllipseCoords(1.1f*boundBox.size.width *0.5f, 25.0f, 20.0f, 2.0f, 30);
	drawNode->setPosition(boundBox.origin + Vec2(boundBox.size.width / 2.0f, boundBox.size.height / 2.0f));
	drawNode->drawPoly(&ellipseCoords[0], ellipseCoords.size(), true, Color4F::GREEN);
	//drawNode->drawRect(boundBox.origin, boundBox.origin + Vec2(boundBox.size.width, boundBox.size.height), Color4F::GREEN);
	this->addChild(drawNode);
}

void TopBoundaryNode::setCurrent(int _current){
	if (current == _current){
		return;
	}
	current = _current;

	currentLabel->setString(to_string(current));
}

void TopBoundaryNode::setTotal(int _total) {
	if (total == _total) {
		return;
	}
	total = _total;

	totalLabel->setString(to_string(total));
}

Rect TopBoundaryNode::getBoundingBox() const {

	std::vector<Rect> rects;
	rects.push_back(spr->getBoundingBox());
	rects.push_back(currentLabel->getBoundingBox());
	rects.push_back(totalLabel->getBoundingBox());
	rects.push_back(slash->getBoundingBox());

	return combineBoundingBoxes(rects);
}



std::vector<Vec2> TopBoundaryNode::getSuperEllipseCoords(float a, float b, float m, float n, int numOfPoints){

	std::function<float(float)> sgn = [](float t){
		float ret;
		if (t < 0){
			ret = -1.0f;
		}
		else if (t == 0){
			ret = 0.0f;
		}
		else if (t > 0){
			ret = 1.0f;
		}

		return ret;
	};


	std::vector<Vec2> ret;

	const float endAlpha = 0.0f;
	const float  begAlpha = 2.0f * M_PI;
	const float alphaStep = (endAlpha - begAlpha) / (float)numOfPoints;

	for (float alpha = begAlpha; alpha > endAlpha; alpha += alphaStep){
		Vec2 vec;

		vec.x = std::pow(std::abs(std::cos(alpha)), 2.0f / m) * a*sgn(std::cos(alpha));
		vec.y = std::pow(std::abs(std::sin(alpha)), 2.0f / n) * b*sgn(std::sin(alpha));


		ret.push_back(vec);
	}

	return ret;
}