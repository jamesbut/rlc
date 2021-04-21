import csv
import matplotlib.pyplot as plt
import numpy as np


def read_csv_file(file_name):

    with open(file_name) as csv_file:
        reader = list(csv.reader(csv_file, delimiter=','))

        avg_cumm_rewards = reader[0]
        avg_ep_steps = reader[1]

    return [float(i) for i in avg_cumm_rewards], [float(i) for i in avg_ep_steps]


def plot_data(no_pot_data, man_pot_data):

    episode_nums = list(range(1, len(no_pot_data)+1, 1))

    print(episode_nums)
    print(no_pot_data)

    plt.plot(episode_nums, no_pot_data)
    plt.plot(episode_nums, man_pot_data)

    plt.show()


def main():

    no_pot_rew_file_name = '../data/no_potential_reward_results.csv'
    man_pot_rew_file_name = '../data/manhattan_potential_reward_results.csv'

    no_pot_avg_cumm_rwds, no_pot_avg_ep_steps = read_csv_file(no_pot_rew_file_name)
    man_pot_avg_cumm_rwds, man_pot_avg_ep_steps = read_csv_file(man_pot_rew_file_name)

    plot_data(no_pot_avg_cumm_rwds, man_pot_avg_cumm_rwds)
    plot_data(no_pot_avg_ep_steps, man_pot_avg_ep_steps)


if __name__ == '__main__':

    main()
