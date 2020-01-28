#pragma once

#include <vector>

using namespace std;

class TElem {
private:
  int v, w;
  TElem* next;
public:
  TElem(int _v = 0, int _w = 0, TElem* _next = 0) { v = _v; w = _w; next = _next; };
  int GetV() { return v; };
  int GetW() { return w; };
  TElem* GetNext() { return next; };
  void SetV(int _v) { v = _v; };
  void SetW(int _w) { w = _w; };
  void SetNext(TElem* _next) { next = _next; };
};
 
class TList {
private:
  TElem* begin;
public:
  TList() { begin = 0; };
  void PutBegin(int _v, int _w) { TElem* temp = new TElem(_v,_w, begin); begin = temp; };
  vector<int> Get(int tek) {
    vector<int> temp(2);
    TElem* i=begin;
    int j = 0;
    for (j; j < tek; i = i->GetNext()) {
      j++;
    }
    temp[0] = i->GetV();
    temp[1] = i->GetW();
    return temp;
  };
  TElem* GetBegin() { return begin; };
  int Count()
  {
    TElem* i;
    int count = 0;
    for (i = begin; i != 0; i = i->GetNext())
      count++;
    return count;
  };

};
