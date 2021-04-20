#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>
#include <stdio.h>

struct EpisodeResults
{
    const double reward_cumm;
    const unsigned total_steps;
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
            q_learning_step(q_table, q_params, experience);

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
struct QTable train_agent(struct CliffWalking cliff_walking,
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

    printf("Final Q-table:\n");
    print_q_table(&q_table);
    printf("Cummulative rewards:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%f ", cumm_rewards[i]);
    printf("Episode lengths:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%u ", episode_lengths[i]);
    printf("\n");

    return q_table;

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

int main()
{

    //Domain
    const unsigned cliff_width = 12;
    const unsigned cliff_height = 4;
    struct CliffWalking cliff_walking = create_cliff_walking(cliff_width, cliff_height);
    const unsigned num_episodes = 500;

    //Q-Learning
    const double alpha = 0.5;
    const double gamma = 0.9;
    const double epsilon = 0.1;
    const struct QLearningParams q_params = {.alpha = alpha, .gamma = gamma,
                                             .epsilon = epsilon};

    //Train agent
    const struct QTable learnt_q_table = train_agent(cliff_walking,
                                                     q_params,
                                                     num_episodes);

    //sleep(5);
    //Test agent by doing an environment run through with no exploration
    test_agent(cliff_walking, learnt_q_table);

    return 0;

}
