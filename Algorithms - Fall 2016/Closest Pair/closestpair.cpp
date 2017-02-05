// Author: Jessie Cisneros

#include "closestpair.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sys/time.h>  // used to seed srand for extra credit
using namespace std;

Outcome brute(const vector<Point>& data) {
    Outcome best(data[0], data[1], distSquared(data[0],data[1]));
    for (unsigned int j=1; j < data.size(); j++) {
        for (unsigned int k=0; k < j; k++) {
            long long temp = distSquared(data[j], data[k]);
            if (temp < best.dsq) {
                best = Outcome(data[j], data[k], temp);
            }
        }
    }
    return best;
}

// Copy a vector from a start value to a end value
vector<Point> copyVector(const vector<Point>& data, int begin, int end){
    int size = end - begin;
    vector<Point> copy(size);
    for(int i=0; i < size; i++) {
	copy[i] = data[begin+i];
    }
    return copy;
}

Outcome divide_and_conquer(const vector<Point>& data) {
    // Base Case
    if(data.size() <= 3) {
	return brute(data);
    }
    
    int i;
    int half_size = data.size()/2;
    long middle = data[half_size].x;

    Outcome minLeft;
    Outcome minRight;
    vector<Point> Point_L(half_size);
    vector<Point> Point_R(half_size);

    // Divide all points into two vectors, a right vector and a left vector
    Point_L = copyVector(data, 0, half_size);
    Point_R = copyVector(data, half_size, data.size());

    // Recurse until it hits the base case for both right and left
    minLeft = divide_and_conquer(Point_L);
    minRight = divide_and_conquer(Point_R);

    // Retrieve the minimum dsq from both outcomes
    Outcome minOutcome;
    if(minLeft.dsq <= minRight.dsq) {
        minOutcome = minLeft;
    }
    else if(minRight.dsq <= minLeft.dsq) {
        minOutcome = minRight;
    }
    
    int data_size = data.size();
    int vector_len = 0;
    long absolute_len;
    vector<Point> middle_Points(data.size());
  
    // Find all points in the middle range
    for(i=0; i<data_size; i++){
        absolute_len = pow(abs(data[i].x - middle),2);
	if(absolute_len <= minOutcome.dsq) {
	    middle_Points[vector_len] = data[i];
            vector_len++;
	}
    }

    // Resize the vector to the appropriate size
    middle_Points.resize(vector_len);
    
    // Look for the lowest dsq from the middle points
    Outcome minMiddle;
    minMiddle = brute(middle_Points);
    
    // Compare the lowest middle point from the lowest outcome
    if(minMiddle.dsq <= minOutcome.dsq && minMiddle.dsq >= 0) {
        return minMiddle;
    }    
   
    return minOutcome;
}

Outcome efficient(const vector<Point>& data) {
    int size = data.size();
    
    // Create a new vector for sorting values
    vector<Point> sorted_data(size);
    sorted_data = data;

    // Sort the point list by increasing x values
    sort(sorted_data.begin(), sorted_data.begin()+data.size(), compareByX);
 
    // Store the returned outcome
    Outcome store;
    store = divide_and_conquer(sorted_data);
    
    return store;
}

Outcome extra(const vector<Point>& data) {
    return Outcome();
}
