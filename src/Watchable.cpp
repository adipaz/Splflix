//
// Created by iris on 24/11/2019.
//

#include "../include/Watchable.h"
#include "../include/User.h"
#include "../include/Session.h"
#include <string>

using namespace std;


/***********************Watchable***********************/
Watchable::Watchable(long id, int length, const std::vector<std::string> &tags) : id(id), length(length), tags() {
    int size = tags.size();
    for(int i = 0; i < size; i++)
    {
        this->tags.push_back(tags[i]);
    }
}

int Watchable::getLength() { return this->length;}

int Watchable::getId() { return  this->id;}

std::vector<string>& Watchable::getTags() {return this->tags;}

Watchable::~Watchable() {
    this->tags.clear();
}

Watchable* Watchable::copy() {return nullptr;}

Watchable& Watchable::operator=(Watchable &other) {
    this->tags.clear();
    return *other.copy();
}




/*********************** Episode ***********************/
Episode::Episode(long id, const std::string &seriesName, int length, int season, int episode, const std::vector<std::string> &tags): Watchable(id,length,tags),
                                                                                                                                     seriesName(seriesName),  season(season),episode(episode),nextEpisodeId(this->getId() + 1) {


}


Watchable* Episode::getNextWatchable(Session &session) const {
    //check if the next episode exists
   if(this->nextEpisodeId!=-1)
       return  session.getContent()[this->nextEpisodeId-1];
    //otherwise
    return session.getActiveUser()->getRecommendation(session);
}

std::string Episode::toString() const {
    string msg = this->seriesName + "S0" + to_string(this->season) + "E0" + to_string(this->episode);
    return msg;
}

void Episode::setNextEpisodeId(int nextId) { this->nextEpisodeId = nextId;}

Watchable* Episode::copy() {
    return new Episode(*this);
}


Episode::Episode(Episode &other) :  Watchable(other.getId(),other.getLength(),other.getTags()), seriesName(other.seriesName), season(other.season), episode(other.episode), nextEpisodeId(other.nextEpisodeId){

}





/***********************Movie***********************/
Movie::Movie(long id, const string &name, int length, const vector<std::string> &tags) : Watchable(id,length,tags), name(name){
}

Watchable* Movie::getNextWatchable(Session &session) const {
    return session.getActiveUser()->getRecommendation(session);
}

std::string Movie::toString() const {
    return this->name;
}


void Movie::setNextEpisodeId(int nextId) {}

Watchable* Movie::copy() {
    return new Movie(*this);
}


Movie::Movie(Movie &other)  :  Watchable(other.getId(),other.getLength(),other.getTags()), name(other.name){
}
