#pragma once
#include "DrawableGameObject.h"
#include "Vector2D.h"

//#define 

class Vehicle : public DrawableGameObject
{
public:
	virtual HRESULT initMesh(ID3D11Device* pd3dDevice);
	virtual void update(const float deltaTime);

	void setMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	void setCurrentSpeed(const float speed); // a ratio: a value between 0 and 1 (1 being max speed)
	void setPositionTo(Vector2D positionTo); // a position to move to
	void setVehiclePosition(Vector2D position); // the current position - this resets positionTo
	void setDest(Vector2D position);
	float getCurrentSpeed() { return m_currentSpeed; }
	float getMaxSpeed() { return m_maxSpeed; }
	Vector2D getCurrentDest() { return m_destination; }

	bool getMove() { return m_isMoving; }
	void setMove(bool move) { m_isMoving = move; }

	vector<Vector2D> getPath() { return m_Positions; }
	void setPath(vector<Vector2D> path) { m_Positions = path; }

	int getCurrentPathPos() { return m_CurrentPath; }
	void setCurrentPathPos(int cPath) { m_CurrentPath = cPath; }

protected:
	float m_maxSpeed;
	float m_currentSpeed;
	
	Vector2D m_currentPosition;
	Vector2D m_startPosition;
	Vector2D m_positionTo;
	Vector2D m_lastPosition;
	Vector2D m_destination;
	vector<Vector2D> m_Positions;
	bool m_isMoving;

	int m_CurrentPath;

};

