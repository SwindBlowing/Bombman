#include "mapoperator.h"

#include <QQueue>
#include <qDebug>
#include <QTimer>
#include <QPainter>
#include <QSoundEffect>
#include <imagetransform.h>

MapOperator::MapOperator()
{
    memset(flag, 0, sizeof(flag));
    for (int i = 0; i < 4; i++)
        paint[i] = new QGraphicsTextItem();
}

void MapOperator::setPlayerProps(int x, int y, GameObject *obj)
{
    PlayerProps[x][y] = obj;
}

void MapOperator::clear()
{
    Q.clear();
    firstUpdate =1;
    nowTime = 0;
    for (int i = 1; i <= 20; i++)
        for (int j = 1; j <= 15; j++) {
            M[i][j] = nullptr;
            P[i][j] = nullptr;
        }
    memset(flag, 0, sizeof(flag));
    memset(vis, 0, sizeof(vis));
    memset(num, 0, sizeof(num));
    memset(initialMap, 0, sizeof(initialMap));
    bombs.clear();
    bombsToAttach.clear();
    players.clear();
    assert(players.empty());
}

void MapOperator::onUpdate(float deltaTime)
{
    if (firstUpdate) {
        firstUpdate = 0;
        for (int i = 1; i <= 4; i++) {
            paint[i - 1]->setPlainText(QString("%1").arg(num[i]));
            paint[i - 1]->setTextWidth(300);
            paint[i - 1]->setPos(zoneX[i], zoneY[i]);
            paint[i - 1]->setScale(2);
        }
    }
    for (auto player : players) {
        int id = player->getId();
        if (num[id] != player->returnCredit()) {
            num[id] = player->returnCredit();
            paint[id - 1]->setPlainText(QString("%1").arg(num[id]));
        }
    }
    if (players.size() <= 1)
        this->gameObject->gameOver();
    nowTime += deltaTime * 1000;
    while (!Q.empty()) {
        tmp2 now = Q.front();
        if (nowTime - now.startTime > 800) {
            Q.pop_front();
            endBombing(now.x, now.y);
        }
        else break;
    }
    if (!bombsToAttach.empty()) {
        for (auto bomb : bombsToAttach)
            bombs.emplace_back(bomb);
        for (auto bomb : bombsToAttach)
            bombsToAttach.removeAll(bomb);
    }
}

void MapOperator::addItems(int x, int y, GroundItem *gnd)
{
    M[x][y] = gnd;
}

bool MapOperator::canVisit(int x, int y)
{
    return check(x, y) && M[x][y]->canVisit();
}

bool MapOperator::cannotBomb(int x, int y)
{
    if (!check(x, y) || M[x][y] == nullptr) return true;
    return M[x][y]->getType() == 3;
}

bool MapOperator::check(int x, int y)
{
    return x > 0 && x <= 20 && y > 0 && y <= 15;
}

bool MapOperator::startBombing(int x, int y, QString st)
{
    flag[x][y]++;
    Q.push_back((tmp2){x, y, nowTime});
    return M[x][y]->receiveBombing(st);
}
void MapOperator::endBombing(int x, int y)
{
    flag[x][y]--;
    M[x][y]->endBombing();
}

bool MapOperator::isBombing(int x, int y)
{
    return flag[x][y];
}

void MapOperator::dfs(int x, int y)
{
    vis[x][y] = 1;
    for (int k = 0; k < 4; k++)
        if (!vis[x + dx[k]][y + dy[k]] && check(x + dx[k], y + dy[k]) && initialMap[x + dx[k]][y + dy[k]] != 3)
            dfs(x + dx[k], y + dy[k]);
    return ;
}

bool MapOperator::checkMap()
{
    memset(vis, 0, sizeof(vis));
    dfs(1, 1);
    if (vis[20][15] && vis[1][15] && vis[20][1]) return 1;
    return 0;
}

void MapOperator::addProps(int x, int y, Props *prop)
{
    P[x][y] = prop;
}

bool MapOperator::isProp(int x, int y)
{
    return P[x][y] != nullptr;
}

void MapOperator::delP(int x, int y)
{
    P[x][y]->del();
    P[x][y] = nullptr;
}

short MapOperator::propType(int x, int y)
{
    return P[x][y]->getType();
}

void MapOperator::addBombs(GameObject *bomb)
{
    bombsToAttach.emplace_back(bomb);
}

void MapOperator::detachBombs(GameObject *bomb)
{
    bombs.removeAll(bomb);
}

void MapOperator::pushBombs(int x, int y, int towards)
{
    for (auto bomb : bombs) {
        if (bomb->getComponent<Bomb>()->getX() == x && bomb->getComponent<Bomb>()->getY() == y && bomb->getComponent<Bomb>()->canBePushed())
            bomb->getComponent<Bomb>()->startMove(towards);
    }
}

void MapOperator::addPlayers(Player *player)
{
    players.emplace_back(player);
}

void MapOperator::detachPlayers(Player *player)
{
    players.removeAll(player);
}

QPoint MapOperator::getEnemy(int x, int y)
{
    short minDis = 10000;
    QPoint point = QPoint(0, 0);
    for (auto player : players) {
        short nowx = player->getX(), nowy = player->getY();
        short nowDis = abs(nowx - x) + abs(nowy - y);
        if (nowDis && nowDis < minDis) {
            point = QPoint(nowx, nowy);
            minDis = nowDis;
        }
    }
    return point;
}

short MapOperator::getVal(int x, int y, int id)
{
    int val = 0;
    if (!check(x, y) || isBombing(x, y)) return 20000;
    if (M[x][y]->getType() == 2 || M[x][y]->getType() == 3) return 10000;
    for (auto player1 : players)
        if (id != player1->getId() && player1->getX() == x && player1->getY() == y)
            return 10000;
    if (P[x][y] != nullptr) val -= 75;
    for (auto bomb : bombs) {
        short nowx = bomb->getComponent<Bomb>()->getX(), nowy = bomb->getComponent<Bomb>()->getY();
        short nowPower = bomb->getComponent<Bomb>()->getPower();
        for (int k = 0; k <= nowPower; k++) {
            if ((x == nowx && abs(y - nowy) == k) || (y == nowy && abs(x - nowx) == k))
                val += (5 - k) * 25;
        }
    }
    return val;
}

bool MapOperator::willBeBombed(int x, int y)
{
    for (auto bomb : bombs) {
        short nowx = bomb->getComponent<Bomb>()->getX(), nowy = bomb->getComponent<Bomb>()->getY();
        if ((x == nowx && abs(y - nowy) <= bomb->getComponent<Bomb>()->getPower()) || (y == nowy && abs(x - nowx) <= bomb->getComponent<Bomb>()->getPower()))
            return 1;
    }
    return 0;
}

short MapOperator::shortestPath(int s, int t, int x, int y)
{
    QQueue<tmp> Q;
    while (!Q.empty()) Q.pop_front();
    memset(vis, 0, sizeof(vis));
    vis[s][t] = 1;
    int beg = rand() % 4;
    for (int i = 0; i < 4; i++) {
        int k = (beg + i) % 4;
        if (check(s + dx[k], t + dy[k]) && M[s + dx[k]][t + dy[k]]->getType() != 3) {
            Q.push_back((tmp){{s + dx[k], t + dy[k]}, k});
            vis[s + dx[k]][t + dy[k]] = 1;
        }
    }
    while (!Q.empty()) {
        tmp now = Q.front();
        Q.pop_front();
        if (now.location.first == x && now.location.second == y) return now.fromPoint;
        for (int k = 0; k < 4; k++) {
            int nowx = now.location.first, nowy = now.location.second;
            if (check(nowx + dx[k], nowy + dy[k]) && !vis[nowx + dx[k]][nowy + dy[k]] && M[nowx + dx[k]][nowy + dy[k]]->getType() != 3) {
                Q.push_back((tmp){{nowx + dx[k], nowy + dy[k]}, now.fromPoint});
                vis[nowx + dx[k]][nowy + dy[k]] = 1;
            }
        }
    }
    return -1;
}

bool MapOperator::existSoftWallOrPlayers(Player *player, short towards)
{
    short x = player->getX(), y = player->getY(), len = player->getPower();
    for (int i = 1; i <= len; i++)
        for (int k = towards; k <= towards; k++)
            if (check(x + i * dx[k], y + i * dy[k]) && M[x + i * dx[k]][y + i * dy[k]]->getType() == 2)
                return 1;
    for (auto player1 : players)
        if (player1 != player){
            if ((player1->getX() == x && abs(player1->getY() - y) <= len)
                    || (player1->getY() == y && abs(player1->getX() - x) <= len))
                return 1;
        }
    return 0;
}

void MapOperator::getInit(int x, int y, int type)
{
    initialMap[x][y] = type;
}

int MapOperator::winner()
{
    int maxn = num[1], win = 1;
    for (int i = 2; i <= 4; i++)
        if (num[i] > maxn) maxn = num[i], win = i;
    for (int i = 1; i <= 4; i++)
        if (num[i] == maxn && win != i) return 5;
    return win;
}

void MapOperator::playDeadSound()
{
    auto deadSound = new QSoundEffect(this);
    deadSound->setSource(QUrl::fromLocalFile(QString(":/new/Sound/Dead.wav")));
    deadSound->play();
}

void MapOperator::dealWithPropsShow(int id, int type, int op)
{
    if (op) PlayerProps[id - 1][type - 1]->getComponent<ImageTransform>()->setImage(QString(":/Image/prop%1.png").arg(type));
    else PlayerProps[id - 1][type - 1]->getComponent<ImageTransform>()->setImage(QString(":/Image/GroundType1.png"));
}
