#include <time.h>
#include "string.h"

#include "Field.h"

Field::Field() {
  for (int i = 0; i < height - 2; i++) {
    for (int j = 0; j < width - 2; j++) {
      dataRows[i][j] = false;
    }
  }

  for (int i = 0; i < height - 2; i++) {
    dataShells[i][0] = -1;
    dataShells[i][1] = -1;
  }

  points = 0;

  loseTrigger = false;
  menuTrigger = true;
  gameTrigger = false;
  tabTrigger = false;
  gameState = false;

  sizeField = 2;
  difficulty = 1;

  menuState = 0;
  loseState = 0;

  Name[0] = 65;
  Name[1] = 65;
  Name[2] = 65;
}

Field::~Field() {}

//ищем свободный ряд(пустые с координатами -1, -1), кода находи записываем туда соотвествющие координаты снаряда
void Field::createShell(int _x) {
  for (int i = 0; i < height - 2; i++) {
    if (dataShells[i][0] == -1) {
      dataShells[i][0] = _x;
      dataShells[i][1] = height - 2;
      break;
    }
  }
}

void Field::deleteShell(int pos) {
  dataShells[pos][0] = -1;
  dataShells[pos][1] = -1;
}

//генерация происходит путём руандомизации, также проверяем не полная ли строка получилась, иначе делаём пропуск.  
void Field::createRow() {
  srand(time(NULL));
  int t = 0;
  for (int j = 0; j < width - 2; j++) {
    dataRows[0][j] = random() % 2;
    if (dataRows[0][j] == 1) {
      t++;
    }
  }
  if (t == width - 2) {
    dataRows[0][random() % 18] = false;
  }
}

//в нижний ряд записываем верхний(двигаемся с низу вверх), потом создаем строку на 1 ряду, а такжн проверяем не проирал ли игрок
void Field::fallingRow() {
  for (int i = height - 3; i > 0; i--) {
    for (int j = 0; j < width - 2; j++) {
      dataRows[i][j] = dataRows[i - 1][j];
    }
  }

  createRow();
  if (checkLose()) {
    lose();
  }
}

//зануляем строку соотвествующию строку, такце виличиваем счётчик очков на 1
void Field::deleteRow(int _y) {
  for (int j = 0; j < width - 2; j++) {
    dataRows[_y][j] = false;
  }
  points++;
}

//проверяем нет ли блока в ряду выше 
bool Field::checkCollision(int _x, int _y) {
  return (dataRows[--_y][_x] == true);
}

//полёт осуществляется путём уменьшения координаты y, а также проверяем не было ди столкновения, иначе удаляем снаряд и добавляем блок в ряд, и проверяем не полный ли ряд, очищаем его
void Field::flightShells() {
  for (int i = 0; i < height - 2; i++) {
    if (dataShells[i][0] != -1) {
      dataShells[i][1]--;
      if ((checkCollision(dataShells[i][0], dataShells[i][1])) || (dataShells[i][1] == 0)) {
        changeRow(dataShells[i][0], dataShells[i][1]);
        if (checkRow(dataShells[i][1])) {
          deleteRow(dataShells[i][1]);
          rewriteRows(dataShells[i][1]);
        }
        deleteShell(i);
      }
      if (checkLose()) {
        lose();
      }
    }
  }
}

void Field::changeRow(int _x, int _y) {
  dataRows[_y][_x] = true;
}

//не полная ли строка
bool Field::checkRow(int _y) {
  bool t = true;
  for (int j = 0; j < width - 2; j++) {
    if (dataRows[_y][j] == false) {
      t = false;
      break;
    }
  }
  return t;
}

//проверяем не пустая ли нижняя строка
bool Field::checkLose() {
  bool t = false;
  for (int j = 0; j < width - 2; j++) {
    if (dataRows[height - 3][j] == 1) {
      t = true;
      break;
    }
  }
  return t;
}

void Field::lose() {
  gameTrigger = false;
  loseTrigger = true;
  gameState = false;
  loseState = 0;
  Name[0] = 65;
  Name[1] = 65;
  Name[2] = 65;
}

void Field::newGame() {
  for (int i = 0; i < height - 2; i++) {
    for (int j = 0; j < width - 2; j++) {
      dataRows[i][j] = false;
    }
  }
  for (int i = 0; i < height - 2; i++) {
    dataShells[i][0] = -1;
    dataShells[i][1] = -1;
  }
  points = 0;
  menuTrigger = false;
  gameTrigger = true;
  gameState = true;
}

void Field::rewriteRows(int _y) {
  for (int i = _y; i < height - 3; i++) {
    for (int j = 0; j < width - 2; j++) {
      dataRows[i][j] = dataRows[i + 1][j];
    }
  }
}

void Field::toMenu() {
  loseTrigger = false;
  menuTrigger = true;
}

void Field::plusMenuState() {
  ++menuState;
  if (menuState > 4) {
    menuState = 0;
  }
}

void Field::minusMenuState() {
  --menuState;
  if (menuState < 0) {
    menuState = 4;
  }
}

void Field::toGame() {
  menuTrigger = false;
  gameTrigger = true;
}

void Field::plusSizeField() {
  ++sizeField;
  if (sizeField > 2) {
    sizeField = 1;
  }
}

void Field::plusDifficulty() {
  ++difficulty;
   if (difficulty > 3) {
    difficulty = 1;
  }
}

void Field::plusLoseState() {
  ++loseState;
}

void Field::plusName(int i) {
  ++Name[i];
  if (Name[i] > 90) {
    Name[i] = 65;
  }
}

void Field::minusName(int i) {
  --Name[i];
  if (Name[i] < 65) {
    Name[i] = 90;
  }
}

const int Field::getDataShellsX(int _i) {
  return dataShells[_i][0];
}

const int Field::getDataShellsY(int _i) {
  return dataShells[_i][1];
}

const bool Field::getDataRows(int _x, int _y) {
  return dataRows[_x][_y];
}

const int Field::getPoints() {
  return points;
}

const bool Field::getLoseTrigger() {
  return loseTrigger;
}

const bool Field::getMenuTrigger() {
  return menuTrigger;
}

const bool Field::getGameTrigger() {
  return gameTrigger;
}

const bool Field::getTabTrigger() {
  return tabTrigger;
}

const int Field::getSizeField() {
  return sizeField;
}

const int Field::getDifficulty() {
  return difficulty;
}

const int Field::getMenuState() {
  return menuState;
}

const bool Field::getGameState() {
  return gameState;
}

const int Field::getLoseState() {
  return loseState;
}

const char Field::getName(int i) {
  return Name[i];
}
