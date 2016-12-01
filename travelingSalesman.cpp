//George Sustare
//Mark Giles
//Ryan Fleming
//Group 44
//CS325 Project 4
//2-Opt  algorithm for TSP
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <limits>
#include <string>
#include <fstream>
#include <cmath>
#include <sstream>
using namespace std;


struct City
{
	int idNumber;
	int x;
	int y;
};


//nearest neighbor function makes tour from city to city
vector<int> tour(vector<vector<int> > adjacencyMatrix, int cityCount);
//gets distance
int getDistance(vector<vector<int> > adjacencyMatrix, vector<int> tour);
//runs 2-opt comparison algorithm
vector<int> twoOpt(vector<int> tour, vector<vector<int> > adjacencyMatrix, int cityCount, clock_t timer);
//helper for 2-Opt
vector<int> swap(vector<int> tour, int front, int back, int cityCount);
//function to intake file and covert into an adjacency matrix
void intakeFile(string fileName, vector<vector<int> > &adjacencyMatrix);

int main(int argc, char* argv[])
{
	//if user does not format input file correctly, quit
	if (argc != 2)
	{
		return 1;
	}
	//timing purposes for competition
	double start = clock();
	double end;
	//make adjacency matrix and pipe the input file into it
	vector<vector<int> > adjacencyMatrix;
	string inputFile = argv[1];
	intakeFile(inputFile, adjacencyMatrix);
	int cityCount = adjacencyMatrix.size();
	//create tour using nearest neighbor algorithm
	vector<int> mtour = tour(adjacencyMatrix, cityCount);
	//run 2-opt algorithm using pre-optimized tour
	vector<int> twoOpt1 = twoOpt(mtour, adjacencyMatrix, cityCount, start);
	//run 2-opt algorithm using tour created by first optimization
	vector<int> twoOpt2 = twoOpt(twoOpt1, adjacencyMatrix, cityCount, start);
	//open output file with name + tour
	ofstream outputFile;
	inputFile += ".tour";
	outputFile.open(inputFile.c_str());
	//get tour length
	int distance = getDistance(adjacencyMatrix, twoOpt2);
	//testing
	cout << "Optimized Distance: " << distance << endl;
	//put completed tour and distance in file
	outputFile << distance << endl;
	for (int i = 0; i < cityCount; i++)
	{
		
		outputFile << twoOpt2[i] << endl;
	}
	outputFile.close();
	//timing for competition
	end = clock();
	end -= start;
	end /= CLOCKS_PER_SEC;
	cout << "Total Time: " << end << endl;
	return 0;
}

//nearest neighbor algorithm quickly creates a somewhat optimal tour
//https://en.wikipedia.org/wiki/Nearest_neighbour_algorithm#Example_with_the_traveling_salesman_problem
vector<int> tour(vector<vector<int> > adjacencyMatrix, int cityCount)
{
	int currentCity = 0;
	int nextCity;
	int minimumDistance;
	vector<int> tour;
	tour.resize(cityCount + 1, -1);
	for (int i = 0; i < cityCount; i++)
	{
		minimumDistance = numeric_limits<int>::max();
		tour[i] = currentCity;
		for (int j = 0; j < cityCount; j++)
		{
			if (j != currentCity && adjacencyMatrix[currentCity][j] < minimumDistance && !(find(tour.begin(), tour.end(), j) != tour.end()))
			{
				minimumDistance = adjacencyMatrix[currentCity][j];
				nextCity = j;
			}
		}
		currentCity = nextCity;
	}
	tour[cityCount] = 0;
	return tour;
}

//calculate distance of a given tour by adding up all of the distances in an adjacency matrix
int getDistance(vector<vector<int> > adjacencyMatrix, vector<int> tour)
{
	int distance;
	int totalDistance = 0;
	int currentCity;
	int nextCity;
	int tourLength = tour.size();
	for (int i = 0; i < (tourLength - 1); i++)
	{
		currentCity = tour[i];
		nextCity = tour[i + 1];
		distance = adjacencyMatrix[currentCity][nextCity];
		totalDistance += distance;
	}
	return totalDistance;
}

//2-opt algorithm optimizes an already found solution/tour for TSP. So by running nearest neighbor which is quick but not optimal,
//then running 2-opt which is accurate but slow on the tour given by nearest neighbor, we have a somewhat quick but still accurate solution
//https://en.wikipedia.org/wiki/2-opt
vector<int> twoOpt(vector<int> tour, vector<vector<int> > adjacencyMatrix, int cityCount, clock_t timer)
{
	int size = tour.size();
	vector<int> solutionTour = tour;
	//we take in the timer from main and put time limits on solving the algorithm so we can stay under 3 minutes for competition
	//if optimization reaches time limit it just outputs what it has optimized so far
	clock_t timeLimit = CLOCKS_PER_SEC * 15;
	clock_t solveTime;
	clock_t currentTime;
	bool optimizeMore = false;
	while (!optimizeMore && timer < timeLimit)
	{
		int optimalDistance = getDistance(adjacencyMatrix, solutionTour);
		for(int i = 1; i < (size - 2); i++)
		{
			for (int j = (i + 1); j < (size - 1); j++)
			{
				vector<int> newTour = swap(solutionTour, i, j, cityCount);
				int newDistance = getDistance(adjacencyMatrix, newTour);
				if(newDistance < optimalDistance)
				{
					optimizeMore = false;
					solutionTour = newTour;
					optimalDistance = newDistance;
				}
				currentTime = clock();
				solveTime = currentTime - timer;
				//if timed out, output the interrupted solve time for debugging and testing purposes
				if (solveTime > timeLimit)
				{
					cout << "Interrupted Solve Time: " << double((solveTime / CLOCKS_PER_SEC)) << endl;
					return solutionTour;
				}
			}
		}
		optimizeMore == true;
	}
	//if completed without timer time out output complete time for debugging and testing
	cout << "Completed Solve Time: " << double((solveTime / CLOCKS_PER_SEC)) << endl;
	return solutionTour;
}

//swap helper function for 2-opt
//https://en.wikipedia.org/wiki/2-opt
vector<int> swap(vector<int> tour, int front, int back, int cityCount)
{
	int size = tour.size();
	vector<int> newTour = tour;
	int counter = 0;
	for (int i = 0; i < (front - 1); i++)
	{
		newTour[i] = tour[i];
	}
	for(int i = front; i <= back; i++)
	{
		newTour[i] = tour[back - counter];
		counter++;
	}
	for(int i = (back + 1); i < size; i++)
	{
		newTour[i] = tour[i];
	}
	return newTour;
}

//intakes file, puts each line into a City struct
//put all cities into a vector
//calculate distance between cities and put in adjacency matrix
//https://en.wikipedia.org/wiki/Adjacency_matrix
void intakeFile(string fileName, vector<vector<int> > &adjacencyMatrix)
{
	vector<City> allCities;
	ifstream inputFile;
	string argumentLine;
	inputFile.open(fileName.c_str());
	while(getline(inputFile, argumentLine))
	{
		istringstream iss(argumentLine);
		City nextCity;
		iss >> nextCity.idNumber;
		iss >> nextCity.x;
		iss >> nextCity.y;
		allCities.push_back(nextCity);
	}
	inputFile.close();
	int cityCount = allCities.size();
	adjacencyMatrix.resize(cityCount, vector<int>(cityCount, -1));
	for (int i = 0; i < cityCount; i++)
	{
		for (int j = 0; j < cityCount; j++)
		{
			//distance = sqrt((x2 - x1)^2 + (y2 -y1))^2)
			//https://en.wikipedia.org/wiki/Distance#Geometry
			adjacencyMatrix[i][j] = round(sqrt(pow((allCities[i].x - allCities[j].x), 2) + pow((allCities[i].y - allCities[j].y), 2)));
		}
	}
}	
