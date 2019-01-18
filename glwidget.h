#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <iostream>
#include <QApplication>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <plyreader.h>
#include <QTimer>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <glm/glm.hpp>
#include "trianglemesh.h"
#include "animation.h"
#include "watergrid.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
	GLWidget(QWidget *parent);
	~GLWidget();

	void loadMesh(const QString &filename);
	void setPolygonMode(bool bFill);

    void update_updating_mode(int mode);

    void resetAnimation();
    void define_animation();

    void animateWater();



public slots:
    void animate();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void setProjection(float aspect);
	void setModelview();

    void initializePlane(glm::vec3 rb, glm::vec3 rt, glm::vec3 lt, glm::vec3 lb);

    std::string basicPath = "/home/al/Documents/Un/animation/lab/3waterSimulation/";






    bool bPolygonFill;
	float angleX, angleY, distance;
	QPoint lastMousePos;

    QOpenGLShaderProgram *program, *programGeneral, *programWater;
    TriangleMesh mesh, waterCube;
    GLuint vao;
    std::vector<GLuint> planesVBO, trianglesVBO;

    animation an;

    waterGrid wt;

    //update mode euler-semi-verlet
    enum update_modes { euler, semi, verlet};
    update_modes selected_update_mode = euler;

    //dimension of the room
    float room_dim_param = 2.0f;
    float floor = 0.0f;
    float roof = 4.0f;
    float cube_dimension = 0.5f;

public:
    int selected_mode = 2;
    bool stop = false;




};

#endif // GLWIDGET_H
