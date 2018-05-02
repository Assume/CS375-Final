//
//  main2.cpp
//  375Final
//
//  Created by Adam Berelowitz on 5/1/18.
//  Copyright © 2018 Adam Berelowitz. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string.h>
#include <algorithm>
#include <chrono>

#include "main2.hpp"



class Node {
public:
	int parentXY;
	int hCost;
	int gCost;
	int infoBits = 0;
	/*Info bits is used as follows:
	 -infoBits[0] = If node is currently in the queue
	 -infoBits[1] = If node has been through the queue yet
	 -infoBits<2:3> = Direction of next node in path
	 -infoBits[4] = If node is in path
	 -infoBits[5] = If node is obstacle
	 -infoBits[6] = If node is source
	 -infoBits[7] = If node is destination
	 */
};



void aStar(Node ** graph, int size, int sourceX, int sourceY) {
	int inPath[size * size];
	int destXandY[] = {size/4, size/2};
	//Start A star algorithm
	//Setup
	bool queueEmpty = true;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (graph[i][j].infoBits & 0b10000000) {
				queueEmpty = false;
			}
		}
	}
	
	//Algorithm start
	while (!queueEmpty) {
		Node *current;
		int minScore = size * 100;
		int xMin, yMin = size+1;
		for (int i = 0; i < size; i++){
			for (int j = 0; j < size; j++){
				if (graph[i][j].hCost + graph[i][j].gCost < minScore &&
					graph[i][j].infoBits & 0b10000000){
					xMin = i;
					yMin = j;
					current = &graph[i][j];
					minScore = current->hCost + current->gCost;
				}
			}
		}
		if (current->infoBits & 0b1) { // if current is destination
			int sourceXY = (sourceX << 11) | sourceY;
			std::cout << "Found Destination Node" << std::endl;
			int count = 0;
			int index = 3;
			inPath[1] = destXandY[0];
			inPath[2] = destXandY[1];
			while (current->parentXY != sourceXY) {
				current->infoBits |= 0b1000; // add to path
				count++;
				inPath[index] = current->parentXY >> 11;
				index++;
				inPath[index] = current->parentXY & 0b1111;
				index++;
				current = &(graph[current->parentXY >> 11][current->parentXY & 0b1111]);
				if (current->infoBits & 0b1000) {
					//Serial.println("ADDED TO PATH");
				}
			/*	Serial.print(count);
				Serial.println(" nodes in path, excluding source/destination");
				Serial.print("Current parent X, Y = ");
				Serial.print(current->parentXY >> 11);
				Serial.print(", ");
				Serial.println(current->parentXY & 0b1111);
				Serial.print("Source X, Y = ");
				Serial.print(sourceX);
				Serial.print(", ");
				Serial.println(sourceY);
				Serial.println();*/
			}
			inPath[0] = count + 2;
			inPath[count * 2 + 3] = sourceX;
			inPath[count * 2 + 4] = sourceY;
		/*	Serial.print(count);
			Serial.println(" ADDED TO PATH");
		 */
			std::cout << "SUCCESS" << std::endl;
			//Debug
		//	Serial.println("AFTER A STAR");
			//Serial.println("in path coords.");
			/*for (int k = 0; k <= inPath[0] * 2; k++) {
				Serial.print(inPath[k]);
				if (k == (inPath[0] * 2)) {
					Serial.println();
				}
				else if ((k % 2) == 1) {
					Serial.print(", ");
				}
				else {
					Serial.print("; ");
				}
			}
			Serial.println();*/
			return;
		}
		
		//Remove min score from queue
		current->infoBits &= 0b01111111;  //Remove from queue bit
		current->infoBits |= 0b01000000;  //Add to visited
		
		////////// setup for loop //////////////////////
		//Check if neighbor is new, then add it to the open queue
		//Not in the queue and not been visited
		Node *rightNeb, *leftNeb, *topNeb, *botNeb;
		Node* nebArr[4];
		for (int i = 0; i < 4; i++) {
			nebArr[i] = 0;
		}
		//Must initialize all the neighbors' direction bits
		if (xMin != size-1) {
			rightNeb = &graph[xMin + 1][yMin];
			rightNeb->infoBits |= 0b010000;
			nebArr[0] = rightNeb;
			
		}
		if (xMin != 0) {
			leftNeb = &graph[xMin - 1][yMin];
			leftNeb->infoBits |= 0b110000;
			nebArr[1] = leftNeb;
		}
		if (yMin != 0) {
			topNeb = &graph[xMin][yMin - 1];
			topNeb->infoBits |= 0b000000;
			nebArr[2] = topNeb;
		}
		if (yMin != size-1) {
			botNeb = &graph[xMin][yMin + 1];
			botNeb->infoBits |= 0b100000;
			nebArr[3] = botNeb;
		}
		int dist, h_offset;
		for (int i = 0; i < 4; i++) {
			if (nebArr[i] == 0)  continue;  // no neighbor
			if (nebArr[i]->infoBits & 0b100)  continue; // is an obstacle
			if (nebArr[i]->infoBits & 0b01000000) continue; // in closed set
			if (!(nebArr[i]->infoBits & 0b10000000))
				nebArr[i]->infoBits |= 0b10000000; // add to queue
			
			//Update direction bits
			//Define the cost of each neighbor
			switch (((current->infoBits & 0b110000) ^ (nebArr[i]->infoBits & 0b110000)) >> 11) {
				case 0:
					dist = 1;
					//          current node stays the same direction
					break;
				case 1:
					dist = 2;
					//          current->infoBits |= nebArr[i]->infoBits & 0b110000;
					break;
				case 2:
					dist = 3;
					//          current->infoBits |= nebArr[i]->infoBits & 0b110000;
					break;
				case 3:
					dist = 2;
					//          current->infoBits |= nebArr[i]->infoBits & 0b110000;
					break;
			}
			//while (dist == 100) Serial.println("PROBLEM WITH DIST SWITCH STATEMENT");
			int tent_cost = current->gCost + dist;
			if (tent_cost >= nebArr[i]->gCost)  continue;
			nebArr[i]->parentXY = (xMin << 11) | yMin;
			
			nebArr[i]->gCost = tent_cost;
			nebArr[i]->hCost = abs(destXandY[0] - xMin) + abs(destXandY[1] - yMin) - 1;
			
		}
		
		queueEmpty = true;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (graph[i][j].infoBits & 0b10000000) {
					queueEmpty = false;
				}
			}
		}
		
	}
}



Node** generate_graph(int size, int num_obstacles){
	
	// obstacle X's and Y's
	const int destXandY[] = {size/2,size-5};
	const int srcXandY[] = {0,0};
	
	
	Node** graph = 0;
	graph = new Node*[size];
	for (int i = 0; i < size; i++) {
		graph[i] = new Node[size];
		for (int j = 0; j < size; j++){
			Node obstacle = Node();
			graph[i][j] = obstacle;
		}
	}
	//  Serial.print('b');
	
	// set all g and h costs of each node in graph to infinity(255)
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			graph[i][j].gCost = (size * size) - 1;
			graph[i][j].hCost = (size * size) - 1;
		}
	}
	
	// set start node g cost to zero
	graph[srcXandY[0]][srcXandY[1]].gCost = 0;
	
	// set heuristic value of start node
	graph[srcXandY[0]][srcXandY[1]].hCost = std::abs((destXandY[0] - srcXandY[0]))
	+ std::abs((destXandY[1] - srcXandY[1]));
	
	// put the startnode in the queue, set the source node bit
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b10001010;
	
	// set the info bit for known node obstacles
	for (int i = 0; i < num_obstacles/size; i++)
		(graph[i][i]).infoBits |= 0b100;
	//(graph[std::rand() % size][std::rand() % size]).infoBits |= 0b100;
	
	
	
	// set the destination node bits
	graph[destXandY[0]][destXandY[1]].infoBits |= 0b1001;
	
	// set the in queue bit for the start node
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b10000000;
	// set source's parent x y to itself
	graph[srcXandY[0]][srcXandY[1]].parentXY = (srcXandY[0] << 11) | srcXandY[1];
	//  Serial.println(graph[srcXandY[0]][srcXandY[1]].parentXY);
	// set direction bit of source (started out facing east)
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b010000;
	
	return graph;
}


int main(){
	int size = 300;
	Node** graph = generate_graph(size, 5);
	auto start_time = std::chrono::high_resolution_clock::now();
	aStar(graph,size,0,0);
	auto end_time = std::chrono::high_resolution_clock::now();
	auto runtime = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
	return 0;
	
}
