#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <string.h>
#include <algorithm>
#include <chrono>
// add a timer
<<<<<<< Updated upstream
// 
=======
//
>>>>>>> Stashed changes

#define OBSTACLE_SIZE 3

class Node{
public:
	int parentXY;
	int hCost;
	int gCost;
	int infoBits = 0;
};


void setup() {
	//  Serial.print('a');
	// obstacle X's and Y's
	const int obstacleX[] = {};
	const int obstacleY[] = {};
	const int srcXandY[] = {0,0};
	const int destXandY[] = {0,15};
	
	Node graph[16][16];
	//  Serial.print('b');
	
	// set all g and h costs of each node in graph to infinity(255)
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			graph[i][j].gCost = 255;
			graph[i][j].hCost = 255;
		}
	}
	//  Serial.print('c');
	
	// set start node g cost to zero
	graph[srcXandY[0]][srcXandY[1]].gCost = 0;
	
	// set heuristic value of start node
	graph[srcXandY[0]][srcXandY[1]].hCost = std::abs((destXandY[0] - srcXandY[0]))
	+ std::abs((destXandY[1] - srcXandY[1]));
	
	//  Serial.println(graph[srcXandY[0]][srcXandY[1]].hCost);
	//  while(1){}
	// put the startnode in the queue, set the source node bit
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b10001010;
	
	// set the info bit for known node obstacles
	//  for (int i = 0; i < OBSTACLE_SIZE; i++){
	//    (graph[obstacleX[i]][obstacleY[i]]).infoBits |= 0b100;
	//  }
	
	// set the destination node bits
	graph[destXandY[0]][destXandY[1]].infoBits |= 0b1001;
	
	// set the in queue bit for the start node
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b10000000;
	// set source's parent x y to itself
	graph[srcXandY[0]][srcXandY[1]].parentXY = (srcXandY[0] << 4) | srcXandY[1];
	//  Serial.println(graph[srcXandY[0]][srcXandY[1]].parentXY);
	// set direction bit of source (started out facing east)
	graph[srcXandY[0]][srcXandY[1]].infoBits |= 0b010000;
	
	
	auto start_time = std::chrono::high_resolution_clock::now();
	
	
	
	// start A star algorithm
	// setup
	bool queueEmpty = true;
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++){
			if (graph[i][j].infoBits & 0b10000000){
				queueEmpty = false;
			}
		}
	}
	//  Serial.print('d');
	
	// algorithm start
	while (!queueEmpty){
		//    Serial.print('e');
		Node *current;
		int minScore = 510;
		int xMin, yMin = 17;
		for (int i = 0; i < 16; i++){
			for (int j = 0; j < 16; j++){
				if (graph[i][j].hCost + graph[i][j].gCost < minScore &&
					graph[i][j].infoBits & 0b10000000){
					xMin = i;
					yMin = j;
					current = &graph[i][j];
					//          Serial.print("c----");
					//          Serial.println(current->parentXY);
					minScore = current->hCost + current->gCost;
				}
			}
		}
		std::cout << std::endl;
		std::cout << "xMin: " << std::endl;
		
		
		std::cout<< "yMin: " << yMin << std::endl;
		
		
		if (current->infoBits & 0b1){ // if current is destination
			//      Serial.println(srcXandY[0] << 4);
			//      Serial.println(srcXandY[1]);
			int sourceXY = (srcXandY[0] << 4) | srcXandY[1];
			//      int num = 0;
			//      current->infoBits |= 0b1000; // add to path
			std::cout <<"Found Destination Node" << std::endl;
			std::cout <<"Current parentXY = " << std::endl;
			std::cout << current->parentXY << std::endl;
			std::cout <<"Source XY = " << std::endl;
			std::cout <<sourceXY << std::endl;
			while (current->parentXY != sourceXY){
				//        Serial.print('f');
				//        Serial.println("curent's parent: ");
				//        Serial.println(current->parentXY);
				//        Serial.println("source xy: ");
				//        Serial.println(sourceXY);
				//        Serial.println("reconstruct");
				current->infoBits |= 0b1000; // add to path
				current = &(graph[current->parentXY >> 4][current->parentXY & 0b1111]);
				//        Serial.println("node num "); Serial.println(num);
				//        Serial.println("path x, y"); Serial.println(current->parentXY >> 4);
				//        Serial.println(current->parentXY & 0b1111);
				std::cout << std::endl;
				std::cout << "Found Destination Node" << std::endl;
				std::cout << "Current parentXY = " << std::endl;
				std::cout <<current->parentXY << std::endl;
				std::cout << "Source XY = " << std::endl;
				std::cout << sourceXY << std::endl;
				
			}
			
			std::cout <<"SUCCESS" <<std::endl;
<<<<<<< Updated upstream
=======
			auto end_time = std::chrono::high_resolution_clock::now();
			auto runtime = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
			std::cout << runtime  << std::endl;
>>>>>>> Stashed changes
			return;
		}
		
		// remove min score from queue
		current->infoBits &= 0b01111111;  //remove from queue bit
		current->infoBits |= 0b01000000;  //add to visited
		////////// setup for loop //////////////////////
		// check if neighbor is new, then add it to the open queue
		// not in the queue and not been visited
		Node *rightNeb, *leftNeb, *topNeb, *botNeb;
		Node* nebArr[4];
		for (int i=0;i<4;i++){
			nebArr[i] = 0;
		}
		// must initialize all the neighbors' direction bits
		if (xMin != 15){
			rightNeb = &graph[xMin+1][yMin];
			rightNeb->infoBits |= 0b010000;
			nebArr[0] = rightNeb;
			
		}
		if (xMin != 0){
			leftNeb = &graph[xMin-1][yMin];
			leftNeb->infoBits |= 0b110000;
			nebArr[1] = leftNeb;
		}
		if (yMin != 0){
			topNeb = &graph[xMin][yMin-1];
			topNeb->infoBits |= 0b000000;
			nebArr[2] = topNeb;
		}
		if (yMin != 15){
			botNeb = &graph[xMin][yMin+1];
			botNeb->infoBits |= 0b100000;
			nebArr[3] = botNeb;
		}
		int dist, h_offset;
		for (int i=0;i<4;i++){
			if (nebArr[i] == 0)  continue;  // no neighbor
			if (nebArr[i]->infoBits & 0b01000000) continue; // in closed set
			if (!(nebArr[i]->infoBits & 0b10000000))
				nebArr[i]->infoBits |= 0b10001010; // add to queue
			
			//      Serial.print("d----");
			//      Serial.println(current->parentXY);
			
			// update direction bits
			// define the cost of each neighbor
			switch (((current->infoBits & 0b110000) ^ (nebArr[i]->infoBits & 0b110000)) >> 4){
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
			while (dist == 100)
				std::cout << "PROBLEM WITH DIST SWITCH STATEMENT" << std::endl;
			int tent_cost = current->gCost + dist;
			if (tent_cost >= nebArr[i]->gCost)  continue;
			nebArr[i]->parentXY = (xMin << 4) | yMin;
			
			nebArr[i]->gCost = tent_cost;
			nebArr[i]->hCost = abs(destXandY[0] - xMin) + abs(destXandY[1] - yMin) -1;
			//      Serial.println("yo");
			
		}
		
		queueEmpty = true;
		for (int i = 0; i < 16; i++){
			for (int j = 0; j < 16; j++){
				if (graph[i][j].infoBits & 0b10000000){
					queueEmpty = false;
				}
			}
		}
		
	}
}



int main(){
	
	setup();
	return 0;
	
}
