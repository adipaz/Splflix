//
// Created by iris on 24/11/2019.
//
#include "../include/Action.h"
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <string>
#include <vector>

using namespace std;

/*******************************BaseAction*******************************/
BaseAction::BaseAction() :errorMsg(""), status(PENDING){}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

void BaseAction::complete() {
    this->status = COMPLETED;
    this->errorMsg = "";
}

void BaseAction::error(const std::string &errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
    cout << "Error: " << errorMsg << endl;
}

std::string BaseAction::getErrorMsg() const { return this->errorMsg;}

BaseAction & BaseAction::operator=(const BaseAction &action)
{
    // check for "self assignment" and do nothing in that case
    if (this == &action) {
        return *this;
    }
    //clear
    this->status = PENDING;
    this->errorMsg = "";
    //copy
    this->status = action.status;
    this->errorMsg = action.errorMsg;
    return *this;
}

void BaseAction::setErrorMsg(const string error) {this->errorMsg = error;}

void BaseAction::setStatus(const ActionStatus status) {this->status = status;}

BaseAction::~BaseAction() {}




/*******************************CreateUser*******************************/
void CreateUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check valid input
    if((input[1].compare("len") != 0) & (input[1].compare("rer") != 0) & (input[1].compare("gen") != 0))
    {
        error("3-letter code is invalid - no such algorithm!");
        return;
    }
    if(sess.isUserExists(input[0]))
    {
        error("There is already user with that name!");
        return;
    }


    //all the parameters are fine - create new user
    User* newUser;
    if(input[1].compare("len") == 0 )
    {
        newUser = new LengthRecommenderUser(input[0]);
    }
    else if(input[1].compare("rer") == 0 )
    {
        newUser = new RerunRecommenderUser(input[0]);
    }
    else
    {
        newUser = new GenreRecommenderUser(input[0]);
    }
    sess.addNewUser(input[0], newUser);
    complete();

    //clear memory
    //free(input);
}


std::string CreateUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Createuser ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Createuser COMPLETED";
    else
        msg = "Createuser PENDING";
    return msg;
}

CreateUser::CreateUser(CreateUser &other) : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *CreateUser::copy() {
    return new CreateUser(*this);
}

CreateUser::CreateUser() : BaseAction(){

}


/*******************************DeleteUser*******************************/
void DeleteUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check input
    if(!sess.isUserExists(input[0]))
    {
        error("The user that you wish to delete does not exists!");
        return;
    }

    //delete user
    sess.deleteUser(input[0]);
    complete();

    //clear memory
    //delete[] input;
}


std::string DeleteUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Deleteuser ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Deleteuser COMPLETED";
    else
        msg = "Deleteuser PENDING";
    return msg;
}

DeleteUser::DeleteUser(DeleteUser &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *DeleteUser::copy() {
    return new DeleteUser(*this);
}

DeleteUser::DeleteUser()  : BaseAction(){

}


/*******************************DuplicateUser*******************************/
void DuplicateUser::act(Session &sess) {
    string* input = sess.getActionInput();

    string newUserName = input[1];
    string originalUserName = input[0];

    //check input
    if(!sess.isUserExists(originalUserName))
    {
        string errorMsg = "No such user " + originalUserName;
        error(errorMsg);
        return;
    }
    else if(sess.isUserExists(newUserName))
    {
        string errorMsg = "The user " + newUserName + " already exists!";
        error(errorMsg);
        return;
    }

    //duplicate the user
    User* newuser = sess.getUser(originalUserName)->copy();
    /*User* newuser;
    if(LengthRecommenderUser* l = dynamic_cast<LengthRecommenderUser*>(sess.getUser(originalUserName))) //TODO: Working
    {
        newuser = new LengthRecommenderUser(*l);
        //newuser = sess.getUser(originalUserName)->copy();
    }*/
    newuser->setName(newUserName);
    sess.addNewUser(newUserName, newuser);
    complete();
    //clear memory
    //delete[] input;
}

std::string DuplicateUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Duplicateuser ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Duplicateuser COMPLETED";
    else
        msg = "Duplicateuser PENDING";
    return msg;
}

DuplicateUser::DuplicateUser(DuplicateUser &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *DuplicateUser::copy() {
    return  new DuplicateUser(*this);
}

DuplicateUser::DuplicateUser()  : BaseAction(){

}


/*******************************PrintWatchHistory*******************************/
void PrintWatchHistory::act(Session &sess) {
    cout << "Watch history for " << sess.getActiveUser()->getName() << endl;
    cout << "Where " << sess.getActiveUser()->getName() << " is the name of the current active user" << endl;
    int size = sess.getActiveUser()->get_history().size();
    for(int i = 0;i< size; i++)
    {
        cout << i+1 << ". " << sess.getActiveUser()->get_history()[i]->toString() << endl;
    }
    complete();
}

std::string PrintWatchHistory::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Watchhist ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Watchhist COMPLETED";
    else
        msg = "Watchhist PENDING";
    return msg;
}

PrintWatchHistory::PrintWatchHistory(PrintWatchHistory &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *PrintWatchHistory::copy() {
    return new PrintWatchHistory(*this);
}

PrintWatchHistory::PrintWatchHistory()  : BaseAction(){

}


/*******************************PrintActionsLog*******************************/
void PrintActionsLog::act(Session &sess) {
    //printing from the last to first - in vector the first place is the last action
    int size = sess.getActionsLog().size();
    for(int i = 0; i < size; i++) {
        cout << sess.getActionsLog()[i]->toString() << endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "log ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "log COMPLETED";
    else
        msg = "log PENDING";
    return msg;
}

PrintActionsLog::PrintActionsLog(PrintActionsLog &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *PrintActionsLog::copy() {
    return new PrintActionsLog(*this);
}

PrintActionsLog::PrintActionsLog()  : BaseAction(){

}


/*******************************ChangeActiveUser*******************************/
void ChangeActiveUser::act(Session &sess) {
    string* input = sess.getActionInput();
    //check valid input
    if(!sess.isUserExists(input[0]))
    {
        error("There is no such user");
        return;
    }

    //change active user
    sess.setActiveUser(input[0]);
    complete();

    //clear memory
    //delete[] input;
}


std::string ChangeActiveUser::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Changeuser Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Changeuser COMPLETED";
    else
        msg = "Changeuser PENDING";
    return msg;
}

ChangeActiveUser::ChangeActiveUser(ChangeActiveUser &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *ChangeActiveUser::copy() {
    return  new ChangeActiveUser(*this);
}

ChangeActiveUser::ChangeActiveUser()  : BaseAction(){

}


/*******************************PrintContentList*******************************/
void PrintContentList::act(Session &sess) {
    vector<Watchable*> content=sess.getContent();
    //print content list
    int size = content.size();
    for(int i=0;i< size;i++)
    {
        string id=to_string(i+1)+". ";
        string name=content[i]->toString()+" ";
        string length=to_string(content[i]->getLength())+" minutes ";
        vector<string> vecTags=content[i]->getTags();
        string tags="[";
        int s = vecTags.size();
        for(int j=0;j< s;j++)
        {
            tags+=vecTags[j];
            int l = vecTags.size()-1;
            if(j!=l)
                tags+=", ";
        }
        tags+="]";

        cout<<id << name << length << tags << endl;
    }

    complete();
}


std::string PrintContentList::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "content ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "content COMPLETED";
    else
        msg = "content PENDING";
    return msg;
}

PrintContentList::PrintContentList(PrintContentList &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *PrintContentList::copy() {
    return  new PrintContentList(*this);
}

PrintContentList::PrintContentList()  : BaseAction(){

}


/*******************************Watch*******************************/
void Watch::act(Session &sess) {

    string* input=sess.getActionInput();
    vector<Watchable*> contents=sess.getContent();

    //print watch command
    cout<<"Watching " << contents[stoi(input[0])-1]->toString() <<endl;
    complete();
    //adds content to watching history
    sess.getActiveUser()->addToHistory( *contents[stoi(input[0])-1]);
    sess.getActionsLog().push_back(new Watch(*this));
    //recommendation
    Watchable* recommend = sess.getContent()[stoi(input[0])-1]->getNextWatchable(sess);
    cout<<"We recommend watching " << recommend->toString() << ", continue watching? [y/n]" << endl;


    string continueWatching;
    cin >> continueWatching;
    //Watch recommended content
    if(continueWatching.compare("y") == 0)
    {
        int index = 0;
        for (auto & elem : contents)
        {
            if (elem->getId() == recommend->getId())
            {
                break;
            }
            index++;
        }
        sess.setInputForWatch(index+1);
        this->act(sess);
    }


    //end watching


    //clear memory
    //delete[] input;
    //delete(recommend);
}

std::string Watch::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Watch Error: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Watch COMPLETED";
    else
        msg = "Watch PENDING";
    return msg;
}

Watch::Watch(Watch &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *Watch::copy() {
    return  new Watch(*this);
}

Watch::Watch()  : BaseAction(){

}


/*******************************Exit*******************************/
void Exit::act(Session &sess) {
    complete();
}


std::string Exit::toString() const {
    string msg = "";
    if(this->getStatus() == ActionStatus::ERROR)
        msg = "Exit ERROR: " + this->getErrorMsg();
    else if(this->getStatus() == ActionStatus::COMPLETED)
        msg = "Exit COMPLETED";
    else
        msg = "Exit PENDING";
    return msg;
}

Exit::Exit(Exit &other)  : BaseAction(){
    this->setStatus(other.getStatus());
    this->setErrorMsg(other.getErrorMsg());
}

BaseAction *Exit::copy() {
    return new Exit(*this);
}

Exit::Exit()  : BaseAction(){

}
