//for YandexPracticum from obegrand aka Andrey Melnikov
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

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

void PrintTasksInfo(TasksInfo tasks_info) {
	cout << tasks_info[TaskStatus::NEW] << " new tasks"s
		<< ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
		<< ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
		<< ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

class TeamTasks {
public:
	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person) {
		++developers_[person][TaskStatus::NEW];
	}

	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo& GetPersonTasksInfo(const string& person) const {
		return developers_.at(person);
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
		TasksInfo updated_tasks, untouched_tasks;
		TasksInfo& tasks = developers_[person];

		for (auto count = static_cast<int>(TaskStatus::NEW); count < static_cast<int>(TaskStatus::DONE); ++count) {
			updated_tasks[static_cast<TaskStatus>(count + 1)] = min(task_count, tasks[static_cast<TaskStatus>(count)]);
			task_count -= updated_tasks[static_cast<TaskStatus>(count + 1)];
		}
		for (auto count = static_cast<int>(TaskStatus::NEW); count < static_cast<int>(TaskStatus::DONE); ++count) {
			untouched_tasks[static_cast<TaskStatus>(count)] = tasks[static_cast<TaskStatus>(count)] - updated_tasks[static_cast<TaskStatus>(count + 1)];
			tasks[static_cast<TaskStatus>(count)] += updated_tasks[static_cast<TaskStatus>(count)] - updated_tasks[static_cast<TaskStatus>(count + 1)];
		}
		tasks[TaskStatus::DONE] += updated_tasks[TaskStatus::DONE];

		RemoveZeros(updated_tasks);
		RemoveZeros(untouched_tasks);
		RemoveZeros(developers_.at(person));

		return { updated_tasks, untouched_tasks };
	}
private:
	map<string, TasksInfo> developers_;

	void RemoveZeros(TasksInfo& tasks_info) {
		// Соберём те статусы, которые нужно убрать из словаря
		vector<TaskStatus> statuses_to_remove;
		for (const auto& task_item : tasks_info) {
			if (task_item.second == 0) {
				statuses_to_remove.push_back(task_item.first);
			}
		}
		for (const TaskStatus status : statuses_to_remove) {
			tasks_info.erase(status);
		}
	}
};

int main() {
	TeamTasks tasks;
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan"s);
	}
	cout << "Ivan's tasks: "s;
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));
	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);
	system("pause");
}