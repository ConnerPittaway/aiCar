#pragma once
#include "Waypoint.h"
#include "Vector2D.h"
#include <vector>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define WAYPOINT_RESOLUTION 20

using namespace std;

struct EdgeCost
{
	Waypoint* waypointFROM;
	Waypoint* waypointTO;
	double cost;

	EdgeCost(Waypoint* from, Waypoint* to, double newCost)
	{
		waypointFROM = from;
		waypointTO = to;
		cost = newCost;
	}
};

struct AStarNode 
{
	Waypoint* internalWaypoint;
	AStarNode* parentWaypoint;
	double cost;

	AStarNode(Waypoint* currentWaypoint, AStarNode* ParentalWaypoint, double newCost)
	{
		internalWaypoint = currentWaypoint;
		parentWaypoint = ParentalWaypoint;
		cost = newCost;
	}
};

typedef vector<Waypoint*> vecWaypoints;
class PathFinder
{
private:
	vector<EdgeCost*> EDGECOST_List;
	vector<AStarNode*> OPEN_List;
	vector<AStarNode*> CLOSED_List;
	
	vecWaypoints            m_waypoints;

	//Functions
	void				SetEdgeCosts();
	Waypoint*			GetNearestWaypointToPosition(Vector2D position);
	double				GetCostBetweenWaypoints(Waypoint* from, Waypoint* to);
	vector<Vector2D>	ConstructPath(AStarNode* targetNode, Vector2D endPos);
	bool				IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck);
	double				GetHeuristCost(Vector2D pos1, Vector2D pos2);
	Waypoint* getWaypoint(const int x, const  int y);

public:
	PathFinder();
	~PathFinder();
	vector<Vector2D>	GetPathBetween(Vector2D startPosition, Vector2D endPosition);
	void CreateWaypoints();
	vector<Waypoint*> findNeighbours(const int x, const int y);
};

