#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>
#include <stdio.h>

int main()
{

    //Domain
    //const unsigned cliff_width = 12;
    //const unsigned cliff_height = 4;
    const unsigned cliff_width = 5;
    const unsigned cliff_height = 3;
    struct CliffWalking cliff_walking = create_cliff_walking(cliff_width, cliff_height);
    const unsigned num_episodes = 500;

    //enum Action moves[] = {Up, Up, Right, Right, Right, Right, Down, Down, Up, Up};
    enum Action moves[] = {Up, Up, Right, Right, Down, Down, Up, Up};
    const unsigned num_moves = sizeof(moves) / sizeof(moves[0]);

    //Q-Learning
    const double alpha = 0.5;
    const double gamma = 0.9;
    const double epsilon = 0.1;
    const struct QLearningParams q_params = {.alpha = alpha, .gamma = gamma};
    const unsigned num_states = cliff_width * cliff_height;
    const unsigned num_actions = 4;
    struct QTable q_table = create_q_table(num_states, num_actions);

    const bool verbosity = false;
    double cumm_rewards[num_episodes];
    unsigned episode_lengths[num_episodes];

    for(unsigned i = 0; i < num_episodes; i++)
    {
        //Setup episode
        reset_agent(&cliff_walking.cliff);
        if(verbosity)
        {
            display_cliff(&cliff_walking.cliff);
            print_q_table(&q_table);
        }
        double reward_cumm = 0.;
        unsigned step = 0;

        //Start episode
        while(true)
        {
            //sleep(1);

            //Sample action
            const unsigned action = sample_action(&q_table,
                                                  cliff_walking.cliff.agent.state,
                                                  epsilon);

            //Take step in environment
            const struct Experience experience = agent_step(&cliff_walking, action);

            //Update QTable
            q_learning_step(&q_table, &q_params, experience);

            if(verbosity)
            {
                printf("Episode: %u\n", i);
                printf("Step: %u\n", step);
                printf("Action: %u\n", action);
                printf("Reward: %f\n", experience.r);
                display_cliff(&cliff_walking.cliff);
                print_q_table(&q_table);
            }

            reward_cumm += experience.r;
            step += 1;

            if(experience.episode_end)
            {
                cumm_rewards[i] = reward_cumm;
                episode_lengths[i] = step;
                break;
            }

        }

    }

    printf("Final Q-table:\n");
    print_q_table(&q_table);
    printf("Cummulative rewards:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%f ", cumm_rewards[i]);
    printf("Episode lengths:\n");
    for(unsigned i = 0; i < num_episodes; i++)
        printf("%u ", episode_lengths[i]);

    return 0;

}
