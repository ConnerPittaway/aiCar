#pragma once
#include "DrawableGameObject.h"
class Waypoint :
    public DrawableGameObject
{
public:
    virtual HRESULT initMesh(ID3D11Device* pd3dDevice, const int index);

    bool isOnTrack() { return m_isOnTrack; }
    bool isCheckPoint() { return m_isCheckpoint; }
    void setOnTrack(bool track) { m_isOnTrack = track; }
    void setVisible(const bool visible) { m_visible = visible; }
    bool isVisible() {return m_visible;}
    
    int getX() { return m_x; }
    void setX(int x) { m_x = x; }

    int getY() { return m_y; }
    void setY(int y) { m_y = y; }

    void SetTrack(int index);
private:
    bool m_isOnTrack;
    bool m_isCheckpoint;
    bool m_visible;
    int m_x;
    int m_y;
};

