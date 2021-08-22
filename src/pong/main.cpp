#include <GL/glut.h>

#include <iostream>
#include <string>

#include "GL/freeglut_ext.h"

constexpr double kPi = 3.14159265358979323846;

constexpr int kScreenWidth = 600;
constexpr int kScreenHeight = 400;
constexpr int kFrameRate = 60;

class Canvas {
 public:
  Canvas(int width, int height, const char* window_title);
  void SetWindow(float left, float right, float bottom, float top);
  // void SetViewport(int left, int right, int bottom, int top);
  void ClearScreen();
  void SetBackgroundColor(int red, int green, int blue);
  void SetColor(int red, int green, int blue);
  void DisplayUpdate();

  void DrawRect(int x1, int y1, int x2, int y2);
  void DrawLine(int x1, int y1, int x2, int y2);
  void DrawPolygon(int n, int cx, int cy, float radius, float rot_angle);
  void DrawCircle(int x, int y, float radius);
  void DrawString(int cx, int cy, const std::string& string);
};

Canvas::Canvas(int width, int height, const char* window_title) {
  int argc = 0;
  glutInit(&argc, nullptr);
  glutInitWindowSize(width, height);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow(window_title);
  SetWindow(0, width, 0, height);
  // SetViewport(0, width, 0, height);
}

void Canvas::SetWindow(float left, float right, float bottom, float top) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(left, right, bottom, top);
}

/*
void Canvas::SetViewport(int left, int right, int bottom, int top) {
  glViewport(left, bottom, right - left, top - bottom);
}
*/

void Canvas::ClearScreen() { glClear(GL_COLOR_BUFFER_BIT); }

void Canvas::SetBackgroundColor(int red, int green, int blue) {
  glClearColor(red / 255.0, green / 255.0, blue / 255.0, 0);
}

void Canvas::SetColor(int red, int green, int blue) {
  glColor3d(red / 255.0, green / 255.0, blue / 255.0);
}

void Canvas::DisplayUpdate() { glFlush(); }

void Canvas::DrawRect(int x1, int y1, int x2, int y2) {
  glRecti(x1, y1, x2, y2);
}

void Canvas::DrawLine(int x1, int y1, int x2, int y2) {
  glBegin(GL_LINES);
  glVertex2i(x1, y1);
  glVertex2i(x2, y2);
  glEnd();
}

void Canvas::DrawPolygon(int n, int cx, int cy, float radius, float rot_angle) {
  if (n < 3) return;

  double angle = rot_angle * kPi / 180;
  double angle_inc = 2 * kPi / n;

  glBegin(GL_POLYGON);
  glVertex2f(radius * cos(angle) + cx, radius * sin(angle) + cy);

  for (int i = 0; i < n; ++i) {
    angle += angle_inc;
    glVertex2f(radius * cos(angle) + cx, radius * sin(angle) + cy);
  }

  glEnd();
}

void Canvas::DrawCircle(int cx, int cy, float radius) {
  // int num_verts = 50;
  DrawPolygon(50, cx, cy, radius, 0);
}

void Canvas::DrawString(int cx, int cy, const std::string& string) {
  void* font = GLUT_BITMAP_HELVETICA_18;
  int width = glutBitmapLength(font, (unsigned char*)string.c_str());
  int height = glutBitmapHeight(font);

  glRasterPos2i(cx - width / 2, cy - height / 2);
  glutBitmapString(font, (unsigned char*)string.c_str());
}

Canvas canvas(kScreenWidth, kScreenHeight, "tennis");

int ball_x = kScreenWidth / 2;
int ball_y = kScreenHeight / 2;
int ball_xv = 3;
int ball_yv = 3;
int ball_r = 20;

int paddle1_x = 10;
int paddle1_y = 10;
int paddle1_w = 25;
int paddle1_h = 100;

int paddle2_x = kScreenWidth - 35;
int paddle2_y = 10;
int paddle2_w = 25;
int paddle2_h = 100;

int player1_score = 0;
int player2_score = 0;

bool key_w = false;
bool key_s = false;

bool key_up = false;
bool key_down = false;

void DisplayCallback() {
  canvas.ClearScreen();
  canvas.SetColor(255, 255, 255);
  canvas.DrawRect(paddle1_x, paddle1_y, paddle1_x + paddle1_w,
    paddle1_y + paddle1_h);
  canvas.DrawRect(paddle2_x, paddle2_y, paddle2_x + paddle2_w,
    paddle2_y + paddle2_h);
  canvas.SetColor(238, 96, 2);
  canvas.DrawLine(kScreenWidth / 2, 0, kScreenWidth / 2, kScreenHeight);
  canvas.SetColor(238, 2, 144);
  canvas.DrawCircle(ball_x, ball_y, ball_r);
  canvas.SetColor(255, 255, 255);
  canvas.DrawString(kScreenWidth / 4, kScreenHeight - 10,
    std::to_string(player1_score));
  canvas.DrawString(kScreenWidth / 4 * 3, kScreenHeight - 10,
    std::to_string(player2_score));

  canvas.DisplayUpdate();
}

void foo() {
  if (key_w) {
    paddle1_y += 5;
  }

  if (key_s) {
    paddle1_y -= 5;
  }

  if (key_up) {
    paddle2_y += 5;
  }

  if (key_down) {
    paddle2_y -= 5;
  }

  ball_x += ball_xv;
  ball_y += ball_yv;

  if (ball_y - ball_r <= 0 || ball_y + ball_r >= kScreenHeight) {
    ball_yv *= -1;
  }

  if (paddle1_y < 0) {
    paddle1_y = 0;
  } else if (paddle1_y + paddle1_h > kScreenHeight) {
    paddle1_y = kScreenHeight - paddle1_h;
  }

  if (paddle2_y < 0) {
    paddle2_y = 0;
  } else if (paddle2_y + paddle2_h > kScreenHeight) {
    paddle2_y = kScreenHeight - paddle2_h;
  }

  if (ball_x - ball_r <= paddle1_x + paddle1_w && ball_y >= paddle1_y &&
    ball_y <= paddle1_y + paddle1_h) {
    ball_xv *= -1;
  }

  if (ball_x + ball_r >= paddle2_x && ball_y >= paddle2_y &&
    ball_y <= paddle2_y + paddle2_h) {
    ball_xv *= -1;
  }

  if (ball_x <= 0) {
    player2_score += 1;
    ball_x = kScreenWidth / 2;
    ball_y = kScreenHeight / 2;
    // ball_xv = 3;  //
    // ball_yv = 3;
  } else if (ball_x >= kScreenWidth) {
    player1_score += 1;
    ball_x = kScreenWidth / 2;
    ball_y = kScreenHeight / 2;
    // ball_xv = 3;
    // ball_yv = 3;
  }
}

void TimerCallback(int value) {
  glutTimerFunc(1000 / kFrameRate, TimerCallback, 0);
  foo();
  glutPostRedisplay();
}

void KeyboardCallback(unsigned char key, int x, int y) {
  switch (key) {
  case 'w':
  case 'W': {
    key_w = true;
    break;
  }
  case 's':
  case 'S': {
    key_s = true;
    break;
  }
  case 27: {
    glutLeaveMainLoop();
    break;
  }
  default: {
    std::clog << "unsupported key: " << key << std::endl;
  }
  }
}

void KeyboardUpCallback(unsigned char key, int x, int y) {
  switch (key) {
  case 'w':
  case 'W': {
    key_w = false;
    break;
  }
  case 's':
  case 'S': {
    key_s = false;
    break;
  }
  }
}

void SpecialCallback(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP: {
    key_up = true;
    break;
  }
  case GLUT_KEY_DOWN: {
    key_down = true;
    break;
  }
  default: {
    std::clog << "unsupported key: " << key << std::endl;
  }
  }
}

void SpecialUpCallback(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_UP: {
    key_up = false;
    break;
  }
  case GLUT_KEY_DOWN: {
    key_down = false;
    break;
  }
  }
}

int main(int argc, char* argv[]) {
  canvas.SetBackgroundColor(0, 0, 0);
  glutDisplayFunc(DisplayCallback);
  glutTimerFunc(30, TimerCallback, 0);
  glutKeyboardFunc(KeyboardCallback);
  glutKeyboardUpFunc(KeyboardUpCallback);
  glutSpecialFunc(SpecialCallback);
  glutSpecialUpFunc(SpecialUpCallback);
  glutMainLoop();
}