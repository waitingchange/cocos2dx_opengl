#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::visit(Renderer *renderer, const Mat4 &transform, bool transformUpdated)
{
	Layer::visit(renderer, transform, transformUpdated);

	_command.init(_globalZOrder);
	_command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
	Director::getInstance()->getRenderer()->addCommand(&_command);
}

void HelloWorld::onDraw()
{
	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins();

	auto size = Director::getInstance()->getWinSize();
	float vertervies[] = {
		0, 0,
		size.width, 0,
		size.width / 2, size.height
	};
	float color[] = {
		0, 1, 0, 1,
		1, 0, 0, 1,
		0, 0, 1, 1
	};

	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertervies);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);

	CHECK_GL_ERROR_DEBUG();
}
