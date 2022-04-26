#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

const unsigned num_episodes = 100;

struct EpisodeResults
{
    const double reward_cumm;
    const unsigned total_steps;
};

struct TrainResults
{
    double cumm_rewards[num_episodes];
    unsigned ep_lengths[num_episodes];
    struct QTable final_q_table;
};

struct EpisodeResults run_episode(struct CliffWalking* cliff_walking,
                                  const struct QLearningParams* q_params,
                                  struct QTable* q_table,
                                  const bool training,
                                  const bool verbosity)
{
    //Setup episode
    reset_agent(&cliff_walking->cliff);
    if(verbosity)
    {
        display_cliff(&cliff_walking->cliff);
        if(training)
            print_q_table(q_table);
    }
    double reward_cumm = 0.;
    unsigned step = 0;

    //Start episode
    while(true)
    {
        //sleep(1);

        //Sample action
        unsigned action;
        if(training)
            action = sample_action(q_table,
                                   cliff_walking->cliff.agent.state,
                                   q_params->epsilon);
        else
            action = calculate_max_action(q_table, cliff_walking->cliff.agent.state);

        //Take step in environment
        const struct Experience experience = agent_step(cliff_walking, action);

        //Update QTable
        if(training)
            q_learning_step(q_table, q_params, experience, &cliff_walking->cliff);

        if(verbosity)
        {
            printf("Step: %u\n", step);
            printf("Action: %u\n", action);
            printf("Reward: %f\n", experience.r);
            display_cliff(&cliff_walking->cliff);
            if(training)
                print_q_table(q_table);
        }

        reward_cumm += experience.r;
        step += 1;

        if(experience.episode_end)
            break;

    }

    const struct EpisodeResults ep_results  = {.reward_cumm = reward_cumm,
                                               .total_steps = step};
    return ep_results;
}

//Trains agent a returns learnt QTable
struct TrainResults train_agent(struct CliffWalking cliff_walking,
                                const struct QLearningParams q_params,
                                const unsigned num_episodes)
{
    const unsigned num_states = cliff_walking.cliff.width * cliff_walking.cliff.height;
    const unsigned num_actions = 4;
    struct QTable q_table = create_q_table(num_states, num_actions);
    const bool verbosity = false;
    const bool training = true;

    double cumm_rewards[num_episodes];
    unsigned episode_lengths[num_episodes];

    for(unsigned i = 0; i < num_episodes; i++)
    {
        const struct EpisodeResults ep_results =
            run_episode(&cliff_walking, &q_params, &q_table, training, verbosity);
        cumm_rewards[i] = ep_results.reward_cumm;
        episode_lengths[i] = ep_results.total_steps;
    }

    /*
    printf("Final Q-table:\n");
    print_q_table(&q_table);
    printf("Cummulative rewards:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%f ", cumm_rewards[i]);
    printf("\nEpisode lengths:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%u ", episode_lengths[i]);
    printf("\n");
    */

    struct TrainResults train_results;
    for(unsigned i = 0; i < num_episodes; i++)
    {
        train_results.cumm_rewards[i] = cumm_rewards[i];
        train_results.ep_lengths[i] = episode_lengths[i];
    }
    train_results.final_q_table = q_table;
    return train_results;

}

void test_agent(struct CliffWalking cliff_walking, struct QTable q_table)
{
    const bool verbosity = true;
    const bool training = false;
    const struct EpisodeResults ep_results =
        run_episode(&cliff_walking, NULL, &q_table, training, verbosity);

    printf("Episode reward: %f\n", ep_results.reward_cumm);
    printf("Num steps taken: %u\n", ep_results.total_steps);

}

void write_exp_data(const double avg_cumm_reward[], const double avg_total_steps[],
                    const unsigned potential_reward_type)
{
    char* file_path;
    switch(potential_reward_type)
    {
        case 0:
            file_path = "../data/no_potential_reward_results.csv";
            break;
        case 1:
            file_path = "../data/manhattan_potential_reward_results.csv";
            break;
    }

    FILE* fpt = fopen(file_path, "w+");
    for(unsigned i = 0; i < num_episodes; i++)
    {
        if(i != (num_episodes-1))
            fprintf(fpt, "%f,", avg_cumm_reward[i]);
        else
            fprintf(fpt, "%f", avg_cumm_reward[i]);
    }
    fprintf(fpt, "\n");
    for(unsigned i = 0; i < num_episodes; i++)
    {
        if(i != (num_episodes-1))
            fprintf(fpt, "%f,", avg_total_steps[i]);
        else
            fprintf(fpt, "%f", avg_total_steps[i]);
    }
    fclose(fpt);
}

int main()
{
    //Initialise random seed
    srand(time(NULL));

    //Domain
    const unsigned cliff_width = 12;
    const unsigned cliff_height = 4;
    struct CliffWalking cliff_walking = create_cliff_walking(cliff_width, cliff_height);

    //Q-Learning
    const double alpha = 0.5;
    const double gamma = 0.9;
    const double epsilon = 0.1;
    const unsigned potential_reward_type = 1;
    const struct QLearningParams q_params = {alpha, gamma, epsilon,
                                             potential_reward_type};

    //Run multiple experiments
    const unsigned num_exps = 1;
    double total_cumm_rewards[num_episodes] = {0.};
    unsigned total_steps[num_episodes] = {0};

    for(unsigned i = 0; i < num_exps; i++)
    {
        //Train agent
        const struct TrainResults train_results = train_agent(cliff_walking,
                                                              q_params,
                                                              num_episodes);
        for(unsigned j = 0; j < num_episodes; j++)
        {
            total_cumm_rewards[j] += train_results.cumm_rewards[j];
            total_steps[j] += train_results.ep_lengths[j];
        }

        //Test agent by doing an environment run through with no exploration
        test_agent(cliff_walking, train_results.final_q_table);
    }

    double avg_cumm_rewards[num_episodes];
    double avg_total_steps[num_episodes];
    for(unsigned i = 0; i < num_episodes; i++)
    {
        avg_cumm_rewards[i] = total_cumm_rewards[i] / (double)num_exps;
        avg_total_steps[i] = (double)total_steps[i] / (double)num_exps;
    }

    printf("Average cummulative rewards:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%f ", avg_cumm_rewards[i]);
    printf("\n");
    printf("Average total number of steps:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%f ", avg_total_steps[i]);
    printf("\n");

    write_exp_data(avg_cumm_rewards, avg_total_steps, potential_reward_type);

    return 0;

}
