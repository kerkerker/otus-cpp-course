#include <iostream>

bool can_solve(int f)
{
  const int tasks_count  = 12;
  const int all_minutes  = 4 * 60;
  const int task_minutes = 45;

  int minutes_for_other_tasks = task_minutes * (tasks_count - f);

  return all_minutes - minutes_for_other_tasks > 0;
}

int main()
{
  int f;
  std::cin >> f;
  std::cout << (can_solve(f) ? "YES" : "NO") << std::endl;

  return 0;
}
