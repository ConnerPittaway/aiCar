#pragma once

#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "Pathfinder.h"
#include <vector>

using namespace std;

class Vehicle;
class DrawableGameObject;
class PickupItem;
class Waypoint;

typedef vector<DrawableGameObject*> vecDrawables;
typedef vector<Waypoint*> vecWaypoints;
typedef vector<PickupItem*> vecPickups;

class AIManager
{
public:

	HRESULT initialise(ID3D11Device* pd3dDevice);
	void	update(const float fDeltaTime);
	void	mouseUp(int x, int y);
	void	keyPress(WPARAM param);
	Waypoint* getWaypoint(const int x, const  int y);
	void setArrive(bool a) { m_ArrivingCar = a; }
	bool getArrive() const { return m_ArrivingCar; }

	void setDest(Vector2D dest) { m_Dest = dest; }
	Vector2D getDest() const { return m_Dest; }
protected:
	bool	checkForCollisions();

private:
	vecWaypoints            m_waypoints;
	vecPickups              m_pickups;
	Vehicle*				m_pCar = nullptr;
	Vehicle*				m_pCar2 = nullptr;
	PathFinder*				m_pathfinder = nullptr;
	int						m_laps;
	bool					m_ArrivingCar;
	Vector2D				m_Dest;
};

