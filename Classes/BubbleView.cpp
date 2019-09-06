#include "BubbleView.h"

#include "Global.h"

BubbleView::BubbleView(float _innerRadius, Bubble _bubble) :innerRadius(_innerRadius), bubble(_bubble){
}

BubbleView::~BubbleView(){



}

BubbleView *BubbleView::create(float _innerRadius, Bubble _bubble){
	BubbleView *bubbleView = new BubbleView(_innerRadius, _bubble);
	if (bubbleView && bubbleView->init()){
		bubbleView->autorelease();
	}
	else {
		delete bubbleView;
		bubbleView = nullptr;
	}

	return bubbleView;
}


void BubbleView::onEnter(){
	Node::onEnter();

	bubbleNode = Node::create();
	this->addChild(bubbleNode);

	bubbleSprite = Sprite::create();
	bubbleNode->addChild(bubbleSprite, 0);

	bubbleArmature = Armature::create();
	bubbleNode->addChild(bubbleArmature, 1);

	setBubbleInner(bubble, true);
}

void BubbleView::setBubble(Bubble _bubble){
	if (bubble == _bubble){
		return;
	}


	setBubbleInner(_bubble, false);
}

void BubbleView::setBubbleInner(Bubble _bubble, bool forceInit){

	bubbleSprite->setVisible(false);
	bubbleArmature->setVisible(false);

	bubbleNode->setPosition(Vec2(0.0f, 0.0f));


	switch (_bubble.getBubbleType()){
	case BubbleType::UNDEFINED:
		break;
	case BubbleType::RED:
	case BubbleType::BLUE:
	case BubbleType::GREEN:
	case BubbleType::YELLOW:
	case BubbleType::PURPLE:
	case BubbleType::ICE:
	case BubbleType::WOOD_1:
	case BubbleType::WOOD_2:
	case BubbleType::FOG:
	case BubbleType::FISION:
	case BubbleType::STONE:
	case BubbleType::CHROME:
	{
								std::string bubbleSpriteStr;
								float scale = 1.0f;

								switch (_bubble.getBubbleType()){
								case BubbleType::CHROME:
									bubbleSpriteStr = "balls/pop_chrome.png";
									break;
								case BubbleType::RED:
									bubbleSpriteStr = "balls/pop_red.png";
									break;
								case BubbleType::BLUE:
									bubbleSpriteStr = "balls/pop_blue.png";
									break;
								case BubbleType::GREEN:
									bubbleSpriteStr = "balls/pop_green.png";
									break;
								case BubbleType::YELLOW:
									bubbleSpriteStr = "balls/pop_yellow.png";
									break;
								case BubbleType::PURPLE:
									bubbleSpriteStr = "balls/pop_purple.png";
									break;
								case BubbleType::ICE:
									bubbleSpriteStr = "balls/barrier_ice.png";
									break;
								case BubbleType::WOOD_1:
									bubbleSpriteStr = "balls/barrier_wood_1.png";
									break;
								case BubbleType::WOOD_2:
									bubbleSpriteStr = "balls/barrier_wood_2.png";
									break;
								case BubbleType::FOG:
									bubbleSpriteStr = "balls/ball_fog.png";
									scale = 1.6f;
									break;
								case BubbleType::FISION:
									bubbleSpriteStr = "balls/pop_fission.png";
									break;
								case BubbleType::STONE:
									bubbleSpriteStr = "balls/barrier_stone.png";
									break;
								}

								bubbleSprite->setSpriteFrame(Sprite::create(bubbleSpriteStr)->getSpriteFrame());
								bubbleSprite->setVisible(true);
								
								bubbleNode->setScale(scale * (2.0f*innerRadius / getContWidth(bubbleSprite)));
								
	}
		break;
	case BubbleType::WITCH:
		if (_bubble.getParent() == Dec2(0, 0)){
			bubbleArmature->setVisible(true);
			bubbleSprite->setVisible(true);

			std::string bubbleSpriteStr;

			switch (_bubble.getBubbleTypeWitchFog()){
			case BubbleType::RED:
				bubbleSpriteStr = "balls/witch_red.png";
				break;
			case BubbleType::BLUE:
				bubbleSpriteStr = "balls/witch_blue.png";
				break;
			case BubbleType::GREEN:
				bubbleSpriteStr = "balls/witch_green.png";
				break;
			case BubbleType::YELLOW:
				bubbleSpriteStr = "balls/witch_yellow.png";
				break;
			case BubbleType::PURPLE:
				bubbleSpriteStr = "balls/witch_purple.png";
				break;
			}

			bubbleSprite->setSpriteFrame(Sprite::create(bubbleSpriteStr)->getSpriteFrame());




			if (_bubble.getBubbleType() != bubble.getBubbleType() || forceInit){
				bubbleArmature->removeFromParentAndCleanup(true);
				bubbleArmature = Armature::create("witch_all");
				bubbleArmature->getAnimation()->playWithIndex(0);
				bubbleNode->addChild(bubbleArmature);
			}

			float scale = (6.0f*innerRadius) / getContWidth(bubbleSprite);
			bubbleNode->setScale(scale);
		}
		break;
	case BubbleType::ANIMAL1:
	{
		bubbleArmature->removeFromParentAndCleanup(true);
		bubbleArmature = Armature::create("animal1_btn");
		bubbleArmature->getAnimation()->playWithIndex(0);
		bubbleNode->addChild(bubbleArmature);

		bubbleNode->setScale((2.0f*innerRadius) / getContWidth(bubbleArmature));
	}
		break;
	case BubbleType::ANIMAL2:
		if (_bubble.getParent() == Dec2(0, 0)){
			bubbleArmature->removeFromParentAndCleanup(true);
			bubbleArmature = Armature::create("YL_btn");
			bubbleArmature->getAnimation()->playWithIndex(0);
			bubbleNode->addChild(bubbleArmature);

			bubbleNode->setScale((4.0f*innerRadius) / getContWidth(bubbleArmature));
			bubbleNode->setPosition(Vec2(0.0f, -innerRadius*(3.0f / 4.0f)));
		}
		break;
	case BubbleType::ANIMAL3:
		if (_bubble.getParent() == Dec2(0, 0)){
			bubbleArmature->removeFromParentAndCleanup(true);
			bubbleArmature = Armature::create("animal3_btn");
			bubbleArmature->getAnimation()->playWithIndex(0);
			bubbleNode->addChild(bubbleArmature);

			bubbleNode->setScale((6.0f*innerRadius) / getContWidth(bubbleArmature));
		}
		break;
	}




	bubble = _bubble;
}

Bubble BubbleView::getBubble() const{
	return bubble;
}


Vec2 BubbleView::getBubbleNodePosition(){
	return bubbleNode->getPosition();
}
