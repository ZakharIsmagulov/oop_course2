#include "Object.h"

int Object::m_obj_count = 0;

Object::Object() {
	++m_obj_count;
}

Object::~Object() {
	--m_obj_count;
}

int Object::get_obj_count() {
	return m_obj_count;
}

TaskResultException::TaskResultException(std::string const &class_name) {
	m_class_name = class_name;
}

Task::Task() {
	m_is_done = false;
	m_type = Type::WITHOUT_OUTPUT;
}

std::string Task::toString() const {
	return m_is_done ? "Task is done" : "Task is not done";
}

void Task::do_task() {
	if (m_is_done) {
		throw "Task is already done\n";
	}
	else {
		m_is_done = true;
	}
}

bool Task::is_done() const {
	return m_is_done;
}

Task::Type const Task::get_type() const {
	return m_type;
}

Named::Named(std::string const &name) {
	m_name = name;
}

std::string Named::toString() const {
	return m_name;
}

BinaryOperationTask::BinaryOperationTask(Operator op, double v1, double v2, std::string const &name) : Named(name) {
	m_op = op;
	m_v1 = v1;
	m_v2 = v2;
	m_res = NULL;
	m_type = Type::WITH_OUTPUT;
}

void BinaryOperationTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
	}
	switch (m_op)
	{
	case Operator::add:
		m_res = m_v1 + m_v2;
		break;
	case Operator::sub:
		m_res = m_v1 - m_v2;
		break;
	case Operator::mult:
		m_res = m_v1 * m_v2;
		break;
	case Operator::div:
		if (m_v2 != 0) {
			m_res = m_v1 / m_v2;
		}
		else {
			throw std::overflow_error("Division by zero");
		}
		break;
	}
	Task::do_task();
}

double BinaryOperationTask::get_result() const {
	if (m_res == NULL && Task::is_done()) {
		throw BinaryOperationException();
	}
	else if (m_res != NULL) {
		return m_res;
	}
	throw TaskResultException("BinaryOperationTask");
}

std::string BinaryOperationTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: ";
	switch (m_op)
	{
	case Operator::add:
		out += "add " + std::to_string(m_v2) + " to " + std::to_string(m_v1) + "\n";
		break;
	case Operator::sub:
		out += "subtract " + std::to_string(m_v2) + " from " + std::to_string(m_v1) + "\n";
		break;
	case Operator::mult:
		out += "multiply " + std::to_string(m_v1) + " by " + std::to_string(m_v2) + "\n";
		break;
	case Operator::div:
		out += "divide " + std::to_string(m_v1) + " by " + std::to_string(m_v2) + "\n";
		break;
	}
	if (m_res != NULL) {
		out += "Task is done\nResult: " + std::to_string(m_res) + "\n";
	}
	else if (Task::is_done()) {
		out += "Task is not done, because of an error\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}

ContainerAppendTask::ContainerAppendTask(Container<Task*>* cont, Task *task, std::string const &name) : Named(name) {
	m_cont = cont;
	m_task = task;
	m_type = Type::WITHOUT_OUTPUT;
}

void ContainerAppendTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
	}
	m_cont->push_back(m_task);
	Task::do_task();
}

std::string ContainerAppendTask::toString() const {
	std::string out = "Task without output \"" + Named::toString() + "\":\n";
	out += "Aim: append task\n{\n" + (*m_task).toString() + "\n}\nto container\n";
	if (Task::is_done()) {
		out += "Task is done\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}

ContainerObjectCountTask::ContainerObjectCountTask(Container<Task*> *cont, std::string const &name) : Named(name) {
	m_cont = cont;
	m_result = NULL;
	m_type = Type::WITH_OUTPUT;
}

void ContainerObjectCountTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
	}
	m_result = m_cont->get_count();
	Task::do_task();
}

int ContainerObjectCountTask::get_result() const {
	if (Task::is_done()) {
		return m_result;
	}
	throw TaskResultException("ContainerObjectCountTask");
}

std::string ContainerObjectCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count objects in the container\n";
	if (Task::is_done()) {
		out += "Task is done\nResult: " + std::to_string(m_result) + "\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}

WithOutputCountTask::WithOutputCountTask(Container<Task*> *cont, std::string const &name) : Named(name) {
	m_cont = cont;
	m_result = NULL;
	m_type = Type::WITH_OUTPUT;
}

void WithOutputCountTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
		return;
	}
	m_result = 0;
	for (Container<Task*>::Iterator iter = (*m_cont).begin(); iter != (*m_cont).end(); iter++) {
		if ((*iter)->get_type() == Type::WITH_OUTPUT) {
			m_result++;
		}
	}
	Task::do_task();
}

int WithOutputCountTask::get_result() const {
	if (Task::is_done()) {
		return m_result;
	}
	throw TaskResultException("WithOutputCountTask");
}

std::string WithOutputCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count tasks with output in the container\n";
	if (Task::is_done()) {
		out += "Task is done\nResult: " + std::to_string(m_result) + "\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}

ContainerClearTask::ContainerClearTask(Container<Task*> *cont) {
	m_cont = cont;
	m_type = Type::WITHOUT_OUTPUT;
}

void ContainerClearTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
	}
	if (m_cont->is_empty()) {
		return;
	}
	for (Container<Task*>::Iterator iter = (*m_cont).begin(); iter != (*m_cont).end(); iter++) {
		if (*iter == this) {
			throw "Error: called ContainerClearTask object is in the container\n";
		}
	}
	m_cont->clear();
	Task::do_task();
}

std::string ContainerClearTask::toString() const {
	std::string out = "Task without output:\n";
	out += "Aim: clear the container\n";
	if (Task::is_done()) {
		out += "Task is done\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}

ProgramObjectCountTask::ProgramObjectCountTask(std::string const &name) : Named(name) {
	m_result = NULL;
	m_type = Type::WITH_OUTPUT;
}

void ProgramObjectCountTask::do_task() {
	if (Task::is_done()) {
		throw "Task is already done";
	}
	m_result = get_obj_count();
	Task::do_task();
}

int ProgramObjectCountTask::get_result() const {
	if (Task::is_done()) {
		return m_result;
	}
	throw TaskResultException("ProgramObjectCountTask");
}

std::string ProgramObjectCountTask::toString() const {
	std::string out = "Task with output \"" + Named::toString() + "\":\n";
	out += "Aim: count all existing Object in the program\n";
	if (Task::is_done()) {
		out += "Task is done\nResult: " + std::to_string(m_result) + "\n";
	}
	else {
		out += "Task is not done\n";
	}
	return out;
}
