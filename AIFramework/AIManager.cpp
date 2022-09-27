#include "AIManager.h"
#include "Vehicle.h"
#include "DrawableGameObject.h"
#include "PickupItem.h"
#include "Waypoint.h"

#include "main.h"
#include<string>  

HRESULT AIManager::initialise(ID3D11Device* pd3dDevice)
{
    // create a pickup item ----------------------------------------------

    PickupItem* pPickup = new PickupItem();
    HRESULT hr = pPickup->initMesh(pd3dDevice);
    m_pickups.push_back(pPickup);


    // create the vehicle ------------------------------------------------
    float xPos = 0;
    float yPos = 0;

    m_pCar = new Vehicle();
    hr = m_pCar->initMesh(pd3dDevice);
    m_pCar->setPosition(XMFLOAT3(xPos, yPos, 0));


    // create the vehicle ------------------------------------------------
    float xPos2 = 100;
    float yPos2 = 100;

    m_pCar2 = new Vehicle();
    hr = m_pCar2->initMesh(pd3dDevice);
    m_pCar2->setPosition(XMFLOAT3(xPos2, yPos2, 0));

    if (FAILED(hr))
        return hr;

    // create the waypoints
    float xGap = SCREEN_WIDTH / WAYPOINT_RESOLUTION;
    float yGap = SCREEN_HEIGHT / WAYPOINT_RESOLUTION;
    float xStart = -(SCREEN_WIDTH / 2) + (xGap / 2);
    float yStart = -(SCREEN_HEIGHT / 2) + (yGap / 2);

    unsigned int index = 0;
    for (unsigned int j = 0; j < WAYPOINT_RESOLUTION; j++) {
        for (unsigned int i = 0; i < WAYPOINT_RESOLUTION; i++) {
            Waypoint* wp = new Waypoint();
            hr = wp->initMesh(pd3dDevice, index++);
            wp->setPosition(XMFLOAT3(xStart + (xGap * i), yStart + (yGap * j), 0));
            m_waypoints.push_back(wp);
        }
    }

    m_laps = 0;
    return hr;
}

Waypoint* AIManager::getWaypoint(const int x, const  int y) {
    return m_waypoints[y * WAYPOINT_RESOLUTION + x];
}
void AIManager::update(const float fDeltaTime)
{
    for (unsigned int i = 0; i < m_waypoints.size(); i++) {
        m_waypoints[i]->update(fDeltaTime);
        if (m_waypoints[i]->isOnTrack() == true)
            AddItemToDrawList(m_waypoints[i]); // if you comment this in, it will display the waypoints
    }

    getWaypoint(10, 10)->update(fDeltaTime);
    AddItemToDrawList(getWaypoint(10, 10));

    getWaypoint(13, 4)->update(fDeltaTime);
    AddItemToDrawList(getWaypoint(13, 4));

    for (unsigned int i = 0; i < m_pickups.size(); i++) {
        m_pickups[i]->update(fDeltaTime);
        AddItemToDrawList(m_pickups[i]);
    }

    if (m_pCar->getMove())
    {
        if (m_laps < 3)
        {
            if (m_pCar->getCurrentPathPos() < m_pCar->getPath().size())
            {
                if (Vector2D(m_pCar->getPosition()->x, m_pCar->getPosition()->y) != m_pCar->getPath()[m_pCar->getCurrentPathPos()])
                {
                    m_pCar->setPositionTo(Vector2D(m_pCar->getPath()[m_pCar->getCurrentPathPos()].x, m_pCar->getPath()[m_pCar->getCurrentPathPos()].y));
                }
                else
                {
                    m_pCar->setCurrentPathPos(m_pCar->getCurrentPathPos() + 1);
                }
            }
            else if (m_pCar->getCurrentPathPos() == m_pCar->getPath().size())
            {
                m_pCar->setCurrentPathPos(0);
                m_laps++;
            }

        }
    }     //m_pCar->setPositionTo(Vector2D(getWaypoint(13, 4)->getPosition()->x, getWaypoint(13, 4)->getPosition()->y));
    
    if (getArrive())
    {
        Waypoint* w = getWaypoint(1, 1);
        Vector2D target = Vector2D(w->getPosition()->x, w->getPosition()->y);
        Vector2D position = Vector2D(target.x - m_pCar2->getPosition()->x, target.y - m_pCar2->getPosition()->y);
        double distance = position.Length();
        if (distance > 1) //&& Vector2D(m_pCar2->getPosition()->x, m_pCar2->getPosition()->y) == m_pCar2->getCurrentDest())
        {
            string str = std::to_string(distance);
            string str3 = str + " \n";
            OutputDebugStringA(str3.c_str());

            position.Normalize();
            position = position * m_pCar2->getMaxSpeed() * m_pCar->getCurrentSpeed();
            if (distance < 100.0f)
            {
                m_pCar2->setCurrentSpeed(0.5f);

            }
            if (distance < 2.0f)
            {
                m_pCar2->setCurrentSpeed(0.0f);

            }
            m_pCar2->setPositionTo(position);
            m_pCar2->setDest(Vector2D(w->getPosition()->x, w->getPosition()->y));
            //m_pCar2->setMaxSpeed(200.0f);
        }
        else
        {
            OutputDebugStringA("At location");
            setArrive(false);
            m_pCar2->setCurrentSpeed(200.0f);
            m_pCar2->setPositionTo(Vector2D(m_pCar2->getPosition()->x, m_pCar2->getPosition()->y));
        }
    }
    m_pCar->update(fDeltaTime);
    m_pCar2->update(fDeltaTime);

    checkForCollisions();

    AddItemToDrawList(m_pCar);
    AddItemToDrawList(m_pCar2);
}

void AIManager::mouseUp(int x, int y)
{
    m_pCar->setPositionTo(Vector2D(x, y));
    string str = std::to_string(x);
    string str2 = std::to_string(x);
    string str3 = str + " " + str2;
    OutputDebugStringA(str3.c_str());

    
    //9 9, 16/11
     //vector<Vector2D> positions = m_pathfinder->GetPathBetween(Vector2D(40, 40), Vector2D(-281, -281));
    //m_pCar->setPositionTo(Vector2D(getWaypoint(9, 9)->getPosition()->x, getWaypoint(9, 9)->getPosition()->y));
    
    
    //m_pathfinder->GetPathBetween(Vector2D(40, 40), Vector2D(-281, -281));
    
    //m_pCar->setPositionTo(Vector2D(getWaypoint(10, 10)->getPosition()->x, getWaypoint(10, 10)->getPosition()->y));
    
    /*for (int i = 0; i < positions.size(); i++)
    {
        m_pCar->setPositionTo(Vector2D(positions[i].x, positions[i].y));
    }*/
}

void AIManager::keyPress(WPARAM param)
{
    switch (param)
    {
        case VK_NUMPAD0:
        {
            OutputDebugStringA("0 pressed \n");
            
            //m_pCar->setPositionTo(Vector2D(m_waypoints[1]->getPosition()->x, m_waypoints[1]->getPosition()->y));
            //Pursuit
            m_pCar2->setPositionTo(Vector2D(m_pCar->getPosition()->x, m_pCar->getPosition()->y));
            break;
        }
        case VK_NUMPAD1:
        {
            //Flee
            OutputDebugStringA("1 pressed \n");
            Vector2D position = Vector2D(m_pCar->getPosition()->x -m_pCar2->getPosition()->x, m_pCar->getPosition()->y -m_pCar2->getPosition()->y );
            position.Normalize();
            position = position * m_pCar2->getCurrentSpeed() * - 2;
            m_pCar2->setPositionTo(position);
            break;
        }
        case VK_NUMPAD2:
        {
            //Seek
            OutputDebugStringA("2 pressed \n");
            Vector2D position = Vector2D(m_pCar->getPosition()->x - m_pCar2->getPosition()->x, m_pCar->getPosition()->y - m_pCar2->getPosition()->y);
            double distance = position.Length();
            if (distance > 0)
            {
                position.Normalize();
                position = position * (m_pCar2->getCurrentSpeed() * 4);
                m_pCar2->setPositionTo(position);
            }
            break;
        }
        case VK_NUMPAD3:
        {
            //Wander
            int randomPosx = rand() % 20;
            int randomPosy = rand() % 20;
            /*string str = std::to_string(randomPosx);
            string str2 = std::to_string(randomPosy);
            string str3 = str + " " + str2;
            OutputDebugStringA(str3.c_str());*/
            Waypoint* w = getWaypoint(randomPosx, randomPosy);
            if (Vector2D(m_pCar2->getPosition()->x, m_pCar2->getPosition()->y) == m_pCar2->getCurrentDest())
            {
                m_pCar2->setPositionTo(Vector2D(w->getPosition()->x, w->getPosition()->y));
                m_pCar2->setDest(Vector2D(w->getPosition()->x, w->getPosition()->y));
            }
            break;
        }
        case VK_NUMPAD4:
        {
            //Arrive
            OutputDebugStringA("2 pressed \n");
            Waypoint* w = getWaypoint(1, 1);
            Vector2D target = Vector2D(w->getPosition()->x, w->getPosition()->y);
            Vector2D position = Vector2D(target.x - m_pCar2->getPosition()->x, target.y - m_pCar2->getPosition()->y);
            setArrive(true);
            setDest(Vector2D(w->getPosition()->x, w->getPosition()->y));
            /*double distance = position.Length();
            if (distance > 0) //&& Vector2D(m_pCar2->getPosition()->x, m_pCar2->getPosition()->y) == m_pCar2->getCurrentDest())
            {
                string str = std::to_string(distance);
                string str3 = str + " \n";
                OutputDebugStringA(str3.c_str());

                position.Normalize();
                position = position * (m_pCar2->getCurrentSpeed() * 4);
                if (distance < 400.0f)
                {
                    //m_pCar2->setCurrentSpeed(0.5f);
                    
                }
                m_pCar2->setPositionTo(position);
                m_pCar2->setDest(Vector2D(w->getPosition()->x, w->getPosition()->y));
                //m_pCar2->setMaxSpeed(200.0f);
                
                
            }
            else
            {
                OutputDebugStringA("At location");
            }*/
           /* Vector2D position = Vector2D(m_pCar->getPosition()->x - m_pCar2->getPosition()->x, m_pCar->getPosition()->y - m_pCar2->getPosition()->y);
            double distance = position.Length();
            if (distance > 0)
            {
                position.Normalize();
                position = position * (m_pCar2->getCurrentSpeed() * 4);
                m_pCar2->setPositionTo(position);
            }*/
            break;
        }
        case VK_NUMPAD5:
        {
            //A*
            m_pathfinder = new PathFinder();
            vector<Vector2D> positions = m_pathfinder->GetPathBetween(Vector2D(getWaypoint(10, 10)->getPosition()->x, getWaypoint(10, 10)->getPosition()->y),
                Vector2D(getWaypoint(11, 15)->getPosition()->x, getWaypoint(11, 15)->getPosition()->y));

            vector<Vector2D> positions2 = m_pathfinder->GetPathBetween(Vector2D(getWaypoint(11, 15)->getPosition()->x, getWaypoint(11, 15)->getPosition()->y),
                Vector2D(getWaypoint(2, 8)->getPosition()->x, getWaypoint(2, 8)->getPosition()->y));

            positions.insert(positions.end(), positions2.begin(), positions2.end());

            vector<Vector2D> positions3 = m_pathfinder->GetPathBetween(Vector2D(getWaypoint(2, 8)->getPosition()->x, getWaypoint(2, 8)->getPosition()->y),
                Vector2D(getWaypoint(10, 10)->getPosition()->x, getWaypoint(10, 10)->getPosition()->y));

            positions.insert(positions.end(), positions3.begin(), positions3.end());

            m_pCar->setMove(true);
            m_pCar->setPath(positions);


            break;
        }
        // etc
        default:
            break;
    }
}

bool AIManager::checkForCollisions()
{
    if (m_pickups.size() == 0)
        return false;

    XMVECTOR dummy;

    // the car
    XMVECTOR carPos;
    XMVECTOR carScale;
    XMMatrixDecompose(
        &carScale,
        &dummy,
        &carPos,
        XMLoadFloat4x4(m_pCar->getTransform())
    );

    XMFLOAT3 scale;
    XMStoreFloat3(&scale, carScale);
    BoundingSphere boundingSphereCar;
    XMStoreFloat3(&boundingSphereCar.Center, carPos);
    boundingSphereCar.Radius = scale.x;

    // a pickup - !! NOTE it is only referring the first one in the list !!
    XMVECTOR puPos;
    XMVECTOR puScale;
    XMMatrixDecompose(
        &puScale,
        &dummy,
        &puPos,
        XMLoadFloat4x4(m_pickups[0]->getTransform())
    );

    XMStoreFloat3(&scale, puScale);
    BoundingSphere boundingSpherePU;
    XMStoreFloat3(&boundingSpherePU.Center, puPos);
    boundingSpherePU.Radius = scale.x;

    // test
    if (boundingSphereCar.Intersects(boundingSpherePU))
    {
        OutputDebugStringA("Collision!\n");
        return true;
    }

    return false;
}



