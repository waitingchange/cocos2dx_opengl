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

	typedef struct {
		float Position[2];
		float Color[4];
	} Vertex;
	Vertex data[] = {
		{ { -1, -1 }, { 1, 0, 0, 1 } },
		{ { 1, -1 }, { 0, 1, 0, 1 } },
		{ { -1, 1 }, { 0, 0, 1, 1 } },
		{ { 1, 1 }, { 1, 0, 0, 1 } },
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	//获取vertex attribute "a_position"的入口点
	//打开入a_position入口点
	//传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
	GLuint positionLocation = glGetAttribLocation(glProgram->getProgram(), "a_position");
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));

	GLuint colorLocation = glGetAttribLocation(glProgram->getProgram(), "a_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));

    float uColor[] = {1.0, 1.0, 1.0, 1.0};
    GLuint uColorLocation = glGetUniformLocation(glProgram->getProgram(), "u_color");
	glUniform4fv(uColorLocation, 1, uColor);

	GLbyte indices[] = {
		0, 1, 2,
		2, 3, 1
	};
	glGenBuffers(1, &indexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    Director::getInstance()->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);

	auto glProgram = getGLProgram();
	glProgram->use();
	glProgram->setUniformsForBuiltins();

	glBindVertexArray(vertexVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
	glBindVertexArray(0);

	CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
	CHECK_GL_ERROR_DEBUG();

    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    Director::getInstance()->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}
