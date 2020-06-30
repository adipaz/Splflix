#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>
class Watchable;
class Session;
using namespace std;

class User{
public:
    User(const std::string& name);
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    //our addition
    User(User& other);
    virtual ~User(); //delete the vector's content
    int getIndex();
    void setIndex(int i);
    void addToHistory(Watchable& content);
    void setName(string name);
    virtual User* copy();
    virtual User* copy(User* other);
    void clear();
    User& operator=(User& other);
    bool isExistsInHistory(Watchable& other);
protected:
    std::vector<Watchable*> history;
private:
    std::string name;
    //our addition
    int index;
};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //our addition
    virtual User* copy();
    LengthRecommenderUser(User& other);//copy ctor

};



class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //our addition
    virtual User* copy();
    RerunRecommenderUser(User& other);//copy ctor
    virtual User* copy(User* other);
private:

};




class GenreRecommenderUser : public User {
public:
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    //our addition
    virtual User* copy();
    GenreRecommenderUser(User& other);//copy ctor
    virtual User* copy(User* other);
private:
    //our addition
    vector<pair<string,int>> sortTags(map<string,int>& map);

};

#endif
