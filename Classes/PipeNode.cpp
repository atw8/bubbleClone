#include "PipeNode.h"
#include "Global.h"

PipeNode::PipeNode(float _bodyHeight, float _bodyWidth, PhysicsMaterial _pipeNodeMaterial):bodyHeight(_bodyHeight), bodyWidth(_bodyWidth), pipeNodeMaterial(_pipeNodeMaterial){

}

PipeNode *PipeNode::create(float _bodyHeight, float _bodyWidth, PhysicsMaterial _pipeNodeMaterial){
	PipeNode *pipeNode = new PipeNode(_bodyHeight, _bodyWidth, _pipeNodeMaterial);

	if (pipeNode && pipeNode->init()){
		pipeNode->autorelease();
	}
	else {
		delete pipeNode;
		pipeNode = nullptr;
	}

	return pipeNode;
}

bool PipeNode::init(){
	if (!Node::init()){
		return false;
	}


	pipeBody = Sprite::create("balls/pipe_body.png");
	pipeTop = Sprite::create("balls/pipe_top.png");
	pipeBottom = Sprite::create("balls/pipe_bottom.png");


	pipeTop->setScale(bodyWidth / getContWidth(pipeTop));
	pipeBottom->setScale(pipeTop->getScale());
	pipeBody->setScaleX(pipeTop->getScale());
	pipeBody->setScaleY(bodyHeight - getBoundHeight(pipeTop) - getBoundHeight(pipeBottom));

	pipeTop->setPosition(Vec2(0.0f, getBoundHeight(pipeTop) / 2.0f + getBoundHeight(pipeBody)/2.0f));
	this->addChild(pipeTop);


	this->addChild(pipeBody);


	pipeBottom->setPosition(Vec2(0.0f, -getBoundHeight(pipeBottom) / 2.0f - getBoundHeight(pipeBody) / 2.0f));
	this->addChild(pipeBottom);






	float offset = -(getBoundHeight(pipeTop) - getBoundHeight(pipeBottom))/2.0f;
	pipeBottom->setPositionY(pipeBottom->getPositionY() + offset);
	pipeTop->setPositionY(pipeTop->getPositionY() + offset);
	pipeBody->setPositionY(pipeBody->getPositionY() + offset);

	//DrawNode *dNode = DrawNode::create();
	//dNode->drawSolidRect(Vec2(-bodyWidth / 2.0f, -bodyHeight / 2.0f), Vec2(bodyWidth / 2.0f, bodyHeight / 2.0f), Color4F::RED);
	//this->addChild(dNode, -10);


	Vec2 displacement;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID 
	displacement = Vec2(0.0f, 0.0f);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	displacement = Vec2(-bodyWidth / 2.0f, -bodyHeight / 2.0f);
#endif
	
	

	std::vector<Vec2> edgeChain1Points;
	edgeChain1Points.push_back(Vec2(pipeTop->getPosition() + Vec2(-getBoundWidth(pipeTop) / 2.0f, getBoundHeight(pipeTop) / 2.0f)) + displacement);
	edgeChain1Points.push_back(Vec2(pipeTop->getPosition() + Vec2(-getBoundWidth(pipeTop) / 2.0f, -getBoundHeight(pipeTop) / 2.0f)) + displacement);
	edgeChain1Points.push_back(Vec2(pipeBody->getPosition() + Vec2(-getBoundWidth(pipeBody) / 2.0f, getBoundHeight(pipeBody) / 2.0f)) + displacement);
	edgeChain1Points.push_back(Vec2(pipeBottom->getPosition() + Vec2(-getBoundWidth(pipeBottom) / 2.0f, -getBoundHeight(pipeBottom) / 2.0f)) + displacement);

	std::vector<Vec2> edgeChain2Points;
	edgeChain2Points.push_back(Vec2(pipeBottom->getPosition() + Vec2(getBoundWidth(pipeBottom) / 2.0f, -getBoundHeight(pipeBottom) / 2.0f)) + displacement);
	edgeChain2Points.push_back(Vec2(pipeBody->getPosition() + Vec2(getBoundWidth(pipeBody) / 2.0f, getBoundHeight(pipeBody) / 2.0f)) + displacement);
	edgeChain2Points.push_back(Vec2(pipeTop->getPosition() + Vec2(getBoundWidth(pipeTop) / 2.0f, -getBoundHeight(pipeTop) / 2.0f)) + displacement);
	edgeChain2Points.push_back(Vec2(pipeTop->getPosition() + Vec2(getBoundWidth(pipeTop) / 2.0f, getBoundHeight(pipeTop) / 2.0f)) + displacement);


	PhysicsShapeEdgeChain *edgeChain1 = PhysicsShapeEdgeChain::create(&edgeChain1Points[0], edgeChain1Points.size(), pipeNodeMaterial);

	PhysicsShapeEdgeChain *edgeChain2 = PhysicsShapeEdgeChain::create(&edgeChain2Points[0], edgeChain2Points.size(), pipeNodeMaterial);

	PhysicsBody *body = PhysicsBody::create();
	body->setDynamic(false);
	body->addShape(edgeChain1);
	body->addShape(edgeChain2);

	this->setPhysicsBody(body);

	return true;
}

void PipeNode::onEnter(){
	Node::onEnter();
}


const Size &PipeNode::getContentSize() const{
	Size ret;
	ret.width = bodyWidth;
	ret.height = bodyHeight;

	return ret;
}


void PipeNode::setBodyHeight(float _bodyHeight){

}