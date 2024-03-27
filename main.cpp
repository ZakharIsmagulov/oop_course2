#include <iostream>
#include <string>
#include "Container.h"
#include "Object.h"

int int_rand(int from, int to) {
	return (std::rand() % (to - from + 1)) + from;
}

double double_rand(double from, double to) {
	return (double(std::rand()) / RAND_MAX) * (to - from) + from;
}

void rand_cont(Container<Task*> * const cont, int num) {
	int class_num = int_rand(0, 4);
	switch (class_num)
	{
	case 0: {
		int op = int_rand(0, 3);
		switch (op)
		{
		case 0:
			cont->push_back(new BinaryOperationTask(BinaryOperationTask::Operator::add, double_rand(0.0, 9999.0),
				double_rand(0.0, 9999.0), "add" + std::to_string(num)));
			break;
		case 1:
			cont->push_back(new BinaryOperationTask(BinaryOperationTask::Operator::sub, double_rand(0.0, 9999.0),
				double_rand(0.0, 9999.0), "subtract" + std::to_string(num)));
			break;
		case 2:
			cont->push_back(new BinaryOperationTask(BinaryOperationTask::Operator::mult, double_rand(0.0, 9999.0),
				double_rand(0.0, 9999.0), "multiply" + std::to_string(num)));
			break;
		case 3:
			cont->push_back(new BinaryOperationTask(BinaryOperationTask::Operator::div, double_rand(0.0, 9999.0),
				double_rand(0.0, 9999.0), "divide" + std::to_string(num)));
			break;
		}
		break;
	}
	case 1:
		cont->push_back(new ContainerAppendTask(cont, new ProgramObjectCountTask("from appending task"), "container append" + std::to_string(num)));
		break;
	case 2:
		cont->push_back(new ContainerObjectCountTask(cont, "container count" + std::to_string(num)));
		break;
	case 3:
		cont->push_back(new WithOutputCountTask(cont, "container with output count" + std::to_string(num)));
		break;
	case 4:
		cont->push_back(new ProgramObjectCountTask("program count" + std::to_string(num)));
		break;
	}
}

std::ostream &operator<<(std::ostream &os, Task const &task) {
	return os << task.toString();
}

template<class T> std::ostream &operator<<(std::ostream &os, typename Container<T>::Cell const &cell) {
	std::cout << cell.data;
}

int main() {

	int seed = 1;
	std::srand(seed);
	Container<Task*> cont;
	for (int i = 1; i <= int_rand(10, 20); i++) {
		rand_cont(&cont, i);
	}
	
	Container<std::string> cont_info;

	ProgramObjectCountTask *pr_count = new ProgramObjectCountTask("object count");
	pr_count->do_task();
	std::cout << "There is " + std::to_string(pr_count->get_result()) + " objects in the program\n";

	while (!cont.is_empty()) {
		try {
			cont.get_head()->do_task();
			cont_info.push_back(cont.get_head()->toString());
			Task *head = cont.get_head();
			cont.del_head();
			delete head;
		}
		catch (std::overflow_error) {
			cont_info.push_back("Missing task (divide by zero)\n");
			Task *head = cont.get_head();
			cont.del_head();
			delete head;
		}
		catch (...) {
			cont_info.push_back("Missing task\n");
			Task *head = cont.get_head();
			cont.del_head();
			delete head;
		}
	}

	for (Container<std::string>::Iterator iter = cont_info.begin(); iter != cont_info.end(); iter++) {
		std::cout << *iter << "\n";
	}

	ContainerClearTask *clear1 = new ContainerClearTask(&cont);
	clear1->do_task();
	delete clear1;
	cont_info.clear();
	delete pr_count;

	std::cout << "There is " + std::to_string(Object::get_obj_count()) << " objects in the program\n";
}
