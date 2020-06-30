//
// Created by iris on 24/11/2019.
//

#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

/*****************User*****************/

User::User(const std::string &name) : history(), name(name), index(){}

std::string User::getName() const {return this->name;}

std::vector<Watchable*> User::get_history() const {return this->history;}

User::~User() {
   for (auto p : this->history)
    {
        //if(p != nullptr)
        delete p;
    }
    this->history.clear();
}

int User::getIndex() {
    return  this->index;
}

void User::setIndex(int i) {
    this->index=i;
}

void User::addToHistory(Watchable &content) {
    this->history.push_back(content.copy());
}

void User::setName(string name) {
    this->name=name;
}

User* User::copy() {
    return nullptr;
}

User* User::copy(User* other) {
    return nullptr;
}

void User::clear() {
    for(auto elem : this->history)
    {
        delete elem;
    }
    history.clear();
}

User &User::operator=(User &other) {
    if(this != &other)
    {
        clear();
    }
    return *other.copy();
}

User::User(User &other): history(),name(other.name),index(other.getIndex())  {
    int size = other.get_history().size();
    for(int i = 0; i < size; i++)
    {
        this->history.push_back(other.get_history().at(i)->copy());
    }
}

bool User::isExistsInHistory(Watchable &other) {
    for(auto elem : this->history)
    {
        if(elem->getId()==other.getId())
            return true;
    }
    return false;
}

/*****************LengthRecommenderUser *****************/

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) : User(name){}

LengthRecommenderUser::LengthRecommenderUser(User &other) : User(other){}



Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    //calculating the average of the contents' length
    int size = this->get_history().size();
    double avg = 0.0, minDistanceFromAvg = numeric_limits<double>::max(), distance = 0.0;
    int closest = 0, j = -1;
    for(int i = 0; i < size; i++)
    {
        avg += this->get_history().at(i)->getLength();
    }
    avg /= size;
    size = s.getContent().size();
    //finding the closest movie/episode by length to the average
    for(int i = 0; i < size; i++)
    {
        if(!isExistsInHistory(*s.getContent()[i])) //didn't watch the content yet
        {
            distance = abs(s.getContent()[i]->getLength() - avg);
            if(minDistanceFromAvg > distance)
            {
                if(s.getContent()[i]->getLength() > closest)
                {
                    j = i;
                    closest = s.getContent().at(i)->getLength();
                    minDistanceFromAvg = distance;
                }
            }

        }
    }
    if(j == -1)//didn't find any movie/episode
        return nullptr;
    return s.getContent()[j];
}

User* LengthRecommenderUser::copy() {
 /*   LengthRecommenderUser* temp = new LengthRecommenderUser(this->getName());
    for(auto elem : this->get_history())
    {
        temp->history.push_back(elem->copy());
    }
    temp->setIndex(this->getIndex());
    return temp;*/
    return new LengthRecommenderUser(*this);

}

/*****************RerunRecommenderUser*****************/
RerunRecommenderUser::RerunRecommenderUser(const std::string &name) : User(name){
    this->setIndex(-1);
}

RerunRecommenderUser::RerunRecommenderUser(User &other) : User(other){}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {

    //the user didn't watch any content
    if(this->history.size()==0)
        return nullptr;
    //otherwise
    else if(this->getIndex()==-1)
        this->setIndex(0);
    else
        this->setIndex((this->getIndex()+1)%(this->history.size()));
    return this->history[this->getIndex()];

}


User* RerunRecommenderUser::copy() {
    /*RerunRecommenderUser* temp = new RerunRecommenderUser(this->getName());
    for(auto elem : this->get_history())
    {
        temp->history.push_back(elem->copy());
    }
    temp->setIndex(this->getIndex());
    return temp;*/
    return new RerunRecommenderUser(*this);
}


User* RerunRecommenderUser::copy(User* other) {
    other = new RerunRecommenderUser(*this);
    return other;
}


/*****************GenreRecommenderUser*****************/
GenreRecommenderUser::GenreRecommenderUser(const std::string &name) : User(name){}

GenreRecommenderUser::GenreRecommenderUser(User &other) : User(other){}

Watchable* GenreRecommenderUser::getRecommendation(Session &s) {
    //the user didn't watch any content
    if(this->history.size()==0)
        return nullptr;

    //create a map of tags and their popularity in the user's history
    map<string,int> tags;
    int size = this->history.size();
    for(int i=0;i< size; i++)
    {
        int sizei = this->history[i]->getTags().size();
        for(int j = 0 ;j < sizei;j++)
        {
            if(tags.count(this->history[i]->getTags()[j])!=0)
                tags[this->history[i]->getTags()[j]]+=1;
            else
                tags.insert(pair<string,int>(this->history[i]->getTags()[j],1));
        }
    }

    //sort the tags by their popularity
    vector<pair<string,int>> vec=this->sortTags(tags);


    //get recommendation

    //for each tag
    int length = vec.size();
    for(int i=0;i<length;i++)
    {
        string tag=vec[i].first;

        //for each content
        int l = s.getContent().size();
        for(int j=0;j<l;j++)
        {
            //check if this content doesn't belong to the user's history
            if(!isExistsInHistory(*s.getContent()[j]))
            {
                //check if this content contains tag
                vector<string>::iterator it1 = find(s.getContent()[j]->getTags().begin(),s.getContent()[j]->getTags().end(),tag);
                if(it1!=s.getContent()[j]->getTags().end())
                {
                    //clear memory
                    vec.clear();
                    tags.clear();


                    //return recommendation
                    return s.getContent()[j];
                }
            }
        }
    }

    //there is no content to be recommended
    return nullptr;


}



vector<pair<string, int>> GenreRecommenderUser::sortTags(map<string, int> &map) {
    //create an empty vector of pairs
    vector<pair<string,int>> vec;

    //copy the pairs from the map to the vector
    std::copy(map.begin(),map.end(),std::back_inserter<std::vector<pair<string,int>>>(vec));

    //sort
    std::sort(vec.begin(), vec.end(),
              [](const pair<string,int>& l, const pair<string,int>& r) {
                  if (l.second != r.second)
                      return l.second > r.second;

                  return l.first < r.first;
              });

    //return sorted vector
    return vec;
}


User* GenreRecommenderUser::copy() {
    /*GenreRecommenderUser* temp = new GenreRecommenderUser(this->getName());
    for(auto elem : this->get_history())
    {
        temp->history.push_back(elem->copy());
    }
    temp->setIndex(this->getIndex());
    return temp;*/
    return new GenreRecommenderUser(*this);
}

User* GenreRecommenderUser::copy(User* other) {
    other = new GenreRecommenderUser(*this);
    return other;
}

