#include "gameObjNode.h"
#include "gameTile.h"
#include "main.h"
#include "hdnesPackEditormainForm.h"

gameObjNode::gameObjNode()
{
    //ctor
    effectedByMove = false;
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
    objectWidth = 0;
    objectHeight = 0;
    bgColour = 0;
    isSprite = false;
}

gameObjNode::~gameObjNode()
{
    //dtor
}

void gameObjNode::addTile(gameTile g){
    tiles.push_back(g);
    addToObjectSize(tiles.size() - 1);
}

void gameObjNode::addToObjectSize(int gIdx){
    x1 = min(x1, tiles[gIdx].objCoordX);
    x2 = max(x2, tiles[gIdx].objCoordX + 8);
    y1 = min(y1, tiles[gIdx].objCoordY);
    y2 = max(y2, tiles[gIdx].objCoordY + 8);
    objectWidth = x2 - x1;
    objectHeight = y2 - y1;
}

void gameObjNode::clearAllTiles(){
    tiles.clear();
    x1 = 0;
    x2 = 0;
    y1 = 0;
    y2 = 0;
    objectWidth = 0;
    objectHeight = 0;
}

void gameObjNode::load(fstream& file, wxTreeItemId newItm){
    string line;
    string lineHdr;
    string lineTail;

    getline(file, line);
    while(line != "<endGameObject>"){
        size_t found = line.find_first_of(">");
        if(found!=string::npos){
            lineHdr = line.substr(0, found + 1);
            lineTail = line.substr(found + 1);
            if(lineHdr == "<type>"){
                nodeType = atoi(lineTail.c_str());
            }
            if(lineHdr == "<name>"){
                nodeName = lineTail;
            }
            if(lineHdr == "<childObjects>"){
                main::mForm->loadChildGameObjs(file, newItm);
            }
        }
        getline(file, line);
    }
}

void gameObjNode::save(fstream& file, wxTreeItemId newItm){
    file << "<gameObject>\n";
    file << "<type>" + main::intToStr(nodeType) + "\n";

    if(nodeType != GAME_OBJ_NODE_TYPE_ROOT){
        file << "<name>" + nodeName + "\n";
    }
    if(nodeType == GAME_OBJ_NODE_TYPE_OBJECT){
        /*
        file << "<tiles>\n";
        file << "<endTiles>\n";
        */
    }
    else{
        file << "<childObjects>\n";
        main::mForm->saveChildGameObjs(file, newItm);
        file << "<endChildObjects>\n";
    }
    file << "<endGameObject>\n";
}

