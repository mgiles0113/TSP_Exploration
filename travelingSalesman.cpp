//Ryan Fleming
//CS325 Project 4
//2-Opt  algorithm for TSP
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <limits>
using namespace std;
//random generator for testing DELETE LATER
vector<vector<int> > randomAdjacencyMatrix(int cityCount);
//nearest neighbor function makes tour from city to city
vector<int> tour(vector<vector<int> > adjacencyMatrix, int cityCount);
//gets distance
int getDistance(vector<vector<int> > adjacencyMatrix, vector<int> tour);
//runs 2-opt comparison algorithm
vector<int> twoOpt(vector<int> tour, vector<vector<int> > adjacencyMatrix, int cityCount, clock_t timer);
//helper for 2-Opt
vector<int> swap(vector<int> tour, int front, int back, int cityCount);

int main()
{
	double start = clock();
	double end;
	vector<vector<int> > adjacencyMatrix;
	//testing stuff
	int cityCount = 5000;
	adjacencyMatrix = randomAdjacencyMatrix(cityCount);
	cityCount = adjacencyMatrix.size();
	vector<int> mtour = tour(adjacencyMatrix, cityCount);
	cout << "Pre-Optimized tour: " << endl;
//	for (int i = 0; i < (cityCount - 1); i++)
//	{
//		cout << mtour[i] << endl;
//	}
	int distance = getDistance(adjacencyMatrix, mtour);
	cout << "Pre-optimized Distance: " << distance << endl;
	vector<int> twoOpt1 = twoOpt(mtour, adjacencyMatrix, cityCount, start);
	vector<int> twoOpt2 = twoOpt(twoOpt1, adjacencyMatrix, cityCount, start);
	distance = getDistance(adjacencyMatrix, twoOpt2);
	cout << "Optimized Distance: " << distance << endl;
	cout << "Optimized tour: " <<endl;
//	for (int i = 0; i < cityCount; i++)
//	{
//		cout << twoOpt2[i] << endl;
//	}
	end = clock();
	end -= start;
	end /= CLOCKS_PER_SEC;
	cout << "Total Time: " << end << endl;
	return 0;
}

vector<vector<int> > randomAdjacencyMatrix(int cityCount)
{
	vector<vector<int> > adjacencyMatrix;
	int random;
	adjacencyMatrix.resize(cityCount, vector<int>(cityCount, 0));
	for (int i = 0; i < cityCount; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			random = rand() % 200 + 1;
			if (i == j)
			{
				random = -1;
			}
			adjacencyMatrix[i][j] = random;
			adjacencyMatrix[j][i] = random;
		}
	}
	return adjacencyMatrix;
}

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

vector<int> twoOpt(vector<int> tour, vector<vector<int> > adjacencyMatrix, int cityCount, clock_t timer)
{
	int size = tour.size();
	vector<int> solutionTour = tour;
	clock_t timeLimit = CLOCKS_PER_SEC * 15;
	clock_t solveTime;
	clock_t currentTime;
	bool optimize = false;
	while (!optimize && timer < timeLimit)
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
					optimize = false;
					solutionTour = newTour;
					optimalDistance = newDistance;
				}
				currentTime = clock();
				solveTime = currentTime - timer;
				if (solveTime > timeLimit)
				{
					cout << "Interrupted Solve Time: " << double((solveTime / CLOCKS_PER_SEC)) << endl;
					return solutionTour;
				}
			}
		}
		optimize = true;
	}
	cout << "Completed Solve Time: " << double((solveTime / CLOCKS_PER_SEC)) << endl;
	return solutionTour;
}

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
	
