#include <domains/cliff_walking.h>
#include <unistd.h>
#include <optimisers/qlearning.h>
#include <stdio.h>

int main()
{

    //Domain
    const unsigned cliff_width = 5;
    const unsigned cliff_height = 3;
    struct CliffWalking cliff_walking = create_cliff_walking(cliff_width, cliff_height);

    //enum Action moves[] = {Up, Up, Right, Right, Right, Right, Down, Down, Up, Up};
    enum Action moves[] = {Up, Up, Right, Right, Down, Down, Up, Up};
    const unsigned num_moves = sizeof(moves) / sizeof(moves[0]);

    //Q-Learning
    const double alpha = 0.1;
    const double gamma = 0.99;
    const double epsilon = 0.1;
    const struct QLearningParams q_params = {.alpha = alpha, .gamma = gamma};
    const unsigned num_states = cliff_width * cliff_height;
    const unsigned num_actions = 4;
    struct QTable q_table = create_q_table(num_states, num_actions);

    display_cliff(&cliff_walking.cliff);
    print_q_table(&q_table);
    //for(size_t i = 0; i < num_moves; i++)
    while(true)
    {
        //sleep(1);

        //Sample action
        const unsigned action = sample_action(&q_table,
                                              cliff_walking.cliff.agent.state,
                                              epsilon);
        printf("Action: %u\n", action);

        //Take step in environment
        const struct Experience experience = agent_step(&cliff_walking, action);

        display_cliff(&cliff_walking.cliff);
        printf("Reward: %f\n", experience.r);

        //Update QTable
        q_learning_step(&q_table, &q_params, experience);
        print_q_table(&q_table);

    }

    return 0;

}
