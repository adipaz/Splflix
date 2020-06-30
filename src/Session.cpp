//
// Created by iris on 24/11/2019.
//
#include "../include/Session.h"
#include "../include/json.hpp"
#include "../include/User.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "../include/Watchable.h"
using namespace std;
using namespace nlohmann;
using json = nlohmann::json;

Session::Session(const std::string &configFilePath) : content(), actionsLog(), userMap(),activeUser(), actionInput(){
    ifstream json_file(configFilePath);
    nlohmann::json json;
    json_file >> json;

    //delete this->activeUser;
    this->activeUser = new LengthRecommenderUser("default");
    this->userMap["default"] = this->activeUser;

    int i = 0;
    for(; json["movies"][i] != nullptr; i++)
    {
        vector<string> tags;
        for(int j = 0; json["movies"][i]["tags"][j] != nullptr; j++)
        {
            tags.push_back(json["movies"][i]["tags"][j]);
        }
        Watchable* w = new Movie(i,json["movies"][i]["name"],json["movies"][i]["length"],tags);
        this->content.push_back(w);
    }
    int id = i+1;
    for(int k = 0; json["tv_series"][k] != nullptr; k++)
    {
        vector<string> tags2;
        for(int j = 0; json["tv_series"][k]["tags"][j] != nullptr; j++)
        {
            tags2.push_back(json["tv_series"][k]["tags"][j]);
        }
        int size = json["tv_series"][k]["seasons"].size();
        for(int j = 0; j < size; j++)//seasons in every series
        {
            for(int l = 1; l <= json["tv_series"][k]["seasons"][j]; l++,id++)//episodes in each season
            {
                Watchable* e = new Episode(id, json["tv_series"][k]["name"], json["tv_series"][k]["episode_length"], j+1, l, tags2) ;
                int length = json["tv_series"][k]["seasons"].size()-1;
                if((j == length) & (l == json["tv_series"][k]["seasons"][j])) //last episode in season - no next Episode
                {
                    e->setNextEpisodeId(-1);
                }
                this->content.push_back(e);

            }

        }
    }
}
string* Session::getActionInput() {return this->actionInput;}

User*& Session::getActiveUser() { return this->activeUser;}

vector<Watchable*>& Session::getContent() {return this->content;}

User*& Session::getUser(string &userName) {return this->userMap[userName];}

void Session::addNewUser(string &userName, User *user) {
    this->userMap.insert(pair<string,User*>(userName,user));
}

bool Session::isUserExists(string &userName) {
    unordered_map<string,User*>::const_iterator it =this->userMap.find (userName);
    if ( it != this->userMap.end() )
        return true;
    return false;
}

bool Session::deleteUser(string &userName) {
    delete this->userMap[userName];
    return this->userMap.erase(userName);
}

vector<BaseAction*>& Session::getActionsLog() {return this->actionsLog;}

void Session::setActiveUser(string &userName) {this->activeUser = this->userMap[userName];}

void Session::addContentToHistory(Watchable &content) {this->activeUser->get_history().push_back(&content);}

Session::~Session() {
    clear();
}

void Session::setInputForWatch(int id) {
    this->actionInput[0]=to_string(id);
}

void Session::start()
{
        if(this->actionInput == nullptr)
            this->actionInput = new string[2];
        string userInput;
        BaseAction* action;
        cout << "SPLFLIX is now on!" << endl;
        while(userInput.compare("exit") != 0)
        {
            getline(cin,userInput);

            string input_split[3]; //first place will always hold the command the other two places
            istringstream iss(userInput);
            int index = 0;
            for(string s; iss >> s;){
                input_split[index] = s;
                index++;
            }

            if(input_split[0].compare("createuser") == 0)
            {
                this->actionInput[0] = input_split[1];//user name
                this->actionInput[1] = input_split[2];//algorithm

                action=new CreateUser();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("changeuser") == 0)
            {
                this->actionInput[0] = input_split[1];//user name
                this->actionInput[1] = " ";

                action=new ChangeActiveUser();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("deleteuser") == 0)
            {
                this->actionInput[0] = input_split[1];//user name
                this->actionInput[1] =  " ";

                action=new DeleteUser();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("dupuser") == 0)
            {
                this->actionInput[0] = input_split[1];//original user name
                this->actionInput[1] = input_split[2];//new user name

                action=new DuplicateUser();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("content") == 0)
            {
                this->actionInput[0] = " ";
                this->actionInput[1] = " ";

                action=new PrintContentList();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("watchhist") == 0)
            {
                this->actionInput[0] =  " ";
                this->actionInput[1] =  " ";

                action=new PrintWatchHistory();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
            else if(input_split[0].compare("watch") == 0)
            {
                this->actionInput[0] = input_split[1];//content id
                this->actionInput[1] =  " ";

                action=new Watch();
                action->act(*this);
                //this->actionsLog.push_back(action);
                delete action;
            }
            else if(input_split[0].compare("log") == 0)
            {
                this->actionInput[0] =  " ";
                this->actionInput[1] =  " ";

                action=new PrintActionsLog();
                action->act(*this);
                this->actionsLog.push_back(action);
            }
        }
        action = new Exit();
        action->setStatus(COMPLETED);
        this->actionsLog.push_back(action);
        //delete action;
}

Session& Session::operator=(const Session &other) {
    if (this != &other)
    {
        clear();
        copy(other.content, other.actionsLog, other.userMap, other.activeUser);
    }

    return *this;
}

void Session::copy(vector<Watchable *> content, vector<BaseAction *> actionsLog,
                   unordered_map<std::string, User *> userMap, User* activeUser) {
    int size = content.size();
    for(int i = 0; i < size; i++)
    {
        this->content.push_back(content.at(i)->copy());
    }
    size = actionsLog.size();
    for(int i = 0; i < size; i++)
    {
        this->actionsLog.push_back(actionsLog.at(i)->copy());
    }
    for(unordered_map<string,User*>::iterator it = userMap.begin(); it != userMap.end(); it++)
    {
        this->userMap.insert(pair<string,User*>(it->first, userMap[it->first]->copy()));
    }

    this->activeUser = this->userMap[activeUser->getName()];


}

void Session::clear() {
    delete[] this->actionInput;
    /*this->userMap.erase(this->activeUser->getName());
    delete this->activeUser;*/

    //delete the vectors
    for (auto p : this->content)
    {
        delete p;
    }
    this->content.clear();

    for (auto p : this->actionsLog)
    {
        delete p;
    }
    this->actionsLog.clear();

    //delete the map
    for(unordered_map<std::string, User*>::iterator itr = this->userMap.begin(); itr != this->userMap.end(); itr++)
    {
        delete itr->second;
    }
    //delete this->activeUser;
}

//copy c'tor
Session::Session(const Session &other) : content(), actionsLog(), userMap(), activeUser(), actionInput() {
    this->copy(other.content,other.actionsLog, other.userMap, other.activeUser);
}

//move c'tor
Session::Session(Session &&other): content(other.content),actionsLog(other.actionsLog), userMap(other.userMap), activeUser(other.activeUser), actionInput(){
    int size = other.content.size();
    for(int i = 0; i < size; i++)
    {
        other.content.at(i) = nullptr;
    }
    size = other.actionsLog.size();
    for(int i = 0; i < size; i++)
    {
        other.actionsLog.at(i) = nullptr;
    }
    for(auto elem : other.userMap)
    {
        other.userMap[elem.first] = nullptr;
    }
    other.activeUser = nullptr;
    other.actionInput = nullptr;
    //other.actionInput = nullptr;

}

Session& Session::operator=(Session &&other) {

    if (this != &other)
    {
        this->clear();
        this->content = other.content;
        this->actionsLog = other.actionsLog;
        this->userMap = other.userMap;
        this->activeUser = other.activeUser;
        //this->actionInput = other.actionInput;
        int size = other.content.size();
        for(int i = 0; i < size; i++)
        {
            other.content.at(i) = nullptr;
        }
        size = other.actionsLog.size();
        for(int i = 0; i < size; i++)
        {
            other.actionsLog.at(i) = nullptr;
        }
        for(auto elem : other.userMap)
        {
            other.userMap[elem.first] = nullptr;
        }
        other.activeUser = nullptr;
        other.actionInput = nullptr;
    }
    return *this;
}

