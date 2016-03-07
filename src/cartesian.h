#pragma once

// surprising these grid primitive aren't already in any of our GFX libs
// OK the touch screen lib does have its own TSPoint
template <class TPrecision>
struct _Vector
{
  TPrecision  x;
  TPrecision  y;

  template <class TIn>
  _Vector(TIn tin)
  {
    x = tin.x;
    y = tin.y;
  }

  _Vector() {}

  template <class T>
  _Vector<TPrecision>& operator-=(T& in)
  {
    x -= in.x;
    y -= in.y;
    return *this;
  }

  template <class T1, class T2>
  friend _Vector<TPrecision> operator-(T1& v1, T2& v2)
  {
    _Vector<TPrecision> out(v1);

    out -= v2;

    return out;
  }
};

// TODO: get this one up and running, with templates it should end up
// nearly the same code as the non-array versions
template <class TPrecision, uint8_t dimensions>
struct __Vector
{
  TPrecision d[dimensions];
};

template <class TPrecision>
struct _Vector3D : public _Vector<TPrecision>
{
  TPrecision z;

  template <class TIn>
  _Vector3D(TIn tin) : _Vector<TPrecision>(tin)
  {
    z = tin.z;
  }

  _Vector3D() {}


  _Vector3D<TPrecision>& operator-=(_Vector<TPrecision>& in)
  {
    _Vector<TPrecision>::x -= in.x;
    _Vector<TPrecision>::y -= in.y;

    return *this;
  }

  template <class T>
  _Vector3D<TPrecision>& operator-=(T& in)
  {
    _Vector<TPrecision>::x -= in.x;
    _Vector<TPrecision>::y -= in.y;
    z -= in.z;

    return *this;
  }
};



template<class T>
inline Print& operator <<(Print& obj, _Vector<T>& v)
{
  obj.print(v.x);
  obj.print(',');
  obj.print(v.y);
  return obj;
}


template <class TPrecision>
struct _Rectangle
{
//protected:
  // where within drawing area this rectangle begins
  _Vector<TPrecision>  origin;
  // horizontal and vertical size of rectangle
  _Vector<TPrecision>  size;

//public:
  _Rectangle() {}
  _Rectangle(_Vector<TPrecision>& origin, _Vector<TPrecision>& size) :
    origin(origin), size(size) {}

  TPrecision getX() { return origin.x; }
  TPrecision getY() { return origin.y; }
  TPrecision getWidth() { return size.x; }
  TPrecision getHeight() { return size.y; }
  TPrecision getBoundX() { return origin.x + size.x; }
  TPrecision getBoundY() { return origin.y + size.y; }
};
