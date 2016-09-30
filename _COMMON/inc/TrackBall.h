///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   TrackBall.h
//
//  \brief      Virtual trackball helper class for easy mouse and keyboard based 3D interaction.
//              Based on an example from the book "Advanced Graphics Programming Using OpenGL,
//              Morgan Kaufmann, 2005)
//
//   Usage:     Brief usage instructions for modifiers keys (SHIFT, CTRL, ALT):
//              No Key: mouse click and drag --> rotate model (default)
//              CTRL:   mouse click and drag --> translate model in +/- X/Y direction
//              SHIFT:  mouse click and drag --> translate model in +/- Z direction
//              ALT:    mouse click and drag --> scale model up/down
//              All modifier keys can also be used with the cursor keys and work identical.
//
//              The HOME key resets the TrackBall transformation and puts the model into
//              the initial position/orientation.
//
//              Where available the MOUSEWHEEL can be used to scale the model up or down.
//
//  \history
//     yyyy-mm-dd   Version   Author   Comment
//     2015-09-10   1.00      klu      Initial file release
//     2016-03-16   1.10      klu      Update for CPP course apps
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////



// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <glm/glm.hpp>



class TrackBall
///////////////////////////////////////////////////////////////////////////////////////////////////
{
private:
	enum TrackballModeT { TM_ROTATE, TM_TRANSLATEXY, TM_TRANSLATEZ, TM_SCALE, TM_INVALID };

	static void axisAmountToMat(float aa[], float mat[]);
	static void matToAxisAmount(float mat[], float aa[]);
	static void rotateTrackball(int dx, int dy, float rotation[4]);
	static TrackballModeT evaluateTrackballMode(TrackballModeT new_mode = TM_INVALID);

private:
	// callback function pointers
	static void (*_AppDoubleClickFunctionCB) (int x, int y);
	static void (*_AppMouseButtonFunctionCB) (int x1, int y1, int x2, int y2); 
	static void (*_AppMouseMotionFunctionCB) (int x1, int y1, int x2, int y2);

	static float _Translation[3];
	static float _Offset[3];
	static float _Rotation[4];
	static float _Scale[3];
	static glm::mat4 _TrackBallMatrix;

	static int	 _OldMouseX;
	static int	 _OldMouseY;
	static bool	 _MouseButtonPressed;

public:
	// glut callback functions
	static void glutMouseMotionCB(int x, int y);
	static void glutMouseButtonCB(int button, int state, int x, int y);
	static void glutSpecialFuncCB(int key, int x, int y);

	static void registerDoubleClick(void (*func)(int x, int y) = 0);
	static void registerMouseButton(void (*func)(int x1, int y1, int x2, int y2) = 0);
	static void registerMouseMotion(void (*func)(int x1, int y1, int x2, int y2) = 0);

	// use and reset trackball transformation
	static void applyTransformation(void);
	static glm::mat4& getTransformation(void);
	static void resetTransformation(void);

	// if required, set model origin offset
	static void setOffset(const float offset[3]) {for (int i=0; i<3; i++) _Offset[i] = offset[i];};
	static void getOffset(float offset[3]) { offset = _Offset; };
};
// class TrackBall ////////////////////////////////////////////////////////////////////////////////

