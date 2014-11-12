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

	auto glProgram = GLProgramCache::getInstance()->getGLProgram("myShader");
	if (glProgram == nullptr)
	{
		glProgram = new GLProgram();
		glProgram->initWithFilenames("myShader.vert", "myShader.frag");
		glProgram->link();
		//set uniform locations
		glProgram->updateUniforms();
		GLProgramCache::getInstance()->addGLProgram(glProgram, "myShader");
	}
	this->setGLProgram(glProgram);

	//创建和绑定vao
	glGenVertexArrays(1, &vertexVAO);
	glBindVertexArray(vertexVAO);

	//创建和绑定vbo
	glGenBuffers(1, &vertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

	auto size = Director::getInstance()->getWinSize();
	float vertercies[] = {
		0, 0,
		size.width, 0,
		size.width / 2, size.height
	};
	float color[] = {
		0, 1, 0, 1,
		1, 0, 0, 1,
		0, 0, 1, 1
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
	//获取vertex attribute "a_position"的入口点
	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	//打开入a_position入口点
	glEnableVertexAttribArray(positionLocation);
	//传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	//set for color
	glGenBuffers(1, &colorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

	GLuint colorLocation = glGetAttribLocation(glProgram->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

    float uColor[] = {1.0, 0.0, 0.0, 1.0};
    GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	glUniform4fv(uColorLocation, 1, uColor);

	//for safty
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

	glBindVertexArray(vertexVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
	CHECK_GL_ERROR_DEBUG();
}
