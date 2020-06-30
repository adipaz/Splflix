#ifndef SESSION_H_
#define SESSION_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"
using namespace std;
class User;
class Watchable;

class Session{
public:
    Session(const std::string &configFilePath);
    ~Session();
    void start();
    //our addition
    string* getActionInput();
    bool isUserExists(string& userName);
    void addNewUser(string& userName, User* user);
    bool deleteUser(string& userName);
    User*& getUser(string& userName);
    User*& getActiveUser();
    vector<Watchable*>& getContent();
    vector<BaseAction*>& getActionsLog();
    void setActiveUser(string& userName);
    void addContentToHistory(Watchable& content);
    void setInputForWatch(int id);
    // Copy Assignment
    Session& operator=(const Session &other);
    void copy(vector<Watchable*> content, vector<BaseAction*> actionsLog, unordered_map<std::string,User*> userMap, User* activeUser);
    void clear();
    Session& operator=(Session &&other);//move operator
    Session(Session &&other);//move c'tor
    Session(const Session& other);//copy c'tor

private:
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    string* actionInput; //size = 2
};
#endif
