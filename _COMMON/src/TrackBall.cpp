///////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file
//
//  \filename   TrackBall.cpp
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
//  \endverbatim
*/
///////////////////////////////////////////////////////////////////////////////////////////////////



// system includes ////////////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <iostream> // only for debugging output


// OpenGL helper includes /////////////////////////////////////////////////////////////////////////
#include <FL/glut.H>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// application includes ///////////////////////////////////////////////////////////////////////////
#include "../inc/TrackBall.h"


// init static class members //////////////////////////////////////////////////////////////////////
void (*TrackBall::_AppDoubleClickFunctionCB) (int, int) = 0;
void (*TrackBall::_AppMouseButtonFunctionCB) (int, int, int, int) = 0; 
void (*TrackBall::_AppMouseMotionFunctionCB) (int, int, int, int) = 0; 

float TrackBall::_Translation[] = {0.0f, 0.0f, 0.0f};
float TrackBall::_Offset[] = {0.0f, 0.0f, 0.0f};
float TrackBall::_Rotation[] = {0.0f, 0.0f, 1.0f, 0.0f}; // rotation axis may not be zero!!!!
float TrackBall::_Scale[] = {1.0f, 1.0f, 1.0f};
glm::mat4 TrackBall::_TrackBallMatrix = glm::mat4(1.0f);

int   TrackBall::_OldMouseX = 0;
int   TrackBall::_OldMouseY = 0;
bool  TrackBall::_MouseButtonPressed = false;



void TrackBall::registerDoubleClick(void(*func)(int x, int y))
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (func != NULL) _AppDoubleClickFunctionCB = func;
}
// TrackBall::registerDoubleClick() ///////////////////////////////////////////////////////////////


void TrackBall::registerMouseButton(void (*func)(int, int, int, int))
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (func != NULL) _AppMouseButtonFunctionCB = func;
}
// TrackBall::registerMouseButton() //////////////////////////////////////////////////////////////


void TrackBall::registerMouseMotion(void (*func)(int, int, int, int))
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (func != NULL) _AppMouseMotionFunctionCB = func;
}
// TrackBall::registerMouseMotion() //////////////////////////////////////////////////////////////



void TrackBall::axisAmountToMat(float aa[], float mat[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	float c, s, t;

	c = (float)cos(aa[3]);
	s = (float)sin(aa[3]);
	t = 1.0f - c;

	mat[0] = t * aa[0] * aa[0] + c;
	mat[1] = t * aa[0] * aa[1] + s * aa[2];
	mat[2] = t * aa[0] * aa[2] - s * aa[1];
	mat[3] = t * aa[0] * aa[1] - s * aa[2];
	mat[4] = t * aa[1] * aa[1] + c;
	mat[5] = t * aa[1] * aa[2] + s * aa[0];
	mat[6] = t * aa[0] * aa[2] + s * aa[1];
	mat[7] = t * aa[1] * aa[2] - s * aa[0];
	mat[8] = t * aa[2] * aa[2] + c;
}
// TrackBall::axisAmountToMat() ///////////////////////////////////////////////////////////////////



void TrackBall::matToAxisAmount(float mat[], float aa[])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	float c;
	float s;

	c = (mat[0] + mat[4] + mat[8] - 1.0f) / 2.0f;
	aa[3] = (float)acos(c);
	s = (float)sin(aa[3]);
	if(fabs(s / glm::pi<float>() - (int)(s / glm::pi<float>())) < .0000001) {
		aa[0] = 0.0f;
		aa[1] = 1.0f;
		aa[2] = 0.0f;
	} else {
		aa[0] = (mat[5] - mat[7]) / (2.0f * s);
		aa[1] = (mat[6] - mat[2]) / (2.0f * s);
		aa[2] = (mat[1] - mat[3]) / (2.0f * s);
	}
}
// TrackBall::matToAxisAmount() ///////////////////////////////////////////////////////////////////



void TrackBall::rotateTrackball(int dx, int dy, float rotation[4])
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	double dist;
	float oldMat[9];
	float rotMat[9];
	float newRot[4];

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport); // GLint x, GLint y, GLsizei width, GLsizei height

	dist = sqrt((double)(dx * dx + dy * dy));
	if(fabs(dist) < 0.99) return;

	newRot[0] = float(dy / dist);
	newRot[1] = float(dx / dist);
	newRot[2] = 0.0f;
	newRot[3] = float(glm::pi<float>() * dist / viewport[2]); // window width

	axisAmountToMat(rotation, oldMat);
	axisAmountToMat(newRot, rotMat);
	glm::mat3 mult = glm::make_mat3(oldMat) * glm::make_mat3(rotMat);
	matToAxisAmount(glm::value_ptr(mult), rotation);

	dist = sqrt(rotation[0] * rotation[0] + rotation[1] * rotation[1] + rotation[2] * rotation[2]);

	rotation[0] /= float(dist);
	rotation[1] /= float(dist);
	rotation[2] /= float(dist);
}
// TrackBall::rotateTrackball() ///////////////////////////////////////////////////////////////////



TrackBall::TrackballModeT TrackBall::evaluateTrackballMode(TrackBall::TrackballModeT new_mode)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	static TrackballModeT mode = TM_ROTATE;

	if (new_mode != TM_INVALID)
	{
		// set trackball mode explicitly (for mouse buttons)
		mode = new_mode;
		return new_mode;
	}

	// get control keys through GLUT functions (init with GLUT inactive state)
	int key_state = 0;
	static int key_modifier_prev = 0;
	int key_modifier = glutGetModifiers();

	if (key_modifier_prev != key_modifier)
	{
		// std::cout << "DEBUG: Glut modifier key: " << key_modifier << std::endl;

		if (key_modifier > 0)
		{
			// new modifier key pressed, return it
			key_modifier_prev = key_modifier;;
			key_state = GLUT_DOWN;
		}
		else
		{
			// modifier key released, return previous modifier
			key_state = GLUT_UP;
			std::swap(key_modifier_prev, key_modifier);
		}
	}

	switch(key_modifier)
	{
		case GLUT_ACTIVE_SHIFT:
		{
			if (key_state == GLUT_DOWN)
			{
				// std::cout << "DEBUG: SHIFT DOWN\n";
				mode = TM_TRANSLATEZ;
			}
			else
			{
				// std::cout << "DEBUG: SHIFT UP\n";
				if (mode == TM_TRANSLATEZ) mode = TM_ROTATE;
			}
			break;
		}
		case GLUT_ACTIVE_CTRL:
		{
			if (key_state == GLUT_DOWN)
			{
				// std::cout << "DEBUG: CTRL DOWN\n";
				mode = TM_TRANSLATEXY;
			} else {
				// std::cout << "DEBUG: CTRL UP\n";
				if (mode == TM_TRANSLATEXY) mode = TM_ROTATE;
			}
			break;
		}
		case GLUT_ACTIVE_ALT:
		{
			if (key_state == GLUT_DOWN)
			{
				// std::cout << "DEBUG: ALT DOWN\n";
				mode = TM_SCALE;
			}
			else
			{
				// std::cout << "DEBUG: ALT UP\n";
				if (mode == TM_SCALE) mode = TM_ROTATE;
			}
			break;
		}
		default:
		{
			// std::cout << "ERROR: unknown modifier key pressed\n";
			break;
		}
	} // end switch

	return mode;
}
// TrackBall::evaluateTrackballMode() /////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: applyTransformation()
// purpose:  Use this function for OpenGL Legacy or Compatibility Profile applications to apply
//           the trackball transformation directly onto the OpenGL matrix stack.
///////////////////////////////////////////////////////////////////////////////////////////////////
void TrackBall::applyTransformation(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	glm::mat4 trackball = getTransformation();
	glMultMatrixf(glm::value_ptr(trackball));
}
// TrackBall::applyTransformation() ///////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
// function: getTransformation()
// purpose:  Use this function for OpenGL Core Profile applications to return the GLM trackball 
//           transformation matrix to be used in your transformation calculation.
///////////////////////////////////////////////////////////////////////////////////////////////////
glm::mat4& TrackBall::getTransformation(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	_TrackBallMatrix = glm::mat4(1.0);
	_TrackBallMatrix = glm::translate(_TrackBallMatrix, glm::vec3(_Offset[0], _Offset[1], _Offset[2]));
	_TrackBallMatrix = glm::translate(_TrackBallMatrix, glm::vec3(_Translation[0], _Translation[1], _Translation[2]));
	_TrackBallMatrix = glm::rotate(_TrackBallMatrix, float(_Rotation[3]), glm::vec3(_Rotation[0], _Rotation[1], _Rotation[2]));
	_TrackBallMatrix = glm::scale(_TrackBallMatrix, glm::vec3(_Scale[0], _Scale[1], _Scale[2]));
	_TrackBallMatrix = glm::translate(_TrackBallMatrix, glm::vec3(-_Offset[0], -_Offset[1], -_Offset[2]));

	return _TrackBallMatrix;
}
// TrackBall::getTransformation() /////////////////////////////////////////////////////////////////



void TrackBall::resetTransformation(void)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// std::cout << "DEBUG: reseting trackball transformation\n";
	_Translation[0] = _Translation[1] = _Translation[2] = 0.0f;
	//_Offset[0] = _Offset[1] = _Offset[2] = 0.0f;
	_Rotation[0] = _Rotation[1] = _Rotation[2] = _Rotation[3] = 0.0f;
	_Rotation[2] = 1.0f; // rotation axis may not be zero!!!!

	_Scale[0] = _Scale[1] = _Scale[2] = 1.0f;

	_OldMouseX = _OldMouseY = 0;
}
// TrackBall::resetTransformation() ///////////////////////////////////////////////////////////////



void TrackBall::glutMouseMotionCB(int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// std::cout << "DEBUG: TrackBall::glutMouseMotionCB(" << x << "," << y << ")" << std::endl;

	int dx, dy;

	// check if we have pressed a mouse button during mouse motion
	if (_MouseButtonPressed)
	{
		// use mouse motion for trackball update
		dx = x - _OldMouseX;
		dy = y - _OldMouseY;

		if (_AppMouseMotionFunctionCB != NULL)
		{
			_AppMouseMotionFunctionCB(_OldMouseX, _OldMouseY, x, y);
		}

		_OldMouseX = x;
		_OldMouseY = y;
		}
	else
	{
		// ignore mouse motion, while in pop-up menu selection mode
		// std::cout << "DEBUG: reset glutMouseMotionCB() " << std::endl;
		dx = 0;
		dy = 0;
	}

	switch( evaluateTrackballMode() )
	{
		case TM_ROTATE:
		{
			rotateTrackball(dx, dy, _Rotation);
			break;
		}
		case TM_SCALE:
		{
			float scale = dy / 40.0f;
			_Scale[0] = _Scale[1] = _Scale[2] += scale;

			if(_Scale[0] < 0.025)
			{
				_Scale[0] = _Scale[1] = _Scale[2] = 0.025f;
			}
			else if(_Scale[0] > 5.0)
			{
				_Scale[0] = _Scale[1] = _Scale[2] = 5.0f;
			}
			break;
		}
		case TM_TRANSLATEXY:
		{
			_Translation[0] += dx / 100.0f;
			_Translation[1] -= dy / 100.0f;
			break;
		}
		case TM_TRANSLATEZ:
		{
			_Translation[2] += dy / 40.0f;
			break;
		}
	}

	glutPostRedisplay();
}
// TrackBall::glutMouseMotionCB() /////////////////////////////////////////////////////////////////



void TrackBall::glutMouseButtonCB(int button, int state, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	// std::cout << "DEBUG: TrackBall::glutMouseButtonCB(" << x << "," << y << ")" << std::endl;

	static clock_t dblclick_start = 0;
	static clock_t dblclick_finish = 0;
	const clock_t  dblclick_diff = 400; // double click time difference [ms]

	_OldMouseX = x;
	_OldMouseY = y;

	if (state == GLUT_UP)
	{
		// std::cout << "DEBUG: Mouse Button UP (GLUT_UP): " << button  << std::endl;
		_MouseButtonPressed = false;
		if (button == GLUT_MIDDLE_BUTTON) evaluateTrackballMode(TM_ROTATE);
	}
	else
	{
		// std::cout << "DEBUG: Mouse Button DOWN (GLUT_DOWN): " << button  << std::endl;

		if ((button==GLUT_LEFT_BUTTON) | (button==GLUT_MIDDLE_BUTTON) | (button==GLUT_RIGHT_BUTTON))
		{
			_MouseButtonPressed = true;

			if (_AppMouseButtonFunctionCB != NULL)
			{
				_AppMouseButtonFunctionCB(x, y, x, y);
			}

			if (button == GLUT_MIDDLE_BUTTON) evaluateTrackballMode(TM_TRANSLATEZ);

			// detect mouse double click (ignore mouse button)
			dblclick_finish = std::clock();

			// std::cout << "DEBUG: CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << std::endl;
			// std::cout << "DEBUG: Time difference: " << (dblclick_finish - dblclick_start) << " [ms]\n";

			if ( ((dblclick_finish - dblclick_start) < dblclick_diff) && (_AppDoubleClickFunctionCB != NULL) )
			{
				_AppDoubleClickFunctionCB(x, y);
			}
			dblclick_start = dblclick_finish;
		}
		else // the mouse wheel was turned (FLTK)
		{
			// std::cout << "DEBUG: FLTK Mouse Wheel Event (FL_MOUSEWHEEL): " << button << std::endl;

			// this is sort of a hack, since no appropriate FLTK constants wher found
			if (button == GLUT_RIGHT_BUTTON + 1) 
			{
				// FL_MOUSEWHEEL_UP
				_Scale[0] = _Scale[1] = _Scale[2] += 0.05f;
				if (_Scale[0] > 5.0) _Scale[0] = _Scale[1] = _Scale[2] = 5.0f;
			}
			else
			{
				// FL_MOUSEWHEEL_DOWN
				_Scale[0] = _Scale[1] = _Scale[2] -= 0.05f;
				if (_Scale[0] < 0.025) _Scale[0] = _Scale[1] = _Scale[2] = 0.025f;
			}
		}
	}

	glutPostRedisplay();
}
// TrackBall::glutMouseButtonCB()//////////////////////////////////////////////////////////////////



void TrackBall::glutSpecialFuncCB(int key, int x, int y)
///////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (key == GLUT_KEY_HOME)
	{
		resetTransformation();
	}
	else
	{
		switch( evaluateTrackballMode() )
		{
			case TM_ROTATE:
			{
				switch(key)
				{
					case GLUT_KEY_UP:    rotateTrackball(0, -10, _Rotation); break;
					case GLUT_KEY_DOWN:  rotateTrackball(0,  10, _Rotation); break;
					case GLUT_KEY_LEFT:  rotateTrackball(-10, 0, _Rotation); break;
					case GLUT_KEY_RIGHT: rotateTrackball( 10, 0, _Rotation); break;
					default: return;
				}
				break;
			}
			case TM_TRANSLATEXY:
			{
				switch(key)
				{
				case GLUT_KEY_UP:    _Translation[1] += 0.1f; break;
				case GLUT_KEY_DOWN:  _Translation[1] -= 0.1f; break;
				case GLUT_KEY_LEFT:  _Translation[0] -= 0.1f; break;
				case GLUT_KEY_RIGHT: _Translation[0] += 0.1f; break;
				default: return;
				}
				break;
			}
			case TM_TRANSLATEZ:
			{
				switch(key)
				{
				case GLUT_KEY_UP:    _Translation[2] -= 0.1f; break;
				case GLUT_KEY_DOWN:  _Translation[2] += 0.1f; break;
				//case GLUT_KEY_LEFT:  _Translation[2] -= 0.1f; break;
				//case GLUT_KEY_RIGHT: _Translation[2] += 0.1f; break;
				default: return;
				}
				break;
			}
			case TM_SCALE:
			{
				switch(key)
				{
					case GLUT_KEY_UP:
					//case GLUT_KEY_LEFT:
					{
						_Scale[0] = _Scale[1] = _Scale[2] += 0.05f;
						if (_Scale[0] > 5.0) _Scale[0] = _Scale[1] = _Scale[2] = 5.0f;
						break;
					}
					case GLUT_KEY_DOWN:
					//case GLUT_KEY_RIGHT:
					{
						_Scale[0] = _Scale[1] = _Scale[2] -= 0.05f;
						if (_Scale[0] < 0.025) _Scale[0] = _Scale[1] = _Scale[2] = 0.025f;
						break;
					}
					default: return;
				}
				break;
			}
			default: return;
		} // end switch
	} // end if

	glutPostRedisplay();
}
// TrackBall::glutSpecialFuncCB() /////////////////////////////////////////////////////////////////
