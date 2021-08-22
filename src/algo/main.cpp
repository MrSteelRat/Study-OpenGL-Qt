#include <GL/glut.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

using std::chrono_literals::operator""ms;

constexpr int kWidth = 640;
constexpr int kHeight = 480;
constexpr int kPointSize = 10;

void Init() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glColor3d(13.0 / 255, 71.0 / 255, 161.0 / 255);
  glPointSize(kPointSize);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, kWidth, 0.0, kHeight);
}

void Draw(const std::vector<uint32_t>& vector) {
  glClear(GL_COLOR_BUFFER_BIT);
  for (int i = 0; i < vector.size(); ++i) {
    glColor3d(13.0 / vector[i], 71.0 / 255, 161.0 / vector[i]);
    glBegin(GL_POINTS);
    glVertex2i(i * kPointSize, vector[i]);
    glEnd();
  }
  glFlush();
}

void BubbleSort(std::vector<uint32_t>& vector) {
  for (int i = 0; i < vector.size(); ++i) {
    for (int j = i + 1; j < vector.size(); ++j) {
      if (vector[j] < vector[i]) {
        std::swap(vector[i], vector[j]);
        std::this_thread::sleep_for(30ms);
        Draw(vector);
      }
    }
  }
}

void SelectSort(std::vector<uint32_t>& vector) {
  for (int i = 0; i < vector.size() - 1; ++i) {
    int min = i;
    for (int j = i + 1; j < vector.size(); ++j) {
      if (vector[min] > vector[j]) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(vector[i], vector[min]);
      std::this_thread::sleep_for(30ms);
      Draw(vector);
    }
  }
}

void Display() {
  std::vector<uint32_t> vector;
  constexpr int size = kWidth / kPointSize;
  vector.reserve(size);

  for (int i = 0; i < size; ++i) {
    vector.push_back(i * kPointSize * kHeight / kWidth);
  }

  std::random_device random_device;
  std::default_random_engine random_engine(random_device());
  std::shuffle(vector.begin(), vector.end(), random_engine);

  Draw(vector);

  // BubbleSort(vector);
  SelectSort(vector);
  Draw(vector);
}

int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitWindowSize(kWidth, kHeight);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutCreateWindow("study");

  glutDisplayFunc(Display);

  Init();
  glutMainLoop();
}