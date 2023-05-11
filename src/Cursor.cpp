#include "Cursor.h"

Cursor::Cursor()
{
  cursors.cursor.load("cursors/pointer.png");
  cursors.cursor.resize(25, 25);
  cursors.fist.load("cursors/fist.png");
  cursors.fist.resize(25, 25);
  cursors.bucket.load("cursors/bucket.png");
  cursors.bucket.resize(25, 25);
  cursors.hand.load("cursors/hand.png");
  cursors.hand.resize(25, 25);
  cursors.cross.load("cursors/cross.png");
  cursors.cross.resize(25, 25);
}

void Cursor::draw() const
{
  ofDisableDepthTest();
  switch (state)
  {
    case normal:
      cursors.cursor.draw(x, y, cursors.cursor.getWidth(), cursors.cursor.getHeight());
      break;
    case drag:
      cursors.fist.draw(x, y, cursors.fist.getWidth(), cursors.fist.getHeight());
      break;
    case bucket:
      cursors.bucket.draw(x, y, cursors.bucket.getWidth(), cursors.bucket.getHeight());
      break;
    case touch:
      cursors.hand.draw(x, y, cursors.hand.getWidth(), cursors.hand.getHeight());
      break;
    case cross:
      cursors.cross.draw(x, y, cursors.fist.getWidth(), cursors.fist.getHeight());
      break;
  }
  ofEnableDepthTest();
}

ofRectangle Cursor::getRectangle()
{
  return ofRectangle(x, y, 25, 25);
}
