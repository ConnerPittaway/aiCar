#include "Pathfinder.h"

PathFinder::PathFinder()
{
    CreateWaypoints();
	SetEdgeCosts();
}

PathFinder::~PathFinder()
{
	//Clear Lists
}

void PathFinder::CreateWaypoints()
{
    std::vector<int> vecOffTrack400{ 0,1,2,3,23,8,9,10,11,12,13,14,15,16,17,18,19,39,32,31,30,
    29,28,23,22,21,20,41,42,62,81,61,100,101,121,140,160,180,200,380,217,157,197,177,178,198,
    322,323,303,302,301,281,282,262,261,242,222,203,183,163,143,124,144,125,105,104,85,86,
    66,65,29,30,32,33,49,50,51,52,72,71,70,109,129,149,169,188,168,148,128,147,167,207,
    206,186,225,171,151,131,111,91,71,112,132,152,172,173,153,174,175,89,69,110,130,150,
    170,324,304,325,286,287,307,268,288,289,269,249,250,290,271,251,231, 252,272,253,233,
    234,254,255,235,236,237,137,117,116,115,95,94,76,97,117,79,59,60,80,120,187,388,389,
    390,391,392,393,394,395,396,397,398,399,379,359,339,319,299,279,318,338,358,378,397,
    377,357,317,336,356,376,395,375,355,335,374,393,373,353,333,372,392,391,371,370,390,
    389,369,388,387,367,386,385,40,7,96,92,90,298,337,316,334,354,315,352,350,368,360,381,
    340,181,161,141,5,6,38,99,34,305,306,270,232, 139,82,102,63,43,24,4,119,220,223,223,218,226};

    std::vector<int> vecCheckpoint{ 311,239,159,58,54,93,176,209,264,87,47,44,162,240,362 };

    // create the waypoints
    float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
    float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
    float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
    float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);
    unsigned int index = 0;
    for (unsigned int j = 0; j < WAYPOINT_RESOLUTION; j++) {
        for (unsigned int i = 0; i < WAYPOINT_RESOLUTION; i++) {
            Waypoint* wp = new Waypoint();
            wp->SetTrack(index++);
            wp->setPosition(XMFLOAT3(xStart + (xGap * i), yStart + (yGap * j), 0));
            wp->setX(i);
            wp->setY(j);
            m_waypoints.push_back(wp);
        }
    }

   /* for (int x = 0; x < m_waypoints.size(); x++)
    {
        if (std::find(vecOffTrack400.begin(), vecOffTrack400.end(), index) == vecOffTrack400.end()) {
            m_waypoints[x]->setOnTrack(true);
            if (std::find(vecCheckpoint.begin(), vecCheckpoint.end(), index) != vecCheckpoint.end()) {

            }
        }
        else {
            m_waypoints[x]->setOnTrack(false);
        }
    }*/
}

void PathFinder::SetEdgeCosts()
{
    for (int i = 0; i < m_waypoints.size(); i++)
    {
        Waypoint* fromWaypoint = m_waypoints[i]; 
        vector<Waypoint*> currentNeighbours = findNeighbours(m_waypoints[i]->getX(), m_waypoints[i]->getY());
        for (int j = 0; j < currentNeighbours.size(); j++)
        {
            Waypoint* toWaypoint = currentNeighbours[j];
            EDGECOST_List.push_back(new EdgeCost(fromWaypoint, toWaypoint, 1.0f));
        }
    }
}

Waypoint* PathFinder::GetNearestWaypointToPosition(Vector2D position)
{
    float nearestDistance = FLT_MAX;
    Waypoint* nearestWaypoint = NULL;
    for (int i = 0; i < m_waypoints.size(); i++)
    {
        Vector2D vecBetweenPoints = position - (Vector2D(m_waypoints[i]->getPosition()->x, m_waypoints[i]->getPosition()->y));
        float newDistance = vecBetweenPoints.Length();
        if (newDistance < nearestDistance)
        {
            nearestDistance = newDistance;
            nearestWaypoint = m_waypoints[i];
        }
    }
    return nearestWaypoint;
}

double PathFinder::GetCostBetweenWaypoints(Waypoint* from, Waypoint* to)
{
    for (int i = 0; i < EDGECOST_List.size(); i++)
    {
        if (EDGECOST_List[i]->waypointFROM == from && EDGECOST_List[i]->waypointTO == to) //&& to->isOnTrack() == true)
        {
            return EDGECOST_List[i]->cost;
        }
    }
    return FLT_MAX;
}

vector<Vector2D> PathFinder::ConstructPath(AStarNode* targetNode, Vector2D endPos)
{
    vector<Vector2D> path;
    vector<Vector2D> pathInReverse;
    AStarNode* currentAStarNode;

    pathInReverse.push_back(endPos);
    currentAStarNode = targetNode;

    while (currentAStarNode != NULL )
    {
        if (currentAStarNode->internalWaypoint->isOnTrack() == true)
        {
            pathInReverse.push_back(Vector2D(currentAStarNode->internalWaypoint->getPosition()->x, currentAStarNode->internalWaypoint->getPosition()->y));
            currentAStarNode = currentAStarNode->parentWaypoint;
        }
        else
        {
            pathInReverse.push_back(Vector2D(currentAStarNode->internalWaypoint->getPosition()->x, currentAStarNode->internalWaypoint->getPosition()->y));
            currentAStarNode = currentAStarNode->parentWaypoint;
        }
    }

    vector<Vector2D>::iterator iter = pathInReverse.end();
    while (iter != pathInReverse.begin())
    {
        path.push_back(*(--iter));
    }
    return path;
}

bool PathFinder::IsInList(vector<AStarNode*> listToCheck, Waypoint* waypointToCheck)
{
    for (int i = 0; i < listToCheck.size(); i++)
    {
        if (listToCheck[i]->internalWaypoint == waypointToCheck) //&& waypointToCheck->isOnTrack() == true)
        {
            return true;
        }
    }
    return false;
}

double PathFinder::GetHeuristCost(Vector2D pos1, Vector2D pos2)
{
	return Vector2D(pos1 - pos2).Length();
}

Waypoint* PathFinder::getWaypoint(const int x, const int y)
{
    if (x >= 0 && y >= 0 && x < WAYPOINT_RESOLUTION && y < WAYPOINT_RESOLUTION)
    {
        return m_waypoints[y * WAYPOINT_RESOLUTION + x];
    }
    return nullptr;
}

vector<Vector2D> PathFinder::GetPathBetween(Vector2D startPosition, Vector2D endPosition)
{
    OPEN_List.clear();
    CLOSED_List.clear();

    vector<Vector2D> path;

    //Vehicle Position
    Waypoint* nearestToVehicle;
    nearestToVehicle = GetNearestWaypointToPosition(startPosition);

    //Waypoint End
    Waypoint* nearestToEnd;
    nearestToEnd = GetNearestWaypointToPosition(endPosition);

    if (nearestToVehicle == NULL || nearestToEnd == NULL || nearestToVehicle == nearestToEnd)
    {
        path.push_back(endPosition);
        return path;
    }
    
    //Add the nearest waypoint to the start to the open list
    OPEN_List.push_back(new AStarNode(nearestToVehicle, NULL, 0.0f));
    AStarNode* currentNode = nullptr;
    //currentNode->internalWaypoint->setOnTrack(true);
    while (!OPEN_List.empty())
    {
       
        for (int i = 0; i < OPEN_List.size(); i++)
        {
            if (currentNode == NULL || OPEN_List[i]->cost < currentNode->cost)//&& currentNode->internalWaypoint->isOnTrack())
            {
                currentNode = OPEN_List[i];
            }
        }

        if (currentNode->internalWaypoint->isOnTrack())
        {
            if (currentNode->internalWaypoint == nearestToEnd)
            {
                path = ConstructPath(currentNode, endPosition);
                return path;
            }
            else
            {
                vector<Waypoint*> connectedWaypoints = findNeighbours(currentNode->internalWaypoint->getX(), currentNode->internalWaypoint->getY());
                for (int i = 0; i < connectedWaypoints.size(); i++)
                {
                    if (!IsInList(OPEN_List, connectedWaypoints[i]) && !IsInList(CLOSED_List, connectedWaypoints[i]))
                    {
                        //if (connectedWaypoints[i]->isOnTrack())
                        //{
                        double g = currentNode->cost + GetCostBetweenWaypoints(currentNode->internalWaypoint, connectedWaypoints[i]);
                        double h = GetHeuristCost(Vector2D(connectedWaypoints[i]->getPosition()->x, connectedWaypoints[i]->getPosition()->y), endPosition);
                        double f = g + h;
                        OPEN_List.push_back(new AStarNode(connectedWaypoints[i], currentNode, f));
                        //}
                    }
                }

                //if(currentNode->internalWaypoint->isOnTrack() == true)
                CLOSED_List.push_back(currentNode);

                vector<AStarNode*>::iterator iter = OPEN_List.begin();
                while (iter != OPEN_List.end())
                {
                    if (*iter == currentNode)
                        iter = OPEN_List.erase(iter);
                    else
                        ++iter;
                }
                currentNode = NULL;
            }
        }
    }
    return path;
}



/*waypoint(5, 1) - the neighbours are   (4, 2)(5, 2)(6, 2)
                                        (4, 1)      (6, 1)
                                        (4, 0)(5, 0)(6, 0)*/
                                              
                                            //5              //1
vector<Waypoint*> PathFinder::findNeighbours(const int wayX, const int wayY)
{
    vector<Waypoint*> neighbours;
    int currentWaypointx = wayX - 1;
    int currentWaypointy = wayY - 1;

    //Nest Loop to check 3x3 grid
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            if (currentWaypointx != wayX || currentWaypointy != wayY)
            {
                Waypoint* currentNeighbour = getWaypoint(currentWaypointx, currentWaypointy);
                if (currentNeighbour != nullptr)
                {
                    if(currentNeighbour->isOnTrack())
                        neighbours.push_back(currentNeighbour);
                }
            }
            currentWaypointy++;
        }
        currentWaypointx++;
        currentWaypointy = wayY - 1;
    }
    return neighbours;
}
