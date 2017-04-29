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
  float upper_bound;
  vector<int> classes;
  vector<int> user;
};

// Sort per ratio (price/width)
bool cmp(User first, User second) {
  double price_per_width1 = (double) first.p / first.w;
  double price_per_width2 = (double) second.p / second.w;

  return price_per_width1 > price_per_width2;
}

// Add a class to the node
void add_class_to_node(Node &v, int x) {
  if(find(v.classes.begin(), v.classes.end(), x) != v.classes.end())
    return;
  else
    v.classes.push_back(x);
}

// Check if a class has not already been added to the node
bool is_new_class(Node v, int x) {
  if(find(v.classes.begin(), v.classes.end(), x) != v.classes.end()) {
    return false;
  }
  else{
    return true;
  }
}

// Calculate the upper bound
float find_upper_bound(Node u, vector<User> &users, int n, int d, int B) {
  float upper_bound = u.price;
  float total_width = u.width;
  int partial = 0;
  int i;
  Node v;
  v.classes = u.classes;

  // If node is already over the size of the bag
  // Set upper bound to zero
  if(u.width > B)
    return 0;

  // Check other steps in the tree
  for(i = u.step + 1; i < n; i++) {

    // If user is from a new class
    // add separator with width 'd'
    if(!v.classes.empty() && is_new_class(v, users[i].c)) {
      if(total_width + d < B){
        total_width += d;
        add_class_to_node(v, users[i].c);
      }
      else {
        break;
      }
    }

    // Verify it a new user can be added without
    // oversizing the width
    if(users[i].w + total_width > B) {
      partial = 1;
      break;
    }
    else {
      total_width += users[i].w;
      upper_bound += users[i].p;
    }
  }

  // Check if last one can be partially added
  if(partial) {
    upper_bound += users[i].p * (B - total_width) / users[i].w;
  }

  return upper_bound;
}

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){
  vector<User> users;
  vector<Node> nodes;
  vector<Node> solution;
  vector<int> track;
  Node u, v;
  float best_bound = 0;

  // Timer
  time_t start,end;
  time (&start);
  int dif;


  // Save all values in a User struct
  for(int i = 0; i < n; i++) {
    users.push_back(User());
    users[i].p = p[i];
    users[i].w = w[i];
    users[i].c = c[i];
    users[i].order = i;
  }

  // Sort by ratio (price per width)
  sort(users.begin(), users.end(), cmp);

  // First node (before any user)
  // This is the root of the tree
  u.width = 0;
  u.step = -1;
  u.price = 0;

  // Push the root node to the tree
  int max_price = 0;
  nodes.push_back(u);
  while(!nodes.empty()) {

    // If time is over, return false
    time (&end);
    dif = (int) difftime (end,start);

    if(t != 0 && dif >= t) {
      return false;
    }

    // Get node
    u = nodes.back();
    nodes.pop_back();

    // If node U is not a leaf
    // check the next users
    if(u.step < n - 1) {
      // now checking next users
      v.step = u.step + 1;
      v.price = u.price + users[v.step].p;
      v.classes = u.classes;
      v.user = u.user;

      // If class of next user is different
      // add a separator with d width
      if(!u.classes.empty() && is_new_class(v, users[v.step].c)) {
        v.width = u.width + users[v.step].w + d;
      }
      else{
        v.width = u.width + users[v.step].w;
      }
      add_class_to_node(v, users[v.step].c);

      // If a better solution is found
      // update the lower bound and
      // save the current path
      if(v.width <= B && v.price > max_price) {
        max_price = v.price;

        // save if the users were
        // added or not in the track vector
        if(u.step != -1)
          track = u.user;
        track.push_back(1);

        // update the sol vector
        // with the new solution
        fill(sol.begin(), sol.end(), 0);
        for(int i=0; i<track.size(); i++) {
          if(track[i]) {
            sol[users[i].order] = track[i];
          }
        }
      }

      // Calculate the upper bound
      v.upper_bound = find_upper_bound(v, users, n, d, B);
      // And check if subtree should be searched
      v.user.push_back(1);
      if(v.upper_bound > max_price) {
        nodes.push_back(v);
      }

      // Now verify if subtree without the next user
      // should be searched
      v.width = u.width;
      v.price = u.price;
      v.classes = u.classes;
      v.upper_bound = find_upper_bound(v, users, n, d, B);
      v.user.pop_back();
      v.user.push_back(0);

      // Add node to subtree if
      // it should be searched
      if(v.upper_bound > max_price) {
        nodes.push_back(v);
      }
    }

  }

  return true;
}
