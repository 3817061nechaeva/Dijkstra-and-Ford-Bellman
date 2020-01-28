#include "list.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include<string>
#include <iomanip>
using namespace std;

#define INF 1e9

void Print(int n, TList* mas) {
  for (int i = 0; i < n; ++i) {
    cout << "V" << i << ":\t";
    TElem* t = mas[i].GetBegin();
    if (t == NULL)
      cout << "NULL" << endl;
    while (t != NULL) {
      cout << "(" << t->GetV() << " with w " << t->GetW() << ");";
      t=t->GetNext();
    }
    cout << endl;
  }
}
TList* Rand(int n, int m, int q, int r) {
  if (m > n*(n - 1))
    cout << "error\n";
  srand(static_cast<unsigned int>(time(0)));
  TList* mas;
  mas = new TList[n];
  int a;
  int count;
  int *kol;
  kol = new int[n];
  for (int i = 0; i < n; i++)
    kol[i] = 0;
  int temp = m;
  while (temp != 0) {
    for (int i = 0; i < n; ++i) {
      if (kol[i] == n - 1)
        continue;
      count = rand() % n;
      if (temp - count <= 0) {
        count = temp;
        temp = temp - count;
      }
      else {
        temp = temp - count;
      }
      for (int j = 0; j < count; j++) {
        if (kol[i] == n - 1) {
          temp += count - j;
          break;
        }
        a = rand() % n;
        while (1)
        {
          TElem* t = mas[i].GetBegin();
          for (int s = 0; s < kol[i]; ++s)
          {
            if (a == t->GetV() || a == i)
            {
              a = rand() % n;
              s = -1;
              t = mas[i].GetBegin();
            }
            else
              t = t->GetNext();
          }
          if (a != i)
            break;
          else
            a = rand() % n;
        }
        mas[i].PutBegin(a, q + rand() % r);
        kol[i]++;
      }
    }
  }
  return mas;
}
int left_child(int n, int i) {
  int j = (i * 10) + 1;
  if (j > n-1)
    return -1;
  else
    return j;
}
int right_child(int n, int i) {
  int j = left_child(n, i);
  if (j == -1)
    return -1;
  else
    return min(j + 10 - 1, n - 1);
}
int min_child(int n,int* key, int i) {
  int kl = left_child(n, i);
  if (kl == -1)
    return -1;

 int kr = right_child(n,i);
 int min_key = key[kl];
 int min_child = kl;

 for (int s = kl + 1; s <= kr; s++)
 {
   if (key[s] < min_key) {
     min_key = key[s];
     min_child = s;
   }
 }
 return min_child;
}
int parent(int n, int i) {
  if (i == 0)
    return -1;
  else
    return (i - 1) / 10;
}
void diving(int i, int* key, int* name, int n, int* index) {
  int j1, j2, key0, name0,index0;
  j1 = i;
  j2 = min_child(n, key, i);
  key0 = key[i];
  name0 = name[i];

  while ((j2 != -1) && (key0 > key[j2])) {
    key[j1] = key[j2];
    name[j1] = name[j2];
    index[name[j1]] = j1;

    j1 = j2;
    j2 = min_child(n, key, j1);
  }

  key[j1] = key0;
  name[j1] = name0;
  index[name[j1]] =j1;
}

void emersion(int i, int* key, int* name, int n, int* index) {
  int j1, j2, key0, name0, index0;

  j1 = parent(n, i);
  j2 = i;
  key0 = key[i];
  name0 = name[i];

  while ((j2 > 0) && (key[j1] > key0)) {
    key[j2] = key[j1];
    name[j2] = name[j1];
    index[name[j2]] = j2;
    j2 = j1;
    j1 = parent(n, j2);
  }

  key[j2] = key0;
  name[j2] = name0;
  index[name[j2]] = j2;
}
void delete_min(int &name1, int &key1, int *name, int *key, int &n, int* index) {
  name1 = name[0];
  key1 = key[0];

  name[0] = name[n-1];  
  key[0] = key[n-1];

  name[n-1] = name1; 
  key[n-1] = key1;
  n--;
  diving(0, key, name, n, index);
}
void buld(int* name, int* key, int n, int* index) {

  for (int i = n - 1; i >= 0; --i)
    diving(i, key, name, n, index);
}

int* DIJKSTRA(TList* mas, int n, int s, int *up) {

  int *dist, *key, *name, *index;
  int name0, key0, t, vname, ind;
  TElem* p;
    dist = new int[n];
    key = new int[n];
    name = new int[n];
    index = new int[n];

  for (int i = 0; i < n; ++i) {
    up[i] = -1;
    dist[i] = INF;
    key[i] = INF;
    name[i] = i;
    index[i] = i;
  }

  key[s] = 0;
  int N = n;
  buld(name, key, n, index);
  while (N > 0) {

   delete_min(name0, key0,name,key,N,index);
    t = name0;
    dist[t] = key0;
    p = mas[t].GetBegin();

    if (p == NULL)
      continue;

    while (p != NULL) {

      vname = p->GetV();
      ind = index[vname];
      if (dist[vname] == INF) {
        if (key[ind] > (dist[t] + p->GetW()))
        {
          key[ind] = dist[t] + p->GetW();
          emersion(ind, key, name, n, index);
          up[vname] = t;
        }
      }
          p = p->GetNext();
    }
  }
  return dist;
}

int* LDG_FORD_BELLMAN(int* dist, int* up, TList* ADJ, int n, int start)
{
  // cout << "<Bellman-Ford> algorithm started..." << endl;
  for (int i = 0; i < n; ++i)
  {
    up[i] = -1; // предыдущие вершины в путях - изначально нет таких
    dist[i] = INF; // достаточно большое значение в рамках задачи
  }
  dist[start] = 0;

  for (int k = 0; k < n - 1; ++k)
  {
    for (int i = 0; i < n; ++i)
    {
      TElem* p = ADJ[i].GetBegin();
      if (p == NULL)
        continue;
      while (p != NULL)
      {
        int j = p->GetV();
        if (j != start)
        {
          if (dist[j] > (dist[i] + p->GetW()))
          {
            dist[j] = dist[i] + p->GetW();
            up[j] = i;
          }
        }
        p = p->GetNext();
      }
    }
  }
  // cout << "<Bellman-Ford> algorithm is complete." << endl;
  return dist;
}

int main() {
  TList* mas;
  mas = new TList[5];
  int* dist, *up;
  int n,m,q,r;
  int t;
  int k=0;
  //while (k != 3) {
  //  cout << "1)Rand graph.\n2)Algoritms.\n3)Exit.\n ";
  //  cin >> t;
  //  switch (t) {
  //  case 1: {
  //    cout << "Enter count of v: ";
  //    cin >> n;
  //    cout << "Enter count of m: ";
  //    cin >> m;
  //    cout << "Enter count of q: ";
  //    cin >> q;
  //    cout << "Enter count of r: ";
  //    cin >> r;
  //    mas = Rand(n, m,q,r); //Создаем граф
  //    Print(n, mas);
  //  } break;
  //  case 2: {
  //    up = new int[n];
  //    dist = DIJKSTRA(mas, n, 0, up);
  //    for (int i = 0; i < n; ++i) {
  //      if (dist[i] == INF)
  //        cout << "NOT\t";
  //      else
  //        cout << dist[i] << "\t";
  //    }
  //    cout << endl;
  //    for (int i = 0; i < n; ++i)
  //      cout << up[i] << "\t";
  //    dist = LDG_FORD_BELLMAN(dist, up, mas, n, 0);
  //    cout << endl;
  //    for (int i = 0; i < n; ++i) {
  //      if (dist[i] == INF)
  //        cout << "NOT\t";
  //      else
  //        cout << dist[i] << "\t";
  //    }
  //    cout << endl;
  //    for (int i = 0; i < n; ++i)
  //      cout << up[i] << "\t";
  //    cout << endl;
  //  } break;
  //  case 3: k = 3;
  //    break;
  //  }
  //}
  ofstream outdata;
  outdata.open("aaron.csv", ios::out);

  for (int i = 0; i <= 500; i += 100) {
    mas = Rand(300, i, 1, 1000000);
    up = new int[300];
    dist = new int[300];
    clock_t time;
    time = clock();
    LDG_FORD_BELLMAN(dist, up, mas, 300, 0);
    time = clock() - time;
    clock_t time1=0;
    time1 = clock();
    DIJKSTRA(mas, 300, 0, up);
    time1 = clock() - time1;
    cout << (double)i/500*100  << "\r";
    outdata << i << ";" << (double)time / 1000.0 << ";" << (double)time1 / 1000.0 << "\n";
  }

  //outdata.close();
  system("pause");

  return 0;
}