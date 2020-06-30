#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>
#include "../include/Action.h"
using namespace std;

class Session;

class Watchable{
public:
    Watchable(long id, int length, const std::vector<std::string>& tags);
    virtual ~Watchable();
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;

    //our addition
    int getLength();
    int getId();
    std::vector<string>& getTags();
    virtual void setNextEpisodeId(int nextId) = 0;
    virtual Watchable* copy();
    virtual Watchable& operator=(Watchable &other);


private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, const std::string& name, int length, const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //our addition
    virtual Watchable* copy();
    virtual void setNextEpisodeId(int nextId);
private:
    std::string name;
    Movie(Movie& other); //copy ctor
};


class Episode: public Watchable{
public:
    Episode(long id, const std::string& seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    //our addition
    virtual void setNextEpisodeId(int nextId);
    virtual Watchable* copy();

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
    Episode(Episode& other);//copy ctor
};

#endif
