/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include <time.h>

///Preencher aqui para facilitar a correcao.
// Nome1: André Nogueira Brandão
// RA1: 116130
// Nome2:
// RA2:
///
// Bactracking function:
///
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
	return false;
}

struct User {
  int p;
  int w;
  int c;
  int order;
};

struct Node {
  int step;
  int width;
  int price;
  int current_class;
  float bound;
  vector<int> user;
};

// Sort per ratio (price/width)
bool cmp(User a, User b) {
  double price_per_width1 = (double) a.p / a.w;
  double price_per_width2 = (double) b.p / b.w;

  return price_per_width1 > price_per_width2;
}

// Function to sort the the classes per ratio
void sort_classes(vector<User> &users, int n) {
  int i;
  int begin, end;
  begin = 0;

  for(i=0; i < n; i++) {
    if(users[i].c != users[i-1].c) {
      sort(users.begin() + begin, users.begin() + i, cmp);
      begin = i;
    }
  }
  sort(users.begin() + begin, users.begin() + i, cmp);
}

// Sort per class
bool cmp2(User a, User b) {
  return a.c < b.c;
}

// Calculate the upper bound
float bound(Node u, vector<User> &users, int n, int d, int B) {
  float bound = u.price;
  float total_width = u.width;
  int partial = 0;
  int i;
  int last_class = u.current_class;

  // Check other steps in the tree
  for(i = u.step + 1; i < n; i++) {

    // If user is from another class
    // add separator with width 'd'
    if(last_class != -1 && users[i].c != last_class) {
      total_width += d;
    }
    last_class = users[i].c;

    // Add user
    if(users[i].w + total_width > B) {
      partial = 1;
      break;
    }
    else {
      total_width += users[i].w;
      bound += users[i].p;
    }
  }

  // Check if last one can be partially added
  if(partial) {
    bound += users[i].p * (B - total_width) / users[i].w;
  }

  return bound;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
  vector<User> users;
  vector<Node> nodes;
  vector<Node> solution;
  vector<int> track;
  int dif;
  Node u, v;


  for(int i = 0; i < n; i++) {
    users.push_back(User());
    users[i].p = p[i];
    users[i].w = w[i];
    users[i].c = c[i];
    users[i].order = i;
  }

  // Sort first by class
  sort(users.begin(), users.end(), cmp2);

  // Sort each class using price per width
  sort_classes(users, n);

  // First node (before any user)
  u.width = 0;
  u.step = -1;
  u.price = 0;
  u.current_class = -1;

  // Timer
  time_t start,end;
  time (&start);

  int max_price = 0;
  nodes.push_back(u);
  while(!nodes.empty()) {
    // If time is over, return false
    time (&end);
    dif = (int) difftime (end,start);
    if(t != 0 && dif >= t) {
      return false;
    }

    // get root
    u = nodes.back();
    nodes.pop_back();

    // Keep going only if u is not a leaf node
    if(u.step < n - 1) {
      // now checking next users
      v.step = u.step + 1;
      v.price = u.price + users[v.step].p;
      v.current_class = users[v.step].c;
      v.user = u.user;

      // If class of next user is different
      // add a separator with d width
      if(u.current_class != -1 && v.current_class != u.current_class) {
        v.width = u.width + users[v.step].w + d;
      }
      else
        v.width = u.width + users[v.step].w;

      // Calculate upper bound
      v.bound = bound(v, users, n, d, B);

      // If a better solution is found
      // update the lower bound and
      // save the current path
      if(v.width <= B && v.price > max_price) {
        max_price = v.price;

        // save path
        if(u.step != -1)
          track = u.user;
        track.push_back(1);

        // update the sol vector
        fill(sol.begin(), sol.end(), 0);
        for(int i=0; i<track.size(); i++) {
          if(track[i]) {
            sol[users[i].order] = track[i];
          }
        }
      }

      // Check if subtree should be searched
      v.user.push_back(1);
      if(v.bound > max_price) {
        nodes.push_back(v);
      }

      // Now verify if subtree without the next user
      // should be searched
      v.width = u.width;
      v.price = u.price;
      v.current_class = u.current_class;
      v.bound = bound(v, users, n, d, B);
      v.user.pop_back();
      v.user.push_back(0);

      // Add node to subtree if
      // it should be searched
      if(v.bound > max_price) {
        nodes.push_back(v);
      }
    }

  }

  return true;
}
