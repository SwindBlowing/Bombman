#ifndef MAPOPERATOR_H
#define MAPOPERATOR_H

#include <component.h>
#include <gameobject.h>
#include <transform.h>
#include <imagetransform.h>
#include <grounditem.h>
#include <props.h>
#include <bomb.h>

#include <QQueue>
#include <QList>

class MapOperator: public Component
{
    struct tmp2
    {
        int x, y;
        float startTime = 0;
    };

    struct tmp
    {
        std::pair<int, int> location;
        int fromPoint;
    };

public:
    MapOperator();

    void onUpdate(float deltaTime) override;

    void addItems(int x, int y, GroundItem *gnd);
    bool canVisit(int x, int y);
    void getInit(int x, int y, int type);

    bool cannotBomb(int x, int y);
    bool check(int x, int y);
    bool startBombing(int x, int y, QString st);
    void endBombing(int x, int y);
    bool isBombing(int x, int y);

    bool checkMap();
    void dfs(int x, int y);

    void addProps(int x, int y, Props *prop);
    bool isProp(int x, int y);
    void delP(int x, int y);
    short propType(int x, int y);

    void addBombs(GameObject *bomb);
    void pushBombs(int x, int y, int towards);
    void detachBombs(GameObject *bomb);

    void addPlayers(Player *player);
    void detachPlayers(Player *player);

    QPoint getEnemy(int x, int y);
    short getVal(int x, int y, int id);
    bool willBeBombed(int x, int y);
    short shortestPath(int s, int t, int x, int y);
    bool existSoftWallOrPlayers(Player *player, short towards);

    void clear();
    int winner();
    QGraphicsTextItem *paint[4] = {nullptr};

    void playDeadSound();

    void setPlayerProps(int x, int y, GameObject *obj);
    void dealWithPropsShow(int id, int type, int op);

private:
    GroundItem *M[25][25] = {nullptr};
    Props *P[25][25] = {nullptr};
    const int dx[4] = {0, 1, 0, -1}, dy[4] = {-1, 0, 1, 0};
    short flag[25][25] = {0};
    bool vis[25][25] = {0};
    QList<GameObject *> bombs;
    QList<GameObject *> bombsToAttach;
    QList<Player *> players;
    short initialMap[25][25] = {0};
    QQueue<tmp2> Q;
    float nowTime = 0;
    int num[5] = {0};
    bool firstUpdate = 1;

    const float zoneX[5] = {0, 870, 990, 870, 990}, zoneY[5] = {0, 195, 195, 305, 305};

    GameObject *PlayerProps[4][4] = {nullptr};
};

#endif // MAPOPERATOR_H
