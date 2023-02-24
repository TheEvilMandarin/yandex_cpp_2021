#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;
// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const {
		return team_info.at(person);
	};

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person) {
		team_info[person][TaskStatus::NEW]++;
	};

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(
		const string& person, int task_count) {
		TasksInfo updated, untouched, result_tasks;
		TasksInfo& tasks = team_info.at(person);
		auto Next = [](TaskStatus status) {
			return (static_cast<TaskStatus>(static_cast<int>(status) + 1));
		};
		for (TaskStatus status = TaskStatus::NEW; status != TaskStatus::DONE; status = Next(status)) {
			int to_update = min(tasks[status], task_count);
			task_count -= to_update;
			result_tasks[Next(status)] += updated[Next(status)] = to_update;
			result_tasks[status] += untouched[status] = tasks[status] - to_update;
		}
		result_tasks[TaskStatus::DONE] += tasks[TaskStatus::DONE];
		tasks = result_tasks;
		return { updated, untouched };
	};
private:
	map<string, TasksInfo> team_info;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
	cout << tasks_info[TaskStatus::NEW] << " new tasks"s <<
		", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s <<
		", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s <<
		", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
	TeamTasks tasks;
	tasks.AddNewTask("Ilia"s);
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan"s);
	}
	cout << "Ilia's tasks: "s;
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
	cout << "Ivan's tasks: "s;
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);
}