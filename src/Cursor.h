#pragma once

#include <ofMain.h>

class Cursor
{
public:
  Cursor();
  enum State
  {
    normal,
    drag,
    bucket,
    touch,
    cross
  };

  void draw() const;
  ofRectangle getRectangle();

  int state = normal;

  int x = -1;
  int y = -1;
  int x2d = -1;
  int y2d = -1;

  struct Cursors
  {
    ofImage cursor;
    ofImage fist;
    ofImage bucket;
    ofImage hand;
    ofImage cross;
  } cursors;
};
