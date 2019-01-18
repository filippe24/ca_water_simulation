#include "glwidget.h"


using namespace std;


const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 50.0f;



GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), bPolygonFill(true), angleX(45.0f), angleY(45.0f), distance(20.0f)
{

    std::cout<< "0.glWidget"<<std::endl;

    program = nullptr;
    programGeneral = nullptr;
    programWater = nullptr;
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    setFormat(format);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(33);

}

GLWidget::~GLWidget()
{
	if(program) delete program;
}


void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();

    std::cout<< " 1. initialize GL" <<std::endl;



    if(true)
    {

        //FUNTIONS DEFINTIONS


        //********************************
        //*********** WATER * CUBE *******
        //********************************
        programWater = new QOpenGLShaderProgram();
        //QString::fromStdString(basicPath) +"/shaders/
        programWater->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(basicPath) +"/shaders/water.vert");
        programWater->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(basicPath) +"/shaders/water.frag");
        programWater->link();
        if(!programWater->isLinked())
        {
                cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << programWater->log().toStdString();
                QApplication::quit();
        }
        programWater->bind();

        waterCube.buildCube();
        if(!waterCube.init(programWater))
        {
                cout << "Could not create vbo" << endl;
                QApplication::quit();
        }
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        programWater->release();




        //*****************************
        //********** GENERAL **********
        //*****************************
        programGeneral = new QOpenGLShaderProgram();
        programGeneral->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
        programGeneral->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");

        programGeneral->bindAttributeLocation("position",0);

        programGeneral->link();
        if(!programGeneral->isLinked())
        {
                cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << programGeneral->log().toStdString();
                QApplication::quit();
        }
        programGeneral->bind();

        std::cout << " room dimension :" << room_dim_param << std::endl;
        float d = room_dim_param +0.01f;
        float f = floor;
        float r = roof;

        //floor
        initializePlane(glm::vec3(d, f, -d),glm::vec3(d, f, d),glm::vec3(-d, f, d),glm::vec3(-d, f, -d));
        //right
        initializePlane(glm::vec3(d, f, -d),glm::vec3(d, r, -d),glm::vec3(d, r, d),glm::vec3(d, f, d));
        //front
        initializePlane(glm::vec3(d, f, d),glm::vec3(-d, f, d),glm::vec3(-d, r, d),glm::vec3(d, r, d));
        //left
        initializePlane(glm::vec3(-d, f, -d),glm::vec3(-d, r, -d),glm::vec3(-d, r, d),glm::vec3(-d, f, d));
        //back
        initializePlane(glm::vec3(-d, f, -d),glm::vec3(d, f, -d),glm::vec3(d, r, -d),glm::vec3(-d, r, -d));

        //RELEASE
        programGeneral->release();
        glClearColor(0.8f, 0.8f, 0.8f, 0.8f);
        glEnable(GL_DEPTH_TEST);
        glUseProgram(0);

        //ANIMATE
        define_animation();

    }
    else
    {
        program = new QOpenGLShaderProgram();
        program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
        program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
        program->link();
        if(!program->isLinked())
        {
                cout << "Shader program has not linked" << endl << endl << "Log: " << endl << endl << program->log().toStdString();
                QApplication::quit();
        }
        program->bind();

        mesh.buildCube();
        if(!mesh.init(program))
        {
                cout << "Could not create vbo" << endl;
                QApplication::quit();
        }

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);

    }


}

void GLWidget::resizeGL(int w, int h)
{
    std::cout<< " resize GL" <<std::endl;


	glViewport(0,0,w,h);
    setProjection(float(w/h));
	setModelview();
}

void GLWidget::paintGL()
{


    if(true)
    {

        typedef void (APIENTRY *_glGenVertexArrays) (GLsizei, GLuint*);
        typedef void (APIENTRY *_glBindVertexArray) (GLuint);
        _glGenVertexArrays glGenVertexArrays;
         _glBindVertexArray glBindVertexArray;
        glGenVertexArrays = _glGenVertexArrays(QOpenGLWidget::context()->getProcAddress("glGenVertexArrays"));
        glBindVertexArray = _glBindVertexArray(QOpenGLWidget::context()->getProcAddress("glBindVertexArray"));


        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //********************************
        //*********** WATER * CUBE *******
        //********************************

        programWater->bind();
//        programWater->setUniformValue("bLighting", bPolygonFill);
        programWater->setUniformValue("color", QVector4D(0.1f, 0.3f, 0.6f, 0.3f));

        for(uint cub = 0; cub < wt.water.size(); cub ++)
        {

            float u = 0.5f + wt.water[cub].heigth;
            QMatrix4x4 scaling;
            scaling.scale(1.0f,u,1.0f);
            glm::vec3 translation = wt.getCubePosition(cub);

            programWater->setUniformValue("disposition", QVector3D(translation.x, (cube_dimension/2.0f)*u, translation.z));
            programWater->setUniformValue("scale", scaling);
            waterCube.render(*this);

        }
        programWater->release();



        //*****************************
        //********** GENERAL **********
        //*****************************
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        programGeneral->bind();
        programGeneral->setUniformValue("bLighting", bPolygonFill);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.5f, 1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_POLYGON_OFFSET_FILL);
        programGeneral->setUniformValue("color", QVector4D(0.05f, 0.05f, 0.15f, 1.0f));

        //print planes
        for(uint i = 0; i < planesVBO.size(); i++)
        {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER,planesVBO[i]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }

        //set back to fill
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bPolygonFill = false;


        //unbind
        glBindBuffer(GL_ARRAY_BUFFER,0);
        programGeneral->release();
        glBindVertexArray(0);



    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program->bind();
        program->setUniformValue("bLighting", bPolygonFill);

        if(bPolygonFill)
            program->setUniformValue("color", QVector4D(0.75f, 0.8f, 0.9f, 1.0f));
        else
        {
            program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0.5f, 1.0f);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //cube.render(*this);
            mesh.render(*this);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDisable(GL_POLYGON_OFFSET_FILL);
            program->setUniformValue("color", QVector4D(0.05f, 0.05f, 0.15f, 1.0));
        }


        //cube.render(*this);
        mesh.render(*this);



        program->release();

    }
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastMousePos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	// Rotation
	if(event->buttons() & Qt::LeftButton)
	{
		angleX += rotationFactor * (event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor * (event->x() - lastMousePos.x());
	}
	// Zoom
	if(event->buttons() & Qt::RightButton)
	{
        distance += 0.1f * (event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

    lastMousePos = event->pos();

	makeCurrent();
	setModelview();
	doneCurrent();
	update();
}

void GLWidget::setProjection(float aspect)
{
    std::cout<< "       --set projection" <<std::endl;

	QMatrix4x4 projectionMatrix;

    projectionMatrix.perspective(60, aspect, 0.01f, 100.0);
//    program->bind();
//    program->setUniformValue("projection", projectionMatrix);
//    program->release();

    programGeneral->bind();
    programGeneral->setUniformValue("projection", projectionMatrix);
    programGeneral->release();

    programWater->bind();
    programWater->setUniformValue("projection", projectionMatrix);
    programWater->release();

//    glUseProgram(activeId);
//    glUniformMatrix4fv(glGetUniformLocation(activeId, "projection"), 1, GL_FALSE, projectionMatrix.data());
//    glUseProgram(0);

}

void GLWidget::setModelview()
{
	QMatrix4x4 modelviewMatrix;

	modelviewMatrix.translate(0, 0, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
//    program->bind();
//    program->setUniformValue("modelview", modelviewMatrix);
//    program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
//    program->release();

    programGeneral->bind();
    programGeneral->setUniformValue("modelview", modelviewMatrix);
    programGeneral->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
    programGeneral->release();

    programWater->bind();
    programWater->setUniformValue("modelview", modelviewMatrix);
    programWater->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
    programWater->release();

//    glUseProgram(activeId);
//    glUniformMatrix4fv(glGetUniformLocation(activeId, "modelview"), 1, GL_FALSE, modelviewMatrix.data());
//    glUniformMatrix4fv(glGetUniformLocation(activeId, "normalMatrix"), 1, GL_FALSE, modelviewMatrix.data());
//    glUseProgram(0);

}

void GLWidget::setPolygonMode(bool bFill)
{

    std::cout<< "       --set polygonal mode" <<std::endl;

    bPolygonFill = bFill;

    makeCurrent();
    if(bFill)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    doneCurrent();
	update();
}

void GLWidget::loadMesh(const QString &filename)
{
	PLYReader reader;

	mesh.destroy();
	reader.readMesh(filename, mesh);
	makeCurrent();
    if(!mesh.init(programGeneral))
	{
			cout << "Could not create vbo" << endl;
			QApplication::quit();
	}
	doneCurrent();
	update();
}


void GLWidget::animate()
{
    //function called every frame
    if(!stop)
        wt.updateWater();

    update();
}


void GLWidget::animateWater()
{
    //function called every frame
    wt.updateWater();

    update();
}


void GLWidget::initializePlane( glm::vec3 rb, glm::vec3 rt, glm::vec3 lt, glm::vec3 lb)
{

    GLfloat planeVertices[] =
    {
         // Positions
        rb.x,  rb.y, rb.z,
        rt.x,  rt.y, rt.z,
        lt.x,  lt.y, lt.z,
        lb.x,  lb.y, lb.z,
    };
    GLuint tempVBO;
    //position
    glGenBuffers(1, &tempVBO);
    //Generate, bind and fill VBO for vertices
    glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    planesVBO.push_back(tempVBO);


}


void GLWidget::resetAnimation()
{
    define_animation();
}


void GLWidget::define_animation()
{
    //initialize animation
    wt = waterGrid(room_dim_param, cube_dimension, selected_mode);

}
