#!/bin/python3

import os

class Result:
    def __init__(self):
        self.satus = -1
        self.log = ""

    def status_set(self, value):
        self.status = value

    def status_get(self):
        return self.status

    def log_set(self, value):
        self.log = value

    def log_add(self, value):
        self.log += value

    def log_print(self):
        if self.log != "":
            print(self.log)

class Test:
    def __init__(self, rules):
        self.rules = rules
        self.out = []
        self.nb_philo = 0
        self.ck_msg = Result()
        self.ck_forks = Result()
        self.ck_meals = Result()
        self.ck_death = Result()
        self.stat = {}

    def run(self, mode=0):
        self.load()
        self.count_philo()
        self.count_stat()
        self.check_msg()
        self.check_forks()
        self.check_meals()
        self.check_death()

    def verbose(self):
        self.ck_msg.log_print()
        self.ck_forks.log_print()
        self.ck_meals.log_print()
        self.ck_death.log_print()
        if (self.stat["time_to_eat"]):
            print("Average time to eat:", self.stat["time_to_eat"])
        if (self.stat["time_to_sleep"]):
            print("Average time to sleep:", self.stat["time_to_sleep"])

    def load(self):
        os.system("./philo {} > .tmp".format(self.rules))
        with open(".tmp", 'r') as outfile:
            for line in outfile:
                self.out.append(line.split())

    def count_philo(self):
        for line in self.out:
            if int(line[1]) > self.nb_philo:
                self.nb_philo = int(line[1])

    def count_stat(self):
        meal_time = []
        sleep_time = []
        for id in range(self.nb_philo):
            for line in self.out:
                if int(line[1]) == id:
                    if len(line) > 3:
                        if "eating" in line[3]:
                            last_meal = int(line[0])
                        elif "sleeping" in line[3]:
                            last_sleep = int(line[0])
                            meal_time.append(last_sleep - last_meal)
                        elif "thinking" in line[3]:
                            last_think = int(line[0])
                            sleep_time.append(last_think - last_sleep)
        time_to_eat = 0
        for i in meal_time:
            time_to_eat += i
        if len(meal_time):
            time_to_eat /= len(meal_time)
        time_to_sleep = 0
        for i in sleep_time:
            time_to_sleep += i
        if len(sleep_time):
            time_to_sleep /= len(sleep_time)
        self.stat["time_to_eat"] = int(time_to_eat)
        self.stat["time_to_sleep"] = int(time_to_eat)

    def check_msg_each(self, msg):
        models = ["has taken a fork", "is eating", "is sleeping", "is thinking", "died"]
        model_sp = []
        for m in models:
            model_sp.append(m.split())
        try:
            int(msg[0])
        except:
            return 0
        try:
            int(msg[1])
        except:
            return 0
        if len(msg) > 6 or len(msg) < 3:
            return 0
        for m in model_sp:
            if m == msg[2:]:
                return 1
        return 0

    def check_msg(self):
        self.ck_msg.status_set(1)
        for m in self.out:
            if self.check_msg_each(m) != 1:
                self.ck_msg.log_add("\"{}\": this message seems to be wrongly formatted.\n".format(' '.join(m)))
            self.ck_msg.status_set(0)
        if self.ck_msg.status_get():
            self.ck_msg.log_set("All messages were correctly formated.")

    def check_forks_nb(self, forks):
        nb = 0
        for f in forks:
            nb += f
        return f

    def check_forks_around(self, forks, id):
        left = id - 1
        right = id + 1
        if right >= self.nb_philo:
            right = 0
        if left < 0:
            left = self.nb_philo - 1
        if forks[left] + forks[id] >= 3:
            return 0
        if forks[right] + forks[id] >= 3:
            return 0
        return 1

    def check_forks(self):
        forks = []
        meal_time = []
        meal_flag = []
        line_nb = 0
        error_flag = 0
        for i in range(self.nb_philo):
            meal_time.append(0)
            meal_flag.append(0)
            forks.append(0)
        for line in self.out:
            line_nb += 1
            if len(line) >= 4 and "eating" in line[3]:
                meal_time[int(line[1]) - 1] = int(line[0])
                meal_flag[int(line[1]) - 1] = 1
            time = int(line[0])
            for i in range(len(meal_time)):
                if time - meal_time[i] >= self.stat["time_to_eat"] and meal_flag[i]:
                    forks[i] = 0
                    meal_flag[i] = 0
            if len(line) == 6:
                forks[int(line[1]) - 1] += 1
                error_msg = ""
                if forks[int(line[1]) - 1] > 2:
                    error_msg = "{} has to many forks...".format(int(line[1]))
                if self.check_forks_nb(forks) > self.nb_philo:
                    error_msg = "There seems to be too many forks in use..."
                if self.check_forks_around(forks, int(line[1]) - 1) != 1:
                    error_msg = "{} have stolen a fork...".format(int(line[1]))
                if error_msg != "":
                    self.ck_forks.log_add("({}:{}) {}: {}\n".format(sys.argv[1], str(line_nb), ' '.join(line), error_msg))
                    error_flag = 1
        if error_flag == 0:
            self.ck_forks.log_set("No fork were mishandled")

    def count_meals_total(self):
        total_meal = 0
        for line in self.out:
            if len(line) >= 4 and "eating" in line[3]:
                total_meal += 1
        return total_meal

    def count_meals_each(self):
        each_meals = []
        for i in range(0, self.nb_philo):
            each_meals.append(0)
        for line in self.out:
            if len(line) >= 4 and "eating" in line[3]:
                each_meals[int(line[1]) - 1] += 1
        return each_meals

    def count_meals_min(self, each_meals):
        min_meal = [-1,-1]
        for i in range(0, self.nb_philo):
            if each_meals[i] < min_meal[0] or min_meal[0] == -1:
                min_meal[0] = each_meals[i]
                min_meal[1] = i + 1
        return min_meal

    def check_meals(self):
        each_meals = self.count_meals_each()
        total_meal = self.count_meals_total()
        min_meal = self.count_meals_min(each_meals)
        self.ck_meals.status_set(min_meal)
        self.ck_meals.log_add("{} philosophers have eaten a total of {} meals.\n".format(self.nb_philo, total_meal))
        self.ck_meals.log_add("philo {} is the smallest eater with {} meals.".format(str(min_meal[1]), str(min_meal[0])))

    def get_last_meal(self, id):
        for i in reversed(range(len(self.out))):
            if len(self.out[i]) >= 4 and int(self.out[i][1]) == id and "eating" in self.out[i][3]:
                return int(self.out[i][0])
        return 0

    def count_death(self):
        death_list = []
        for line in self.out:
            if "died" in line[2]:
                time = int(line[0]) - self.get_last_meal(int(line[1]))
                death_list.append((int(line[1]), time))
        return death_list

    def check_death(self):
        death_list = self.count_death()
        if len(death_list) == 0:
            self.ck_death.status_set(0)
            self.ck_death.log_set("No one died.")
        else:
            for d in range(0, len(death_list)):
                self.ck_death.log_add("Philosopher {} is dead. His last meal was {}ms ago.\n".format(str(death_list[d][0]), str(death_list[d][1])))
            if out[-1][2] not in "died":
                self.ck_death.log_add("Something was printed after a death. This is wrong.\n")
        if len(death_list) > 1:
            self.ck_death.log_add("Several philosophers had their death printed. This is not correct.\n")
            self.ck_death.status_set(len(death_list))

def main():
    test = Test("4 700 150 150 5")
    test.run()
    test.verbose()

main()
