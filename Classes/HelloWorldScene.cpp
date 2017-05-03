#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "network/WebSocket.h"

USING_NS_CC;

class WebSocketClientDelegate : public network::WebSocket::Delegate
{
	public:

		//virtual ~WebSocketClientDelegate(); // Not needed since this is a singleton for testing purposes.
		static WebSocketClientDelegate* getInstance()
		{
			static WebSocketClientDelegate instance;
			return &instance;
		}

		// Assuming the websocket connection is established, the client
		// delegate callbacks will be called as they are written here:
		virtual void onOpen(network::WebSocket* ws) override
		{
			CCLOG("onOpen");
		}

		virtual void onMessage(network::WebSocket* ws, const network::WebSocket::Data& data) override
		{
			CCLOG("onMessage");
		}

		virtual void onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error) override
		{
			CCLOG("onError");
		}

		virtual void onClose(network::WebSocket* ws) override
		{
			CCLOG("onClose");

			// Clear the memory
			CC_SAFE_DELETE(ws);
		}
};


Scene* HelloWorld::createScene()
{
	return HelloWorld::create();
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Scene::init() )
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
								origin.y + closeItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width/2,
							origin.y + visibleSize.height - label->getContentSize().height));

	// add the label as a child to this layer
	this->addChild(label, 1);

	// add "HelloWorld" splash screen"
	auto sprite = Sprite::create("HelloWorld.png");

	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

	// add the sprite as a child to this layer
	this->addChild(sprite, 0);

	// Start the websocket connection:
	auto websocket_test = new network::WebSocket();
	if(!websocket_test->init(*WebSocketClientDelegate::getInstance(), "ws://127.0.0.1:7681")) 
	{
		delete websocket_test;
		websocket_test = nullptr;
	}

	// If we want to close all the connections at once, this is how it's done:
	//Director::getInstance()->getScheduler()->performFunctionInCocosThread(
	//	[] ()
	//	{
	//		network::WebSocket::closeAllConnections();
	//	}
	//	);


	return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
	#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
