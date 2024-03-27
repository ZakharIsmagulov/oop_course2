#pragma once
#include <iostream>
#include <string>
#include "Container.h"

class Object {
public:
	Object();
	~Object();

	virtual std::string toString() const = 0;
	static int get_obj_count();
private:
	static int m_obj_count;
};

struct BinaryOperationException {
};

struct TaskResultException {
	TaskResultException(std::string const &class_name);
	std::string m_class_name;
};

class Task : virtual public Object {
public:
	enum class Type { WITH_OUTPUT, WITHOUT_OUTPUT };
	Task();

	virtual void do_task();
	bool is_done() const;
	Type const get_type() const;
	std::string toString() const override;
private:
	bool m_is_done;
protected:
	Type m_type;
};

class Named : virtual public Object {
public:
	Named(std::string const &name);

	std::string toString() const override;
private:
	std::string m_name;
};

class BinaryOperationTask : public Task, public Named {
public:
	enum class Operator { add, sub, mult, div };
	BinaryOperationTask(Operator op, double v1, double v2, std::string const &name);

	void do_task() override;
	double get_result() const;
	std::string toString() const override;
private:
	Operator m_op;
	double m_v1;
	double m_v2;
	double m_res;
};

class ContainerAppendTask : public Task, public Named {
public:
	ContainerAppendTask(Container<Task*> *cont, Task *task, std::string const &name);

	void do_task() override;
	std::string toString() const override;
private:
	Container<Task*> *m_cont;
	Task *m_task;
};

class ContainerObjectCountTask : public Task, public Named {
public:
	ContainerObjectCountTask(Container<Task*> *cont, std::string const &name);

	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	Container<Task*> *m_cont;
	int m_result;
};

class WithOutputCountTask : public Task, public Named {
public:
	WithOutputCountTask(Container<Task*> *cont, std::string const &name);

	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	Container<Task*> *m_cont;
	int m_result;
};

class ContainerClearTask : public Task {
public:
	ContainerClearTask(Container<Task*> *cont);

	void do_task() override;
	std::string toString() const override;
private:
	Container<Task*> *m_cont;
};

class ProgramObjectCountTask : public Task, public Named {
public:
	ProgramObjectCountTask(std::string const &name);
	
	void do_task() override;
	int get_result() const;
	std::string toString() const override;
private:
	int m_result;
};
