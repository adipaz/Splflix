#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
using namespace std;
class Session;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Session& sess)=0;
    virtual std::string toString() const=0;
    //our addition
	virtual ~BaseAction();
    BaseAction&operator=(const BaseAction &action);
    void setStatus(const ActionStatus status);
    void setErrorMsg(const string error);
    virtual BaseAction* copy()=0;
protected:
	void complete();
	void error(const std::string& errorMsg);
	std::string getErrorMsg() const;
private:
	std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	//our addition
	CreateUser();
	CreateUser(CreateUser& other); //copy c'tor
    virtual BaseAction* copy();
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
	//our addition
	ChangeActiveUser();
	ChangeActiveUser(ChangeActiveUser& other);//copy c'tor
    virtual BaseAction* copy();
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    //our addition
    DeleteUser();
    DeleteUser(DeleteUser& other);//copy c'tor
    virtual BaseAction* copy();
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    //our addition
    DuplicateUser();
    DuplicateUser(DuplicateUser& other);//copy c'tor
    virtual BaseAction* copy();
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    //our addition
    PrintContentList();
    PrintContentList(PrintContentList& other);//copy c'tor
    virtual BaseAction* copy();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    //our addition
    PrintWatchHistory();
    PrintWatchHistory(PrintWatchHistory& other);//copy c'tor
    virtual BaseAction* copy();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    //our addition
    Watch();
    Watch(Watch& other);//copy c'tor
    virtual BaseAction* copy();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    //our addition
    PrintActionsLog();
    PrintActionsLog(PrintActionsLog& other);//copy c'tor
    virtual BaseAction* copy();
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    //our addition
    Exit();
    Exit(Exit& other);//copy c'tor
    virtual BaseAction* copy();
};
#endif
